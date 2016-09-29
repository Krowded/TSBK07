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
	
	gl_Position.x = gl_Position.x + 20*sin(gl_Position.y);
	gl_Position.z = gl_Position.z + 20*cos(gl_Position.y);

	mat3 normalTransform = mat3(mv);
	outNormal = normalTransform * inNormal;

	outTexCoord = inTexCoord;
}


