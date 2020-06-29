/****************************************************************
 * Includes
 ****************************************************************/
#include "web_data.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
const char* WEB_CLASSES_JS = "class RegionController \n \
{ \n \
	constructor(regionIndex, startIndex, endIndex, shader, red, green, blue) \n \
	{ \n \
		this.region = regionIndex; \n \
		 \n \
		this.div = document.createElement(\"div\"); \n \
		this.div.setAttribute(\"class\", \"region\"); \n \
		this.div.regionController = this; \n \
		 \n \
		var table = document.createElement(\"table\"); \n \
		table.setAttribute(\"class\", \"region table\"); \n \
	 \n \
		// button_Delete \n \
		this.button_Delete = document.createElement(\"button\"); \n \
		this.button_Delete.innerHTML = \"X\"; \n \
		this.button_Delete.onclick = this.Delete; \n \
		this.button_Delete.setAttribute(\"class\", \"region delete_button\"); \n \
 \n \
		// header \n \
		var tr_header = document.createElement(\"tr\"); \n \
		tr_header.setAttribute(\"class\", \"region header\"); \n \
		this.td_label_header = document.createElement(\"td\"); \n \
		this.td_label_header.innerHTML = \"Region \" + regionIndex; \n \
		this.td_label_header.style=\"font-weight:bold; padding-top: 5px;\"; \n \
		var td_header_delete_button = document.createElement(\"td\"); \n \
		td_header_delete_button.style = \"text-align:right;\"; \n \
		td_header_delete_button.appendChild(this.button_Delete); \n \
		tr_header.appendChild(this.td_label_header); \n \
		tr_header.appendChild(td_header_delete_button); \n \
		table.appendChild(tr_header); \n \
	 \n \
		// textBox_Start \n \
		var tr_textBox_Start = document.createElement(\"tr\"); \n \
		var td_label_textBox_Start = document.createElement(\"td\"); \n \
		td_label_textBox_Start.innerHTML = \"Start Index\"; \n \
		var td_textBox_Start = document.createElement(\"td\"); \n \
		this.textBox_Start = document.createElement(\"input\"); \n \
		this.textBox_Start.setAttribute(\"type\", \"text\"); \n \
		this.textBox_Start.placeholder = \"0\"; \n \
		this.textBox_Start.value = startIndex; \n \
		this.textBox_Start.onchange = this.Update; \n \
		td_textBox_Start.appendChild(this.textBox_Start); \n \
		tr_textBox_Start.appendChild(td_label_textBox_Start); \n \
		tr_textBox_Start.appendChild(td_textBox_Start); \n \
		table.appendChild(tr_textBox_Start); \n \
		 \n \
		// textBox_End \n \
		var tr_textBox_End = document.createElement(\"tr\"); \n \
		var td_label_textBox_End = document.createElement(\"td\"); \n \
		td_label_textBox_End.innerHTML = \"End Index\"; \n \
		var td_textBox_End = document.createElement(\"td\"); \n \
		this.textBox_End = document.createElement(\"input\"); \n \
		this.textBox_End.setAttribute(\"type\", \"text\"); \n \
		this.textBox_End.placeholder = num_leds; \n \
		this.textBox_End.value = endIndex; \n \
		this.textBox_End.onchange = this.Update; \n \
		td_textBox_End.appendChild(this.textBox_End); \n \
		tr_textBox_End.appendChild(td_label_textBox_End); \n \
		tr_textBox_End.appendChild(td_textBox_End); \n \
		table.appendChild(tr_textBox_End); \n \
		 \n \
		// dropdown_Shader \n \
		var tr_dropdown_Shader = document.createElement(\"tr\"); \n \
		var td_label_dropdown_Shader = document.createElement(\"td\"); \n \
		td_label_dropdown_Shader.innerHTML = \"Shader\"; \n \
		var td_dropdown_Shader = document.createElement(\"td\"); \n \
		this.dropdown_Shader = document.createElement(\"select\"); \n \
		for (i = 0; i < max_shaders; i++) \n \
		{ \n \
			var shader_option = document.createElement(\"option\"); \n \
			shader_option.innerHTML = shaderNames[i]; \n \
			this.dropdown_Shader.appendChild(shader_option); \n \
		}		 \n \
		this.dropdown_Shader.selectedIndex = shader; \n \
		this.dropdown_Shader.onchange = this.Update; \n \
		td_dropdown_Shader.appendChild(this.dropdown_Shader); \n \
		tr_dropdown_Shader.appendChild(td_label_dropdown_Shader); \n \
		tr_dropdown_Shader.appendChild(td_dropdown_Shader); \n \
		table.appendChild(tr_dropdown_Shader); \n \
		 \n \
		// slider_Red \n \
		var tr_slider_Red = document.createElement(\"tr\"); \n \
		var td_label_slider_Red = document.createElement(\"td\"); \n \
		td_label_slider_Red.innerHTML = \"Red\"; \n \
		var td_slider_Red = document.createElement(\"td\"); \n \
		this.slider_Red = document.createElement(\"input\"); \n \
		this.slider_Red.setAttribute(\"type\", \"range\"); \n \
		this.slider_Red.min = 0; \n \
		this.slider_Red.max = 255; \n \
		this.slider_Red.value = red; \n \
		this.slider_Red.onchange = this.Update; \n \
		this.slider_Red.oninput = this.UpdateColour; \n \
		this.slider_Red.setAttribute(\"class\", \"region slider red\"); \n \
		td_slider_Red.appendChild(this.slider_Red); \n \
		tr_slider_Red.appendChild(td_label_slider_Red); \n \
		tr_slider_Red.appendChild(td_slider_Red); \n \
		table.appendChild(tr_slider_Red); \n \
		 \n \
		// slider_Green \n \
		var tr_slider_Green = document.createElement(\"tr\"); \n \
		var td_label_slider_Green = document.createElement(\"td\"); \n \
		td_label_slider_Green.innerHTML = \"Green\"; \n \
		var td_slider_Green = document.createElement(\"td\");		 \n \
		this.slider_Green = document.createElement(\"input\"); \n \
		this.slider_Green.setAttribute(\"type\", \"range\"); \n \
		this.slider_Green.min = 0; \n \
		this.slider_Green.max = 255; \n \
		this.slider_Green.value = green; \n \
		this.slider_Green.onchange = this.Update; \n \
		this.slider_Green.oninput = this.UpdateColour; \n \
		this.slider_Green.setAttribute(\"class\", \"region slider green\"); \n \
		td_slider_Green.appendChild(this.slider_Green); \n \
		tr_slider_Green.appendChild(td_label_slider_Green); \n \
		tr_slider_Green.appendChild(td_slider_Green); \n \
		table.appendChild(tr_slider_Green); \n \
		 \n \
		// slider_Blue \n \
		var tr_slider_Blue = document.createElement(\"tr\"); \n \
		var td_label_slider_Blue = document.createElement(\"td\"); \n \
		td_label_slider_Blue.innerHTML = \"Blue\"; \n \
		var td_slider_Blue = document.createElement(\"td\");			 \n \
		this.slider_Blue = document.createElement(\"input\"); \n \
		this.slider_Blue.setAttribute(\"type\", \"range\"); \n \
		this.slider_Blue.min = 0; \n \
		this.slider_Blue.max = 255; \n \
		this.slider_Blue.value = blue; \n \
		this.slider_Blue.onchange = this.Update; \n \
		this.slider_Blue.oninput = this.UpdateColour; \n \
		this.slider_Blue.setAttribute(\"class\", \"region slider blue\"); \n \
		td_slider_Blue.appendChild(this.slider_Blue); \n \
		tr_slider_Blue.appendChild(td_label_slider_Blue); \n \
		tr_slider_Blue.appendChild(td_slider_Blue); \n \
		table.appendChild(tr_slider_Blue); \n \
 \n \
		// mixer \n \
		var tr_mixer = document.createElement(\"tr\"); \n \
		var td_label_mixer = document.createElement(\"td\"); \n \
		this.td_mixer = document.createElement(\"td\"); \n \
		this.td_mixer.style = \"height: 17.5px; background: rgb(\" + this.red + \",\" + this.green + \",\" + this.blue + \");\"; \n \
		tr_mixer.appendChild(td_label_mixer); \n \
		tr_mixer.appendChild(this.td_mixer); \n \
		table.appendChild(tr_mixer); \n \
		 \n \
		this.div.appendChild(table); \n \
	} \n \
	 \n \
	get regionIndex() \n \
	{ \n \
		return this.region; \n \
	} \n \
	 \n \
	set regionIndex(value) \n \
	{ \n \
		this.region = value; \n \
		this.td_label_header.innerHTML = \"Region \" + value; \n \
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
		return this.dropdown_Shader.selectedIndex; \n \
	} \n \
	set shader(value) \n \
	{ \n \
		this.dropdown_Shader.selectedIndex = value; \n \
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
		Http_MakeRequest(\"/api?region-create:\" + this.startIndex + \",\" + this.endIndex + \",\" + this.shader + \",\" + this.red + \",\" + this.green + \",\" + this.blue); \n \
	} \n \
	 \n \
	Update() \n \
	{ \n \
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController; \n \
		Http_MakeRequest(\"/api?region-update:\" + rc.regionIndex + \",\" + rc.startIndex + \",\" + rc.endIndex + \",\" + rc.shader + \",\" + rc.red + \",\" + rc.green + \",\" + rc.blue); \n \
	} \n \
	 \n \
	UpdateColour() \n \
	{ \n \
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController; \n \
		rc.td_mixer.style = \"height: 17.5px; background: rgb(\" + rc.red + \",\" + rc.green + \",\" + rc.blue + \");\"; \n \
	} \n \
	 \n \
	Delete() \n \
	{ \n \
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController; \n \
		Http_MakeRequest(\"/api?region-delete:\" + rc.regionIndex); \n \
		 \n \
		tab_regions.removeChild(rc.div); \n \
 \n \
		regionControllers.splice(regionControllers.indexOf(rc), 1); \n \
	} \n \
	 \n \
	Remove() \n \
	{		 \n \
		tab_regions.removeChild(this.div); \n \
 \n \
		regionControllers.splice(regionControllers.indexOf(this), 1); \n \
	} \n \
} \n";

const char* WEB_INDEX_HTML = "<!DOCTYPE html> \n \
<html> \n \
	<head> \n \
		<meta charset=\"UTF-8\"> \n \
		<title>ESP-IDF</title> \n \
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\"> \n \
		<script src=\"scripts.js\"></script> \n \
		<script src=\"classes.js\"></script> \n \
		 \n \
		<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"> \n \
	</head> \n \
	<body> \n \
		<div class=\"tab bar\"> \n \
			<button class=\"tab button\" id=\"tab_button_controls\" onclick=\"TabControls_Button_OnClick(this, 'tab_controls')\">Controls</button> \n \
			<button class=\"tab button\" id=\"tab_button_regions\" onclick=\"TabControls_Button_OnClick(this, 'tab_regions')\">Regions</button> \n \
			<button class=\"tab button\" id=\"tab_button_anchors\" onclick=\"TabControls_Button_OnClick(this, 'tab_anchors')\">Anchors</button> \n \
		</div> \n \
		<div id=\"tab_container\" class=\"tab container\"> \n \
			<div class=\"tab content\" id=\"tab_controls\"> \n \
				<div> \n \
					<button id=\"controls_button_toggle_power\" class=\"tab content_button\" onclick=\"Controls_Toggle_Power_OnClick()\">Toggle Power</button> \n \
				</div> \n \
			</div> \n \
			<div class=\"tab content\" id=\"tab_regions\"> \n \
				<div> \n \
					<button id=\"regions_button_refresh\" class=\"tab content_button\" onclick=\"Regions_Refresh_OnClick()\">Refresh</button> \n \
					<button id=\"regions_button_add\" class=\"tab content_button\" onclick=\"Regions_Add_OnClick()\">Add</button> \n \
					<button id=\"regions_button_clear\" class=\"tab content_button\" onclick=\"Regions_Clear_OnClick()\">Clear</button> \n \
				</div> \n \
			</div> \n \
		</div> \n \
		<script>Page_OnLoad();</script> \n \
	</body> \n \
</html> \n";

const char* WEB_SCRIPTS_JS = "var tab_control_regions; \n \
var tab_control_anchors; \n \
 \n \
var tab_regions; \n \
var tab_anchors; \n \
 \n \
var regionControllers = []; \n \
var anchorControllers = []; \n \
 \n \
var shaderNames = []; \n \
 \n \
var max_regions = 0; \n \
var max_anchors = 0; \n \
var max_shaders = 0; \n \
 \n \
var num_leds = 0; \n \
 \n \
var region_load_progress = 0; \n \
var anchor_load_progress = 0; \n \
 \n \
function Page_OnLoad() \n \
{ \n \
	tab_control_regions = document.getElementById(\"tab_control_regions\"); \n \
	 \n \
	tab_regions = document.getElementById(\"tab_regions\"); \n \
	 \n \
	Http_MakeRequest(\"/api?shader-get_names\"); \n \
	Http_MakeRequest(\"/api?config-num_leds\"); \n \
	Http_MakeRequest(\"/api?region-get_max\"); \n \
	 \n \
	document.getElementById(\"tab_button_controls\").click(); \n \
} \n \
 \n \
/**************************************************************** \n \
 * HTTP \n \
 ****************************************************************/ \n \
function Http_Response_Region(response) \n \
{ \n \
	switch (response[1]) \n \
	{ \n \
		case \"get_max\": \n \
			max_regions = response[2]; \n \
			region_load_progress = 0; \n \
			 \n \
			Http_MakeRequest(\"api?region-get:0\"); \n \
			break; \n \
		case \"get\": \n \
			region_load_progress++; \n \
			if (region_load_progress == max_regions) \n \
			{ \n \
				document.getElementById(\"regions_button_refresh\").style = \"\"; \n \
			} \n \
			else \n \
			{ \n \
				var prog = (region_load_progress / max_regions) * 100; \n \
				document.getElementById(\"regions_button_refresh\").style = \"background: linear-gradient(90deg, rgb(90,130,150) \" + prog + \"%, rgb(80,80,80) \" + prog + \"%);\"; \n \
				if (response[2] != \"null\") \n \
				{ \n \
					var rc = new RegionController(response[2], response[3], response[4], response[5], response[6], response[7], response[8]); \n \
					regionControllers.push(rc); \n \
					tab_regions.appendChild(rc.div); \n \
					rc.div.id = \"region-\" + response[2]; \n \
				} \n \
				Http_MakeRequest(\"api?region-get:\" + region_load_progress); \n \
			} \n \
			break; \n \
		case \"create\": \n \
			for (i = 0; i < regionControllers.length; i++) \n \
			{ \n \
				if (regionControllers[i].regionIndex === null) \n \
				{ \n \
					console.log(\"Updated region index of region controller \" + i + \" to \" + response[2]);  \n \
					regionControllers[i].regionIndex = response[2]; \n \
					regionControllers[i].div.id = \"region-\" + response[2]; \n \
					tab_regions.appendChild(regionControllers[i].div); \n \
					document.getElementById(\"regions_button_add\").disabled = false; \n \
				} \n \
			} \n \
			break; \n \
		default: \n \
			break; \n \
	} \n \
} \n \
 \n \
function Http_Response_Shader(response) \n \
{ \n \
	switch (response[1]) \n \
	{ \n \
		case \"get_names\": \n \
			max_shaders = response[2]; \n \
			 \n \
			for (i = 0; i < max_shaders; i++) \n \
			{ \n \
				shaderNames.push(response[i + 3]); \n \
			} \n \
			break; \n \
		default: \n \
			break; \n \
	} \n \
} \n \
 \n \
function Http_Response_Config(response) \n \
{ \n \
	switch (response[1]) \n \
	{ \n \
		case \"num_leds\": \n \
			num_leds = response[2]; \n \
			break; \n \
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
		case \"shader\": \n \
			Http_Response_Shader(response); \n \
			break; \n \
		case \"config\": \n \
			Http_Response_Config(response); \n \
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
function TabControls_Button_OnClick(sender, name) \n \
{ \n \
	var tab_contents = document.getElementsByClassName(\"tab content\"); \n \
	for (i = 0; i < tab_contents.length; i++) \n \
	{ \n \
		tab_contents[i].style.display = \"none\"; \n \
	} \n \
	document.getElementById(name).style.display = \"block\"; \n \
	 \n \
	var tab_buttons = document.getElementsByClassName(\"tab button\"); \n \
	for (i = 0; i < tab_buttons.length; i++) \n \
	{ \n \
		tab_buttons[i].disabled = false; \n \
		tab_buttons[i].classList.remove(\"active\"); \n \
	} \n \
	sender.disabled = true; \n \
	sender.classList.add(\"active\"); \n \
} \n \
 \n \
/**************************************************************** \n \
 * Controls \n \
 ****************************************************************/ \n \
function Controls_Toggle_Power_OnClick() \n \
{ \n \
	Http_MakeRequest(\"/api?config-toggle_power\"); \n \
} \n \
 \n \
/**************************************************************** \n \
 * Regions \n \
 ****************************************************************/ \n \
function Regions_Add_OnClick() \n \
{ \n \
	if (regionControllers.length < max_regions) \n \
	{ \n \
		var rc = new RegionController(null, 0, 0, 0, 0, 0, 0); \n \
		 \n \
		rc.Create(); \n \
		 \n \
		regionControllers.push(rc); \n \
		 \n \
		document.getElementById(\"regions_button_add\").disabled = true; \n \
	} \n \
} \n \
 \n \
function Regions_Refresh_OnClick() \n \
{ \n \
	var len = regionControllers.length; \n \
	for (i = 0; i < len; i++) \n \
	{ \n \
		regionControllers[0].Remove(); \n \
	} \n \
	 \n \
	Http_MakeRequest(\"/api?region-get_max\"); \n \
} \n \
 \n \
function Regions_Clear_OnClick() \n \
{ \n \
	var len = regionControllers.length; \n \
	for (i = 0; i < len; i++) \n \
	{ \n \
		regionControllers[0].Remove(); \n \
	} \n \
	 \n \
	Http_MakeRequest(\"/api?region-clear\"); \n \
} \n";

const char* WEB_STYLE_CSS = ":root { \n \
	--color-text: rgb(255,255,255); \n \
	--color-page-header: rgb(32,113,154); \n \
	--color-page-background: rgb(45,45,45); \n \
	--color-sidebar-background: rgb(80,80,80); \n \
	--color-button-outline: rgb(120,120,120); \n \
	--color-button-background: rgb(80,80,80); \n \
	--color-item-header: rgb(80,80,80); \n \
	--color-item-background: rgb(53,53,53); \n \
	--color-input: rgb(32,33,36); \n \
} \n \
 \n \
body { \n \
	color: var(--color-text); \n \
	background: var(--color-page-background); \n \
	font-family: sans-serif; \n \
	margin: 0px; \n \
} \n \
 \n \
input[type=\"text\"] { \n \
	color: var(--color-text); \n \
	background: var(--color-input); \n \
	border: none; \n \
	border-radius: 100px; \n \
	padding: 3px; \n \
	padding-left: 5px; \n \
	width: 97%; \n \
	height:17.5px; \n \
} \n \
 \n \
select { \n \
	color: var(--color-text); \n \
	background: var(--color-input); \n \
	border: none; \n \
	padding: 3px; \n \
	border-radius: 100px; \n \
	width: 100%; \n \
} \n \
 \n \
.tab.bar { \n \
	background: var(--color-page-header); \n \
	padding-left: 10px; \n \
	height: 50px; \n \
	box-shadow: 0px 0px 5px 3px rgba(0,0,0,0.75); \n \
} \n \
 \n \
.tab.button { \n \
	color: var(--color-text); \n \
	background: rgba(255,255,255,0); \n \
	border: none; \n \
	font-family: sans-serif; \n \
	font-size: 18px; \n \
	margin: 0px; \n \
	height: 50px; \n \
} \n \
 \n \
.tab.button.active { \n \
	background: rgba(255,255,255,0.25); \n \
} \n \
 \n \
.tab.content_button { \n \
	color: var(--color-text); \n \
	background: var(--color-button-background); \n \
	border: 2px solid var(--color-button-outline); \n \
	height: 30px; \n \
	border-radius: 100px; \n \
	margin-top: 20px; \n \
	margin-left:20px; \n \
	margin-bottom: 5px; \n \
	font-size: 15px; \n \
	padding-left: 10px; \n \
	padding-right: 10px; \n \
} \n \
 \n \
.region.table { \n \
	background: linear-gradient(0deg, var(--color-item-background) 83%, var(--color-item-header) 83%); \n \
	margin: 20px; \n \
	margin-left: 10px; \n \
	padding: 5px; \n \
	border-radius: 5px; \n \
	box-shadow: 0px 0px 22px 0px rgba(0,0,0,0.5); \n \
	max-width: 500px; \n \
	width: 95%; \n \
} \n \
.region.header { \n \
	height: 40px; \n \
	vertical-align: top; \n \
} \n \
.region.delete_button { \n \
	color: var(--color-text); \n \
	background: var(--color-button-background); \n \
	border: 2px solid var(--color-button-outline); \n \
	width: 25px; \n \
	height: 25px; \n \
	border-radius: 100px; \n \
	text-align: center; \n \
} \n \
.region.slider { \n \
	-webkit-appearance: none; \n \
	border-radius: 100px; \n \
	width: 98%; \n \
} \n \
.region.slider.red { \n \
	background: linear-gradient(to right, rgb(0,0,0), rgb(255,0,0) ); \n \
} \n \
.region.slider.green { \n \
	background: linear-gradient(to right, rgb(0,0,0), rgb(0,255,0) ); \n \
} \n \
.region.slider.blue { \n \
	background: linear-gradient(to right, rgb(0,0,0), rgb(0,0,255) ); \n \
} \n";

/****************************************************************
 * Function definitions
 ****************************************************************/
esp_err_t web_classes_js_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
	httpd_resp_send(req, WEB_CLASSES_JS, strlen(WEB_CLASSES_JS));
	return ESP_OK;
}

esp_err_t web_index_html_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
	httpd_resp_send(req, WEB_INDEX_HTML, strlen(WEB_INDEX_HTML));
	return ESP_OK;
}

esp_err_t web_scripts_js_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
	httpd_resp_send(req, WEB_SCRIPTS_JS, strlen(WEB_SCRIPTS_JS));
	return ESP_OK;
}

esp_err_t web_style_css_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
	httpd_resp_send(req, WEB_STYLE_CSS, strlen(WEB_STYLE_CSS));
	return ESP_OK;
}

void WebData_Populate(httpd_handle_t server)
{
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/classes.js", .method = HTTP_GET, .handler = web_classes_js_handler, .user_ctx = NULL });
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/index.html", .method = HTTP_GET, .handler = web_index_html_handler, .user_ctx = NULL });
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/scripts.js", .method = HTTP_GET, .handler = web_scripts_js_handler, .user_ctx = NULL });
	httpd_register_uri_handler(server, &(httpd_uri_t){ "/style.css", .method = HTTP_GET, .handler = web_style_css_handler, .user_ctx = NULL });
}