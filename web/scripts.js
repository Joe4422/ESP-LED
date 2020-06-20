var tab_control_regions;
var tab_control_anchors;

var tab_regions;
var tab_anchors;

var regionControllers = [];

function Page_OnLoad()
{
	tab_control_regions = document.getElementById("tab_control_regions");
	tab_control_anchors = document.getElementById("tab_control_anchors");
	
	tab_regions = document.getElementById("tab_regions");
	tab_anchors = document.getElementById("tab_anchors");
}

function Http_MakeRequest(request)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open("GET", request, true);
	
	xmlHttp.send(null);
}

/****************************************************************
 * Classes
 ****************************************************************/
function RegionController(textBox_Start, textBox_End, ticker_Shader, slider_Red, slider_Green, slider_Blue)
{
	this.textBox_Start = textBox_Start;
	this.textBox_End = textBox_End;
	this.ticker_Shader = ticker_Shader;
	this.slider_Red = slider_Red;
	this.slider_Green = slider_Green;
	this.slider_Blue = slider_Blue;
}

/****************************************************************
 * Tab controls
 ****************************************************************/
function TabControls_Button_Regions_OnClick()
{
	tab_regions.style.display = "block";
	tab_control_regions.disabled = true;
	
	tab_anchors.style.display = "none";
	tab_control_anchors.disabled = false;
}

function TabControls_Button_Anchors_OnClick()
{
	tab_anchors.style.display = "block";
	tab_control_anchors.disabled = true;
	
	tab_regions.style.display = "none";
	tab_control_regions.disabled = false;
}

/****************************************************************
 * Regions
 ****************************************************************/
function Regions_CreateRegionController()
{
	var div = document.createElement("div");
	
	// textBox_Start
	var div_textBox_Start = document.createElement("div");
	var label_textBox_Start = document.createElement("label");
	label_textBox_Start.innerHTML = "Start Index:";
	var textBox_Start = document.createElement("input");
	textBox_Start.setAttribute("type", "text");
	textBox_Start.placeholder = "0";
	div_textBox_Start.appendChild(label_textBox_Start);
	div_textBox_Start.appendChild(textBox_Start);
	
	// textBox_End
	var div_textBox_End = document.createElement("div");
	var label_textBox_End = document.createElement("label");
	label_textBox_End.innerHTML = "End Index:";
	var textBox_End = document.createElement("input");
	textBox_End.setAttribute("type", "text");
	textBox_End.placeholder = "149";
	div_textBox_End.appendChild(label_textBox_End);
	div_textBox_End.appendChild(textBox_End);
	
	// ticker_Shader
	var div_ticker_Shader = document.createElement("div");
	var label_ticker_Shader = document.createElement("label");
	label_ticker_Shader.innerHTML = "Shader:";
	var ticker_Shader = document.createElement("input");
	ticker_Shader.setAttribute("type", "number");
	ticker_Shader.max = 3;
	ticker_Shader.min = 0;
	ticker_Shader.value = 0;
	div_ticker_Shader.appendChild(label_ticker_Shader);
	div_ticker_Shader.appendChild(ticker_Shader);
	
	// slider_Red
	var div_slider_Red = document.createElement("div");
	var label_slider_Red = document.createElement("label");
	label_slider_Red.innerHTML = "Red:";
	var slider_Red = document.createElement("input");
	slider_Red.setAttribute("type", "range");
	slider_Red.min = 0;
	slider_Red.max = 255;
	slider_Red.value = 0;
	div_slider_Red.appendChild(label_slider_Red);
	div_slider_Red.appendChild(slider_Red);
	
	// slider_Green
	var div_slider_Green = document.createElement("div");
	var label_slider_Green = document.createElement("label");
	label_slider_Green.innerHTML = "Green:";
	var slider_Green = document.createElement("input");
	slider_Green.setAttribute("type", "range");
	slider_Green.min = 0;
	slider_Green.max = 255;
	slider_Green.value = 0;
	div_slider_Green.appendChild(label_slider_Green);
	div_slider_Green.appendChild(slider_Green);
	
	// slider_Blue
	var div_slider_Blue = document.createElement("div");
	var label_slider_Blue = document.createElement("label");
	label_slider_Blue.innerHTML = "Blue:";
	var slider_Blue = document.createElement("input");
	slider_Blue.setAttribute("type", "range");
	slider_Blue.min = 0;
	slider_Blue.max = 255;
	slider_Blue.value = 0;
	div_slider_Blue.appendChild(label_slider_Blue);
	div_slider_Blue.appendChild(slider_Blue);
	
	// button_Save
	var button_Save = document.createElement("button");
	button_Save.innerHTML = "Save";
	button_Save.setAttribute("onclick", "Regions_RegionController_Save_OnClick(" + regionControllers.length + ");");
	
	div.appendChild(div_textBox_Start);
	div.appendChild(div_textBox_End);
	div.appendChild(div_ticker_Shader);
	div.appendChild(div_slider_Red);
	div.appendChild(div_slider_Green);
	div.appendChild(div_slider_Blue);
	div.appendChild(button_Save);
	
	var regionController = new RegionController(textBox_Start, textBox_End, ticker_Shader, slider_Red, slider_Green, slider_Blue);
	
	regionControllers.push(regionController);
	
	return div;
}

function Regions_RegionController_Save_OnClick(index)
{
	var rc = regionControllers[index];
	
	Http_MakeRequest("/region?create-" + rc.textBox_Start.value + "-" + rc.textBox_End.value + "-" + rc.ticker_Shader.value + "-" + rc.slider_Red.value + "-" + rc.slider_Green.value + "-" + rc.slider_Blue.value);
}
 
function Regions_AddRegion_OnClick()
{
	tab_regions.appendChild(Regions_CreateRegionController());
}