/****************************************************************
 * Includes
 ****************************************************************/
#include "web_data.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
const char* WEB_INDEX_HTML = "<head> \n \
	<script src=\"scripts.js\"></script> \n \
	<script src=\"RegionController.js\"></script> \n \
</head> \n \
<body> \n \
	<div id=\"tab_control\"> \n \
		<button id=\"tab_control_regions\" onclick=\"TabControls_Button_Regions_OnClick()\" disabled=\"true\">Regions</button> \n \
		<button id=\"tab_control_anchors\" onclick=\"TabControls_Button_Anchors_OnClick()\">Anchors</button> \n \
	</div> \n \
	<div id=\"tab_regions\"> \n \
		Regions \n \
		<button onclick=\"Regions_AddRegion_OnClick()\">Add Region</button> \n \
	</div> \n \
	<div id=\"tab_anchors\" style=\"display:none;\"> \n \
		Anchors \n \
	</div> \n \
	<script>Page_OnLoad();</script> \n \
</body> \n";

const char* WEB_REGIONCONTROLLER_JS = "class RegionController \n \
{ \n \
	constructor(regionIndex, startIndex, endIndex, shader, red, green, blue) \n \
	{ \n \
		this.regionIndex = regionIndex; \n \
		 \n \
		this.div = document.createElement(\"div\"); \n \
	 \n \
		// textBox_Start \n \
		var div_textBox_Start = document.createElement(\"div\"); \n \
		var label_textBox_Start = document.createElement(\"label\"); \n \
		label_textBox_Start.innerHTML = \"Start Index:\"; \n \
		this.textBox_Start = document.createElement(\"input\"); \n \
		this.textBox_Start.setAttribute(\"type\", \"text\"); \n \
		this.textBox_Start.placeholder = \"0\"; \n \
		this.textBox_Start.value = startIndex; \n \
		div_textBox_Start.appendChild(label_textBox_Start); \n \
		div_textBox_Start.appendChild(this.textBox_Start); \n \
		 \n \
		// textBox_End \n \
		var div_textBox_End = document.createElement(\"div\"); \n \
		var label_textBox_End = document.createElement(\"label\"); \n \
		label_textBox_End.innerHTML = \"End Index:\"; \n \
		this.textBox_End = document.createElement(\"input\"); \n \
		this.textBox_End.setAttribute(\"type\", \"text\"); \n \
		this.textBox_End.placeholder = \"149\"; \n \
		this.textBox_End.value = endIndex; \n \
		div_textBox_End.appendChild(label_textBox_End); \n \
		div_textBox_End.appendChild(this.textBox_End); \n \
		 \n \
		// ticker_Shader \n \
		var div_ticker_Shader = document.createElement(\"div\"); \n \
		var label_ticker_Shader = document.createElement(\"label\"); \n \
		label_ticker_Shader.innerHTML = \"Shader:\"; \n \
		this.ticker_Shader = document.createElement(\"input\"); \n \
		this.ticker_Shader.setAttribute(\"type\", \"number\"); \n \
		this.ticker_Shader.min = 0; \n \
		this.ticker_Shader.max = 3; \n \
		this.ticker_Shader.value = shader; \n \
		div_ticker_Shader.appendChild(label_ticker_Shader); \n \
		div_ticker_Shader.appendChild(this.ticker_Shader); \n \
		 \n \
		// slider_Red \n \
		var div_slider_Red = document.createElement(\"div\"); \n \
		var label_slider_Red = document.createElement(\"label\"); \n \
		label_slider_Red.innerHTML = \"Red:\"; \n \
		this.slider_Red = document.createElement(\"input\"); \n \
		this.slider_Red.setAttribute(\"type\", \"range\"); \n \
		this.slider_Red.min = 0; \n \
		this.slider_Red.max = 255; \n \
		this.slider_Red.value = red; \n \
		div_slider_Red.appendChild(label_slider_Red); \n \
		div_slider_Red.appendChild(this.slider_Red); \n \
		 \n \
		// slider_Green \n \
		var div_slider_Green = document.createElement(\"div\"); \n \
		var label_slider_Green = document.createElement(\"label\"); \n \
		label_slider_Green.innerHTML = \"Green:\"; \n \
		this.slider_Green = document.createElement(\"input\"); \n \
		this.slider_Green.setAttribute(\"type\", \"range\"); \n \
		this.slider_Green.min = 0; \n \
		this.slider_Green.max = 255; \n \
		this.slider_Green.value = green; \n \
		div_slider_Green.appendChild(label_slider_Green); \n \
		div_slider_Green.appendChild(this.slider_Green); \n \
		 \n \
		// slider_Blue \n \
		var div_slider_Blue = document.createElement(\"div\"); \n \
		var label_slider_Blue = document.createElement(\"label\"); \n \
		label_slider_Blue.innerHTML = \"Blue:\"; \n \
		this.slider_Blue = document.createElement(\"input\"); \n \
		this.slider_Blue.setAttribute(\"type\", \"range\"); \n \
		this.slider_Blue.min = 0; \n \
		this.slider_Blue.max = 255; \n \
		this.slider_Blue.value = blue; \n \
		div_slider_Blue.appendChild(label_slider_Blue); \n \
		div_slider_Blue.appendChild(this.slider_Blue); \n \
		 \n \
		// button_Save \n \
		this.button_Save = document.createElement(\"button\"); \n \
		this.button_Save.innerHTML = \"Save\"; \n \
		this.button_Save.setAttribute(\"onclick\", \"Regions_RegionController_Save_OnClick(\" + regionControllers.length + \");\"); \n \
		 \n \
		// button_Delete \n \
		this.button_Delete = document.createElement(\"button\"); \n \
		this.button_Delete.innerHTML = \"Delete\"; \n \
		this.button_Delete.setAttribute(\"onclick\", \"Regions_RegionController_Delete_OnClick(\" + regionControllers.length + \");\"); \n \
		 \n \
		this.div.appendChild(div_textBox_Start); \n \
		this.div.appendChild(div_textBox_End); \n \
		this.div.appendChild(div_ticker_Shader); \n \
		this.div.appendChild(div_slider_Red); \n \
		this.div.appendChild(div_slider_Green); \n \
		this.div.appendChild(div_slider_Blue); \n \
		this.div.appendChild(this.button_Save); \n \
		this.div.appendChild(this.button_Delete); \n \
	} \n \
	 \n \
	get startIndex() \n \
	{ \n \
		return this.textBox_Start.value; \n \
	} \n \
	set startIndex(value) \n \
	{ \n \
		this.textBox_Start.value = value; \n \
	} \n \
	 \n \
	get endIndex() \n \
	{ \n \
		return this.textBox_End.value; \n \
	} \n \
	set endIndex(value) \n \
	{ \n \
		this.textBox_End.value = value; \n \
	} \n \
	 \n \
	get shader() \n \
	{ \n \
		return this.ticker_Shader.value; \n \
	} \n \
	set shader(value) \n \
	{ \n \
		this.ticker_Shader.value = value; \n \
	} \n \
	 \n \
	get red() \n \
	{ \n \
		return this.slider_Red.value; \n \
	} \n \
	set red(value) \n \
	{ \n \
		this.slider_Red.value = value; \n \
	} \n \
	 \n \
	get green() \n \
	{ \n \
		return this.slider_Green.value; \n \
	} \n \
	set green(value) \n \
	{ \n \
		this.slider_Green.value = value; \n \
	} \n \
	 \n \
	get blue() \n \
	{ \n \
		return this.slider_Blue.value; \n \
	} \n \
	set blue(value) \n \
	{ \n \
		this.slider_Blue.value = value; \n \
	} \n \
	 \n \
	Create() \n \
	{ \n \
		Http_MakeRequest(\"/region?create-\" + this.startIndex + \"-\" + this.endIndex + \"-\" + this.shader + \"-\" + this.red + \"-\" + this.green + \"-\" + this.blue); \n \
	} \n \
	 \n \
	Update() \n \
	{ \n \
		Http_MakeRequest(\"/region?update-\" + this.regionIndex + \"-\" + this.startIndex + \"-\" + this.endIndex + \"-\" + this.shader + \"-\" + this.red + \"-\" + this.green + \"-\" + this.blue); \n \
	} \n \
	 \n \
	Delete() \n \
	{ \n \
		Http_MakeRequest(\"/region?delete-\" + this.regionIndex); \n \
	} \n \
	 \n \
	SetIndex(index) \n \
	{ \n \
		this.button_Save.setAttribute(\"onclick\", \"Regions_RegionController_Save_OnClick(\" + index + \");\"); \n \
		this.button_Delete.setAttribute(\"onclick\", \"Regions_RegionController_Delete_OnClick(\" + index + \");\"); \n \
	} \n \
} \n";

