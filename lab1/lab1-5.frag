#version 150

out vec4 out_Color;
in vec3 color;

void main(void)
{
	//out_Color = vec4(1.0,1.0,0,0);
	out_Color = vec4(color, 0.0);
}
