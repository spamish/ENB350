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

#define FESTO_DISABLED 0
#define FESTO_ENABLED 1

#define FESTO_PLATFORM_RAISE 0
#define FESTO_PLATFORM_LOWER 1

#define FESTO_EJECTOR_EXTEND 0
#define FESTO_EJECTOR_RETRACT 1

#define FESTO_PIECE_METALLIC 0
#define FESTO_PIECE_OTHER 1

#define FESTO_COLOR_ORANGE 0
#define FESTO_COLOR_OTHER 1

typedef struct FestoStationDriver
{
	// calculated values
	uint8_t	Piece_sensor;
	uint8_t	Color_sensor;
	uint8_t	Material_sensor;
	uint8_t	Height_sensor;

	// raw values
	uint8_t	Piece_raw;
	uint8_t	Color_raw;
	uint8_t	Material_raw;
	uint8_t	Height_raw;

	// state variables
	uint8_t Driver_state;
	uint8_t Piece_state;
	uint8_t Platform_state;
	uint8_t Ejector_state;

} FestoStationDriver;

uint8_t Control_Platform(FestoStationDriver* Driver, uint8_t option);
uint8_t Control_Ejector(FestoStationDriver* Driver, uint8_t option);
uint8_t Control_Driver(FestoStationDriver* Driver, uint8_t option);

uint8_t Init_FestoStationDriver(FestoStationDriver* Driver)
{
	Driver->Piece_sensor = 0;
	Driver->Color_sensor = 0;
	Driver->Material_sensor = 0;
	Driver->Height_sensor = 0;

	// raw values
	Driver->Piece_raw = 0;
	Driver->Color_raw = 0;
	Driver->Material_raw = 0;
	Driver->Height_raw = 0;

	// state variables
	Driver->Driver_state = FESTO_DISABLED;
	Driver->Piece_state = 0;
	Driver->Platform_state = 0;
	Driver->Ejector_state = 0;
	return 0;
}

uint8_t Control_Platform(FestoStationDriver* Driver, uint8_t option)
{
	if (Driver->Driver_state == FESTO_DISABLED)
	{
		return 1;
	}

	if (option == FESTO_PLATFORM_RAISE)
	{
		// raise platform
		// check if the platform it is not raised yet
	}
	else if (option == FESTO_PLATFORM_LOWER)
	{
		// lower platform
		// check if the platform it is not lowered yet
	}
	else
	{
		return 1;
	}

	return 0;
}
uint8_t Control_Ejector(FestoStationDriver* Driver, uint8_t option)
{
	if (Driver->Driver_state == FESTO_DISABLED)
	{
		return 1;
	}

	if (option == FESTO_EJECTOR_EXTEND)
	{
		// extend ejector
		// check if the ejector is not extended yet
	}
	else if (option == FESTO_EJECTOR_RETRACT)
	{
		// retract ejector
		// check if the ejector is not retracted yet
	}
	else
	{
		return 1;
	}

	return 0;
}

uint8_t Control_Driver(FestoStationDriver* Driver, uint8_t option)
{
	if (option == FESTO_DISABLED)
	{
		Driver->Driver_state = FESTO_DISABLED;
	}
	else if (option == FESTO_ENABLED)
	{
		Driver->Driver_state = FESTO_ENABLED;
	}
	else
	{
		return 1;
	}
	return 0;
}

#endif