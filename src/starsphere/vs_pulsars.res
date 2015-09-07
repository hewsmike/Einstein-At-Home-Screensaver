#version 150

// This is a vertex shader. Color as literal.

in vec3 position;

out vec3 pass_color;

uniform mat4 CameraMatrix;

void main()
{
  gl_Position = CameraMatrix * vec4(position, 1.0f);
  pass_color = vec3(0.80, 0.0, 0.85);
}
