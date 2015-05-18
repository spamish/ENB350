/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Festo Station Driver */
#include "FestoStationDriver.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>

/* Example/Board Header files */
#include "Board.h"

/* Kentec Screen Header files */
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "drivers/frame.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "utils/ustdlib.h"
#include "inc/hw_memmap.h"



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

	while(1)
	{
		GrStringDraw(&g_sContext,
				"LCD_update FESTO 12", 19, 25, GrContextDpyHeightGet(&g_sContext)/2, 1);
		ADCProcessorTrigger(ADC0_BASE, 0);
		while(!ADCIntStatus(ADC0_BASE, 0, false))
		{
		}
		uint32_t adcdata;
		ADCSequenceDataGet(ADC0_BASE, 0, &adcdata);

	    System_printf("ADC data: %d\n", adcdata);
	    System_flush();
		Task_sleep(1000);
	}
}

//*****************************************************************************
//
//! Festo Station Task.
//!
//! This task function do all the work related to controlling the Festo
//! Station. The task will first initialize the Driver and then run a infinite
//! loop, waiting and processing external events.
//!
//! \return None.
//
//*****************************************************************************
Void _task_Festo(UArg arg0, UArg arg1)
{
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
			ADCProcessorTrigger(ADC0_BASE, 0);
		}
		else if (EventPosted & Event_Id_03)
		{
		}
		else if (EventPosted & Event_Id_04)
		{
		}
		else if (EventPosted & Event_Id_05)
		{
			uint32_t adcdata;
			ADCSequenceDataGet(ADC0_BASE, 0, &adcdata);
		    System_printf("ADC data: %d\n", adcdata);
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

//*****************************************************************************
//
//! Button Up Callback.
//!
//! This function is executed when the Button Up is pressed, which means a
//! interrupt for this pin was fired.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Button_Up(void)
{
    Event_post(FestoEvents, Event_Id_00);
    GPIO_clearInt(Board_BUTTON1);
}

//*****************************************************************************
//
//! Button Down Callback.
//!
//! This function is executed when the Button Down is pressed, which means a
//! interrupt for this pin was fired.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Button_Down(void)
{
    Event_post(FestoEvents, Event_Id_01);
    GPIO_clearInt(Board_BUTTON2);
}

//*****************************************************************************
//
//! Button Sellect Callback.
//!
//! This function is executed when the Button Sellect is pressed, which means a
//! interrupt for this pin was fired.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Button_Select(void)
{
    Event_post(FestoEvents, Event_Id_02);
    GPIO_clearInt(Board_BUTTON0);
}

//*****************************************************************************
//
//! Festo Riser Callback.
//!
//! This function is executed when the Riser reaches the bottom of the
//! Festo Station platform.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Festo_Riser_Down(void)
{
    Event_post(FestoEvents, Event_Id_03);
    GPIO_clearInt(Board_SENSE_RISER_DOWN);
}

//*****************************************************************************
//
//! Festo Riser Callback.
//!
//! This function is executed when the Riser reaches the top of the Festo
//! Station platform.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Festo_Riser_Up(void)
{
    Event_post(FestoEvents, Event_Id_04);
    GPIO_clearInt(Board_SENSE_RISER_UP);
}

//*****************************************************************************
//
//! ISR for the ADC0 conversion.
//!
//! This function is executed when the conversion is finished on ADC0.
//!
//! \return None.
//
//*****************************************************************************
void _ISR_ADC0(void)
{
    Event_post(FestoEvents, Event_Id_05);
    ADCIntClear(ADC0_BASE, 0);
}

//*****************************************************************************
//
//! Main application function.
//!
//! This function is meant to initialize all devices and tasks used in this
//! application. After the initialization is complete, the tasks start running.
//!
//! \return None.
//
//*****************************************************************************
int main(void)
{
    // Initialize General configurations
    Board_initGeneral();

    // Initialize GPIO
    Board_initGPIO();

    // Initialize UART
    Board_initUART();

    bufferSem = Semaphore_create(1, NULL, NULL);

    // Turn off all LEDS
    GPIO_write(Board_LED0, Board_LED_OFF);
    GPIO_write(Board_LED1, Board_LED_OFF);
    GPIO_write(Board_LED2, Board_LED_OFF);

	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_CH0 | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCSoftwareOversampleConfigure(ADC0_BASE, 0, 64);
	ADCIntClear(ADC0_BASE, 0);
	ADCIntEnable(ADC0_BASE, 0);

	// Initializes interrupts
    GPIO_setCallback(Board_BUTTON0, _callback_Button_Select);
    GPIO_setCallback(Board_BUTTON1, _callback_Button_Up);
    GPIO_setCallback(Board_BUTTON2, _callback_Button_Down);
    GPIO_setCallback(Board_SENSE_RISER_DOWN, _callback_Festo_Riser_Down);
    GPIO_setCallback(Board_SENSE_RISER_UP, _callback_Festo_Riser_Up);

    // Enable Interrupts
    GPIO_enableInt(Board_BUTTON0);
    GPIO_enableInt(Board_BUTTON1);
    GPIO_enableInt(Board_BUTTON2);
    GPIO_enableInt(Board_SENSE_RISER_DOWN);
    GPIO_enableInt(Board_SENSE_RISER_UP);

    // Start BIOS
    BIOS_start();

    return (0);
}
