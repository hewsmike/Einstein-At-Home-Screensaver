#version 330

// This is a plain pass through fragment shader.
in vec3 pass_color;

out vec4 out_color;

void main()
{
	out_color = vec4(pass_color, 1.0);
}
