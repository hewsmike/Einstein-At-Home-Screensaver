#version 150

// This is a vertex shader. Camera transform & point size as uniforms.

in vec3 position;
in vec4 color;

out vec4 pass_color;

uniform float point_size;
uniform mat4 CameraMatrix;

void main()
{
    gl_Position = CameraMatrix * vec4(position, 1.0f);
    pass_color = color;
    gl_PointSize = point_size;
}
