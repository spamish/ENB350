/******************************************************************************
 *
 * main.c - The main file of the festo testing station project.
 *
 * Part of the ENB350 Problem Based Learning Assignment.
 *
 * Authors:
 *  Homero Maciel     -
 *  Igor Ivan Gaudeda -
 *  Samuel Janetzki   - n7402643
 *  Sam Pourzinal     -
 *
 *  The application runs as the processing side of an automated testing
 *  station, where work pieces arrive, are measured and pieces are sorted into
 *  accept and reject bins.
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "drivers/frame.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "drivers/pinout.h"
#include "drivers/sound.h"
#include "drivers/touch.h"

/******************************************************************************
 *
 * Initalise the inputs, outputs, graphics and then start the tasks of the
 * application.
 *
 *****************************************************************************/
int main(void)
{

	// Set system clocks.

	// Configure device pins.

	// Initialise display driver.

	// Initialise the graphics context.

	// Create a semaphore object.

	// Create tasks.

	while(1)
	{
	}
}

/******************************************************************************
 *
 * Operate inputs and outputs.
 *
 *****************************************************************************/
void task1()
{

}

/******************************************************************************
 *
 * Operate graphics.
 *
 *****************************************************************************/
void task2()
{

}
