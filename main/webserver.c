/****************************************************************
 * Includes
 ****************************************************************/
#include "webserver.h"

// cstdlib includes
#include <string.h>
#include <stdarg.h>

// ESP-IDF includes
#include "esp_system.h"
#include "esp_log.h"

// Project includes
#include "strip_manager.h"
#include "shaders.h"
#include "colour.h"
#include "web_data.h"
#include "region_manager.h"
#include "anchor_manager.h"
#include "config.h"

/****************************************************************
 * Typedefs, structs, enums
 ****************************************************************/
typedef enum token_type
{
	TOKEN_TYPE_INT8,
	TOKEN_TYPE_INT16,
	TOKEN_TYPE_INT32,
	TOKEN_TYPE_UINT8,
	TOKEN_TYPE_UINT16,
	TOKEN_TYPE_UINT32,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_COLOUR,
	TOKEN_TYPE_INDEX
} token_type_t;

/****************************************************************
 * Defines, consts
 ****************************************************************/
#define MAX_TOKEN_COUNT		32
#define STRING_BUFFER_SIZE	128

char * RESPONSE_STATIC_FAILURE =	"Failure";
char * RESPONSE_STATIC_SUCCESS =	"Success";

#define 	 WEB_LOG_TAG_BASE		"Web Server "
const char * WEB_LOG_TAG_API =		WEB_LOG_TAG_BASE "(api)";
const char * WEB_LOG_TAG_FILE =		WEB_LOG_TAG_BASE "(file)";

#define TOKEN_SPLITTER	","

#define TOKEN_TO_VALUE_PARSE_INT(type) do { char * end; type * out = va_arg(argp, type *); *out = (type)strtol(tokens[i], &end, 0); if (end == tokens[i]) { return false; } } while (0)

const token_type_t REGION_CREATE_TOKEN_TYPES[] =		{ TOKEN_TYPE_INDEX, TOKEN_TYPE_INDEX, TOKEN_TYPE_UINT8, TOKEN_TYPE_COLOUR };
#define REGION_CREATE_TOKEN_COUNT						(6)
const token_type_t REGION_UPDATE_TOKEN_TYPES[] =		{ TOKEN_TYPE_UINT8, TOKEN_TYPE_INDEX, TOKEN_TYPE_INDEX, TOKEN_TYPE_UINT8, TOKEN_TYPE_COLOUR };
#define REGION_UPDATE_TOKEN_COUNT						(7)
const token_type_t REGION_DELETE_TOKEN_TYPES[] =		{ TOKEN_TYPE_UINT8 };
#define REGION_DELETE_TOKEN_COUNT						(1)
const token_type_t REGION_GET_TOKEN_TYPES[] =			{ TOKEN_TYPE_UINT8 };
#define REGION_GET_TOKEN_COUNT							(1)

const token_type_t ANCHOR_CREATE_TOKEN_TYPES[] =		{ TOKEN_TYPE_STRING, TOKEN_TYPE_UINT16 };
#define ANCHOR_CREATE_TOKEN_COUNT						(2)
const token_type_t ANCHOR_UPDATE_TOKEN_TYPES[] =		{ TOKEN_TYPE_UINT8, TOKEN_TYPE_STRING, TOKEN_TYPE_UINT16 };
#define ANCHOR_UPDATE_TOKEN_COUNT						(3)
const token_type_t ANCHOR_DELETE_TOKEN_TYPES[] =		{ TOKEN_TYPE_UINT8 };
#define ANCHOR_DELETE_TOKEN_COUNT						(1)
const token_type_t ANCHOR_GET_TOKEN_TYPES[] =			{ TOKEN_TYPE_UINT8 };
#define ANCHOR_GET_TOKEN_COUNT							(1)

#define BASE_API_URI				"/api"

#define COMMAND_PART_SEPARATOR		"-"

#define COMMAND_REGION_BASE			"region"
#define COMMAND_REGION_CREATE		"create"
#define COMMAND_REGION_UPDATE		"update"
#define COMMAND_REGION_DELETE		"delete"
#define COMMAND_REGION_CLEAR		"clear"
#define COMMAND_REGION_GET_MAX		"get_max"
#define COMMAND_REGION_GET			"get"

#define COMMAND_ANCHOR_BASE			"anchor"
#define COMMAND_ANCHOR_CREATE		"create"
#define COMMAND_ANCHOR_UPDATE		"update"
#define COMMAND_ANCHOR_DELETE		"delete"
#define COMMAND_ANCHOR_CLEAR		"clear"
#define COMMAND_ANCHOR_GET_MAX		"get_max"
#define COMMAND_ANCHOR_GET			"get"

