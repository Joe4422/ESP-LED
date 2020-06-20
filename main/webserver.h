#ifndef WEBSERVER_H_
#define WEBSERVER_H_

/****************************************************************
 * Includes
 ****************************************************************/
// ESP-IDF includes
#include "esp_http_server.h"

/****************************************************************
 * Function declarations
 ****************************************************************/
httpd_handle_t Webserver_Start();

void Webserver_Stop
(
	const httpd_handle_t server
);

#endif /* WEBSERVER_H_ */
