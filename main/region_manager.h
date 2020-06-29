#ifndef REGION_MANAGER_H_
#define REGION_MANAGER_H_

/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdbool.h>
#include <stdint.h>

// Project includes
#include "colour.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define REGION_FORMAT_VERSION	0
#define REGION_COUNT			16

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef struct region
{
	uint16_t	start;
	uint16_t	end;
	uint8_t		shaderIndex;
	colour_t	colour;
} region_t;

/****************************************************************
 * Function declarations
 ****************************************************************/
bool Region_Init();

bool Region_Create
(
	const region_t	regionData,
	uint8_t *		out_index
);

bool Region_Delete
(
	const uint8_t	regionIndex
);

void Region_DeleteAll(void);

bool Region_Update
(
	const uint8_t 	regionIndex,
	const region_t	newRegionData
);

const region_t * Region_GetData
(
	const uint8_t	regionIndex
);

#endif /* REGION_MANAGER_H_ */
