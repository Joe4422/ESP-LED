/****************************************************************
 * Includes
 ****************************************************************/
#include "shaders.h"

// Project includes
#include "strip_manager.h"

/****************************************************************
 * Function declarations
 ****************************************************************/
void func_shader_default(uint16_t regionStart, uint16_t regionEnd, colour_t colour);
void func_shader_ping(uint16_t regionStart, uint16_t regionEnd, colour_t colour);
void func_shader_rainbow(uint16_t regionStart, uint16_t regionEnd, colour_t colour);
void func_shader_rainbow2(uint16_t regionStart, uint16_t regionEnd, colour_t colour);

/****************************************************************
 * Defines, consts
 ****************************************************************/
const shader_t shader_default =
{
	func_shader_default,
	"default"
};

const shader_t shader_ping =
{
	func_shader_ping,
	"ping"
};

const shader_t shader_rainbow =
{
	func_shader_rainbow,
	"rainbow"
};

const shader_t shader_rainbow2 =
{
	func_shader_rainbow2,
	"rainbow2"
};

const shader_t * SHADERS[] = { &shader_default, &shader_ping, &shader_rainbow, &shader_rainbow2 };

/****************************************************************
 * Function definitions
 ****************************************************************/
void func_shader_default(uint16_t regionStart, uint16_t regionEnd, colour_t colour)
{
	Strip_Buffer_SetArea(regionStart, regionEnd, colour);
}

void func_shader_ping(uint16_t regionStart, uint16_t regionEnd, colour_t colour)
{
	int ledIndex = 0;
	uint8_t foundLed = 0;
	for (ledIndex = regionStart; ledIndex <= regionEnd; ledIndex++)
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
		Strip_Buffer_SetOne(regionStart, colour);
	}

	if (ledIndex == regionEnd)
	{
		Strip_Buffer_SetOne(ledIndex, COLOUR_OFF);
		Strip_Buffer_SetOne(regionStart, colour);
	}
	else
	{
		Strip_Buffer_SetOne(ledIndex, COLOUR_OFF);
		Strip_Buffer_SetOne(ledIndex + 1, colour);
	}
}

void func_shader_rainbow(uint16_t regionStart, uint16_t regionEnd, colour_t colour)
{
	int ledIndex;
	for (ledIndex = regionStart; ledIndex <= regionEnd; ledIndex++)
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

void func_shader_rainbow2(uint16_t regionStart, uint16_t regionEnd, colour_t colour)
{
	int ledIndex;
	for (ledIndex = regionStart; ledIndex <= regionEnd; ledIndex++)
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
