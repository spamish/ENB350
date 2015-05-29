/**
    ENB350 - Assignment 1 - Festo Station
    FestoStationDriver.h
    Description: Provides basic functionality of Festo Station

    @author Igor Ivan Gaudeda
    @version 1.0 01/02/2015

a. Initialize and configure data structures
b. Raise or lower the platform that holds the piece
c. Extend or retract the ejector
d. Bring the platform to its initial position
e. Enable or disable movements
f. Sense a work piece in place or not
g. Return a colour and material measurement
h. Return a height measurement

1 2Y1 O-1 Lifting Cylinder: Lower
2 2Y2 O-2 Lifting Cylinder: Raised
3 2Y3 O-3 Ejecting Cylinder:Extend
13 2B5 I-0 Workpiece is: Metallic
14 2B6 I-1 Workpiece is: In Lifting Device
15 2B7 I-2 Workpiece is: Orange or Metallic
16 2B2 I-3 Lifting Cylinder is: Lowered
17 2B1 I-4 Lifting Cylinder is: Raised
18 2B3 I-5 Ejecting Cylinder is: Retracted
19 2B4 I-6 Ejecting Cylinder is: Extended

*/
#ifndef FESTOSTATIONDRIVER_H
#define FESTOSTATIONDRIVER_H

// include Board Header
#include "Board.h"

// define constants to control the driver
#define FESTO_DISABLED 0
#define FESTO_ENABLED 1

#define FESTO_PLATFORM_RAISE 0
#define FESTO_PLATFORM_LOWER 1

#define FESTO_EJECTOR_EXTEND 0
#define FESTO_EJECTOR_RETRACT 1

// sensor pin
#define FESTO_PIECE_METALLIC 1
#define FESTO_PIECE_OTHER 0

// sensor pin
#define FESTO_COLOR_ORANGE 1
#define FESTO_COLOR_OTHER 0

// sensor pin
#define FESTO_PIECE_PLACED 1
#define FESTO_PIECE_MISSING 0

#define FESTO_MEASURE_DOWN 0
#define FESTO_MEASURE_UP 1

#define FESTO_TIMEOUT 10

#define FESTO_EVENT_BUTTON_UP 		Event_Id_00
#define FESTO_EVENT_BUTTON_DOWN 	Event_Id_01
#define FESTO_EVENT_BUTTON_SELECT	Event_Id_02
#define FESTO_EVENT_RISER_DOWN		Event_Id_03
#define FESTO_EVENT_RISER_UP 		Event_Id_04
#define FESTO_EVENT_ADC_START		Event_Id_05
#define FESTO_EVENT_ADC_FINISH		Event_Id_06
#define FESTO_EVENT_PIECE_IN_PLACE	Event_Id_07
#define FESTO_EVENT_EJECTOR_FINISHED Event_Id_08
#define FESTO_EVENT_TICK			Event_Id_09
#define FESTO_EVENT_COOLDOWN		Event_Id_10
#define FESTO_EVENT_PIECE_NOT_IN_PLACE	Event_Id_11

extern uint32_t GPIO_read(unsigned int index);
extern void 	GPIO_toggle(unsigned int index);
extern void 	GPIO_write(unsigned int index, uint32_t value);


typedef struct FestoStationDriver
{
	// calculated values
	uint8_t	Piece_sensor;
	uint8_t	Colour_sensor;
	uint8_t	Material_sensor;
	uint32_t	Height_sensor;

	// state variables
	uint8_t Driver_state;
	uint8_t Piece_state;
	uint8_t Platform_state;
	uint8_t Ejector_state;

} FestoStationDriver;

uint8_t Festo_Control_Platform(FestoStationDriver* Driver, uint8_t option);
uint8_t Festo_Control_Ejector(FestoStationDriver* Driver, uint8_t option);
uint8_t Festo_Control_Driver(FestoStationDriver* Driver, uint8_t option);
uint8_t Festo_Control_Measure(FestoStationDriver* Driver, uint8_t option);
uint8_t	Festo_Sense_Piece_Placed(FestoStationDriver* Driver);
uint8_t	Festo_Sense_Piece_Colour(FestoStationDriver* Driver);
uint8_t	Festo_Sense_Piece_Material(FestoStationDriver* Driver);
uint32_t	Festo_Sense_Piece_Height(FestoStationDriver* Driver);
void	Festo_Sense_Set_Piece_Height(FestoStationDriver* Driver, uint32_t height);
uint8_t	Festo_Sense_Riser_Down(FestoStationDriver* Driver);
uint8_t	Festo_Sense_Riser_Up(FestoStationDriver* Driver);
uint8_t	Festo_Sense_Ejector_Ready(FestoStationDriver* Driver);
uint8_t	Festo_Sense_Measure_Down(FestoStationDriver* Driver);

