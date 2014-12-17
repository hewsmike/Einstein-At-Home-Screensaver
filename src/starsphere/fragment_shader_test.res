#version 100

// This is a test of fragment shader source code.
varying mediump vec3 pass_color;

void main()
{
	gl_FragColor = vec4(pass_color, 1.0);
}
