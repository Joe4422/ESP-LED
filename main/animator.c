/****************************************************************
 * Includes
 ****************************************************************/
#include "animator.h"

// cstdlib includes
#include <stdbool.h>
#include <stddef.h>

// ESP-IDF includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project includes
#include "shaders.h"
#include "region_manager.h"
#include "strip_manager.h"
#include "config.h"

/****************************************************************
 * Local variables
 ****************************************************************/
bool runAnimation;

/****************************************************************
 * Function definitions
 ****************************************************************/
void Animator_Start(void)
{
	int i;

	runAnimation = true;
	while (runAnimation)
	{
		if (global_config.strip_on == false) continue;

		for (i = 0; i < REGION_COUNT; i++)
		{
			const region_t * regionData = Region_GetData(i);
			if (regionData == NULL)
			{
				continue;
			}
			else
			{
				SHADERS[regionData->shaderIndex]->nextFrame(regionData);
			}
		}
		Strip_Buffer_Push();
		vTaskDelay(ANIMATION_DELAY_MS / portTICK_PERIOD_MS);
	}
}
