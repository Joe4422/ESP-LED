var tab_control_regions;
var tab_control_anchors;

var tab_regions;
var tab_anchors;

var regionControllers = [];
var anchorControllers = [];

var shaderNames = [];

var max_regions = 0;
var max_anchors = 0;
var max_shaders = 0;

var num_leds = 0;

var region_load_progress = 0;
var anchor_load_progress = 0;

function Page_OnLoad()
{
	tab_control_regions = document.getElementById("tab_control_regions");
	tab_control_anchors = document.getElementById("tab_control_anchors");
	
	tab_regions = document.getElementById("tab_regions");
	tab_anchors = document.getElementById("tab_anchors");
	
	Http_MakeRequest("/api?shader-get_names");
	Http_MakeRequest("/api?config-num_leds");
	Http_MakeRequest("/api?region-get_max");
	Http_MakeRequest("/api?anchor-get_max");
	
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
			
			Http_MakeRequest("api?region-get:0");
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
				if (response[2] != "null")
				{
					var rc = new RegionController(response[2], response[3], response[4], response[5], response[6], response[7], response[8]);
					regionControllers.push(rc);
					tab_regions.appendChild(rc.div);
					rc.div.id = "region-" + response[2];
				}
				Http_MakeRequest("api?region-get:" + region_load_progress);
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
			break;
		default:
			break;
	}
}

function Http_Response_Shader(response)
{
	switch (response[1])
	{
		case "get_names":
			max_shaders = response[2];
			
			for (i = 0; i < max_shaders; i++)
			{
				shaderNames.push(response[i + 3]);
			}
			break;
		default:
			break;
	}
}

function Http_Response_Config(response)
{
	switch (response[1])
	{
		case "num_leds":
			num_leds = response[2];
			break;
		default:
			break;
	}
}

function Http_Response_Anchor(response)
{
	switch (response[1])
	{
		case "get_max":
			max_anchors = response[2];
			anchor_load_progress = 0;
			
			Http_MakeRequest("api?anchor-get:0");
			break;
		case "get":
			anchor_load_progress++;
			if (anchor_load_progress == max_anchors)
			{
				document.getElementById("anchors_button_refresh").style = "";
			}
			else
			{
				var prog = (anchor_load_progress / max_anchors) * 100;
				document.getElementById("anchors_button_refresh").style = "background: linear-gradient(90deg, rgb(90,130,150) " + prog + "%, rgb(80,80,80) " + prog + "%);";
				if (response[2] != "null")
				{
					var ac = new AnchorController(response[2], response[3], response[4]);
					anchorControllers.push(ac);
					tab_anchors.appendChild(ac.div);
					ac.div.id = "anchor-" + response[2];
				}
				Http_MakeRequest("api?anchor-get:" + anchor_load_progress);
			}
			break;
		case "create":
			for (i = 0; i < anchorControllers.length; i++)
			{
				if (anchorControllers[i].anchorIndex === null)
				{
					console.log("Updated anchor index of anchor controller " + i + " to " + response[2]); 
					anchorControllers[i].anchorIndex = response[2];
					anchorControllers[i].div.id = "anchor-" + response[2];
					tab_anchors.appendChild(anchorControllers[i].div);
					document.getElementById("anchors_button_add").disabled = false;
				}
			}
			break;
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
		case "config":
			Http_Response_Config(response);
			break;
		case "anchor":
			Http_Response_Anchor(response);
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
	Http_MakeRequest("/api?config-toggle_power");
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
	
	Http_MakeRequest("/api?region-get_max");
}

function Regions_Clear_OnClick()
{
	var len = regionControllers.length;
	for (i = 0; i < len; i++)
	{
		regionControllers[0].Remove();
	}
	
	Http_MakeRequest("/api?region-clear");
}

/****************************************************************
 * Anchors
 ****************************************************************/
function Anchors_Add_OnClick()
{
	if (anchorControllers.length < max_anchors)
	{
		var ac = new AnchorController(null, "-", 0);
		
		ac.Create();
		
		anchorControllers.push(ac);
		
		document.getElementById("anchors_button_add").disabled = true;
	}
}

function Anchors_Refresh_OnClick()
{
	var len = anchorControllers.length;
	for (i = 0; i < len; i++)
	{
		anchorControllers[0].Remove();
	}
	
	Http_MakeRequest("/api?anchor-get_max");
}

function Anchors_Clear_OnClick()
{
	var len = anchorControllers.length;
	for (i = 0; i < len; i++)
	{
		anchorControllers[0].Remove();
	}
	
	Http_MakeRequest("/api?anchor-clear");
}