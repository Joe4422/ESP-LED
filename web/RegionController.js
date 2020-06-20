class RegionController
{
	constructor(regionIndex, startIndex, endIndex, shader, red, green, blue)
	{
		this.regionIndex = regionIndex;
		
		this.div = document.createElement("div");
	
		// textBox_Start
		var div_textBox_Start = document.createElement("div");
		var label_textBox_Start = document.createElement("label");
		label_textBox_Start.innerHTML = "Start Index:";
		this.textBox_Start = document.createElement("input");
		this.textBox_Start.setAttribute("type", "text");
		this.textBox_Start.placeholder = "0";
		this.textBox_Start.value = startIndex;
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
		div_textBox_End.appendChild(label_textBox_End);
		div_textBox_End.appendChild(this.textBox_End);
		
		// ticker_Shader
		var div_ticker_Shader = document.createElement("div");
		var label_ticker_Shader = document.createElement("label");
		label_ticker_Shader.innerHTML = "Shader:";
		this.ticker_Shader = document.createElement("input");
		this.ticker_Shader.setAttribute("type", "number");
		this.ticker_Shader.min = 0;
		this.ticker_Shader.max = 3;
		this.ticker_Shader.value = shader;
		div_ticker_Shader.appendChild(label_ticker_Shader);
		div_ticker_Shader.appendChild(this.ticker_Shader);
		
		// slider_Red
		var div_slider_Red = document.createElement("div");
		var label_slider_Red = document.createElement("label");
		label_slider_Red.innerHTML = "Red:";
		this.slider_Red = document.createElement("input");
		this.slider_Red.setAttribute("type", "range");
		this.slider_Red.min = 0;
		this.slider_Red.max = 255;
		this.slider_Red.value = red;
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
		div_slider_Blue.appendChild(label_slider_Blue);
		div_slider_Blue.appendChild(this.slider_Blue);
		
		// button_Save
		this.button_Save = document.createElement("button");
		this.button_Save.innerHTML = "Save";
		this.button_Save.setAttribute("onclick", "Regions_RegionController_Save_OnClick(" + regionControllers.length + ");");
		
		// button_Delete
		this.button_Delete = document.createElement("button");
		this.button_Delete.innerHTML = "Delete";
		this.button_Delete.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + regionControllers.length + ");");
		
		this.div.appendChild(div_textBox_Start);
		this.div.appendChild(div_textBox_End);
		this.div.appendChild(div_ticker_Shader);
		this.div.appendChild(div_slider_Red);
		this.div.appendChild(div_slider_Green);
		this.div.appendChild(div_slider_Blue);
		this.div.appendChild(this.button_Save);
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
		return this.ticker_Shader.value;
	}
	set shader(value)
	{
		this.ticker_Shader.value = value;
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
		this.button_Save.setAttribute("onclick", "Regions_RegionController_Save_OnClick(" + index + ");");
		this.button_Delete.setAttribute("onclick", "Regions_RegionController_Delete_OnClick(" + index + ");");
	}
}