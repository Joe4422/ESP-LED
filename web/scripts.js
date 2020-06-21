var tab_control_regions;
var tab_control_anchors;

var tab_regions;
var tab_anchors;

var regionControllers = [];

var shaderNames = [];

var max_regions = 0;
var max_shaders = 0;

var region_load_progress = 0;

function Page_OnLoad()
{
	tab_control_regions = document.getElementById("tab_control_regions");
	tab_control_anchors = document.getElementById("tab_control_anchors");
	
	tab_regions = document.getElementById("tab_regions");
	tab_anchors = document.getElementById("tab_anchors");
	
	Http_MakeRequest("/shader?get_shaders");
	Http_MakeRequest("/region?get_max");
	
	document.getElementById("tab_button_controls").click();
}

/****************************************************************
 * HTTP
 ****************************************************************/
function Http_Response_Region(response)
{
	switch (response[1])
	{
		case "get_max":
			max_regions = response[2];
			region_load_progress = 0;
			
			for (i = 0; i < max_regions; i++)
			{
				Http_MakeRequest("/region?get-" + i);
			}
			break;
		case "get":
			region_load_progress++;
			if (region_load_progress == max_regions)
			{
				document.getElementById("regions_button_refresh").style = "";
			}
			else
			{
				var prog = (region_load_progress / max_regions) * 100;
				document.getElementById("regions_button_refresh").style = "background: linear-gradient(90deg, rgb(90,130,150) " + prog + "%, rgb(80,80,80) " + prog + "%);";
			}
			if (response[2] != "null")
			{
				var rc = new RegionController(response[2], response[3], response[4], response[5], response[6], response[7], response[8]);
				regionControllers.push(rc);
				tab_regions.appendChild(rc.div);
				rc.div.id = "region-" + response[2];
			}
			break;
		case "create":
			for (i = 0; i < regionControllers.length; i++)
			{
				if (regionControllers[i].regionIndex === null)
				{
					console.log("Updated region index of region controller " + i + " to " + response[2]); 
					regionControllers[i].regionIndex = response[2];
					regionControllers[i].div.id = "region-" + response[2];
					tab_regions.appendChild(regionControllers[i].div);
					document.getElementById("regions_button_add").disabled = false;
				}
			}
		default:
			break;
	}
}

function Http_Response_Shader(response)
{
	switch (response[1])
	{
		case "get_shaders":
			max_shaders = response[2];
			
			for (i = 0; i < max_shaders; i++)
			{
				shaderNames.push(response[i + 3]);
			}
		default:
			break;
	}
}
 
function Http_RequestEventListener()
{
	response = this.responseText.split(" ");
	console.log(response);
	
	switch (response[0])
	{
		case "region":
			Http_Response_Region(response);
			break;
		case "shader":
			Http_Response_Shader(response);
			break;
		default:
			break;
	}
}

function Http_MakeRequest(request)
{
	var xmlHttp = new XMLHttpRequest();
	
	console.log("Requesting " + request);
	
	xmlHttp.addEventListener("load", Http_RequestEventListener);
	xmlHttp.open("GET", request);
	
	xmlHttp.send();
}

/****************************************************************
 * Tab controls
 ****************************************************************/
function TabControls_Button_OnClick(sender, name)
{
	var tab_contents = document.getElementsByClassName("tab content");
	for (i = 0; i < tab_contents.length; i++)
	{
		tab_contents[i].style.display = "none";
	}
	document.getElementById(name).style.display = "block";
	
	var tab_buttons = document.getElementsByClassName("tab button");
	for (i = 0; i < tab_buttons.length; i++)
	{
		tab_buttons[i].disabled = false;
		tab_buttons[i].classList.remove("active");
	}
	sender.disabled = true;
	sender.classList.add("active");
}

/****************************************************************
 * Controls
 ****************************************************************/
function Controls_Toggle_Power_OnClick()
{
	Http_MakeRequest("config?toggle_power");
}

/****************************************************************
 * Regions
 ****************************************************************/
function Regions_Add_OnClick()
{
	if (regionControllers.length < max_regions)
	{
		var rc = new RegionController(null, 0, 0, 0, 0, 0, 0);
		
		rc.Create();
		
		regionControllers.push(rc);
		
		document.getElementById("regions_button_add").disabled = true;
	}
}

function Regions_Refresh_OnClick()
{
	var len = regionControllers.length;
	for (i = 0; i < len; i++)
	{
		regionControllers[0].Remove();
	}
	
	Http_MakeRequest("/region?get_max");
}

function Regions_Clear_OnClick()
{
	var len = regionControllers.length;
	for (i = 0; i < len; i++)
	{
		regionControllers[0].Remove();
	}
	
	Http_MakeRequest("/region?clear");
}