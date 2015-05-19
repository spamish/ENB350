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

uint32_t 	piecesProcessed = 0;
uint32_t 	orangeAccepted = 0;
uint32_t 	orangeRejected = 0;
uint32_t 	blackAccepted = 0;
uint32_t 	blackRejected = 0;
uint32_t 	metalAccepted = 0;
uint32_t 	metalRejected = 0;
uint32_t 	plasticAccepted = 0;
uint32_t 	plasticRejected = 0;
float		piecesProcessedPerSecond = 0.0f;
float		heightMeasured = 0.0f;
float		heightCalibrated = 25.6f;
float		upperHeightCalibrated = 25.8f;
float		lowerHeightCalibrated = 25.4f;
float 		heightConstantADC = 1.0f;
uint32_t	timeHMS[3] = {0,0,0};
uint32_t	uptimeSeconds = 0;

Int bufferCounter = 0;
Char buffer[50];
Semaphore_Handle bufferSem;

typedef struct DisplayMessage
{
    uint32_t	ScreenID;
	uint32_t 	piecesProcessed;
	uint32_t 	orangeAccepted;
	uint32_t 	orangeRejected;
	uint32_t 	blackAccepted;
	uint32_t 	blackRejected;
	uint32_t 	metalAccepted;
	uint32_t 	metalRejected;
	uint32_t 	plasticAccepted;
	uint32_t 	plasticRejected;
	uint32_t	piecesProcessedPerSecond;
	uint32_t	heightMeasured;
	uint32_t	heightCalibrated;
	uint32_t	upperHeightCalibrated;
	uint32_t	lowerHeightCalibrated;
	uint32_t	timeHours;
	uint32_t	timeMinutes;
	uint32_t	timeSeconds;
	uint32_t	uptimeSeconds;
	uint32_t	reserved;
}	DisplayMessage;


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

	uint32_t EventPosted;

	DisplayMessage MessageObject;

	while(1)
	{
		EventPosted = Event_pend(DisplayEvents,
						Event_Id_NONE,
						Event_Id_00,
						10);

		if (EventPosted & Event_Id_00)
		{
			 if (Mailbox_pend(DisplayMailbox, &MessageObject, BIOS_NO_WAIT))
			 {
				 // something need to be draw
				 // code goes here
			 }
		}

		Task_sleep(16);
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
Void _task_FESTO(UArg arg0, UArg arg1)
{
	// initialize driver
	FestoStationDriver Driver;

    Festo_Driver_Init(&Driver);

    // Enable Station
    Festo_Control_Driver(&Driver, FESTO_ENABLED);

	uint32_t EventPosted;

	DisplayMessage MessageObject;

	while(1)
	{
		EventPosted = Event_pend(FestoEvents,
						Event_Id_NONE,
						FESTO_EVENT_BUTTON_UP + FESTO_EVENT_BUTTON_DOWN +
						FESTO_EVENT_BUTTON_SELECT + FESTO_EVENT_RISER_DOWN +
						FESTO_EVENT_RISER_UP + FESTO_EVENT_ADC_FINISH,
						FESTO_TIMEOUT);

		if (EventPosted & FESTO_EVENT_BUTTON_UP)
		{
			GPIO_toggle(Board_LED0);
			Festo_Control_Platform(&Driver, FESTO_PLATFORM_RAISE);
		}
		else if (EventPosted & FESTO_EVENT_BUTTON_DOWN)
		{
			GPIO_toggle(Board_LED1);
			Festo_Control_Platform(&Driver, FESTO_PLATFORM_LOWER);
		}
		else if (EventPosted & FESTO_EVENT_BUTTON_SELECT)
		{
			GPIO_toggle(Board_LED2);
			Event_post(FestoEvents, FESTO_EVENT_ADC_START);
		}
		else if (EventPosted & Event_Id_03)
		{
		}
		else if (EventPosted & Event_Id_04)
		{
		}
		else if (EventPosted & FESTO_EVENT_ADC_FINISH)
		{
		    System_printf("ADC data: %d\n", heightMeasured);
		    System_flush();
		    Mailbox_post(DisplayMailbox, &MessageObject, 0);
		}
		else
		{
		}
		Task_sleep(10);
	}
}

//*****************************************************************************
//
//! ADC conversion Task.
//!
//! This task function do all the work related to the ADC conversion real
//!
//! \return None.
//
//*****************************************************************************
Void _task_ADC(UArg arg0, UArg arg1)
{
	// initialize ADC
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_CH0 | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCSoftwareOversampleConfigure(ADC0_BASE, 0, 64);
	ADCIntClear(ADC0_BASE, 0);
	ADCIntEnable(ADC0_BASE, 0);

	// data from ADC
	uint32_t AdcDataRaw = 0;

	uint32_t EventPosted;

	while(1)
	{
		EventPosted = Event_pend(FestoEvents,
						Event_Id_NONE,
						FESTO_EVENT_ADC_START,
						0);

		if (EventPosted & FESTO_EVENT_ADC_START)
		{
			// start an ADC reading
			ADCProcessorTrigger(ADC0_BASE, 0);
		}
		else
		{
			if (ADCIntStatus(ADC0_BASE, 0, false))
			{
				// ADC reading complete
				ADCSequenceDataGet(ADC0_BASE, 0, &AdcDataRaw);
				ADCIntClear(ADC0_BASE, 0);
				//lock resource
				// convert ADC reading to [mm]
				heightMeasured = AdcDataRaw * 1; //should be a constant
			    System_printf("ADC data: %d\n", AdcDataRaw);
			    System_flush();
				//unlock resource
			    Event_post(FestoEvents, FESTO_EVENT_ADC_FINISH);
			}
		}
		Task_sleep(100);
	}
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
