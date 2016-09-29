#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 texCoord;
in vec3 directionLight[4];
in vec3 directionCamera;
uniform sampler2D texUnit;

uniform vec3 lightSourcesColorsArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

void main(void)
{
	vec3 Normal = normalize(normal);
	vec3 light = normalize(directionLight[0]);
	vec3 r = 2*Normal*(dot(light,Normal))-light[0];



/*
	out_Color[0] = dot(Normal,normalize(directionLight[0]))* texture(texUnit, texCoord).r;
	out_Color[1] = 0;//max(dot(Normal,normalize(directionLight[1])), 0) * texture(texUnit, texCoord).g;
	out_Color[2] = 0;//max(dot(Normal,normalize(directionLight[2])), 0) * texture(texUnit, texCoord).b;
	out_Color[3] = 1;
*/
	
	out_Color = vec4(specularExponent[0] * 1 * r*directionCamera, 1);
}	