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

#include "FestoStationDriver.h"


Int bufferCounter = 0;
Char buffer[50];
Semaphore_Handle bufferSem;


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
/*
//	// create the LCD context
//	tContext g_sContext;
//
//	// initialize LCD driver
//	Kentec320x240x16_SSD2119Init(120000000);
//
//	// initialize graphics context
//	GrContextInit(&g_sContext, &g_sKentec320x240x16_SSD2119);
//
//	// draw application frame
//	FrameDraw(&g_sContext, "Festo Station");
//
//	System_printf("Screen Ready\n");
//	System_flush();
//
//
//
//	while(1)
//	{
//		// draw something
//		GrStringDraw(&g_sContext,
//				"LCD_update FESTO", 15, 10, GrContextDpyHeightGet(&g_sContext)/2, 1);
//		FrameDraw(&g_sContext, "Festo Station 2");
//		// sleep for 16ms
//		Task_sleep(16);
//	}
*/
}

Void _task_Festo(UArg arg0, UArg arg1)
{
	System_printf("Festo task Ready\n");
	System_flush();




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

	FestoStationDriver Driver;

    Festo_Driver_Init(&Driver);
    Festo_Control_Driver(&Driver, FESTO_ENABLED);

    System_printf("Extendind Ejector\n");
    System_flush();
    Festo_Control_Ejector(&Driver, FESTO_EJECTOR_EXTEND);

    System_printf("Raising Platform\n");
    System_flush();
    Festo_Control_Platform(&Driver, FESTO_PLATFORM_RAISE);

    System_printf("Lowering Platform\n");
    System_flush();
    Festo_Control_Platform(&Driver, FESTO_PLATFORM_LOWER);

    System_printf("Measuring Down\n");
    System_flush();
    Festo_Control_Measure(&Driver, FESTO_MEASURE_DOWN);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_OFF);
    GPIO_write(Board_LED1, Board_LED_OFF);
    GPIO_write(Board_LED2, Board_LED_OFF);

    System_printf("Place sensor = %d\n", Festo_Sense_Piece_Placed(&Driver));
    System_flush();
    System_printf("Place sensor = %d\n", Festo_Sense_Piece_Placed(&Driver));
    System_flush();

    System_printf("colour sensor = %d\n", Festo_Sense_Piece_Colour(&Driver));
    System_flush();
    System_printf("colour sensor = %d\n", Festo_Sense_Piece_Colour(&Driver));
    System_flush();

    System_printf("mat sensor = %d\n", Festo_Sense_Piece_Material(&Driver));
    System_flush();
    System_printf("mat sensor = %d\n", Festo_Sense_Piece_Material(&Driver));
    System_flush();

    System_printf("riser d sensor = %d\n", Festo_Sense_Riser_Down(&Driver));
    System_flush();
    System_printf("riser d sensor = %d\n", Festo_Sense_Riser_Down(&Driver));
    System_flush();

    System_printf("riser up sensor = %d\n", Festo_Sense_Riser_Up(&Driver));
    System_flush();
    System_printf("riser up sensor = %d\n", Festo_Sense_Riser_Up(&Driver));
    System_flush();

    System_printf("ejct sensor = %d\n", Festo_Sense_Ejector_Ready(&Driver));
    System_flush();
    System_printf("ejct sensor = %d\n", Festo_Sense_Ejector_Ready(&Driver));
    System_flush();

    System_printf("meas sensor = %d\n", Festo_Sense_Measure_Down(&Driver));
    System_flush();
    System_printf("meas sensor = %d\n", Festo_Sense_Measure_Down(&Driver));
    System_flush();


    /* This example has logging and many other debug capabilities enabled */
    System_printf("This example does not attempt to minimize code or data "
                  "footprint\n");
    System_flush();

    System_printf("Starting the UART Echo example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in "
                  "ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

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
				"LCD_update FESTO", 15, 10, GrContextDpyHeightGet(&g_sContext)/2, 1);


    /* Start BIOS */
    BIOS_start();

    return (0);
}
