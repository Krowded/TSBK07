#version 150

out vec4 out_Color;

in vec3 normal;
in vec2 texCoord;
in vec3 worldPosition;

in vec3 lightSources[4];

uniform sampler2D texUnit1, texUnit2;

uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];


uniform mat4 WorldToView;

void main(void)
{
	vec3 newNormal = normalize(normal);

 	vec3 diffuseLight = vec3(0.0);
	vec3 specularLight = vec3(0.0);

	const float Kd = 0.2;
	const float Ks = 0.5;
	const vec3 ambientLight = vec3(0.01);

	for (int i = 0; i < 4; ++i)
	{
		vec3 lightDirection;

		if (isDirectional[i])
		{
			lightDirection = normalize(lightSources[i]);
		}
		else
		{
			lightDirection = normalize(lightSources[i] - worldPosition);
		}

		//Diffuse
		diffuseLight += Kd * max(dot(newNormal, lightDirection), 0.0) * lightSourcesColorArr[i];

		//Specular	
		vec3 directionOfCamera = normalize(-worldPosition);
		vec3 reflectedDirection = reflect(-lightDirection, newNormal);
		//Stop specular lighting on backside
		if (dot(directionOfCamera, newNormal) < 0)
		{
			reflectedDirection = -reflectedDirection;
		}
		specularLight += Ks * pow(max(dot(reflectedDirection, directionOfCamera), 0.0), specularExponent[i]) * lightSourcesColorArr[i];
	
	}

	if (worldPosition.y < 5)
	{
		out_Color = vec4(diffuseLight + specularLight + ambientLight, 1.0f) * texture(texUnit1, texCoord).rgba;
	}
	else
	{
		out_Color = vec4(diffuseLight + specularLight + ambientLight, 1.0f) * texture(texUnit2, texCoord).rgba;		
	}

}	