#define COMMAND_SHADER_BASE			"shader"
#define COMMAND_SHADER_GET_NAMES	"get_names"

#define COMMAND_CONFIG_BASE			"config"
#define COMMAND_CONFIG_TOGGLE_POWER	"toggle_power"
#define COMMAND_CONFIG_NUM_LEDS		"num_leds"


/****************************************************************
 * Function declarations
 ****************************************************************/
esp_err_t get_api_handler(httpd_req_t *req);
esp_err_t get_index_handler(httpd_req_t *req);

bool ReadTokensIntoValues
(
	const char * inputString,
	const token_type_t tokenTypes[],
	uint8_t expectedTokenCount,
	...
);

bool CheckUriParameter(const char * uri, const char * baseAddr, const char * parameter);

/****************************************************************
 * Local variables
 ****************************************************************/
httpd_uri_t uri_api_get =
{
	.uri = BASE_API_URI,
	.method = HTTP_GET,
	.handler = get_api_handler,
	.user_ctx = NULL
};

httpd_uri_t uri_index_get =
{
	.uri = "/",
	.method = HTTP_GET,
	.handler = get_index_handler,
	.user_ctx = NULL
};

/****************************************************************
 * Function definitions
 ****************************************************************/
httpd_handle_t Webserver_Start()
{
	// Generate default configuration
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Empty handle to esp_http_server
    httpd_handle_t server = NULL;

    // Start the httpd server
    if (httpd_start(&server, &config) == ESP_OK) {
        // Register URI handlers
        httpd_register_uri_handler(server, &uri_api_get);
		httpd_register_uri_handler(server, &uri_index_get);
    }

    WebData_Populate(server);

    /* If server failed to start, handle will be NULL */
    return server;
}

void Webserver_Stop(const httpd_handle_t server)
{
	if (server)
	{
		// Stop the server
        httpd_stop(server);
    }
}

bool ReadTokensIntoValues
(
	const char * inputString,
	const token_type_t tokenTypes[],
	uint8_t expectedTokenCount,
	...
)
{
	int tokenCount = 0;
	char* tokens[MAX_TOKEN_COUNT];
	char str[STRING_BUFFER_SIZE];
	char* token;
	va_list argp;
	int i;

	// Tokenise input string
	strcpy(str, inputString);
	token = strtok(str, ":");
	// Discards the first token but that's exactly what we want
	while ((token = strtok(NULL, TOKEN_SPLITTER)) != NULL)
	{
		if (token[0] != '\0')
		{
			tokens[tokenCount++] = token;
		}
	}

	if (tokenCount != expectedTokenCount)
	{
		return false;
	}

	va_start(argp, expectedTokenCount);

	for (i = 0; i < tokenCount; i++)
	{
		switch (tokenTypes[i])
		{
		case TOKEN_TYPE_INT8:
			TOKEN_TO_VALUE_PARSE_INT(int8_t);
			break;
		case TOKEN_TYPE_INT16:
			TOKEN_TO_VALUE_PARSE_INT(int16_t);
			break;
		case TOKEN_TYPE_INT32:
			TOKEN_TO_VALUE_PARSE_INT(int32_t);
			break;
		case TOKEN_TYPE_UINT8:
			TOKEN_TO_VALUE_PARSE_INT(uint8_t);
			break;
		case TOKEN_TYPE_UINT16:
			TOKEN_TO_VALUE_PARSE_INT(uint16_t);
			break;
		case TOKEN_TYPE_UINT32:
			TOKEN_TO_VALUE_PARSE_INT(uint32_t);
			break;
		case TOKEN_TYPE_STRING:
		{
			char * out = va_arg(argp, char *);
			strcpy(out, tokens[i]);
			break;
		}
		case TOKEN_TYPE_COLOUR:
		{
			char * end;
			colour_t * out = va_arg(argp, colour_t *);
			out->red = (uint8_t)strtol(tokens[i], &end, 0);
			if (end == tokens[i]) return false;
			out->green = (uint8_t)strtol(tokens[i + 1], &end, 0);
			if (end == tokens[i + 1]) return false;
			out->blue = (uint8_t)strtol(tokens[i + 2], &end, 0);
			if (end == tokens[i + 2]) return false;
			i += 2;
			break;
		}
		case TOKEN_TYPE_INDEX:
		{
			char * end;
			uint16_t * out = va_arg(argp, uint16_t *);
			uint16_t val = strtol(tokens[i], &end, 0);

			if (end == tokens[i])
			{
				// Index is an anchor
				const anchor_t * anchor = Anchor_GetDataByName(tokens[i]);

				if (anchor == NULL)
				{
					return false;
				}
				else
				{
					*out = anchor->index;
				}
			}
			else
			{
				// Index is not an anchor
				*out = val;
			}
			break;
		}
		default:
			return false;
		}
	}

	return true;
}

