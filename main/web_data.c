/****************************************************************
 * Includes
 ****************************************************************/
#include "web_data.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
const char* WEB_INDEX_HTML = "<head> \
	<script src=\"scripts.js\"></script> \
</head> \
<body> \
	<div id=\"tab_control\"> \
		<button onclick=\"TabControls_Button_Regions_OnClick()\">Regions</button> \
		<button onclick=\"TabControls_Button_Anchors_OnClick()\">Anchors</button> \
	</div> \
	<div> \
		<input type=\"range\" min=\"0\" max=\"255\" value=\"128\" id=\"red\" style=\"background:#FF0000;\"> \
		<input type=\"range\" min=\"0\" max=\"255\" value=\"128\" id=\"green\" style=\"background:#00FF00;\"> \
		<input type=\"range\" min=\"0\" max=\"255\" value=\"128\" id=\"blue\" style=\"background:#0000FF;\"> \
	</div> \
	<div id=\"tab_regions\"> \
		Regions \
	</div> \
	<div id=\"tab_anchors\" style=\"display:none;\"> \
		Anchors \
	</div> \
	<script>Page_OnLoad();</script> \
</body>";

const char* WEB_SCRIPTS_JS = "var slider_red; \
var slider_green; \
var slider_blue; \
 \
var tab_regions; \
var tab_anchors; \
 \
function Page_OnLoad() \
{ \
	slider_red = document.getElementById(\"red\"); \
	slider_green = document.getElementById(\"green\"); \
	slider_blue = document.getElementById(\"blue\"); \
	 \
	slider_red.onchange = updateColours; \
	slider_green.onchange = updateColours; \
	slider_blue.onchange = updateColours; \
	 \
	tab_regions = document.getElementById(\"tab_regions\"); \
	tab_anchors = document.getElementById(\"tab_anchors\"); \
} \
 \
function updateColours() \
{ \
	var xmlHttp = new XMLHttpRequest(); \
	xmlHttp.open(\"GET\", \"/write?all-\" + slider_red.value + \"-\" + slider_green.value + \"-\" + slider_blue.value, true); \
	 \
	xmlHttp.send(null); \
} \
 \
function Http_MakeRequest(request) \
{ \
	var xmlHttp = new XMLHttpRequest(); \
	xmlHttp.open(\"GET\", request, true); \
	 \
	xmlHttp.send(null); \
} \
 \
/**************************************************************** \
 * Tab controls \
 ****************************************************************/ \
function TabControls_Button_Regions_OnClick() \
{ \
	tab_regions.style.display = \"block\"; \
	 \
	tab_anchors.style.display = \"none\"; \
} \
 \
function TabControls_Button_Anchors_OnClick() \
{ \
	tab_anchors.style.display = \"block\"; \
	 \
	tab_regions.style.display = \"none\"; \
}";

