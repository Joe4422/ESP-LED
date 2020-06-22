/****************************************************************
 * Includes
 ****************************************************************/
#include "storage_manager.h"

// cstdlib includes
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// ESP-IDF includes
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "sdkconfig.h"

// Project includes
#include "region_manager.h"
#include "anchor_manager.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
// Size of the region struct array data blob
#define REGION_BLOB_SIZE	(REGION_COUNT * sizeof(region_t))

// Size of the anchor struct array data blob
#define ANCHOR_BLOB_SIZE	(ANCHOR_COUNT * sizeof(anchor_t))

// Logging tag
const char* STORAGE_LOG_TAG =		"Storage Manager";

// Storage keywords
const char* STORAGE_NAMESPACE =		"storage";

const char* KEY__REGIONS_VERSION =	"regions_version";
const char* KEY__REGIONS_BLOB =		"regions_blob";

const char* KEY__ANCHORS_BLOB =		"anchors_blob";

const region_t NULL_REGION =		{ UINT16_MAX, UINT16_MAX, UINT8_MAX, { UINT8_MAX, UINT8_MAX, UINT8_MAX } };

const anchor_t NULL_ANCHOR =		{ "000000000000000", UINT16_MAX };

/****************************************************************
 * Function declarations
 ****************************************************************/
bool ReadGenericBlob
(
	const char *	key,
	const size_t	blob_size,
	void * 			out
);

/****************************************************************
 * Local variables
 ****************************************************************/
nvs_handle_t storage_handle;

bool storage_init_done = false;

/****************************************************************
 * Function definitions
 ****************************************************************/
bool Storage_Init()
{
	ESP_LOGI(STORAGE_LOG_TAG, "Initialising NVS flash...");
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();

    // Check init outcome
    switch (ret)
    {
    case ESP_OK: // Init succeeded normally
    	ESP_LOGI(STORAGE_LOG_TAG, "Initialised NVS flash.");
		break;
    case ESP_ERR_NVS_NO_FREE_PAGES:		// Init failed but flash can be erased + re-initialised
    case ESP_ERR_NVS_NEW_VERSION_FOUND:
		ESP_LOGW(STORAGE_LOG_TAG, "NVS flash init failed! Erasing and trying again...");
		ret = nvs_flash_erase();
		// Check if erase failed
		if (ret != ESP_OK)
		{
			// Erase failed; return false
			ESP_LOGE(STORAGE_LOG_TAG, "NVS flash erase failed!");
			return false;
		}
		// Try and re-init
		ret = nvs_flash_init();
		// Check if re-init failed
		if (ret != ESP_OK)
		{
			// Re-init failed; return false
			ESP_LOGE(STORAGE_LOG_TAG, "NVS flash re-init after erase failed!");
			return false;
		}
		break;
	// Init failed for any other reason
    default:
		ESP_LOGE(STORAGE_LOG_TAG, "NVS flash init failed!");
		return false;
    }

    // Init succeeded normally or after erase + re-init
    storage_init_done = true;
	return true;
}

