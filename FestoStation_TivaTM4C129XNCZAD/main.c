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
Void task_LCD_update(UArg arg0, UArg arg1)
{
	// create the LCD context
	tContext g_sContext;

	// initialize LCD driver
	Kentec320x240x16_SSD2119Init(120000000);

	// initialize graphics context
	GrContextInit(&g_sContext, &g_sKentec320x240x16_SSD2119);

	// draw application frame
	FrameDraw(&g_sContext, "Echo Team Jaun");

	while(1)
	{
		// draw something
		GrStringDraw(&g_sContext,
				"LCD_update", -1, 10, GrContextDpyHeightGet(&g_sContext)/2, 1);

		// sleep for 16ms
		Task_sleep(16);
	}
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

    Init_FestoStationDriver(&Driver);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);
    GPIO_write(Board_LED1, Board_LED_ON);

    /* This example has logging and many other debug capabilities enabled */
    System_printf("This example does not attempt to minimize code or data "
                  "footprint\n");
    System_flush();

    System_printf("Starting the UART Echo example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in "
                  "ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
