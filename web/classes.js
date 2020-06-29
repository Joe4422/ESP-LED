class RegionController
{
	constructor(regionIndex, startIndex, endIndex, shader, red, green, blue)
	{
		this.region = regionIndex;
		
		this.div = document.createElement("div");
		this.div.setAttribute("class", "region");
		this.div.regionController = this;
		
		var table = document.createElement("table");
		table.setAttribute("class", "region table");
	
		// button_Delete
		this.button_Delete = document.createElement("button");
		this.button_Delete.innerHTML = "X";
		this.button_Delete.onclick = this.Delete;
		this.button_Delete.setAttribute("class", "region delete_button");

		// header
		var tr_header = document.createElement("tr");
		tr_header.setAttribute("class", "region header");
		this.td_label_header = document.createElement("td");
		this.td_label_header.innerHTML = "Region " + regionIndex;
		this.td_label_header.style="font-weight:bold; padding-top: 5px;";
		var td_header_delete_button = document.createElement("td");
		td_header_delete_button.style = "text-align:right;";
		td_header_delete_button.appendChild(this.button_Delete);
		tr_header.appendChild(this.td_label_header);
		tr_header.appendChild(td_header_delete_button);
		table.appendChild(tr_header);
	
		// textBox_Start
		var tr_textBox_Start = document.createElement("tr");
		var td_label_textBox_Start = document.createElement("td");
		td_label_textBox_Start.innerHTML = "Start Index";
		var td_textBox_Start = document.createElement("td");
		this.textBox_Start = document.createElement("input");
		this.textBox_Start.setAttribute("type", "text");
		this.textBox_Start.placeholder = "0";
		this.textBox_Start.value = startIndex;
		this.textBox_Start.onchange = this.Update;
		td_textBox_Start.appendChild(this.textBox_Start);
		tr_textBox_Start.appendChild(td_label_textBox_Start);
		tr_textBox_Start.appendChild(td_textBox_Start);
		table.appendChild(tr_textBox_Start);
		
		// textBox_End
		var tr_textBox_End = document.createElement("tr");
		var td_label_textBox_End = document.createElement("td");
		td_label_textBox_End.innerHTML = "End Index";
		var td_textBox_End = document.createElement("td");
		this.textBox_End = document.createElement("input");
		this.textBox_End.setAttribute("type", "text");
		this.textBox_End.placeholder = num_leds;
		this.textBox_End.value = endIndex;
		this.textBox_End.onchange = this.Update;
		td_textBox_End.appendChild(this.textBox_End);
		tr_textBox_End.appendChild(td_label_textBox_End);
		tr_textBox_End.appendChild(td_textBox_End);
		table.appendChild(tr_textBox_End);
		
		// dropdown_Shader
		var tr_dropdown_Shader = document.createElement("tr");
		var td_label_dropdown_Shader = document.createElement("td");
		td_label_dropdown_Shader.innerHTML = "Shader";
		var td_dropdown_Shader = document.createElement("td");
		this.dropdown_Shader = document.createElement("select");
		for (i = 0; i < max_shaders; i++)
		{
			var shader_option = document.createElement("option");
			shader_option.innerHTML = shaderNames[i];
			this.dropdown_Shader.appendChild(shader_option);
		}		
		this.dropdown_Shader.selectedIndex = shader;
		this.dropdown_Shader.onchange = this.Update;
		td_dropdown_Shader.appendChild(this.dropdown_Shader);
		tr_dropdown_Shader.appendChild(td_label_dropdown_Shader);
		tr_dropdown_Shader.appendChild(td_dropdown_Shader);
		table.appendChild(tr_dropdown_Shader);
		
		// slider_Red
		var tr_slider_Red = document.createElement("tr");
		var td_label_slider_Red = document.createElement("td");
		td_label_slider_Red.innerHTML = "Red";
		var td_slider_Red = document.createElement("td");
		this.slider_Red = document.createElement("input");
		this.slider_Red.setAttribute("type", "range");
		this.slider_Red.min = 0;
		this.slider_Red.max = 255;
		this.slider_Red.value = red;
		this.slider_Red.onchange = this.Update;
		this.slider_Red.oninput = this.UpdateColour;
		this.slider_Red.setAttribute("class", "region slider red");
		td_slider_Red.appendChild(this.slider_Red);
		tr_slider_Red.appendChild(td_label_slider_Red);
		tr_slider_Red.appendChild(td_slider_Red);
		table.appendChild(tr_slider_Red);
		
		// slider_Green
		var tr_slider_Green = document.createElement("tr");
		var td_label_slider_Green = document.createElement("td");
		td_label_slider_Green.innerHTML = "Green";
		var td_slider_Green = document.createElement("td");		
		this.slider_Green = document.createElement("input");
		this.slider_Green.setAttribute("type", "range");
		this.slider_Green.min = 0;
		this.slider_Green.max = 255;
		this.slider_Green.value = green;
		this.slider_Green.onchange = this.Update;
		this.slider_Green.oninput = this.UpdateColour;
		this.slider_Green.setAttribute("class", "region slider green");
		td_slider_Green.appendChild(this.slider_Green);
		tr_slider_Green.appendChild(td_label_slider_Green);
		tr_slider_Green.appendChild(td_slider_Green);
		table.appendChild(tr_slider_Green);
		
		// slider_Blue
		var tr_slider_Blue = document.createElement("tr");
		var td_label_slider_Blue = document.createElement("td");
		td_label_slider_Blue.innerHTML = "Blue";
		var td_slider_Blue = document.createElement("td");			
		this.slider_Blue = document.createElement("input");
		this.slider_Blue.setAttribute("type", "range");
		this.slider_Blue.min = 0;
		this.slider_Blue.max = 255;
		this.slider_Blue.value = blue;
		this.slider_Blue.onchange = this.Update;
		this.slider_Blue.oninput = this.UpdateColour;
		this.slider_Blue.setAttribute("class", "region slider blue");
		td_slider_Blue.appendChild(this.slider_Blue);
		tr_slider_Blue.appendChild(td_label_slider_Blue);
		tr_slider_Blue.appendChild(td_slider_Blue);
		table.appendChild(tr_slider_Blue);

		// mixer
		var tr_mixer = document.createElement("tr");
		var td_label_mixer = document.createElement("td");
		this.td_mixer = document.createElement("td");
		this.td_mixer.style = "height: 17.5px; background: rgb(" + this.red + "," + this.green + "," + this.blue + ");";
		tr_mixer.appendChild(td_label_mixer);
		tr_mixer.appendChild(this.td_mixer);
		table.appendChild(tr_mixer);
		
		this.div.appendChild(table);
	}
	
	get regionIndex()
	{
		return this.region;
	}
	
	set regionIndex(value)
	{
		this.region = value;
		this.td_label_header.innerHTML = "Region " + value;
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
		Http_MakeRequest("/api?region-create:" + this.startIndex + "," + this.endIndex + "," + this.shader + "," + this.red + "," + this.green + "," + this.blue);
	}
	
	Update()
	{
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController;
		Http_MakeRequest("/api?region-update:" + rc.regionIndex + "," + rc.startIndex + "," + rc.endIndex + "," + rc.shader + "," + rc.red + "," + rc.green + "," + rc.blue);
	}
	
	UpdateColour()
	{
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController;
		rc.td_mixer.style = "height: 17.5px; background: rgb(" + rc.red + "," + rc.green + "," + rc.blue + ");";
	}
	
	Delete()
	{
		var rc = this.parentNode.parentNode.parentNode.parentNode.regionController;
		Http_MakeRequest("/api?region-delete:" + rc.regionIndex);
		
		tab_regions.removeChild(rc.div);

		regionControllers.splice(regionControllers.indexOf(rc), 1);
	}
	
	Remove()
	{		
		tab_regions.removeChild(this.div);

		regionControllers.splice(regionControllers.indexOf(this), 1);
	}
}