bool Storage_ReadRegions(region_t ** regions)
{
    esp_err_t ret;

    if (!storage_init_done) return false;

    ESP_LOGI(STORAGE_LOG_TAG, "Beginning to read region data from NVS...");

    // Attempt to open storage
    ret = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    // Check if storage open failed
    if (ret != ESP_OK)
    {
    	// Storage open failed; return false
        ESP_LOGE(STORAGE_LOG_TAG, "Storage failed to open!");
        return false;
    }
    else
    {
    	// Storage open succeeded
    	// Attempt to load region version
        ESP_LOGD(STORAGE_LOG_TAG, "Starting region load...");
        ESP_LOGD(STORAGE_LOG_TAG, "Checking stored region version...");
        uint8_t region_version = REGION_FORMAT_VERSION;
        // Load region version
        ret = nvs_get_u8(storage_handle, KEY__REGIONS_VERSION, &region_version);
        // Check outcome of region version load
        switch (ret)
        {
        // Load succeeds
        case ESP_OK:
        	ESP_LOGI(STORAGE_LOG_TAG, "Regions are version %d.", region_version);
        	break;
        // Load fails because key does not exist
        case ESP_ERR_NVS_NOT_FOUND:
        	// Create key
        	ESP_LOGI(STORAGE_LOG_TAG, "%s not found. Creating with version %d...", KEY__REGIONS_VERSION, region_version);
        	ret = nvs_set_u8(storage_handle, KEY__REGIONS_VERSION, region_version);
        	// Check outcome of key creation
        	if (ret != ESP_OK)
			{
        		// Key creation failed; return false
				ESP_LOGE(STORAGE_LOG_TAG, "Could not create %s!", KEY__REGIONS_VERSION);
				nvs_close(storage_handle);
				return false;
			}
        	break;
        // Load fails for other reason
        default:
        	// Close storage handler and return false
        	ESP_LOGE(STORAGE_LOG_TAG, "Error reading %s!", KEY__REGIONS_VERSION);
            nvs_close(storage_handle);
        	return false;
        }

        // Initialise memory to load region data into
        region_t region_blob[REGION_COUNT];
        int i;
        // Initialise region data as all null region placeholders
        for (i = 0; i < REGION_COUNT; i++)
        {
        	region_blob[i] = NULL_REGION;
        }

        // Check if regions are out of date
        if (region_version != REGION_FORMAT_VERSION)
		{
        	// Regions are out of date; recreate blob with null regions in all slots
			ESP_LOGW(STORAGE_LOG_TAG, "Regions are out of date! Recreating %s...", KEY__REGIONS_BLOB);
			ret = nvs_set_blob(storage_handle, KEY__REGIONS_BLOB, region_blob, REGION_BLOB_SIZE);
			// Check if region recreation succeeds
			if (ret != ESP_OK)
			{
				// Blob creation failed; return false
				ESP_LOGE(STORAGE_LOG_TAG, "Could not recreate %s!", KEY__REGIONS_BLOB);
	            nvs_close(storage_handle);
				return false;
			}
			// Update key
			ret = nvs_set_u8(storage_handle, KEY__REGIONS_VERSION, REGION_FORMAT_VERSION);
			// Check if version write succeeds
			if (ret != ESP_OK)
			{
				// Version write failed; return false
				ESP_LOGE(STORAGE_LOG_TAG, "Could not write updated version number!");
				nvs_close(storage_handle);
				return false;
			}
		}
        else
        {
        	if (ReadGenericBlob(KEY__REGIONS_BLOB, REGION_BLOB_SIZE, region_blob) == false)
        	{
				ESP_LOGE(STORAGE_LOG_TAG, "Error reading %s!", KEY__REGIONS_BLOB);
        		nvs_close(storage_handle);
        		return false;
        	}
        }

        // Populate region_t array passed as parameter
        for (i = 0; i < REGION_COUNT; i++)
        {
        	// Perform null check on region from blob
        	if (memcmp(region_blob + i, &NULL_REGION, sizeof(region_t)) == 0)
        	{
        		// Region is null; assign NULL to array index
        		regions[i] = NULL;
        	}
        	else
        	{
        		// Region is not null; allocate memory to given array index and copy data read from blob to it
        		ESP_LOGI(STORAGE_LOG_TAG, "Attempting to write to index %d.", i);
        		regions[i] = malloc(sizeof(region_t));
        		memcpy(regions[i], &(region_blob[i]), sizeof(region_t));
        	}
        }

        ESP_LOGI(STORAGE_LOG_TAG, "Attempting to commit NVS data...");
        // Attempt to commit changes made to storage
        ret = nvs_commit(storage_handle);
        // Check if commit succeeded
        if (ret != ESP_OK)
        {
        	// Commit failed; return false
        	ESP_LOGE(STORAGE_LOG_TAG, "Error committing NVS data!");
        	nvs_close(storage_handle);
        	return false;
        }
        else
        {
        	// Commit succeeded; return true
        	ESP_LOGI(STORAGE_LOG_TAG, "Region data read from NVS complete.");
			nvs_close(storage_handle);
			return true;
        }
    }
}

