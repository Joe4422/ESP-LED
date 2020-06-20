/****************************************************************
 * Includes
 ****************************************************************/
#include "web_data.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
const char* WEB_INDEX_HTML = "<head> \n \
	<script src=\"scripts.js\"></script> \n \
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
	constructor(startIndex, endIndex, shader, red, green, blue) \n \
	{ \n \
		this.div = document.createElement(\"div\"); \n \
	 \n \
		// textBox_Start \n \
		var div_textBox_Start = document.createElement(\"div\"); \n \
		var label_textBox_Start = document.createElement(\"label\"); \n \
		label_textBox_Start.innerHTML = \"Start Index:\"; \n \
		this.textBox_Start = document.createElement(\"input\"); \n \
		textBox_Start.setAttribute(\"type\", \"text\"); \n \
		textBox_Start.placeholder = \"0\"; \n \
		textBox_Start.value = startIndex; \n \
		div_textBox_Start.appendChild(label_textBox_Start); \n \
		div_textBox_Start.appendChild(textBox_Start); \n \
		 \n \
		// textBox_End \n \
		var div_textBox_End = document.createElement(\"div\"); \n \
		var label_textBox_End = document.createElement(\"label\"); \n \
		label_textBox_End.innerHTML = \"End Index:\"; \n \
		this.textBox_End = document.createElement(\"input\"); \n \
		textBox_End.setAttribute(\"type\", \"text\"); \n \
		textBox_End.placeholder = \"149\"; \n \
		textBox_End.value = endIndex; \n \
		div_textBox_End.appendChild(label_textBox_End); \n \
		div_textBox_End.appendChild(textBox_End); \n \
		 \n \
		// ticker_Shader \n \
		var div_ticker_Shader = document.createElement(\"div\"); \n \
		var label_ticker_Shader = document.createElement(\"label\"); \n \
		label_ticker_Shader.innerHTML = \"Shader:\"; \n \
		this.ticker_Shader = document.createElement(\"input\"); \n \
		ticker_Shader.setAttribute(\"type\", \"number\"); \n \
		ticker_Shader.min = 0; \n \
		ticker_Shader.max = 3; \n \
		ticker_Shader.value = shader; \n \
		div_ticker_Shader.appendChild(label_ticker_Shader); \n \
		div_ticker_Shader.appendChild(ticker_Shader); \n \
		 \n \
		// slider_Red \n \
		var div_slider_Red = document.createElement(\"div\"); \n \
		var label_slider_Red = document.createElement(\"label\"); \n \
		label_slider_Red.innerHTML = \"Red:\"; \n \
		this.slider_Red = document.createElement(\"input\"); \n \
		slider_Red.setAttribute(\"type\", \"range\"); \n \
		slider_Red.min = 0; \n \
		slider_Red.max = 255; \n \
		slider_Red.value = red; \n \
		div_slider_Red.appendChild(label_slider_Red); \n \
		div_slider_Red.appendChild(slider_Red); \n \
		 \n \
		// slider_Green \n \
		var div_slider_Green = document.createElement(\"div\"); \n \
		var label_slider_Green = document.createElement(\"label\"); \n \
		label_slider_Green.innerHTML = \"Green:\"; \n \
		this.slider_Green = document.createElement(\"input\"); \n \
		slider_Green.setAttribute(\"type\", \"range\"); \n \
		slider_Green.min = 0; \n \
		slider_Green.max = 255; \n \
		slider_Green.value = green; \n \
		div_slider_Green.appendChild(label_slider_Green); \n \
		div_slider_Green.appendChild(slider_Green); \n \
		 \n \
		// slider_Blue \n \
		var div_slider_Blue = document.createElement(\"div\"); \n \
		var label_slider_Blue = document.createElement(\"label\"); \n \
		label_slider_Blue.innerHTML = \"Blue:\"; \n \
		this.slider_Blue = document.createElement(\"input\"); \n \
		slider_Blue.setAttribute(\"type\", \"range\"); \n \
		slider_Blue.min = 0; \n \
		slider_Blue.max = 255; \n \
		slider_Blue.value = blue; \n \
		div_slider_Blue.appendChild(label_slider_Blue); \n \
		div_slider_Blue.appendChild(slider_Blue); \n \
		 \n \
		// button_Save \n \
		var button_Save = document.createElement(\"button\"); \n \
		button_Save.innerHTML = \"Save\"; \n \
		button_Save.setAttribute(\"onclick\", \"Regions_RegionController_Save_OnClick(\" + regionControllers.length + \");\"); \n \
		 \n \
		div.appendChild(div_textBox_Start); \n \
		div.appendChild(div_textBox_End); \n \
		div.appendChild(div_ticker_Shader); \n \
		div.appendChild(div_slider_Red); \n \
		div.appendChild(div_slider_Green); \n \
		div.appendChild(div_slider_Blue); \n \
		div.appendChild(button_Save); \n \
	} \n \
	 \n \
	get startIndex() \n \
	{ \n \
		return textBox_Start.value; \n \
	} \n \
	set startIndex(value) \n \
	{ \n \
		textBox_Start.value = value; \n \
	} \n \
	 \n \
	get endIndex() \n \
	{ \n \
		return textBox_End.value; \n \
	} \n \
	set endIndex(value) \n \
	{ \n \
		textBox_End.value = value; \n \
	} \n \
	 \n \
	get shader() \n \
	{ \n \
		return ticker_Shader.value; \n \
	} \n \
	set shader(value) \n \
	{ \n \
		ticker_Shader.value = value; \n \
	} \n \
	 \n \
	get red() \n \
	{ \n \
		return slider_Red.value; \n \
	} \n \
	set red(value) \n \
	{ \n \
		slider_Red.value = value; \n \
	} \n \
	 \n \
	get green() \n \
	{ \n \
		return slider_Green.value; \n \
	} \n \
	set green(value) \n \
	{ \n \
		slider_Green.value = value; \n \
	} \n \
	 \n \
	get blue() \n \
	{ \n \
		return slider_Blue.value; \n \
	} \n \
	set blue(value) \n \
	{ \n \
		slider_Blue.value = value; \n \
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
function Page_OnLoad() \n \
{ \n \
	tab_control_regions = document.getElementById(\"tab_control_regions\"); \n \
	tab_control_anchors = document.getElementById(\"tab_control_anchors\"); \n \
	 \n \
	tab_regions = document.getElementById(\"tab_regions\"); \n \
	tab_anchors = document.getElementById(\"tab_anchors\"); \n \
} \n \
 \n \
function Http_MakeRequest(request) \n \
{ \n \
	var xmlHttp = new XMLHttpRequest(); \n \
	xmlHttp.open(\"GET\", request, true); \n \
	 \n \
	xmlHttp.send(null); \n \
} \n \
 \n \
/**************************************************************** \n \
 * Classes \n \
 ****************************************************************/ \n \
function RegionController(textBox_Start, textBox_End, ticker_Shader, slider_Red, slider_Green, slider_Blue) \n \
{ \n \
	this.textBox_Start = textBox_Start; \n \
	this.textBox_End = textBox_End; \n \
	this.ticker_Shader = ticker_Shader; \n \
	this.slider_Red = slider_Red; \n \
	this.slider_Green = slider_Green; \n \
	this.slider_Blue = slider_Blue; \n \
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
function Regions_CreateRegionController() \n \
{ \n \
	var div = document.createElement(\"div\"); \n \
	 \n \
	// textBox_Start \n \
	var div_textBox_Start = document.createElement(\"div\"); \n \
	var label_textBox_Start = document.createElement(\"label\"); \n \
	label_textBox_Start.innerHTML = \"Start Index:\"; \n \
	var textBox_Start = document.createElement(\"input\"); \n \
	textBox_Start.setAttribute(\"type\", \"text\"); \n \
	textBox_Start.placeholder = \"0\"; \n \
	div_textBox_Start.appendChild(label_textBox_Start); \n \
	div_textBox_Start.appendChild(textBox_Start); \n \
	 \n \
	// textBox_End \n \
	var div_textBox_End = document.createElement(\"div\"); \n \
	var label_textBox_End = document.createElement(\"label\"); \n \
	label_textBox_End.innerHTML = \"End Index:\"; \n \
	var textBox_End = document.createElement(\"input\"); \n \
	textBox_End.setAttribute(\"type\", \"text\"); \n \
	textBox_End.placeholder = \"149\"; \n \
	div_textBox_End.appendChild(label_textBox_End); \n \
	div_textBox_End.appendChild(textBox_End); \n \
	 \n \
	// ticker_Shader \n \
	var div_ticker_Shader = document.createElement(\"div\"); \n \
	var label_ticker_Shader = document.createElement(\"label\"); \n \
	label_ticker_Shader.innerHTML = \"Shader:\"; \n \
	var ticker_Shader = document.createElement(\"input\"); \n \
	ticker_Shader.setAttribute(\"type\", \"number\"); \n \
	ticker_Shader.max = 3; \n \
	ticker_Shader.min = 0; \n \
	ticker_Shader.value = 0; \n \
	div_ticker_Shader.appendChild(label_ticker_Shader); \n \
	div_ticker_Shader.appendChild(ticker_Shader); \n \
	 \n \
	// slider_Red \n \
	var div_slider_Red = document.createElement(\"div\"); \n \
	var label_slider_Red = document.createElement(\"label\"); \n \
	label_slider_Red.innerHTML = \"Red:\"; \n \
	var slider_Red = document.createElement(\"input\"); \n \
	slider_Red.setAttribute(\"type\", \"range\"); \n \
	slider_Red.min = 0; \n \
	slider_Red.max = 255; \n \
	slider_Red.value = 0; \n \
	div_slider_Red.appendChild(label_slider_Red); \n \
	div_slider_Red.appendChild(slider_Red); \n \
	 \n \
	// slider_Green \n \
	var div_slider_Green = document.createElement(\"div\"); \n \
	var label_slider_Green = document.createElement(\"label\"); \n \
	label_slider_Green.innerHTML = \"Green:\"; \n \
	var slider_Green = document.createElement(\"input\"); \n \
	slider_Green.setAttribute(\"type\", \"range\"); \n \
	slider_Green.min = 0; \n \
	slider_Green.max = 255; \n \
	slider_Green.value = 0; \n \
	div_slider_Green.appendChild(label_slider_Green); \n \
	div_slider_Green.appendChild(slider_Green); \n \
	 \n \
	// slider_Blue \n \
	var div_slider_Blue = document.createElement(\"div\"); \n \
	var label_slider_Blue = document.createElement(\"label\"); \n \
	label_slider_Blue.innerHTML = \"Blue:\"; \n \
	var slider_Blue = document.createElement(\"input\"); \n \
	slider_Blue.setAttribute(\"type\", \"range\"); \n \
	slider_Blue.min = 0; \n \
	slider_Blue.max = 255; \n \
	slider_Blue.value = 0; \n \
	div_slider_Blue.appendChild(label_slider_Blue); \n \
	div_slider_Blue.appendChild(slider_Blue); \n \
	 \n \
	// button_Save \n \
	var button_Save = document.createElement(\"button\"); \n \
	button_Save.innerHTML = \"Save\"; \n \
	button_Save.setAttribute(\"onclick\", \"Regions_RegionController_Save_OnClick(\" + regionControllers.length + \");\"); \n \
	 \n \
	div.appendChild(div_textBox_Start); \n \
	div.appendChild(div_textBox_End); \n \
	div.appendChild(div_ticker_Shader); \n \
	div.appendChild(div_slider_Red); \n \
	div.appendChild(div_slider_Green); \n \
	div.appendChild(div_slider_Blue); \n \
	div.appendChild(button_Save); \n \
	 \n \
	var regionController = new RegionController(textBox_Start, textBox_End, ticker_Shader, slider_Red, slider_Green, slider_Blue); \n \
	 \n \
	regionControllers.push(regionController); \n \
	 \n \
	return div; \n \
} \n \
 \n \
function Regions_RegionController_Save_OnClick(index) \n \
{ \n \
	var rc = regionControllers[index]; \n \
	 \n \
	Http_MakeRequest(\"/region?create-\" + rc.textBox_Start.value + \"-\" + rc.textBox_End.value + \"-\" + rc.ticker_Shader.value + \"-\" + rc.slider_Red.value + \"-\" + rc.slider_Green.value + \"-\" + rc.slider_Blue.value); \n \
} \n \
  \n \
function Regions_AddRegion_OnClick() \n \
{ \n \
	tab_regions.appendChild(Regions_CreateRegionController()); \n \
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