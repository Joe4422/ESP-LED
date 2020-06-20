#ifndef STRIP_MANAGER_H_
#define STRIP_MANAGER_H_

/****************************************************************
 * Includes
 ****************************************************************/
// Project includes
#include "colour.h"
#include "region_manager.h"

/****************************************************************
 * Function declarations
 ****************************************************************/
void Strip_Init(void);

void Strip_Buffer_Push(void);

void Strip_Buffer_SetAll
(
	colour_t	colour
);

void Strip_Buffer_SetArea
(
	uint16_t	startIndex,
	uint16_t	endIndex,
	colour_t	colour
);

void Strip_Buffer_SetOne
(
	uint16_t	index,
	colour_t	colour
);

colour_t Strip_Buffer_GetOne
(
	uint16_t	index
);

#endif /* STRIP_MANAGER_H_ */
