/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"

/* Kentec Screen Header files */
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "drivers/frame.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "utils/ustdlib.h"

/* Festo Station Driver */
#include "FestoStationDriver.h"

Int bufferCounter = 0;
Char buffer[50];
Semaphore_Handle bufferSem;

typedef struct MsgObj
{
    uint8_t		id;             /* writer task id */
    char*		string_ptr;     /* message value */
} MsgObj;


/*
 *  ======== echoFxn ========
 *  Task for this function is created statically. See the project's .cfg file.
 */
Void echoFxn(UArg arg0, UArg arg1)
{
    char input;
    UART_Handle uart;
    UART_Params uartParams;
    const char echoPrompt[] = "\fEchoing characters:\r\n";

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;
    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        System_abort("Error opening the UART");
    }

    UART_write(uart, echoPrompt, sizeof(echoPrompt));

    System_printf("UART Ready\n");
    System_flush();

    /* Loop forever echoing */
    while (1) {
       UART_read(uart, &input, 1);
       UART_write(uart, &input, 1);

       Semaphore_pend(bufferSem, BIOS_WAIT_FOREVER);

       buffer[bufferCounter % 50] = input;
       bufferCounter++;

       Semaphore_post(bufferSem);


       Task_sleep(10);
    }
}

/**
    Task for updating the LCD display every 16ms.
*/
Void _task_LCD(UArg arg0, UArg arg1)
{
	// create the LCD context
	tContext g_sContext;

	// initialize LCD driver
	Kentec320x240x16_SSD2119Init(120000000);

	// initialize graphics context
	GrContextInit(&g_sContext, &g_sKentec320x240x16_SSD2119);

	// draw application frame
	FrameDraw(&g_sContext, "Festo Station");

	System_printf("Screen Ready\n");
	System_flush();

	GrStringDraw(&g_sContext,
				"LCD_update FESTO 12", 19, 25, GrContextDpyHeightGet(&g_sContext)/2, 1);
}

Void _task_Festo(UArg arg0, UArg arg1)
{
	System_printf("Festo task Ready\n");
	System_flush();

	FestoStationDriver Driver;

    Festo_Driver_Init(&Driver);
    Festo_Control_Driver(&Driver, FESTO_ENABLED);

	uint32_t EventPosted;

	while(1)
	{
		EventPosted = Event_pend(FestoEvents,
						Event_Id_NONE,
						Event_Id_00 + Event_Id_01 + Event_Id_02 + Event_Id_03 + Event_Id_04,
						FESTO_TIMEOUT);

		if (EventPosted & Event_Id_00)
		{
			GPIO_toggle(Board_LED0);
			Festo_Control_Platform(&Driver, FESTO_PLATFORM_RAISE);
		}
		else if (EventPosted & Event_Id_01)
		{
			GPIO_toggle(Board_LED1);
			Festo_Control_Platform(&Driver, FESTO_PLATFORM_LOWER);
		}
		else if (EventPosted & Event_Id_02)
		{
			GPIO_toggle(Board_LED2);
		}
		else if (EventPosted & Event_Id_03)
		{
			System_printf("FestoEvent Riser Down");
			System_flush();
		}
		else if (EventPosted & Event_Id_04)
		{
			System_printf("FestoEvent Riser Up");
			System_flush();
		}
		else
		{
		}
		Task_sleep(10);
	}

//	Mailbox_Params mbxParams;
//    Mailbox_Params_init(&mbxParams);
//    mbxParams.readerEvent = evt;
//    mbxParams.readerEventId = Event_Id_02;
//    mbx = Mailbox_create(sizeof(MsgObj), 2, &mbxParams, NULL);


}

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on Board_BUTTON0.
 */
void _callback_Button_Up(void)
{
    /* Clear the GPIO interrupt and toggle an LED */
    Event_post(FestoEvents, Event_Id_00);
    GPIO_clearInt(Board_BUTTON1);

}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 *  This may not be used for all boards.
 */
void _callback_Button_Down(void)
{
    /* Clear the GPIO interrupt and toggle an LED */
    Event_post(FestoEvents, Event_Id_01);
    GPIO_clearInt(Board_BUTTON2);

}

/*
 *  ======== gpioButtonFxn2 ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 *  This may not be used for all boards.
 */
void _callback_Button_Select(void)
{
    /* Clear the GPIO interrupt and toggle an LED */
    Event_post(FestoEvents, Event_Id_02);
    GPIO_clearInt(Board_BUTTON0);

}

/*
 *  ======== gpioButtonFxn2 ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 *  This may not be used for all boards.
 */
void _callback_Festo_Riser_Down(void)
{
    /* Clear the GPIO interrupt and toggle an LED */
    Event_post(FestoEvents, Event_Id_03);
    GPIO_clearInt(Board_SENSE_RISER_DOWN);

}

/*
 *  ======== gpioButtonFxn2 ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 *  This may not be used for all boards.
 */
void _callback_Festo_Riser_Up(void)
{
    /* Clear the GPIO interrupt and toggle an LED */
    Event_post(FestoEvents, Event_Id_04);
    GPIO_clearInt(Board_SENSE_RISER_UP);

}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();

    bufferSem = Semaphore_create(1, NULL, NULL);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_OFF);
    GPIO_write(Board_LED1, Board_LED_OFF);
    GPIO_write(Board_LED2, Board_LED_OFF);

    /* This example has logging and many other debug capabilities enabled */
    System_printf("This example does not attempt to minimize code or data "
                  "footprint\n");
    System_flush();

    System_printf("Starting the UART Echo example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in "
                  "ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

	// Interrupts test //
    GPIO_setupCallbacks(&Board_gpioCallbacks0);
    GPIO_setupCallbacks(&Board_gpioCallbacks1);
    GPIO_setupCallbacks(&Board_gpioCallbacks2);
    GPIO_setupCallbacks(&Board_gpioCallbacks3);

    GPIO_enableInt(Board_BUTTON0, GPIO_INT_RISING);
    GPIO_enableInt(Board_BUTTON1, GPIO_INT_RISING);
    GPIO_enableInt(Board_BUTTON2, GPIO_INT_RISING);
    GPIO_enableInt(Board_SENSE_RISER_DOWN, GPIO_INT_RISING);
    GPIO_enableInt(Board_SENSE_RISER_UP, GPIO_INT_RISING);

    // interrupts test end//
    /* Start BIOS */
    BIOS_start();

    return (0);
}