uint8_t Festo_Driver_Init(FestoStationDriver* Driver)
{
	Driver->Piece_sensor = 0;
	Driver->Colour_sensor = 0;
	Driver->Material_sensor = 0;
	Driver->Height_sensor = 0;

	// state variables
	Driver->Driver_state = FESTO_DISABLED;
	Driver->Piece_state = 0;
	Driver->Platform_state = 0;
	Driver->Ejector_state = 0;

	GPIO_write(Board_ACTUATOR_RISER_DOWN, 0);
	GPIO_write(Board_ACTUATOR_RISER_UP, 0);
	GPIO_write(Board_ACTUATOR_EJECTOR, 0);
	GPIO_write(Board_ACTUATOR_MEASURE_DOWN, 0);

	return 0;
}
// Raise or lower the platform
uint8_t Festo_Control_Platform(FestoStationDriver* Driver, uint8_t option)
{
	// Skip if driver disabled
	if (Driver->Driver_state == FESTO_DISABLED)
	{
		return 1;
	}
	// Raise platform
	if (option == FESTO_PLATFORM_RAISE)
	{
		GPIO_write(Board_ACTUATOR_RISER_DOWN, 0);
		GPIO_write(Board_ACTUATOR_RISER_UP, 0xFFFF);
	} // Lower platform
	else if (option == FESTO_PLATFORM_LOWER)
	{
		GPIO_write(Board_ACTUATOR_RISER_UP, 0);
		GPIO_write(Board_ACTUATOR_RISER_DOWN, 0xFFFF);
	} // Skip if unavailable
	else
	{
		return 1;
	}

	return 0;
}
// Extend or retract ejector
uint8_t Festo_Control_Ejector(FestoStationDriver* Driver, uint8_t option)
{
	// Skip if disabled
	if (Driver->Driver_state == FESTO_DISABLED)
	{
		return 1;
	}
	// Extend ejector
	if (option == FESTO_EJECTOR_EXTEND)
	{
		GPIO_write(Board_ACTUATOR_EJECTOR, 0xFFFF);
	} // Retract ejector
	else if (option == FESTO_EJECTOR_RETRACT)
	{
		GPIO_write(Board_ACTUATOR_EJECTOR, 0);
	} // Skip if unavailable
	else
	{
		return 1;
	}

	return 0;
}
// Control driver state
uint8_t Festo_Control_Driver(FestoStationDriver* Driver, uint8_t option)
{
	// Disable driver
	if (option == FESTO_DISABLED)
	{
		Driver->Driver_state = FESTO_DISABLED;
	} // Enable driver
	else if (option == FESTO_ENABLED)
	{
		Driver->Driver_state = FESTO_ENABLED;
	} // Skip if unavailable
	else
	{
		return 1;
	}

	return 0;
}
// 
uint8_t Festo_Control_Measure(FestoStationDriver* Driver, uint8_t option)
{
	// Skip if disabled
	if (Driver->Driver_state == FESTO_DISABLED)
	{
		return 1;
	}
	// Set from maximum measure state
	if (option == FESTO_MEASURE_DOWN)
	{
		GPIO_write(Board_ACTUATOR_MEASURE_DOWN, 0xFFFF);
	} // Set from minimum measure state
	else if (option == FESTO_MEASURE_UP)
	{
		GPIO_write(Board_ACTUATOR_MEASURE_DOWN, 0);
	} // Skip if unavailable
	else
	{
		return 1;
	}

	return 0;
}
// Detect if piece is placed on platform
uint8_t	Festo_Sense_Piece_Placed(FestoStationDriver* Driver)
{
	Driver->Piece_sensor = (GPIO_read(Board_SENSE_SAMPLE_IN_PLACE) > 0);
	return Driver->Piece_sensor;
}
// Detect colour of piece
uint8_t	Festo_Sense_Piece_Colour(FestoStationDriver* Driver)
{
	Driver->Colour_sensor = (GPIO_read(Board_SENSE_SAMPLE_COLOUR) > 0);
	return Driver->Colour_sensor;
}
// Detect material of piece
uint8_t	Festo_Sense_Piece_Material(FestoStationDriver* Driver)
{
	Driver->Material_sensor = (GPIO_read(Board_SENSE_SAMPLE_METALLIC) > 0);
	return Driver->Material_sensor;
}
// Detect height of piece
uint32_t	Festo_Sense_Piece_Height(FestoStationDriver* Driver)
{
	return Driver->Height_sensor;
}
// Set height value of piece (used for relative height)
void Festo_Sense_Set_Piece_Height(FestoStationDriver* Driver, uint32_t height)
{
	Driver->Height_sensor = height;
	return;
}
// Detect if platform is lowered
uint8_t	Festo_Sense_Riser_Down(FestoStationDriver* Driver)
{
	return (GPIO_read(Board_SENSE_RISER_DOWN) > 0);
}
// Detect if platform is raised
uint8_t	Festo_Sense_Riser_Up(FestoStationDriver* Driver)
{
	return (GPIO_read(Board_SENSE_RISER_UP) > 0);
}
// Detect if ejector is retracted
uint8_t	Festo_Sense_Ejector_Ready(FestoStationDriver* Driver)
{
	return (GPIO_read(Board_SENSE_EJECTOR_READY) > 0);
}
// Detect if height measure is down
uint8_t	Festo_Sense_Measure_Down(FestoStationDriver* Driver)
{
	return (GPIO_read(Board_SENSE_MEASURE_DOWN) > 0);
}

#endif
