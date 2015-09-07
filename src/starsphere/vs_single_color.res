#version 150

// This is a vertex shader. Single color as uniform.

in vec3 position;

out vec3 pass_color;

uniform mat4 CameraMatrix;
uniform vec3 color;

void main()
{
  gl_Position = CameraMatrix * vec4(position, 1.0f);
  pass_color = color;
}
