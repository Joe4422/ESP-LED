/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdbool.h>

// ESP-IDF includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Project includes
#include "startup.h"
#include "strip_manager.h"
#include "region_manager.h"
#include "animator.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
const char * MAIN_LOG_TAG = "Main";

/****************************************************************
 * Function definitions
 ****************************************************************/
int app_main()
{
	bool success = false;

	// Attempt to perform startup
	success = Startup_Init();
	// Check if startup succeeded
	if (success == false)
	{
		// Startup failed; do nothing
		ESP_LOGE(MAIN_LOG_TAG, "Startup failed!");
		while (1);
	}
	else
	{
		// Startup succeeded; run animator
		ESP_LOGI(MAIN_LOG_TAG, "Startup successful.");
		Animator_Start();
	}

	return 0;
}
