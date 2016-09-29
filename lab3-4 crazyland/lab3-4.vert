#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;
out vec3 directionLight[4];
out vec3 directionCamera;

uniform vec3 campos;
uniform mat4 WorldToView;
uniform mat4 ModelToView;
uniform mat4 ModelToProjection;

uniform vec3 lightSourcesDirectionPositionArr[4];
uniform vec3 lightSourcesColorsArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

void main(void)
{
	gl_Position = ModelToProjection * vec4(inPosition, 1.0);

	normal = mat3(ModelToView) * inNormal;

	texCoord = inTexCoord;

	vec3 position = vec3((ModelToView) * vec4(inPosition, 1.0) );

	int i;
	for (i = 0; i < 4; ++i)
	{
		if(isDirectional[i])
		{
			directionLight[i] = mat3(WorldToView) * lightSourcesDirectionPositionArr[i];
		}
		else
		{
			vec3 lightSource = vec3(WorldToView * vec4(lightSourcesDirectionPositionArr[i], 1.0));
			directionLight[i] = lightSource - position;
		}
	}

	directionCamera = vec3(gl_Position) - campos;

}


