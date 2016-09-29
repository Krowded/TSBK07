#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
in vec2 inTexCoord;
out vec3 normal;
out vec2 outTexCoord; 

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * model * vec4(in_Position, 1.0);
	normal = in_Normal;
	outTexCoord = inTexCoord;
}


