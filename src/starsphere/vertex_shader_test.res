#version 100

// This is a test of vertex shader source code.

attribute vec4 in_position;
attribute vec3 color;

varying vec3 pass_color;

void main(void)
{
    gl_Position = in_position;
    gl_PointSize = 40.0f;
    pass_color = color;
}
