#version 150

out vec4 out_Color;
out vec3 outColor;
in vec3 normal;
in vec2 texCoord;
in vec3 light;
uniform sampler2D texUnit;

void main(void)
{
	//out_Color = texture(texUnit, texCoord);
	out_Color = dot(normalize(normal),normalize(light)) * texture(texUnit, texCoord).rgba;
	out_Color[3] = 1;
	//outColor = vec3(dot(normal,normalize(light)));
}	