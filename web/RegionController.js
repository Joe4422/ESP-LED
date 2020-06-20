class RegionController
{
	constructor(startIndex, endIndex, shader, red, green, blue)
	{
		this.div = document.createElement("div");
	
		// textBox_Start
		var div_textBox_Start = document.createElement("div");
		var label_textBox_Start = document.createElement("label");
		label_textBox_Start.innerHTML = "Start Index:";
		this.textBox_Start = document.createElement("input");
		textBox_Start.setAttribute("type", "text");
		textBox_Start.placeholder = "0";
		textBox_Start.value = startIndex;
		div_textBox_Start.appendChild(label_textBox_Start);
		div_textBox_Start.appendChild(textBox_Start);
		
		// textBox_End
		var div_textBox_End = document.createElement("div");
		var label_textBox_End = document.createElement("label");
		label_textBox_End.innerHTML = "End Index:";
		this.textBox_End = document.createElement("input");
		textBox_End.setAttribute("type", "text");
		textBox_End.placeholder = "149";
		textBox_End.value = endIndex;
		div_textBox_End.appendChild(label_textBox_End);
		div_textBox_End.appendChild(textBox_End);
		
		// ticker_Shader
		var div_ticker_Shader = document.createElement("div");
		var label_ticker_Shader = document.createElement("label");
		label_ticker_Shader.innerHTML = "Shader:";
		this.ticker_Shader = document.createElement("input");
		ticker_Shader.setAttribute("type", "number");
		ticker_Shader.min = 0;
		ticker_Shader.max = 3;
		ticker_Shader.value = shader;
		div_ticker_Shader.appendChild(label_ticker_Shader);
		div_ticker_Shader.appendChild(ticker_Shader);
		
		// slider_Red
		var div_slider_Red = document.createElement("div");
		var label_slider_Red = document.createElement("label");
		label_slider_Red.innerHTML = "Red:";
		this.slider_Red = document.createElement("input");
		slider_Red.setAttribute("type", "range");
		slider_Red.min = 0;
		slider_Red.max = 255;
		slider_Red.value = red;
		div_slider_Red.appendChild(label_slider_Red);
		div_slider_Red.appendChild(slider_Red);
		
		// slider_Green
		var div_slider_Green = document.createElement("div");
		var label_slider_Green = document.createElement("label");
		label_slider_Green.innerHTML = "Green:";
		this.slider_Green = document.createElement("input");
		slider_Green.setAttribute("type", "range");
		slider_Green.min = 0;
		slider_Green.max = 255;
		slider_Green.value = green;
		div_slider_Green.appendChild(label_slider_Green);
		div_slider_Green.appendChild(slider_Green);
		
		// slider_Blue
		var div_slider_Blue = document.createElement("div");
		var label_slider_Blue = document.createElement("label");
		label_slider_Blue.innerHTML = "Blue:";
		this.slider_Blue = document.createElement("input");
		slider_Blue.setAttribute("type", "range");
		slider_Blue.min = 0;
		slider_Blue.max = 255;
		slider_Blue.value = blue;
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
	}
	
	get startIndex()
	{
		return textBox_Start.value;
	}
	set startIndex(value)
	{
		textBox_Start.value = value;
	}
	
	get endIndex()
	{
		return textBox_End.value;
	}
	set endIndex(value)
	{
		textBox_End.value = value;
	}
	
	get shader()
	{
		return ticker_Shader.value;
	}
	set shader(value)
	{
		ticker_Shader.value = value;
	}
	
	get red()
	{
		return slider_Red.value;
	}
	set red(value)
	{
		slider_Red.value = value;
	}
	
	get green()
	{
		return slider_Green.value;
	}
	set green(value)
	{
		slider_Green.value = value;
	}
	
	get blue()
	{
		return slider_Blue.value;
	}
	set blue(value)
	{
		slider_Blue.value = value;
	}
}