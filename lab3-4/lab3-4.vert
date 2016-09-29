#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;
out vec3 worldPosition;

out vec3 lightSources[4];

uniform vec3 lightSourcesDirPosArr[4];
uniform bool isDirectional[4];

uniform mat4 WorldToView;
uniform mat4 ModelToWorld;
uniform mat4 ModelToProjection;

void main(void)
{
	normal = mat3(WorldToView * ModelToWorld) * inNormal; // View coordinates
	worldPosition = vec3(WorldToView * ModelToWorld * vec4(inPosition, 1.0));
	gl_Position = ModelToProjection * vec4(inPosition, 1.0);
	texCoord = inTexCoord;

	for (int i = 0;  i < 4; ++i)
	{
			if (isDirectional[i])
			{
				lightSources[i] = mat3(WorldToView) * lightSourcesDirPosArr[i];
			}
			else
			{
				lightSources[i] = vec3(WorldToView * vec4(lightSourcesDirPosArr[i], 1.0));	
			}
	}

}