bool Storage_WriteRegions(region_t ** regions)
{
    esp_err_t ret;

    if (!storage_init_done) return false;

    // Attempt to open storage
    ret = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    // Check if storage open failed
    if (ret != ESP_OK)
    {
    	// Storage open failed; return false
        ESP_LOGE(STORAGE_LOG_TAG, "Storage failed to open!");
        return false;
    }

    // Create temporary output storage buffer and populate with region data
    region_t data[REGION_COUNT];
    int i;
    for (i = 0; i < REGION_COUNT; i++)
    {
    	// Check if region is null
    	if (regions[i] == NULL)
    	{
    		// Set output region data to be a null region
    		data[i] = NULL_REGION;
    	}
    	else
    	{
    		// Set output region data to input data
			data[i] = *(regions[i]);
    	}
    }

    // Attempt to write region blob to storage
    ret = nvs_set_blob(storage_handle, KEY__REGIONS_BLOB, data, sizeof(data));
    // Check if write succeeded
    if (ret != ESP_OK)
    {
    	// Write failed; return false
    	ESP_LOGE(STORAGE_LOG_TAG, "Writing region data failed!");
    	nvs_close(storage_handle);
    	return false;
    }
    else
    {
    	ESP_LOGI(STORAGE_LOG_TAG, "Attempting to commit NVS data...");
		// Attempt to commit changes made to storage
		ret = nvs_commit(storage_handle);
		// Check if commit succeeded
		if (ret != ESP_OK)
		{
			// Commit failed; return false
			ESP_LOGE(STORAGE_LOG_TAG, "Error committing NVS data!");
			nvs_close(storage_handle);
			return false;
		}
		else
		{
			// Commit succeeded; return true
			ESP_LOGI(STORAGE_LOG_TAG, "Region data write to NVS complete.");
			nvs_close(storage_handle);
			return true;
		}
    }
}

bool Storage_ReadAnchors(anchor_t ** anchors)
{
	esp_err_t ret;

	if (!storage_init_done) return false;

	ESP_LOGI(STORAGE_LOG_TAG, "Beginning to read anchor data from NVS...");

	// Attempt to open storage
	ret = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
	// Check if storage open failed
	if (ret != ESP_OK)
	{
		// Storage open failed; return false
		ESP_LOGE(STORAGE_LOG_TAG, "Storage failed to open!");
		return false;
	}
	else
	{
		// Storage open succeeded
		// Initialise memory to load anchor data into
		anchor_t anchor_blob[ANCHOR_COUNT];
		int i;
		// Initialise anchor data as all null anchor placeholders
		for (i = 0; i < ANCHOR_COUNT; i++)
		{
			anchor_blob[i] = NULL_ANCHOR;
		}

		if (ReadGenericBlob(KEY__ANCHORS_BLOB, ANCHOR_BLOB_SIZE, anchor_blob) == false)
		{
			ESP_LOGE(STORAGE_LOG_TAG, "Error reading %s!", KEY__ANCHORS_BLOB);
			nvs_close(storage_handle);
			return false;
		}

		// Population anchor_t array passed as parameter
		for (i = 0; i < ANCHOR_COUNT; i++)
		{
			// Perform null check on anchor from blob
			if (memcmp(anchor_blob + i, &NULL_ANCHOR, sizeof(anchor_t)) == 0)
			{
				// Anchor is null; assign NULL to array index
			}
			else
			{
				// Anchor is not null; allocate memory to given array index and copy data read from blob into it
				ESP_LOGI(STORAGE_LOG_TAG, "Attempting to write to index %d.", i);
				anchors[i] = malloc(sizeof(anchor_t));
				memcpy(anchors[i], &(anchor_blob[i]), sizeof(anchor_t));
			}
		}

        ESP_LOGI(STORAGE_LOG_TAG, "Attempting to commit NVS data...");
        // Attempt to commit changes made to storage
        ret = nvs_commit(storage_handle);
        // Check if commit succeeded
        if (ret != ESP_OK)
        {
        	// Commit failed; return false
        	ESP_LOGE(STORAGE_LOG_TAG, "Error committing NVS data!");
        	nvs_close(storage_handle);
        	return false;
        }
        else
        {
        	// Commit succeeded; return true
        	ESP_LOGI(STORAGE_LOG_TAG, "Anchor data read from NVS complete.");
			nvs_close(storage_handle);
			return true;
        }
	}
}

