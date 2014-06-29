#version 100

// This is a test of vertex shader source code.

attribute vec3 position;

void main(void)
{
    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
}
