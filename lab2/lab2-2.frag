#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 outTexCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	//out_Color = vec3(1.0,1.0,0.0);
	//out_Color = normal;
	//out_Color = vec3(0,outTexCoord.t,0);//sin(200*t));
	//float a = sin(outTexCoord.s * 30.0 + t)/2.0 + 0.5;
	//float b = sin(outTexCoord.t * 30.0 * (1.0+sin(t/4.0)))/2.0 + 0.5;
	//out_Color = vec3(a, b, 0);
	out_Color = texture(texUnit, outTexCoord);
}	
