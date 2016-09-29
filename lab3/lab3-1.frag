#version 150

out vec4 out_Color;
out vec3 outColor;
in vec3 outNormal;
in vec2 outTexCoord;
in vec3 color;
uniform float t;
uniform sampler2D texUnit;

const vec3 light = vec3(0.58, 0.58, 0.58);

void main(void)
{
	outColor = vec3(dot(outNormal,normalize(light)));
}	
