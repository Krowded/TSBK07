#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;
out vec3 worldPosition;

uniform mat4 WorldToView;
uniform mat4 ModelToView;
uniform mat4 ModelToProjection;

void main(void)
{
	normal = mat3(ModelToView) * inNormal; // View coordinates
	worldPosition = vec3(ModelToView * vec4(inPosition, 1.0));
	gl_Position = ModelToProjection * vec4(inPosition, 1.0);
	texCoord = inTexCoord;
}



