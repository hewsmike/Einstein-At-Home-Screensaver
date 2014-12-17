#version 100

// This is a test of vertex shader source code.

attribute vec2 position;
attribute vec3 color;
// the "out" is now "varying"
varying vec3 pass_color;

void main()
{
  gl_Position = vec4(position, 0.0, 1.0);
  pass_color = color;
}
