#version 150

// This is a test of vertex shader source code.

in vec2 position;
in vec3 color;
// the "out" is now "varying"
out vec3 pass_color;

blah

void main()
{
  gl_Position = vec4(position, 0.0, 1.0);
  pass_color = color;
}
