#version 150

out vec3 out_Color;
in vec4 normal;

void main(void)
{
	//out_Color = vec3(1.0,1.0,0.0);
	//out_Color = vec3(normal);
	out_Color = max(dot(normal, vec4(0.0, 1.0, 0.0, 0.0)), 0.0) * vec3(1.0, 1.0, 0.0);
}
