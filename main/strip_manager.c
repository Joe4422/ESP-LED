/****************************************************************
 * Includes
 ****************************************************************/
#include "strip_manager.h"

// cstdlib includes
#include <stdlib.h>

// Project includes
#include "ws2812_control.h"
#include "storage_manager.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define SERIAL_RED_OFFSET 	(0)
#define SERIAL_GREEN_OFFSET	(16)
#define SERIAL_BLUE_OFFSET	(8)

/****************************************************************
 * Function declarations
 ****************************************************************/
uint32_t Strip_SerialiseColour(colour_t colour);
colour_t Strip_DeserialiseColour(uint32_t colour);

/****************************************************************
 * Local variables
 ****************************************************************/
struct led_state currentState;

/****************************************************************
 * Function definitions
 ****************************************************************/
void Strip_Init(void)
{
	// Initialise WS2812 controller
	ws2812_control_init();

	// Turn off all LEDs
	Strip_Buffer_SetAll(COLOUR_OFF);
	Strip_Buffer_Push();
}

void Strip_Buffer_Push(void)
{
	ws2812_write_leds(currentState);
}

void Strip_Buffer_SetAll
(
	colour_t colour
)
{
	Strip_Buffer_SetArea(0, NUM_LEDS - 1, colour);
}

void Strip_Buffer_SetArea
(
	uint16_t	startIndex,
	uint16_t	endIndex,
	colour_t	colour
)
{
	int i;

	// Set each LED in the area to the given colour
	for (i = startIndex; i <= endIndex; i++)
	{
		currentState.leds[i] = Strip_SerialiseColour(colour);
	}
}

void Strip_Buffer_SetOne
(
	uint16_t	index,
	colour_t	colour
)
{
	if (index >= NUM_LEDS) return;

	currentState.leds[index] = Strip_SerialiseColour(colour);
}

colour_t Strip_Buffer_GetOne
(
	uint16_t	index
)
{
	// Out-of-range LEDs are always off
	if (index >= NUM_LEDS) return COLOUR_OFF;

	return Strip_DeserialiseColour(currentState.leds[index]);
}

uint32_t Strip_SerialiseColour
(
	colour_t colour
)
{
	// Outputs as 24-bit integer with highest 8 bits all 0; format is GRB
	return (colour.green << SERIAL_GREEN_OFFSET) | (colour.red << SERIAL_BLUE_OFFSET) | (colour.blue << SERIAL_RED_OFFSET);
}

colour_t Strip_DeserialiseColour
(
	uint32_t colour
)
{
	return (colour_t){ (colour >> SERIAL_BLUE_OFFSET) & 0xFF, (colour >> SERIAL_GREEN_OFFSET) & 0xFF, (colour >> SERIAL_RED_OFFSET) & 0xFF };
}
