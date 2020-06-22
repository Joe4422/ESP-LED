#ifndef WEB_DATA_H_
#define WEB_DATA_H_

/****************************************************************
 * Includes
 ****************************************************************/
// ESP-IDF includes
#include "esp_http_server.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
extern const char* WEB_CLASSES_JS;

extern const char* WEB_INDEX_HTML;

extern const char* WEB_SCRIPTS_JS;

extern const char* WEB_STYLE_CSS;

/****************************************************************
 * Function declarations
 ****************************************************************/
void WebData_Populate(httpd_handle_t server);

#endif /* WEB_DATA_H_ */