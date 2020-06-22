#ifndef ANCHOR_MANAGER_H_
#define ANCHOR_MANAGER_H_

/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdbool.h>
#include <stdint.h>

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define ANCHOR_COUNT			32
#define ANCHOR_NAME_SIZE		16

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef struct anchor
{
	char		name[ANCHOR_NAME_SIZE];
	uint16_t	index;
} anchor_t;

/****************************************************************
 * Function declarations
 ****************************************************************/
bool Anchor_Init();

bool Anchor_Create
(
	anchor_t anchorData,
	uint8_t* out_index
);

bool Anchor_Delete
(
	const uint8_t	anchorIndex
);

void Anchor_DeleteAll(void);

bool Anchor_Update
(
	const uint8_t 	anchorIndex,
	const anchor_t	newAnchorData
);

const anchor_t * Anchor_GetData
(
	const uint8_t	anchorIndex
);

const anchor_t * Anchor_GetDataByName
(
	const char *	anchorName
);

#endif /* ANCHOR_MANAGER_H_ */
