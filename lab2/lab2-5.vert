#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 inTexCoord;
out vec3 outNormal;
out vec2 outTexCoord;
out vec3 color;

uniform mat4 mvp;
uniform mat4 mv;

const vec3 light = vec3(0.58, 0.58, 0.58);

void main(void)
{
	gl_Position = mvp * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position,1.0);
	mat3 normalTransform = mat3(mv);
	outNormal = normalTransform * in_Normal;

	outTexCoord = inTexCoord;
	//color = vec3(dot(outNormal, normalize(light)));
}


