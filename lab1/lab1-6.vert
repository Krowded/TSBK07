#version 150

in  vec3 in_Position;
in  vec3 in_Color;
in  vec3 in_Normal;
out vec4 normal;
//uniform mat4 myTranslationMatrix;
uniform mat4 myRotationMatrix;
//uniform mat4 myRotationMatrix2;
//uniform mat4 myRotationMatrix3;
//uniform mat4 myScalingMatrix;

void main(void)
{
	
	gl_Position = vec4(in_Position, 1.0);
	//gl_Position = myScalingMatrix * gl_Position;	
	gl_Position = myRotationMatrix* gl_Position;
//inverse(myTranslationMatrix) * myRotationMatrix * myRotationMatrix2 * myRotationMatrix3 * myTranslationMatrix * gl_Position;
	
	normal = vec4(in_Normal, 1.0);
	normal = myRotationMatrix * normal;
//* myRotationMatrix2 * myRotationMatrix3 * normal;
}


