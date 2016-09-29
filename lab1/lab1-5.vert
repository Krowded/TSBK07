#version 150

in  vec3 in_Position;
in  vec3 in_Color;
out vec3 color;
uniform mat4 myTranslationMatrix;
uniform mat4 myRotationMatrix;
uniform mat4 myRotationMatrix2;
uniform mat4 myRotationMatrix3;
uniform mat4 myScalingMatrix;

void main(void)
{
	gl_Position = vec4(in_Position, 1.0);
	//gl_Position = myScalingMatrix * gl_Position;
	gl_Position = myRotationMatrix * gl_Position;
	gl_Position = myRotationMatrix2 * gl_Position;
	gl_Position = myRotationMatrix3 * gl_Position;
	//gl_Position = myTranslationMatrix * gl_Position;	
	color = in_Color;
}


