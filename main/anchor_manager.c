/****************************************************************
 * Includes
 ****************************************************************/
#include "anchor_manager.h"

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
anchor_t * anchors[ANCHOR_COUNT];

const char * ANCHOR_LOG_TAG = "Anchor Manager";

/****************************************************************
 * Function definitions
 ****************************************************************/
bool Anchor_Init()
{
	// Attempt to load anchors from storage
	if (Storage_ReadAnchors(anchors) == false)
	{
		// Anchor load failed; return false
		ESP_LOGE(ANCHOR_LOG_TAG, "Anchor load failed!");
		return false;
	}

	return true;
}

bool Anchor_Create
(
	anchor_t anchorData,
	uint8_t* out_index
)
{
	uint8_t anchorSlot;
	bool foundEmptyAnchor = false;

	// Try to find empty anchor
	for (anchorSlot = 0; anchorSlot < ANCHOR_COUNT; anchorSlot++)
	{
		// Check if anchor is empty
		if (anchors[anchorSlot] == NULL)
		{
			// Anchor is empty - we can use it
			foundEmptyAnchor = true;
			break;
		}
	}

	// Check if we found an empty anchor
	if (foundEmptyAnchor == false)
	{
		// No free anchors so anchor creation fails - return false
		return false;
	}
	else
	{
		// Anchor is free - malloc it then copy data over
		anchors[anchorSlot] = malloc(sizeof(anchor_t));
		memcpy(anchors[anchorSlot], &anchorData, sizeof(anchor_t));

		// Write change to storage
		Storage_WriteAnchors(anchors);

		*out_index = anchorSlot;

		return true;
	}
}

bool Anchor_Delete
(
	const uint8_t	anchorIndex
)
{
	// Check if anchor index is out of range
	if (anchorIndex >= ANCHOR_COUNT)
	{
		return false;
	}
	// Check if region to delete is already NULL
	else if (anchors[anchorIndex] == NULL)
	{
		return false;
	}
	else
	{
		// Delete anchor
		free(anchors[anchorIndex]);
		anchors[anchorIndex] = NULL;

		// Write change to storage
		Storage_WriteAnchors(anchors);

		return true;
	}
}

void Anchor_DeleteAll(void)
{
	int i;

	for (i = 0; i < ANCHOR_COUNT; i++)
	{
		// Check if index has anchor present
		if (anchors[i] != NULL)
		{
			// Anchor is present, so delete it
			free(anchors[i]);
			anchors[i] = NULL;
		}
	}

	// Write change to storage
	Storage_WriteAnchors(anchors);
}

bool Anchor_Update
(
	const uint8_t 	anchorIndex,
	const anchor_t	newAnchorData
)
{
	// Check if region index is out of range
	if (anchorIndex >= ANCHOR_COUNT)
	{
		return false;
	}
	// Check if region to update is already NULL
	else if (anchors[anchorIndex] == NULL)
	{
		return false;
	}
	else
	{
		// Copy new region data to index
		memcpy(anchors[anchorIndex], &newAnchorData, sizeof(anchor_t));

		// Write change to storage
		Storage_WriteAnchors(anchors);

		return true;
	}
}

const anchor_t * Anchor_GetData
(
	const uint8_t	anchorIndex
)
{
	// Check if region index is out of range
	if (anchorIndex >= ANCHOR_COUNT)
	{
		return NULL;
	}
	else
	{
		return anchors[anchorIndex];
	}
}

const anchor_t * Anchor_GetDataByName
(
	const char *	anchorName
)
{
	uint8_t anchorSlot;

	for (anchorSlot = 0; anchorSlot < ANCHOR_COUNT; anchorSlot++)
	{
		if (strcmp(anchors[anchorSlot]->name, anchorName) == 0)
		{
			return anchors[anchorSlot];
		}
	}

	return NULL;
}