const char* WEB_SCRIPTS_JS = "var tab_control_regions; \n \
var tab_control_anchors; \n \
 \n \
var tab_regions; \n \
var tab_anchors; \n \
 \n \
var regionControllers = []; \n \
 \n \
var max_regions = 0; \n \
 \n \
function Page_OnLoad() \n \
{ \n \
	tab_control_regions = document.getElementById(\"tab_control_regions\"); \n \
	tab_control_anchors = document.getElementById(\"tab_control_anchors\"); \n \
	 \n \
	tab_regions = document.getElementById(\"tab_regions\"); \n \
	tab_anchors = document.getElementById(\"tab_anchors\"); \n \
	 \n \
	Http_MakeRequest(\"/region?get_max\"); \n \
} \n \
 \n \
/**************************************************************** \n \
 * HTTP \n \
 ****************************************************************/ \n \
function Http_Response_Region(response) \n \
{ \n \
	switch (response[1]) \n \
	{ \n \
		case \"max\": \n \
			max_regions = response[2]; \n \
			 \n \
			for (i = 0; i < max_regions; i++) \n \
			{ \n \
				Http_MakeRequest(\"/region?get-\" + i); \n \
			} \n \
			break; \n \
		case \"data\": \n \
			if (response[2] != \"null\") \n \
			{ \n \
				var rc = new RegionController(response[2], response[3], response[4], response[5], response[6], response[7], response[8]); \n \
				regionControllers.push(rc); \n \
				tab_regions.appendChild(rc.div); \n \
				rc.div.id = \"region-\" + response[2]; \n \
			} \n \
			break; \n \
		case \"created\": \n \
			for (i = 0; i < regionControllers.length; i++) \n \
			{ \n \
				if (regionControllers[i].regionIndex === null) \n \
				{ \n \
					console.log(\"Updated region index of region controller \" + i + \" to \" + response[2]);  \n \
					regionControllers[i].regionIndex = response[2]; \n \
					regionControllers[i].div.id = \"region-\" + response[2]; \n \
				} \n \
			} \n \
		default: \n \
			break; \n \
	} \n \
} \n \
  \n \
function Http_RequestEventListener() \n \
{ \n \
	response = this.responseText.split(\" \"); \n \
	console.log(response); \n \
	 \n \
	switch (response[0]) \n \
	{ \n \
		case \"region\": \n \
			Http_Response_Region(response); \n \
			break; \n \
		default: \n \
			break; \n \
	} \n \
} \n \
 \n \
function Http_MakeRequest(request) \n \
{ \n \
	var xmlHttp = new XMLHttpRequest(); \n \
	 \n \
	console.log(\"Requesting \" + request); \n \
	 \n \
	xmlHttp.addEventListener(\"load\", Http_RequestEventListener); \n \
	xmlHttp.open(\"GET\", request); \n \
	 \n \
	xmlHttp.send(); \n \
} \n \
 \n \
/**************************************************************** \n \
 * Tab controls \n \
 ****************************************************************/ \n \
function TabControls_Button_Regions_OnClick() \n \
{ \n \
	tab_regions.style.display = \"block\"; \n \
	tab_control_regions.disabled = true; \n \
	 \n \
	tab_anchors.style.display = \"none\"; \n \
	tab_control_anchors.disabled = false; \n \
} \n \
 \n \
function TabControls_Button_Anchors_OnClick() \n \
{ \n \
	tab_anchors.style.display = \"block\"; \n \
	tab_control_anchors.disabled = true; \n \
	 \n \
	tab_regions.style.display = \"none\"; \n \
	tab_control_regions.disabled = false; \n \
} \n \
 \n \
/**************************************************************** \n \
 * Regions \n \
 ****************************************************************/ \n \
function Regions_RegionController_Save_OnClick(index) \n \
{ \n \
	var rc = regionControllers[index]; \n \
	 \n \
	rc.Update(); \n \
} \n \
 \n \
function Regions_RegionController_Delete_OnClick(index) \n \
{ \n \
	var rc = regionControllers[index]; \n \
	 \n \
	rc.Delete(); \n \
	 \n \
	tab_regions.removeChild(rc.div); \n \
	 \n \
	regionControllers.splice(index, 1); \n \
	 \n \
	for (i = index; i < regionControllers.length; i++) \n \
	{ \n \
		regionControllers[i].SetIndex(i); \n \
	} \n \
} \n \
  \n \
function Regions_AddRegion_OnClick() \n \
{ \n \
	if (regionControllers.length < max_regions) \n \
	{ \n \
		var rc = new RegionController(null, 0, 0, 0, 0, 0, 0); \n \
		 \n \
		rc.Create(); \n \
		 \n \
		regionControllers.push(rc); \n \
		 \n \
		tab_regions.appendChild(rc.div); \n \
	} \n \
} \n";

/****************************************************************
 * Function definitions
 ****************************************************************/
esp_err_t web_index_html_handler(httpd_req_t *req)
{
	httpd_resp_send(req, WEB_INDEX_HTML, strlen(WEB_INDEX_HTML));
	return ESP_OK;
}

esp_err_t web_regioncontroller_js_handler(httpd_req_t *req)
{
	httpd_resp_send(req, WEB_REGIONCONTROLLER_JS, strlen(WEB_REGIONCONTROLLER_JS));
	return ESP_OK;
}

esp_err_t web_scripts_js_handler(httpd_req_t *req)
{
	httpd_resp_send(req, WEB_SCRIPTS_JS, strlen(WEB_SCRIPTS_JS));
	return ESP_OK;
}

void WebData_Populate(httpd_handle_t server)
{
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/index.html", .method = HTTP_GET, .handler = web_index_html_handler, .user_ctx = NULL });
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/RegionController.js", .method = HTTP_GET, .handler = web_regioncontroller_js_handler, .user_ctx = NULL });
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/scripts.js", .method = HTTP_GET, .handler = web_scripts_js_handler, .user_ctx = NULL });
}