var tab_control_regions;
var tab_control_anchors;

var tab_regions;
var tab_anchors;

var regionControllers = [];

var max_regions = 0;

function Page_OnLoad()
{
	tab_control_regions = document.getElementById("tab_control_regions");
	tab_control_anchors = document.getElementById("tab_control_anchors");
	
	tab_regions = document.getElementById("tab_regions");
	tab_anchors = document.getElementById("tab_anchors");
	
	Http_MakeRequest("/region?get_max");
}

/****************************************************************
 * HTTP
 ****************************************************************/
function Http_Response_Region(response)
{
	switch (response[1])
	{
		case "max":
			max_regions = response[2];
			
			for (i = 0; i < max_regions; i++)
			{
				Http_MakeRequest("/region?get-" + i);
			}
			break;
		case "data":
			if (response[2] != "null")
			{
				var rc = new RegionController(response[2], response[3], response[4], response[5], response[6], response[7], response[8]);
				regionControllers.push(rc);
				tab_regions.appendChild(rc.div);
				rc.div.id = "region-" + response[2];
			}
			break;
		case "created":
			for (i = 0; i < regionControllers.length; i++)
			{
				if (regionControllers[i].regionIndex === null)
				{
					console.log("Updated region index of region controller " + i + " to " + response[2]); 
					regionControllers[i].regionIndex = response[2];
					regionControllers[i].div.id = "region-" + response[2];
				}
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
function Regions_RegionController_Save_OnClick(index)
{
	var rc = regionControllers[index];
	
	rc.Update();
}

function Regions_RegionController_Delete_OnClick(index)
{
	var rc = regionControllers[index];
	
	rc.Delete();
	
	tab_regions.removeChild(rc.div);
	
	regionControllers.splice(index, 1);
	
	for (i = index; i < regionControllers.length; i++)
	{
		regionControllers[i].SetIndex(i);
	}
}
 
function Regions_AddRegion_OnClick()
{
	if (regionControllers.length < max_regions)
	{
		var rc = new RegionController(null, 0, 0, 0, 0, 0, 0);
		
		rc.Create();
		
		regionControllers.push(rc);
		
		tab_regions.appendChild(rc.div);
	}
}