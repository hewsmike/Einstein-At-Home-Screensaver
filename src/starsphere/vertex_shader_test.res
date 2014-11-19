#version 100

// This is a test of vertex shader source code.

void main(void)
{
    gl_Position = vec4(0.0, 0.0, -5.0, 1.0);
    gl_PointSize = 40.0f;
}
