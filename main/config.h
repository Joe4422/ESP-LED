#ifndef CONFIG_H_
#define CONFIG_H_

/****************************************************************
 * Includes
 ****************************************************************/
// cstdlib includes
#include <stdint.h>
#include <stdbool.h>

// Project includes
#include "wifi_login.h"

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef struct config
{
	bool strip_on;
} config_t;

/****************************************************************
 * Defines, consts
 ****************************************************************/
extern const config_t CONFIG_DEFAULT;

#define NUM_LEDS	150

#define ANIMATION_DELAY_MS	200

#define WIFI_MAX_RETRIES	10

/****************************************************************
 * Global variables
 ****************************************************************/
extern config_t global_config;

#endif /* CONFIG_H_ */