bool Storage_WriteAnchors(anchor_t ** anchors)
{
    esp_err_t ret;

    if (!storage_init_done) return false;

    // Attempt to open storage
    ret = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    // Check if storage open failed
    if (ret != ESP_OK)
    {
    	// Storage open failed; return false
        ESP_LOGE(STORAGE_LOG_TAG, "Storage failed to open!");
        return false;
    }

    // Create temporary output storage buffer and populate with anchor data
    anchor_t data[ANCHOR_COUNT];
    int i;
    for (i = 0; i < ANCHOR_COUNT; i++)
    {
    	// Check if anchor is null
    	if (anchors[i] == NULL)
    	{
    		// Set output anchor data to be a null anchor
    		data[i] = NULL_ANCHOR;
    	}
    	else
    	{
    		// Set output anchor data to input data
			data[i] = *(anchors[i]);
    	}
    }

    // Attempt to write anchor blob to storage
    ret = nvs_set_blob(storage_handle, KEY__ANCHORS_BLOB, data, sizeof(data));
    // Check if write succeeded
    if (ret != ESP_OK)
    {
    	// Write failed; return false
    	ESP_LOGE(STORAGE_LOG_TAG, "Writing anchor data failed!");
    	nvs_close(storage_handle);
    	return false;
    }
    else
    {
    	ESP_LOGI(STORAGE_LOG_TAG, "Attempting to commit NVS data...");
		// Attempt to commit changes made to storage
		ret = nvs_commit(storage_handle);
		// Check if commit succeeded
		if (ret != ESP_OK)
		{
			// Commit failed; return false
			ESP_LOGE(STORAGE_LOG_TAG, "Error committing NVS data!");
			nvs_close(storage_handle);
			return false;
		}
		else
		{
			// Commit succeeded; return true
			ESP_LOGI(STORAGE_LOG_TAG, "Anchor data write to NVS complete.");
			nvs_close(storage_handle);
			return true;
		}
    }
}

bool ReadGenericBlob
(
	const char *	key,
	const size_t	blob_size,
	void * 			out
)
{
    esp_err_t ret;

	// Initialise memory to load blob into
	void * blob_data = malloc(blob_size);

	// Copy existing data in out into blob_data
	memcpy(blob_data, out, blob_size);

	size_t real_blob_size;

	// Attempt to load blob from storage
	ret = nvs_get_blob(storage_handle, key, blob_data, &real_blob_size);
	// Check outcome of attempted load
	switch (ret)
	{
	// Load succeeds
	case ESP_OK:
		break;
	// Load fails because key does not exist
	case ESP_ERR_NVS_NOT_FOUND:
		// Attempt to create blob
		ret = nvs_set_blob(storage_handle, key, blob_data, blob_size);
		// Check if blob creation succeeded
		if (ret != ESP_OK)
		{
			// Blob creation failed; return false
			return false;
		}
		break;
	// Load fails for other reason
	default:
		return false;
	}

	// Copy data in blob_data into out
	memcpy(out, blob_data, blob_size);
	free(blob_data);
	return true;
}
