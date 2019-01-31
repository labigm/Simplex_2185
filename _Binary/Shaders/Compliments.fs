#version 330

in vec3 Color;
vec3 Spectrum;
uniform vec3 SolidColor = vec3(-1,-1,-1);
//Seth Lambert and Owen Palzer
out vec4 Fragment;

void main()
{	Spectrum=Color;
	//change colors to the CMY compliments
	if(SolidColor.r == -1.0 && SolidColor.g == -1.0 && SolidColor.b == -1.0){
		vec3 compliments= vec3(1.0f-Color.r,1.0f-Color.g,1.0f-Color.b);
		Fragment = vec4(compliments,1);
	}
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
		Fragment = vec4(SolidColor, 1);
	//condition to change back to original RGB spectrum
	if(SolidColor.r == -2.0 && SolidColor.g == -2.0 && SolidColor.b == -2.0){
		Fragment = vec4(Spectrum,1);
	}
	return;
}