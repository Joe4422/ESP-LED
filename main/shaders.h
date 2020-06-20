#ifndef SHADERS_H_
#define SHADERS_H_

/****************************************************************
 * Includes
 ****************************************************************/
#include "colour.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define SHADER_DEFAULT	0
#define SHADER_PING		1
#define SHADER_RAINBOW	2
#define SHADER_RAINBOW2	3

#define NUM_SHADERS 4

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef struct shader
{
	void (* nextFrame)(uint16_t regionStart, uint16_t regionEnd, colour_t colour);
	char *  name;
} shader_t;

/****************************************************************
 * Global variables
 ****************************************************************/
extern const shader_t * SHADERS[];

#endif /* SHADERS_H_ */
