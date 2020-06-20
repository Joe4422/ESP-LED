var slider_red;
var slider_green;
var slider_blue;

var tab_regions;
var tab_anchors;

function Page_OnLoad()
{
	slider_red = document.getElementById("red");
	slider_green = document.getElementById("green");
	slider_blue = document.getElementById("blue");
	
	slider_red.onchange = updateColours;
	slider_green.onchange = updateColours;
	slider_blue.onchange = updateColours;
	
	tab_regions = document.getElementById("tab_regions");
	tab_anchors = document.getElementById("tab_anchors");
}

function updateColours()
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open("GET", "/write?all-" + slider_red.value + "-" + slider_green.value + "-" + slider_blue.value, true);
	
	xmlHttp.send(null);
}

function Http_MakeRequest(request)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open("GET", request, true);
	
	xmlHttp.send(null);
}

/****************************************************************
 * Tab controls
 ****************************************************************/
function TabControls_Button_Regions_OnClick()
{
	tab_regions.style.display = "block";
	
	tab_anchors.style.display = "none";
}

function TabControls_Button_Anchors_OnClick()
{
	tab_anchors.style.display = "block";
	
	tab_regions.style.display = "none";
}