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
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <time.h>
#include <ti/sysbios/hal/Seconds.h>


/* Example/Board Header files */
#include "Board.h"

/* Kentec Screen Header files */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "utils/ustdlib.h"
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
float		heightCalibrated = 25.6f;
float		upperHeightCalibrated = 25.8f;
float		lowerHeightCalibrated = 25.4f;
float 		heightConstantADC = 1.0f;
uint32_t	timeHMS[3] = {0,0,0};
uint32_t	uptimeSeconds = 0;

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

void _Festo_Deactivate_Ejector(UArg arg0);




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

	char StringBuffer[100];

	tRectangle ClearRect;
	ClearRect.i16XMax = 320;
	ClearRect.i16XMin = 0;
	ClearRect.i16YMax = 240;
	ClearRect.i16YMin = 0;

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
				GrContextForegroundSet(&g_sContext, 0x00);
				GrRectFill(&g_sContext, &ClearRect);
				if (MessageObject.ScreenID == 0)
				{
					FrameDraw(&g_sContext, "Festo Station - Stopped");

					GrStringDraw(&g_sContext, "Press [Up] to start.", 	-1, 10, 30, 0);
					GrStringDraw(&g_sContext, "Press [Down] to stop.", 	-1, 10, 50, 0);
					GrStringDraw(&g_sContext, "Press [Select] to calibrate.", 	-1, 10, 70, 0);


					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}

				if (MessageObject.ScreenID == 1)
				{
					FrameDraw(&g_sContext, "Festo Station - Running");

					sprintf(StringBuffer, "Pieces processed = %d", MessageObject.piecesProcessed);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 30, 0);

					sprintf(StringBuffer, "Orange A/R =  %d/%d", MessageObject.orangeAccepted, MessageObject.orangeRejected);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 50, 0);

					sprintf(StringBuffer, "Black A/R =  %d/%d", MessageObject.blackAccepted, MessageObject.blackRejected);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 70, 0);

					sprintf(StringBuffer, "Plastic A/R =  %d/%d", MessageObject.plasticAccepted, MessageObject.plasticRejected);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 90, 0);

					sprintf(StringBuffer, "Pieces processed/sec = %d [p/s]", MessageObject.piecesProcessedPerSecond);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 110, 0);

					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

				    sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}
				if (MessageObject.ScreenID == 2)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");

					GrStringDraw(&g_sContext, "Initializing Calibration...", 	-1, 10, 30, 0);

					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

				    sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);

				}
				if (MessageObject.ScreenID == 3)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");
					//Body
					GrStringDraw(&g_sContext, "Put the standard piece on platform and press [Select].", 	-1, 10, 30, 0);

					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);


				}
				if (MessageObject.ScreenID == 4)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");
					//Body
					GrStringDraw(&g_sContext, "Set the height using [Up] and [Down].", 	-1, 10, 30, 0);
					GrStringDraw(&g_sContext, "When finished, press [Select].", 	-1, 10, 50, 0);
					sprintf(StringBuffer, "Height = %d [mm]", MessageObject.heightCalibrated);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 120, 0);
					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}

				if (MessageObject.ScreenID == 5)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");

					//Body
					GrStringDraw(&g_sContext, "Set the upper limit using [Up] and", 	-1, 10, 30, 0);
					GrStringDraw(&g_sContext, "[Down]. When finished, press [Select].", 	-1, 10, 50, 0);
					sprintf(StringBuffer, "Upper Limit = %d [mm]", MessageObject.upperHeightCalibrated);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 120, 0);

					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}
				if (MessageObject.ScreenID == 6)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");

					//Body
					GrStringDraw(&g_sContext, "Set the lower limit using [Up] and", 	-1, 10, 30, 0);
					GrStringDraw(&g_sContext, "[Down]. When finished, press [Select].", 	-1, 10, 50, 0);
					sprintf(StringBuffer, "Lower Limit = %d [mm]", MessageObject.lowerHeightCalibrated);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 10, 120, 0);


					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}
				if (MessageObject.ScreenID == 7)
				{
					FrameDraw(&g_sContext, "Festo Station - Calibration");
					// Body
					GrStringDraw(&g_sContext, "The Festo Station was calibrated with success!", 	-1, 10, 30, 0);
					//Footer
					sprintf(StringBuffer, "Uptime: %d [s]", MessageObject.uptimeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 180, 0);

					sprintf(StringBuffer, "Time: %d:%d:%d", MessageObject.timeHours, MessageObject.timeMinutes, MessageObject.timeSeconds);
					GrStringDraw(&g_sContext, StringBuffer, 	-1, 100, 200, 0);
				}
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
	FestoStationDriver DriverInstance;
	FestoStationDriver* Driver = &DriverInstance;

    Festo_Driver_Init(Driver);

	uint32_t EventPosted;
	DisplayMessage MessageObject;

	MessageObject.ScreenID = 0;
	MessageObject.timeHours = 0;
	MessageObject.timeMinutes = 0;
	MessageObject.timeSeconds = 0;
	MessageObject.uptimeSeconds = 0;
	MessageObject.piecesProcessed = 0;
	MessageObject.blackAccepted = 0;
	MessageObject.blackRejected = 0;
	MessageObject.plasticAccepted = 0;
	MessageObject.plasticRejected = 0;
	MessageObject.orangeAccepted = 0;
	MessageObject.orangeRejected = 0;
	MessageObject.piecesProcessedPerSecond = 0;
	MessageObject.heightCalibrated = 230;
	MessageObject.upperHeightCalibrated = 245;
	MessageObject.lowerHeightCalibrated = 227;

	uint32_t uptimeSeconds = 0;
	uint32_t piecesProcessed = 0;
	uint32_t blackAccepted = 0;
	uint32_t blackRejected = 0;
	uint32_t plasticAccepted = 0;
	uint32_t plasticRejected = 0;
	uint32_t orangeAccepted = 0;
	uint32_t orangeRejected = 0;
	uint32_t metallicAccepted = 0;
	uint32_t metallicRejected = 0;
	uint32_t piecesProcessedPerSecond = 0;
	uint32_t upperHeightCalibrated = 245;
	uint32_t lowerHeightCalibrated = 227;

	uint32_t *ColourAccepted;
	uint32_t *ColourRejected;
	uint32_t *MaterialAccepted;
	uint32_t *MaterialRejected;

	uint8_t colour = 0;
	uint8_t material = 0;

	uint32_t FestoState = 0;
	// 0 = stopped
	// 1 = idle
	// 2 = initial state
	// 10 = cal

	int32_t LowerLimit = 1200;
	int32_t UpperLimit = 1500;

	int32_t Uptime = 0;
	int32_t Time0 = 0;
	int32_t Time1 = 0;
	uint8_t Running = 0;

	Clock_Params clockParams;
	Clock_Handle myClock;
	Clock_Params_init(&clockParams);
	clockParams.arg = (UArg) Board_ACTUATOR_EJECTOR;
	myClock = Clock_create(_Festo_Deactivate_Ejector, 200, &clockParams, NULL);

	uint32_t heightMeasured = 0;
	uint32_t heightCalibrated = 1200;

	float ConvertFactor = 0.1*MessageObject.heightCalibrated/1200;

    GPIO_write(Board_LED0, Board_LED_OFF);
    GPIO_write(Board_LED1, Board_LED_OFF);
    GPIO_write(Board_LED2, Board_LED_ON);

    Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);

	Clock_start(Clock_1_sec);

	while(1)
	{
		EventPosted = Event_pend(FestoEvents,
						Event_Id_NONE,
						FESTO_EVENT_BUTTON_UP + FESTO_EVENT_BUTTON_DOWN +
						FESTO_EVENT_BUTTON_SELECT + FESTO_EVENT_RISER_DOWN +
						FESTO_EVENT_RISER_UP + FESTO_EVENT_ADC_FINISH +
						FESTO_EVENT_PIECE_IN_PLACE + FESTO_EVENT_EJECTOR_FINISHED +
						FESTO_EVENT_TICK + FESTO_EVENT_COOLDOWN +
						FESTO_EVENT_PIECE_NOT_IN_PLACE,
						FESTO_TIMEOUT);

		if (EventPosted & FESTO_EVENT_BUTTON_UP)
		{
			if (FestoState == 0)
			{
				Festo_Control_Driver(Driver, FESTO_ENABLED);
				FestoState = 1;
				Running = 1;
				GPIO_write(Board_LED0, Board_LED_ON);
				GPIO_write(Board_LED1, Board_LED_OFF);
				GPIO_write(Board_LED2, Board_LED_OFF);
				MessageObject.ScreenID = 1;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
				Time0 = Clock_getTicks();
			}
			else if (FestoState == 12)
			{
				MessageObject.heightCalibrated++;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 13)
			{
				MessageObject.upperHeightCalibrated++;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 14)
			{
				MessageObject.lowerHeightCalibrated++;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
		}
		else if (EventPosted & FESTO_EVENT_BUTTON_DOWN)
		{
			if (FestoState == 1)
			{
				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
				Festo_Control_Driver(Driver, FESTO_DISABLED);
				FestoState = 0;
				Running = 0;
				GPIO_write(Board_LED0, Board_LED_OFF);
				GPIO_write(Board_LED1, Board_LED_OFF);
				GPIO_write(Board_LED2, Board_LED_ON);
			}
			else if (FestoState == 12)
			{
				MessageObject.heightCalibrated--;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 13)
			{
				MessageObject.upperHeightCalibrated--;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 14)
			{
				MessageObject.lowerHeightCalibrated--;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
		}
		else if (EventPosted & FESTO_EVENT_BUTTON_SELECT)
		{
			if (FestoState == 0)
			{
				GPIO_write(Board_LED0, Board_LED_OFF);
				GPIO_write(Board_LED1, Board_LED_ON);
				GPIO_write(Board_LED2, Board_LED_OFF);

				Festo_Control_Driver(Driver, FESTO_ENABLED);
				FestoState = 10;

				MessageObject.ScreenID = 2;

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);

				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				if (Festo_Sense_Riser_Down(Driver) != 1)
				{
					Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
				}
				else
				{
					FestoState = 11;
					MessageObject.ScreenID = 3;
					Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
				}
			}
			else if (FestoState == 11)
			{
				FestoState = 12;
				MessageObject.ScreenID = 4;
				MessageObject.heightCalibrated = 230;

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 12)
			{
				FestoState = 13;

				Festo_Control_Platform(Driver, FESTO_PLATFORM_RAISE);

				Event_post(FestoEvents, FESTO_EVENT_ADC_START);
			}
			else if (FestoState == 13)
			{
				FestoState = 14;
				MessageObject.ScreenID = 6;
				MessageObject.lowerHeightCalibrated = 227;
				UpperLimit = MessageObject.upperHeightCalibrated*
						heightCalibrated/MessageObject.heightCalibrated;

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 14)
			{
				FestoState = 15;
				MessageObject.ScreenID = 7;

				LowerLimit = MessageObject.lowerHeightCalibrated*
							heightCalibrated/MessageObject.heightCalibrated;

				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else if (FestoState == 15)
			{
				FestoState = 0;
				MessageObject.ScreenID = 0;

				GPIO_write(Board_LED0, Board_LED_OFF);
				GPIO_write(Board_LED1, Board_LED_OFF);
				GPIO_write(Board_LED2, Board_LED_ON);

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
			else
			{

			}
		}
		else if (EventPosted & FESTO_EVENT_RISER_UP)
		{
			if (FestoState == 3)
			{
				if (Festo_Sense_Piece_Placed(Driver) == 1)
				{
					FestoState = 4;
					Event_post(FestoEvents, FESTO_EVENT_ADC_START);
				}
				else
				{
					FestoState = 1;
					Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
					Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
				}
			}
			else if (FestoState == 13)
			{
				Event_post(FestoEvents, FESTO_EVENT_ADC_START);
			}
			else
			{

			}
		}
		else if (EventPosted & FESTO_EVENT_RISER_DOWN)
		{
			if (FestoState == 2)
			{
				if (Festo_Sense_Piece_Placed(Driver) == 1)
				{

					colour = Festo_Sense_Piece_Colour(Driver);
					material = Festo_Sense_Piece_Material(Driver);

					FestoState = 3;
					Festo_Control_Platform(Driver, FESTO_PLATFORM_RAISE);
				}
				else
				{
					FestoState = 1;
				}
			}
			if (FestoState == 5)
			{
				Festo_Control_Ejector(Driver, FESTO_EJECTOR_EXTEND);
				Clock_start(myClock);
			}
			if (FestoState == 6)
			{
				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				Clock_start(myClock);
				FestoState = 7;
			}
			if (FestoState == 10)
			{
				FestoState = 11;
				MessageObject.ScreenID = 3;
				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
		}
		else if (EventPosted & FESTO_EVENT_ADC_FINISH)
		{
		    if (Mailbox_pend(ADCMailbox, &heightMeasured, BIOS_NO_WAIT))
		    {
		    	Festo_Sense_Set_Piece_Height(Driver, heightMeasured);
		    }
		    else
		    {
		    	Event_post(FestoEvents, FESTO_EVENT_ADC_START);
		    }
			if (FestoState == 4)
			{
				if (colour == FESTO_COLOR_ORANGE && material == FESTO_PIECE_OTHER)
				{
					ColourAccepted = &orangeAccepted;
					ColourRejected = &orangeRejected;
					MaterialAccepted = &plasticAccepted;
					MaterialRejected = &plasticRejected;
				}
				else if (colour == FESTO_COLOR_OTHER && material == FESTO_PIECE_OTHER)
				{
					ColourAccepted = &blackAccepted;
					ColourRejected = &blackRejected;
					MaterialAccepted = &plasticAccepted;
					MaterialRejected = &plasticRejected;
				}
				else if (colour == FESTO_COLOR_OTHER && material == FESTO_PIECE_METALLIC)
				{
					ColourAccepted = NULL;
					ColourRejected = NULL;
					MaterialAccepted = &metallicAccepted;
					MaterialRejected = &metallicRejected;
				}
				else
				{
					ColourAccepted = NULL;
					ColourRejected = NULL;
					MaterialAccepted = NULL;
					MaterialRejected = NULL;
				}

				if (heightMeasured < UpperLimit && heightMeasured > LowerLimit)//withn range
				{
					if (ColourAccepted != NULL)
					{
						(*ColourAccepted)++;
					}
					if (MaterialAccepted != NULL)
					{
						(*MaterialAccepted)++;
						piecesProcessed++;
					}
					FestoState = 6;
					System_printf("Piece is acceptable\n");
					System_flush();

					Festo_Control_Ejector(Driver, FESTO_EJECTOR_EXTEND);
					Clock_start(myClock);
				}
				else // out of range
				{
					if (ColourAccepted != NULL)
					{
						(*ColourRejected)++;
					}
					if (MaterialAccepted != NULL)
					{
						(*MaterialRejected)++;
						piecesProcessed++;
					}
					System_printf("Piece is NOT acceptable\n");
					System_flush();
					FestoState = 5;
					Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
				}
			}
			else if (FestoState == 13)
			{

				ConvertFactor = 0.1*MessageObject.heightCalibrated/heightMeasured;
				heightCalibrated = heightMeasured;

				MessageObject.ScreenID = 5;
				MessageObject.upperHeightCalibrated = 245;

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);
			}
		}
		else if (EventPosted & FESTO_EVENT_EJECTOR_FINISHED)
		{
			if (FestoState == 6)
			{
				Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
				Clock_start(myClock);
			}
			else if (FestoState == 7)
			{
				FestoState = 1;
			}
			else
			{
				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				Clock_start(myClock);
				FestoState = 7;
			}
		}
		else if (EventPosted & FESTO_EVENT_TICK)
		{
			if (Running)
			{
				Time1 = Clock_getTicks();
				Uptime += (Time1 - Time0);
				Time0 = Time1;

				piecesProcessedPerSecond = piecesProcessed/Uptime;

				MessageObject.piecesProcessed = piecesProcessed;
				MessageObject.blackAccepted = blackAccepted;
				MessageObject.blackRejected = blackRejected;
				MessageObject.plasticAccepted = plasticAccepted;
				MessageObject.plasticRejected = plasticRejected;
				MessageObject.orangeAccepted = orangeAccepted;
				MessageObject.orangeRejected = orangeRejected;
				MessageObject.piecesProcessedPerSecond = piecesProcessedPerSecond;
				MessageObject.uptimeSeconds = uptimeSeconds;
				MessageObject.metalAccepted = metallicAccepted;
				MessageObject.metalRejected = metallicRejected;

				Mailbox_post(DisplayMailbox, &MessageObject, BIOS_NO_WAIT);

				System_printf("Tick = %d\n", Uptime);
				System_flush();
			}

		}
		else if (EventPosted & FESTO_EVENT_PIECE_NOT_IN_PLACE)
		{
			if (FestoState <= 4)
			{
				FestoState = 1;
				Festo_Control_Ejector(Driver, FESTO_EJECTOR_RETRACT);
				Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
			}
		}
		else
		{
			if (FestoState == 1)
			{
				if (Festo_Sense_Piece_Placed(Driver) == 1)
				{
					FestoState = 2;
					if (Festo_Sense_Riser_Down(Driver) == 0)
					{
						Festo_Control_Platform(Driver, FESTO_PLATFORM_LOWER);
					}
					else
					{
					    Event_post(FestoEvents, FESTO_EVENT_RISER_DOWN);
					}
				}
			}
		}
		Task_sleep(100);
	}
}

//*****************************************************************************
//
//! Festo Ejector Callback.
//!
//! This function is executed 1000ms after the ajector is actiavated.
//!
//! \return None.
//
//*****************************************************************************
void _Festo_Deactivate_Ejector(UArg arg0)
{
	Event_post(FestoEvents, FESTO_EVENT_EJECTOR_FINISHED);
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

	UInt32 t;
	time_t t1;
	struct tm *ltm;
	char *curTime;
	Seconds_set(1432690200);
	t = Seconds_get();
	/*
	 * Use overridden time() function to get the current time.
	 * Use standard C RTS library functions with return from time().
	 * Assumes Seconds_set() has been called as above
	 */
	t1 = time(NULL);
	ltm = localtime(&t1);
	curTime = asctime(ltm);
	System_printf("Time(GMT): %s\n", curTime);


	while(1)
	{
		EventPosted = Event_pend(FestoEvents,
						Event_Id_NONE,
						FESTO_EVENT_ADC_START,
						0);

		if (EventPosted & FESTO_EVENT_ADC_START)
		{
			ADCProcessorTrigger(ADC0_BASE, 0);
		}
		else
		{
			if (ADCIntStatus(ADC0_BASE, 0, false))
			{
				// ADC reading complete
				ADCSequenceDataGet(ADC0_BASE, 0, &AdcDataRaw);
				ADCIntClear(ADC0_BASE, 0);
				Mailbox_post(ADCMailbox, &AdcDataRaw, BIOS_NO_WAIT);
			    System_printf("ADC data: %d\n", AdcDataRaw);
			    System_flush();
			}
		}
		Task_sleep(100);
	}
}

//*****************************************************************************
//
//! Clock 1 sec Callback.
//!
//! This function is executed every one second.
//!
//! \return None.
//
//*****************************************************************************
void _clock_1_sec(void)
{
	Event_post(FestoEvents, FESTO_EVENT_TICK);
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
//! Festo Piece Callback.
//!
//! This function is executed when the piece enter the Festo
//! Station platform.
//!
//! \return None.
//
//*****************************************************************************
void _callback_Festo_Piece_In(void)
{
	GPIO_clearInt(Board_SENSE_SAMPLE_IN_PLACE);
 	if (GPIO_read(Board_SENSE_SAMPLE_IN_PLACE) > 0)
	{
 		Event_post(FestoEvents, FESTO_EVENT_PIECE_IN_PLACE);
	}
	else
	{
		Event_post(FestoEvents, FESTO_EVENT_PIECE_NOT_IN_PLACE);
	}
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
    GPIO_setCallback(Board_SENSE_SAMPLE_IN_PLACE, _callback_Festo_Piece_In);

    // Enable Interrupts
    GPIO_enableInt(Board_BUTTON0);
    GPIO_enableInt(Board_BUTTON1);
    GPIO_enableInt(Board_BUTTON2);
    GPIO_enableInt(Board_SENSE_RISER_DOWN);
    GPIO_enableInt(Board_SENSE_RISER_UP);
    GPIO_enableInt(Board_SENSE_SAMPLE_IN_PLACE);

    // Start BIOS
    BIOS_start();

    return (0);
}
