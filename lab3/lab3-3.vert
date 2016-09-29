#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;
out vec3 light;

uniform mat4 mv;
uniform mat4 mvp;
uniform vec3 inLight;

void main(void)
{
	gl_Position = mvp * vec4(inPosition, 1.0);
	
	mat3 normalTransform = mat3(mv);
	normal = normalTransform * inNormal;

	light = normalTransform * inLight;

	texCoord = inTexCoord;
}


