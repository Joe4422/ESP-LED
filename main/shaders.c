/****************************************************************
 * Includes
 ****************************************************************/
#include "shaders.h"

// Project includes
#include "strip_manager.h"

/****************************************************************
 * Function declarations
 ****************************************************************/
void func_shader_default(const region_t * region);
void func_shader_ping(const region_t * region);
void func_shader_rainbow(const region_t * region);
void func_shader_rainbow2(const region_t * region);

void func_init_null(const region_t * region);
void func_init_clear(const region_t * region);

/****************************************************************
 * Defines, consts
 ****************************************************************/
const shader_t shader_default =
{
	func_init_null,
	func_shader_default,
	"default"
};

const shader_t shader_ping =
{
	func_init_clear,
	func_shader_ping,
	"ping"
};

const shader_t shader_rainbow =
{
	func_init_clear,
	func_shader_rainbow,
	"rainbow"
};

const shader_t shader_rainbow2 =
{
	func_init_clear,
	func_shader_rainbow2,
	"rainbow2"
};

const shader_t * SHADERS[] = { &shader_default, &shader_ping, &shader_rainbow, &shader_rainbow2 };

#define SHADER_DEFAULT_DELTA_PARAM	32

/****************************************************************
 * Function definitions
 ****************************************************************/
void func_init_null(const region_t * region) { }

void func_init_clear(const region_t * region)
{
	Strip_Buffer_SetArea(region->start, region->end, COLOUR_OFF);
}

void func_shader_default(const region_t * region)
{
	colour_t currentColour = Strip_Buffer_GetOne(region->start);
	if (currentColour.red < region->colour.red)
	{
		uint8_t delta = (region->colour.red - currentColour.red) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.red += delta;
	}
	else if (currentColour.red > region->colour.red)
	{
		uint8_t delta = (currentColour.red - region->colour.red) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.red -= delta;
	}
	if (currentColour.green < region->colour.green)
	{
		uint8_t delta = (region->colour.green - currentColour.green) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.green += delta;
	}
	else if (currentColour.green > region->colour.green)
	{
		uint8_t delta = (currentColour.green - region->colour.green) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.green -= delta;
	}
	if (currentColour.blue < region->colour.blue)
	{
		uint8_t delta = (region->colour.blue - currentColour.blue) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.blue += delta;
	}
	else if (currentColour.blue > region->colour.blue)
	{
		uint8_t delta = (currentColour.blue - region->colour.blue) / SHADER_DEFAULT_DELTA_PARAM;
		if (delta == 0) delta = 1;
		currentColour.blue -= delta;
	}
	Strip_Buffer_SetArea(region->start, region->end, currentColour);
}

void func_shader_ping(const region_t * region)
{
	int ledIndex = 0;
	uint8_t foundLed = 0;
	for (ledIndex = region->start; ledIndex <= region->end; ledIndex++)
	{
		colour_t col = Strip_Buffer_GetOne(ledIndex);

		if (col.red == 0 && col.green == 0 && col.blue == 0)
		{
			continue;
		}
		else
		{
			foundLed = 1;
			break;
		}
	}

	if (foundLed == 0)
	{
		Strip_Buffer_SetOne(region->start, region->colour);
	}

	if (ledIndex == region->end)
	{
		Strip_Buffer_SetOne(ledIndex, COLOUR_OFF);
		Strip_Buffer_SetOne(region->start, region->colour);
	}
	else
	{
		Strip_Buffer_SetOne(ledIndex, COLOUR_OFF);
		Strip_Buffer_SetOne(ledIndex + 1, region->colour);
	}
}

void func_shader_rainbow(const region_t * region)
{
	int ledIndex;
	for (ledIndex = region->start; ledIndex <= region->end; ledIndex++)
	{
		colour_t col = Strip_Buffer_GetOne(ledIndex);

		if (col.red == COLOUR_RED.red && col.green == COLOUR_RED.green && col.blue == COLOUR_RED.blue)
		{
			col = COLOUR_GREEN;
		}
		else if (col.red == COLOUR_GREEN.red && col.green == COLOUR_GREEN.green && col.blue == COLOUR_GREEN.blue)
		{
			col = COLOUR_BLUE;
		}
		else if (col.red == COLOUR_BLUE.red && col.green == COLOUR_BLUE.green && col.blue == COLOUR_BLUE.blue)
		{
			col = COLOUR_RED;
		}
		else
		{
			col = COLOUR_RED;
		}
		Strip_Buffer_SetOne(ledIndex, col);
	}
}

void func_shader_rainbow2(const region_t * region)
{
	int ledIndex;
	for (ledIndex = region->start; ledIndex <= region->end; ledIndex++)
	{
		colour_t col = Strip_Buffer_GetOne(ledIndex);

		if (col.red == COLOUR_RED.red && col.green == COLOUR_RED.green && col.blue == COLOUR_RED.blue)
		{
			col = COLOUR_GREEN;
		}
		else if (col.red == COLOUR_GREEN.red && col.green == COLOUR_GREEN.green && col.blue == COLOUR_GREEN.blue)
		{
			col = COLOUR_BLUE;
		}
		else if (col.red == COLOUR_BLUE.red && col.green == COLOUR_BLUE.green && col.blue == COLOUR_BLUE.blue)
		{
			col = COLOUR_RED;
		}
		else
		{
			switch (ledIndex % 3)
			{
			case 0:
				col = COLOUR_RED;
				break;
			case 1:
				col = COLOUR_GREEN;
				break;
			case 2:
				col = COLOUR_BLUE;
				break;
			default:
				break;
			}
		}
		Strip_Buffer_SetOne(ledIndex, col);
	}
}
