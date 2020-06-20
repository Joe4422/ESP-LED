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

#define TOKEN_INDEX_RED		0
#define TOKEN_INDEX_GREEN	1
#define TOKEN_INDEX_BLUE	2

#define WRITE_ALL_OFFSET_COLOUR	1

#define WRITE_ONE_OFFSET_INDEX	1
#define WRITE_ONE_OFFSET_COLOUR	2

#define REGION_CREATE_OFFSET_START_INDEX	1
#define REGION_CREATE_OFFSET_END_INDEX		2
#define REGION_CREATE_OFFSET_SHADER_INDEX	3
#define REGION_CREATE_OFFSET_COLOUR			4

char * RESPONSE_STATIC_FAILURE =	"Failure";
char * RESPONSE_STATIC_SUCCESS =	"Success";

#define 	 WEB_LOG_TAG_BASE		"Web Server "
const char * WEB_LOG_TAG_WRITE =	WEB_LOG_TAG_BASE "(write)";
const char * WEB_LOG_TAG_REGION =	WEB_LOG_TAG_BASE "(region)";

#define TOKEN_SPLITTER	"-"

#define TOKEN_TO_VALUE_PARSE_INT(type) do { char * end; type * out = va_arg(argp, type *); *out = (type)strtol(tokens[i], &end, 0); if (end == tokens[i]) { return false; } } while (0)

const token_type_t WRITE_ALL_TOKEN_TYPES[] =			{ TOKEN_TYPE_COLOUR };
#define WRITE_ALL_TOKEN_COUNT							(3)
const token_type_t WRITE_ONE_TOKEN_TYPES[] =			{ TOKEN_TYPE_INDEX, TOKEN_TYPE_COLOUR };
#define WRITE_ONE_TOKEN_COUNT							(4)

const token_type_t REGION_CREATE_TOKEN_TYPES[] =		{ TOKEN_TYPE_INDEX, TOKEN_TYPE_INDEX, TOKEN_TYPE_UINT8, TOKEN_TYPE_COLOUR };
#define REGION_CREATE_TOKEN_COUNT						(6)
const token_type_t REGION_ADD_ANCHOR_TOKEN_TYPES[] =	{ TOKEN_TYPE_STRING, TOKEN_TYPE_UINT16 };
#define REGION_ADD_ANCHOR_TOKEN_COUNT					(2)
const token_type_t REGION_DELETE_ANCHOR_TOKEN_TYPES[] =	{ TOKEN_TYPE_STRING };
#define REGION_DELETE_ANCHOR_TOKEN_COUNT				(1)

#define BASE_WRITE_URI	"/write"
#define BASE_REGION_URI	"/region"

/****************************************************************
 * Function declarations
 ****************************************************************/
esp_err_t get_write_handler(httpd_req_t *req);
esp_err_t get_region_handler(httpd_req_t *req);
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
httpd_uri_t uri_write_get =
{
	.uri = BASE_WRITE_URI,
	.method = HTTP_GET,
	.handler = get_write_handler,
	.user_ctx = NULL
};

httpd_uri_t uri_region_get =
{
	.uri = BASE_REGION_URI,
	.method = HTTP_GET,
	.handler = get_region_handler,
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
	/* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_write_get);
		httpd_register_uri_handler(server, &uri_region_get);
		httpd_register_uri_handler(server, &uri_index_get);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

void Webserver_Stop(const httpd_handle_t server)
{
	if (server) {
        /* Stop the httpd server */
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
	token = strtok(str, TOKEN_SPLITTER);
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
				ESP_LOGI(WEB_LOG_TAG_WRITE, "Token %d is an anchor (%s)", val, tokens[i]);
				// Index is an anchor
				const anchor_t * anchor = Region_GetAnchor(tokens[i]);

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
				ESP_LOGI(WEB_LOG_TAG_WRITE, "Token %d is NOT an anchor (%s)", val, tokens[i]);
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

esp_err_t get_write_handler(httpd_req_t *req)
{
	char** response;

	response = &RESPONSE_STATIC_FAILURE;

	ESP_LOGI(WEB_LOG_TAG_WRITE, "Request at %s", req->uri);
	
	if (CheckUriParameter(req->uri, BASE_WRITE_URI, "all"))
	{
		colour_t colour;

		if (ReadTokensIntoValues(req->uri, WRITE_ALL_TOKEN_TYPES, WRITE_ALL_TOKEN_COUNT, &colour))
		{
			response = &RESPONSE_STATIC_SUCCESS;

			Strip_Buffer_SetAll(colour);
			Strip_Buffer_Push();
		}
	}
	else if (CheckUriParameter(req->uri, BASE_WRITE_URI, "one"))
	{
		uint16_t index;
		colour_t colour;

		if (ReadTokensIntoValues(req->uri, WRITE_ONE_TOKEN_TYPES, WRITE_ONE_TOKEN_COUNT, &index, &colour))
		{
			response = &RESPONSE_STATIC_SUCCESS;

			Strip_Buffer_SetOne(index, colour);
			Strip_Buffer_Push();
		}
	}

	// Send response
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, *response, strlen(*response));
    return ESP_OK;
}

