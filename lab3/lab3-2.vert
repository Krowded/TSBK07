#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
out vec3 outNormal;
out vec2 outTexCoord;

uniform mat4 mv;
uniform mat4 mvp;

void main(void)
{
	gl_Position = mvp * vec4(inPosition, 1.0);
	
	mat3 normalTransform = mat3(mv);
	outNormal = normalTransform * inNormal;

	outTexCoord = inTexCoord;
}


