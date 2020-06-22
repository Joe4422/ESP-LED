#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdbool.h>

// Project includes
#include "region_manager.h"
#include "anchor_manager.h"

/****************************************************************
 * Function declarations
 ****************************************************************/
bool Storage_Init();

bool Storage_ReadRegions(region_t ** regions);
bool Storage_WriteRegions(region_t ** regions);

bool Storage_ReadAnchors(anchor_t ** anchors);
bool Storage_WriteAnchors(anchor_t ** anchors);

#endif /* STORAGE_MANAGER_H_ */
