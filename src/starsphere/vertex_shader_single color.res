#version 330

// This is a vertex shader. Single color as uniform.

in vec2 position;

out vec3 pass_color;

uniform float point_size;
uniform mat4 RotationMatrix;
uniform vec3 color;

void main()
{
    gl_Position = CameraMatrix * vec4(position, 1.0f);
    pass_color = color;
    gl_PointSize = point_size;
}
