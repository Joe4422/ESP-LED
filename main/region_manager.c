/****************************************************************
 * Includes
 ****************************************************************/
#include "region_manager.h"

// cstdlib includes
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// ESP-IDF includes
#include "sdkconfig.h"
#include "esp_log.h"

// Project includes
#include "storage_manager.h"

/****************************************************************
 * Local variables
 ****************************************************************/
region_t * regions[REGION_COUNT];

const char * REGION_LOG_TAG = "Region Manager";

/****************************************************************
 * Function definitions
 ****************************************************************/
bool Region_Init()
{
	// Attempt to load regions from storage
	if (Storage_ReadRegions(regions) == false)
	{
		// Region load failed; return false
		ESP_LOGE(REGION_LOG_TAG, "Region load failed!");
		return false;
	}

	return true;
}

bool Region_Create
(
	const region_t	regionData,
	uint8_t *		out_index
)
{
	uint8_t regionSlot;
	bool foundEmptyRegion = false;

	// Try to find empty region
	for (regionSlot = 0; regionSlot < REGION_COUNT; regionSlot++)
	{
		// Check if region is empty
		if (regions[regionSlot] == NULL)
		{
			// Region is empty - we can use it
			foundEmptyRegion = true;
			break;
		}
	}

	// Check if we found an empty region
	if (foundEmptyRegion == false)
	{
		// No free regions so region creation fails - return false
		return false;
	}
	else
	{
		// Region is free - malloc it then copy data over
		regions[regionSlot] = malloc(sizeof(region_t));
		memcpy(regions[regionSlot], &regionData, sizeof(region_t));

		// Write change to storage
		Storage_WriteRegions(regions);

		*out_index = regionSlot;

		return true;
	}
}

bool Region_Delete
(
	const uint8_t	regionIndex
)
{
	// Check if region index is out of range
	if (regionIndex >= REGION_COUNT)
	{
		return false;
	}
	// Check if region to delete is already NULL
	else if (regions[regionIndex] == NULL)
	{
		return false;
	}
	else
	{
		// Delete region
		free(regions[regionIndex]);
		regions[regionIndex] = NULL;

		// Write change to storage
		Storage_WriteRegions(regions);

		return true;
	}
}

void Region_DeleteAll()
{
	int i;

	for (i = 0; i < REGION_COUNT; i++)
	{
		// Check if index has region present
		if (regions[i] != NULL)
		{
			// Region is present, so delete it
			free(regions[i]);
			regions[i] = NULL;
		}
	}

	// Write change to storage
	Storage_WriteRegions(regions);
}

bool Region_Update
(
	const uint8_t 	regionIndex,
	const region_t	newRegionData
)
{
	// Check if region index is out of range
	if (regionIndex >= REGION_COUNT)
	{
		return false;
	}
	// Check if region to update is already NULL
	else if (regions[regionIndex] == NULL)
	{
		return false;
	}
	else
	{
		// Copy new region data to index
		memcpy(regions[regionIndex], &newRegionData, sizeof(region_t));

		// Write change to storage
		Storage_WriteRegions(regions);

		return true;
	}
}

const region_t * Region_GetData
(
	const uint8_t	regionIndex
)
{
	// Check if region index is out of range
	if (regionIndex >= REGION_COUNT)
	{
		return NULL;
	}
	else
	{
		return regions[regionIndex];
	}
}
