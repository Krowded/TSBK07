#version 150

in  vec3 in_Position;
uniform mat4 myTranslationMatrix;
uniform mat4 myRotationMatrix;
uniform mat4 myScalingMatrix;

void main(void)
{
	gl_Position = vec4(in_Position, 1.0);
	gl_Position = myRotationMatrix * gl_Position;
	//gl_Position = myTranslationMatrix * gl_Position;
	gl_Position = myScalingMatrix * gl_Position;

}


