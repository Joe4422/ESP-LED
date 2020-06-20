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
#define REGION_FORMAT_VERSION	1
#define REGION_COUNT			16

#define ANCHOR_COUNT			32
#define ANCHOR_NAME_SIZE		16

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

typedef struct anchor
{
	char		name[ANCHOR_NAME_SIZE];
	uint16_t	index;
} anchor_t;

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

void Region_DeleteAll();

bool Region_Update
(
	const uint8_t 	regionIndex,
	const region_t	newRegionData
);

const region_t * Region_GetData
(
	const uint8_t	regionIndex
);

void Region_GetUsedIndexes
(
	bool	indexUsed[]
);

bool Region_AddAnchor(anchor_t anchor);

const anchor_t * Region_GetAnchor(char * name);

bool Region_DeleteAnchor(char * name);

void Region_DeleteAllAnchors();

#endif /* REGION_MANAGER_H_ */
