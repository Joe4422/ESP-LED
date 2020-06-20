#ifndef COLOUR_H_
#define COLOUR_H_

/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdint.h>

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef struct colour
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} colour_t;

/****************************************************************
 * Defines, consts
 ****************************************************************/
extern const colour_t COLOUR_RED;
extern const colour_t COLOUR_GREEN;
extern const colour_t COLOUR_BLUE;
extern const colour_t COLOUR_CYAN;
extern const colour_t COLOUR_MAGENTA;
extern const colour_t COLOUR_YELLOW;
extern const colour_t COLOUR_WHITE;
extern const colour_t COLOUR_OFF;

#endif /* COLOUR_H_ */
