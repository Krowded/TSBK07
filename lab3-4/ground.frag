#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 texCoord;
in vec3 worldPosition;

uniform sampler2D texUnit;

uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
uniform vec3 lightSourcesDirPosArr[4];

uniform mat4 WorldToView;

void main(void)
{
	vec3 newNormal = normalize(normal);

 	vec3 diffuseLight = vec3(0.0);
	vec3 specularLight = vec3(0.0);

	const float Kd = 1.0;
	const float Ks = 1.0;
	const vec3 ambientLight = vec3(0.0);

	int i = 0;
	//for (int i = 0; i < 4; ++i)
	
		vec3 rotatedLightSource, lightDirection;

		if (isDirectional[i])
		{
			lightDirection = normalize(mat3(WorldToView)*lightSourcesDirPosArr[i]);
		}
		else
		{
			rotatedLightSource = vec3(WorldToView * vec4(lightSourcesDirPosArr[i], 1.0));
			lightDirection = normalize(rotatedLightSource - worldPosition);
		}

		//Diffuse
		diffuseLight += Kd * max(dot(newNormal, lightDirection), 0) * lightSourcesColorArr[i];

		//Specular	
		vec3 directionOfCamera = normalize(-worldPosition);
		//Stop specular lighting on backside
		if (dot(directionOfCamera, newNormal) > 0)
		{
			vec3 reflectedDirection = reflect(-lightDirection, newNormal);
			specularLight += Ks * pow(max(dot(reflectedDirection, directionOfCamera), 0), specularExponent[i]) * lightSourcesColorArr[i];
		}
	

	out_Color = vec4(diffuseLight + specularLight + ambientLight, 1.0f) * texture(texUnit, texCoord).rgba;


	//if(dot(directionOfCamera,newNormal) < 0)
	//	out_Color = vec4(0, 0, 1, 1);
	
}	