#version 150

// This is a vertex shader. Pass through of two vertex
// associated texture coordinates.

in vec3 position;
in vec2 text_coords;

out vec2 pass_text_coords;

uniform mat4 CameraMatrix;

void main()
{
    gl_Position = CameraMatrix * vec4(position, 1.0f);
    pass_text_coords = text_coords;
}
