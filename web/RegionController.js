class RegionController
{
	constructor(regionIndex, startIndex, endIndex, shader, red, green, blue)
	{
		this.regionIndex = regionIndex;
		
		this.div = document.createElement("div");
		this.div.style = "border:1px solid black;";
	
		// textBox_Start
		var div_textBox_Start = document.createElement("div");
		var label_textBox_Start = document.createElement("label");
		label_textBox_Start.innerHTML = "Start Index:";
		this.textBox_Start = document.createElement("input");
		this.textBox_Start.setAttribute("type", "text");
		this.textBox_Start.placeholder = "0";
		this.textBox_Start.value = startIndex;
		this.textBox_Start.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_textBox_Start.appendChild(label_textBox_Start);
		div_textBox_Start.appendChild(this.textBox_Start);
		
		// textBox_End
		var div_textBox_End = document.createElement("div");
		var label_textBox_End = document.createElement("label");
		label_textBox_End.innerHTML = "End Index:";
		this.textBox_End = document.createElement("input");
		this.textBox_End.setAttribute("type", "text");
		this.textBox_End.placeholder = "149";
		this.textBox_End.value = endIndex;
		this.textBox_End.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_textBox_End.appendChild(label_textBox_End);
		div_textBox_End.appendChild(this.textBox_End);
		
		// dropdown_Shader
		var div_dropdown_Shader = document.createElement("div");
		var label_dropdown_Shader = document.createElement("label");
		label_dropdown_Shader.innerHTML = "Shader:";
		this.dropdown_Shader = document.createElement("select");
		for (i = 0; i < max_shaders; i++)
		{
			var shader_option = document.createElement("option");
			shader_option.innerHTML = shaderNames[i];
			this.dropdown_Shader.appendChild(shader_option);
		}		
		this.dropdown_Shader.selectedIndex = shader;
		this.dropdown_Shader.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_dropdown_Shader.appendChild(label_dropdown_Shader);
		div_dropdown_Shader.appendChild(this.dropdown_Shader);
		
		// slider_Red
		var div_slider_Red = document.createElement("div");
		var label_slider_Red = document.createElement("label");
		label_slider_Red.innerHTML = "Red:";
		this.slider_Red = document.createElement("input");
		this.slider_Red.setAttribute("type", "range");
		this.slider_Red.min = 0;
		this.slider_Red.max = 255;
		this.slider_Red.value = red;
		this.slider_Red.onchange = this.Update;
		this.slider_Red.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_slider_Red.appendChild(label_slider_Red);
		div_slider_Red.appendChild(this.slider_Red);
		
		// slider_Green
		var div_slider_Green = document.createElement("div");
		var label_slider_Green = document.createElement("label");
		label_slider_Green.innerHTML = "Green:";
		this.slider_Green = document.createElement("input");
		this.slider_Green.setAttribute("type", "range");
		this.slider_Green.min = 0;
		this.slider_Green.max = 255;
		this.slider_Green.value = green;
		this.slider_Green.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_slider_Green.appendChild(label_slider_Green);
		div_slider_Green.appendChild(this.slider_Green);
		
		// slider_Blue
		var div_slider_Blue = document.createElement("div");
		var label_slider_Blue = document.createElement("label");
		label_slider_Blue.innerHTML = "Blue:";
		this.slider_Blue = document.createElement("input");
		this.slider_Blue.setAttribute("type", "range");
		this.slider_Blue.min = 0;
		this.slider_Blue.max = 255;
		this.slider_Blue.value = blue;
		this.slider_Blue.setAttribute("onchange", "Regions_RegionController_Form_OnChange(" + regionControllers.length + ");");
		div_slider_Blue.appendChild(label_slider_Blue);
		div_slider_Blue.appendChild(this.slider_Blue);
		
		// button_Delete
		this.button_Delete = document.createElement("button");
		this.button_Delete.innerHTML = "Delete";
		this.button_Delete.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + regionControllers.length + ");");
		
		this.div.appendChild(div_textBox_Start);
		this.div.appendChild(div_textBox_End);
		this.div.appendChild(div_dropdown_Shader);
		this.div.appendChild(div_slider_Red);
		this.div.appendChild(div_slider_Green);
		this.div.appendChild(div_slider_Blue);
		this.div.appendChild(this.button_Delete);
	}
	
	get startIndex()
	{
		return this.textBox_Start.value;
	}
	set startIndex(value)
	{
		this.textBox_Start.value = value;
	}
	
	get endIndex()
	{
		return this.textBox_End.value;
	}
	set endIndex(value)
	{
		this.textBox_End.value = value;
	}
	
	get shader()
	{
		return this.dropdown_Shader.selectedIndex;
	}
	set shader(value)
	{
		this.dropdown_Shader.selectedIndex = value;
	}
	
	get red()
	{
		return this.slider_Red.value;
	}
	set red(value)
	{
		this.slider_Red.value = value;
	}
	
	get green()
	{
		return this.slider_Green.value;
	}
	set green(value)
	{
		this.slider_Green.value = value;
	}
	
	get blue()
	{
		return this.slider_Blue.value;
	}
	set blue(value)
	{
		this.slider_Blue.value = value;
	}
	
	Create()
	{
		Http_MakeRequest("/region?create-" + this.startIndex + "-" + this.endIndex + "-" + this.shader + "-" + this.red + "-" + this.green + "-" + this.blue);
	}
	
	Update()
	{
		Http_MakeRequest("/region?update-" + this.regionIndex + "-" + this.startIndex + "-" + this.endIndex + "-" + this.shader + "-" + this.red + "-" + this.green + "-" + this.blue);
	}
	
	Delete()
	{
		Http_MakeRequest("/region?delete-" + this.regionIndex);
	}
	
	SetIndex(index)
	{
		this.textBox_Start.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.textBox_End.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.dropdown_Shader.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.slider_Red.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.slider_Green.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.slider_Blue.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
		this.button_Delete.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
	}
}