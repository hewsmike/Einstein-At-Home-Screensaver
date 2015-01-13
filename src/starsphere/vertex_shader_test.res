#version 150

// This is a test of vertex shader source code.

in vec2 position;
in vec3 color;

out vec3 pass_color;

uniform mat4 RotationMatrix;

void main()
{
  gl_Position = RotationMatrix * vec4(position, 0.0, 1.0);
  pass_color = color;
}
