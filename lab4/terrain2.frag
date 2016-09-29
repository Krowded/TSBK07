#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
uniform sampler2D tex;

void main(void)
{

	outColor = dot(normalize(normal), vec3(1.0, 1.0, 0.0)) * texture(tex, texCoord);
} 