esp_err_t get_region_handler(httpd_req_t *req)
{
	char** response = &RESPONSE_STATIC_FAILURE;

	ESP_LOGI(WEB_LOG_TAG_WRITE, "Request at %s.", req->uri);

	if (CheckUriParameter(req->uri, BASE_REGION_URI, "enable"))
	{
		ESP_LOGI(WEB_LOG_TAG_REGION, "Enabled regions.");
		global_config.regions_enabled = true;
		response = &RESPONSE_STATIC_SUCCESS;
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "disable"))
	{
		ESP_LOGI(WEB_LOG_TAG_REGION, "Disabled regions.");
		global_config.regions_enabled = false;
		response = &RESPONSE_STATIC_SUCCESS;
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "create"))
	{
		uint16_t startIndex;
		uint16_t endIndex;
		uint8_t shaderIndex;
		colour_t colour;

		if (ReadTokensIntoValues(req->uri, REGION_CREATE_TOKEN_TYPES, REGION_CREATE_TOKEN_COUNT, &startIndex, &endIndex, &shaderIndex, &colour))
		{
			if (shaderIndex < NUM_SHADERS)
			{
				if (Region_Create((region_t){ startIndex, endIndex, shaderIndex, colour }))
				{
					response = &RESPONSE_STATIC_SUCCESS;

					ESP_LOGI(WEB_LOG_TAG_REGION, "Created region from %d to %d with shader %s.", startIndex, endIndex, SHADERS[shaderIndex]->name);
				}
			}
		}
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "clear"))
	{
		ESP_LOGI(WEB_LOG_TAG_REGION, "Deleted all regions.");
		Region_DeleteAll();
		response = &RESPONSE_STATIC_SUCCESS;
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "anchor_add"))
	{
		anchor_t anchor;

		if (ReadTokensIntoValues(req->uri, REGION_ADD_ANCHOR_TOKEN_TYPES, REGION_ADD_ANCHOR_TOKEN_COUNT, anchor.name, &(anchor.index)))
		{
			ESP_LOGI(WEB_LOG_TAG_REGION, "Created anchor %s at %d.", anchor.name, anchor.index);
			response = &RESPONSE_STATIC_SUCCESS;

			Region_AddAnchor(anchor);
		}
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "anchor_delete"))
	{
		char anchor_name[ANCHOR_NAME_SIZE];

		if (ReadTokensIntoValues(req->uri, REGION_DELETE_ANCHOR_TOKEN_TYPES, REGION_DELETE_ANCHOR_TOKEN_COUNT, anchor_name))
		{
			if (Region_DeleteAnchor(anchor_name))
			{
				ESP_LOGI(WEB_LOG_TAG_REGION, "Deleted anchor %s.", anchor_name);
				response = &RESPONSE_STATIC_SUCCESS;
			}
		}
	}
	else if (CheckUriParameter(req->uri, BASE_REGION_URI, "anchor_clear"))
	{
		ESP_LOGI(WEB_LOG_TAG_REGION, "Deleted all anchors.");
		Region_DeleteAllAnchors();
		response = &RESPONSE_STATIC_SUCCESS;
	}

	// Send response
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, *response, strlen(*response));
    return ESP_OK;
}

esp_err_t get_index_handler(httpd_req_t *req)
{
	/* Send a simple response */
    httpd_resp_send(req, basicPage, strlen(basicPage));
    return ESP_OK;
}
