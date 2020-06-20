/****************************************************************
 * Includes
 ****************************************************************/
#include "startup.h"

// ESP-IDF includes
#include "esp_http_server.h"

// Project includes
#include "strip_manager.h"
#include "storage_manager.h"
#include "wifi_manager.h"
#include "webserver.h"
#include "region_manager.h"
#include "config.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define INDICATOR_INDEX				(0)
#define INDICATOR_COLOUR_NOT_READY	(COLOUR_RED)
#define INDICATOR_COLOUR_READY		(COLOUR_GREEN)
#define INDICATOR_COLOUR_FAILED		(COLOUR_YELLOW)
#define INDICATOR_COLOUR_OFF		(COLOUR_OFF)

#define INDICATOR_READY_HOLD_DELAY	(2000)

const char* STARTUP_LOG_TAG	=	"Startup";

/****************************************************************
 * Function declarations
 ****************************************************************/
void SetIndicatorLed(colour_t colour);

/****************************************************************
 * Function definitions
 ****************************************************************/
bool Startup_Init()
{
	bool success = false;

	// Init LED strip
	Strip_Init();

	// Set strip indicator LED to red
	SetIndicatorLed(INDICATOR_COLOUR_NOT_READY);

	// Attempt to initialise storage
	success = Storage_Init();
	// Check if storage init succeeded
	if (success == false)
	{
		// Storage init failed; return false
		ESP_LOGE(STARTUP_LOG_TAG, "Storage init failed!");
		SetIndicatorLed(INDICATOR_COLOUR_FAILED);
		return false;
	}

	// Attempt to init region manager
	success = Region_Init();
	// Check if region manager init succeeded
	if (success == false)
	{
		// Region manager init failed; return false
		ESP_LOGE(STARTUP_LOG_TAG, "Region manager init failed!");
		SetIndicatorLed(INDICATOR_COLOUR_FAILED);
		return false;
	}

	// Attempt to initialise WiFi
    success = WiFi_Init(WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_RETRIES);
    // Check if WiFi init succeeded
    if (success == false)
    {
    	// WiFi init failed; return false
    	ESP_LOGE(STARTUP_LOG_TAG, "WiFi init failed!");
		SetIndicatorLed(INDICATOR_COLOUR_FAILED);
    	return false;
    }

    // Start web server
    const httpd_handle_t http_server_handle = Webserver_Start();
	// Check if web server started successfully
	if (http_server_handle == NULL)
	{
		// Web server start failed; return false
		ESP_LOGE(STARTUP_LOG_TAG, "Web server init failed!");
		SetIndicatorLed(INDICATOR_COLOUR_FAILED);
		return false;
	}

	// Boot finished - set indicator LED green
	SetIndicatorLed(INDICATOR_COLOUR_READY);

	// Wait, then turn off LED
	vTaskDelay(INDICATOR_READY_HOLD_DELAY / portTICK_PERIOD_MS);
	SetIndicatorLed(INDICATOR_COLOUR_OFF);

	return true;
}

void SetIndicatorLed(colour_t colour)
{
	Strip_Buffer_SetOne(INDICATOR_INDEX, colour);
	Strip_Buffer_Push();
}
