#version 150

// This is a vertex shader. Single color and point size as uniforms.

in vec3 position;

out vec3 pass_color;

uniform float point_size;
uniform mat4 CameraMatrix;
uniform vec3 color;

void main()
{
    gl_Position = CameraMatrix * vec4(position, 1.0f);
    pass_color = color;
    gl_PointSize = point_size;
}