// uri =		/api?write-all:255,255,255
// baseAddr =	/api
// parameter =  write-all
bool CheckUriParameter(const char * uri, const char * baseAddr, const char * parameter)
{
	uint8_t baseAddrLen = strlen(baseAddr);
	uint8_t paramLen = strlen(parameter);

	uint8_t i;
	const char * paramStart = uri + baseAddrLen + 1; // + 1 is for "?"

	for (i = 0; i < paramLen; i++)
	{
		if (parameter[i] != paramStart[i])
		{
			return false;
		}
	}
	return true;
}

esp_err_t get_api_handler(httpd_req_t *req)
{
	char response_buffer[STRING_BUFFER_SIZE];
	strcpy(response_buffer, RESPONSE_STATIC_FAILURE);

	ESP_LOGI(WEB_LOG_TAG_API, "Request at a%s.", req->uri);

	// region-create
	if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_CREATE))
	{
		region_t region;

		if (ReadTokensIntoValues(req->uri, REGION_CREATE_TOKEN_TYPES, REGION_CREATE_TOKEN_COUNT, &region.start, &region.end, &region.shaderIndex, &region.colour))
		{
			if (region.shaderIndex < NUM_SHADERS)
			{
				uint8_t index;

				if (Region_Create(region, &index))
				{
					//SHADERS[region.shaderIndex]->init(&region);

					sprintf(response_buffer, "region create %d", index);

					ESP_LOGI(WEB_LOG_TAG_API, "Created region from %d to %d with shader %s.", region.start, region.end, SHADERS[region.shaderIndex]->name);
				}
			}
		}
	}
	// region-update
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_UPDATE))
	{
		uint8_t regionIndex;
		region_t region;

		if (ReadTokensIntoValues(req->uri, REGION_UPDATE_TOKEN_TYPES, REGION_UPDATE_TOKEN_COUNT, &regionIndex, &region.start, &region.end, &region.shaderIndex, &region.colour))
		{
			if (region.shaderIndex < NUM_SHADERS)
			{
				if (Region_Update(regionIndex, region))
				{
					//SHADERS[region.shaderIndex]->init(&region);

					strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);

					ESP_LOGI(WEB_LOG_TAG_API, "Updated region at index %d to from %d to %d with shader %s.", regionIndex, region.start, region.end, SHADERS[region.shaderIndex]->name);
				}
			}
		}
	}
	// region-delete
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_DELETE))
	{
		uint8_t regionIndex;

		if (ReadTokensIntoValues(req->uri, REGION_DELETE_TOKEN_TYPES, REGION_DELETE_TOKEN_COUNT, &regionIndex))
		{
			if (Region_Delete(regionIndex))
			{
				strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);

				ESP_LOGI(WEB_LOG_TAG_API, "Deleted region at index %d.", regionIndex);

				Strip_Buffer_SetAll(COLOUR_OFF);
			}
		}
	}
	// region-clear
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_CLEAR))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Deleted all regions.");
		Region_DeleteAll();
		Strip_Buffer_SetAll(COLOUR_OFF);
		strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);
	}
	// region-get_max
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_GET_MAX))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Request for max regions (%d)", REGION_COUNT);
		sprintf(response_buffer, "region get_max %d", REGION_COUNT);
	}
	// region-get
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_REGION_BASE COMMAND_PART_SEPARATOR COMMAND_REGION_GET))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Request for region.");

		uint8_t regionIndex;

		if (ReadTokensIntoValues(req->uri, REGION_GET_TOKEN_TYPES, REGION_GET_TOKEN_COUNT, &regionIndex))
		{
			const region_t * region = Region_GetData(regionIndex);

			if (region == NULL)
			{
				sprintf(response_buffer, "region get null");
			}
			else
			{
				sprintf(response_buffer, "region get %d %d %d %d %d %d %d", regionIndex, region->start, region->end, region->shaderIndex, region->colour.red, region->colour.green, region->colour.blue);
			}
		}
	}
	// anchor-create
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_CREATE))
	{
		anchor_t anchor;

		if (ReadTokensIntoValues(req->uri, ANCHOR_CREATE_TOKEN_TYPES, ANCHOR_CREATE_TOKEN_COUNT, anchor.name, &(anchor.index)))
		{
			if (anchor.index < NUM_LEDS)
			{
				uint8_t index;

				if (Anchor_Create(anchor, &index))
				{
					sprintf(response_buffer, "anchor create %d", index);

					ESP_LOGI(WEB_LOG_TAG_API, "Created anchor %s at %d.", anchor.name, anchor.index);
				}
			}
		}
	}
	// anchor-update
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_UPDATE))
	{
		uint8_t anchorIndex;
		anchor_t anchor;

		if (ReadTokensIntoValues(req->uri, ANCHOR_UPDATE_TOKEN_TYPES, ANCHOR_UPDATE_TOKEN_COUNT, &anchorIndex, anchor.name, &(anchor.index)))
		{
			if (anchor.index < NUM_LEDS)
			{
				if (Anchor_Update(anchorIndex, anchor))
				{
					strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);

					ESP_LOGI(WEB_LOG_TAG_API, "Updated anchor at index %d to %s at %d.", anchorIndex, anchor.name, anchor.index);
				}
			}
		}
	}
	// anchor-delete
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_DELETE))
	{
		uint8_t anchorIndex;

		if (ReadTokensIntoValues(req->uri, ANCHOR_DELETE_TOKEN_TYPES, ANCHOR_DELETE_TOKEN_COUNT, &anchorIndex))
		{
			if (Anchor_Delete(anchorIndex))
			{
				strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);

				ESP_LOGI(WEB_LOG_TAG_API, "Deleted anchor at index %d.", anchorIndex);
			}
		}
	}
	// anchor-clear
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_CLEAR))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Deleted all anchors.");
		Anchor_DeleteAll();
		strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);
	}
	// anchor-get_max
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_GET_MAX))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Request for max anchors (%d)", ANCHOR_COUNT);
		sprintf(response_buffer, "anchor get_max %d", ANCHOR_COUNT);
	}
	// anchor-get
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_ANCHOR_BASE COMMAND_PART_SEPARATOR COMMAND_ANCHOR_GET))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Request for anchor.");

		uint8_t anchorIndex;

		if (ReadTokensIntoValues(req->uri, ANCHOR_GET_TOKEN_TYPES, ANCHOR_GET_TOKEN_COUNT, &anchorIndex))
		{
			const anchor_t * anchor = Anchor_GetData(anchorIndex);

			if (anchor == NULL)
			{
				sprintf(response_buffer, "anchor get null");
			}
			else
			{
				sprintf(response_buffer, "anchor get %d %s %d", anchorIndex, anchor->name, anchor->index);
			}
		}
	}
	// shader-get_names
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_SHADER_BASE COMMAND_PART_SEPARATOR COMMAND_SHADER_GET_NAMES))
	{
		uint8_t i;
		char * buffer_ptr = response_buffer;

		buffer_ptr += sprintf(response_buffer, "shader get_names %d", NUM_SHADERS);

		for (i = 0; i < NUM_SHADERS; i++)
		{
			buffer_ptr += sprintf(buffer_ptr, " %s", SHADERS[i]->name);
		}
	}
	// config-toggle_power
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_CONFIG_BASE COMMAND_PART_SEPARATOR COMMAND_CONFIG_TOGGLE_POWER))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Toggled power.");
		global_config.strip_on = !global_config.strip_on;
		strcpy(response_buffer, RESPONSE_STATIC_SUCCESS);
	}
	// config-num_leds
	else if (CheckUriParameter(req->uri, BASE_API_URI, COMMAND_CONFIG_BASE COMMAND_PART_SEPARATOR COMMAND_CONFIG_NUM_LEDS))
	{
		ESP_LOGI(WEB_LOG_TAG_API, "Requested number of LEDs. (%d)", NUM_LEDS);
		sprintf(response_buffer, "config num_leds %d", NUM_LEDS);
	}

	// Send response
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, response_buffer, strlen(response_buffer));
    return ESP_OK;
}

esp_err_t get_index_handler(httpd_req_t *req)
{
    httpd_resp_send(req, WEB_INDEX_HTML, strlen(WEB_INDEX_HTML));
    return ESP_OK;
}

