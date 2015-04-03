#version 150

// This is a vertex shader. Single color as uniform.

in vec2 position;

out vec3 pass_color;

uniform mat4 RotationMatrix;
uniform vec3 color;

void main()
{
  gl_Position = RotationMatrix * vec4(position, 0.0, 1.0);
  pass_color = color;
}
