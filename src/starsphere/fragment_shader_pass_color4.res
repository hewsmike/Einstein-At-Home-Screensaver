#version 150

// This is a plain pass through fragment shader.
in vec4 pass_color;

out vec4 out_color;

void main()
{
	out_color = pass_color;
}
