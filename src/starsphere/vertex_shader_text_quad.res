#version 330

// This is a vertex shader. Creates a parallel sided quadrilateral
// area based at lower left corner, with offsets along the sides.
// Both dimensions of texture coordinates are bound/clamped to
// zero and one at extents.

uniform vec3 base_position;
uniform vec3 height_offset;
uniform vec3 width_offset;

out vec2 pass_text_coords;

uniform mat4 CameraMatrix;

void main()
{
    // Start at lower left corner.
    vec3 position = base_position;
    // With texture coordinates of zero.
    pass_text_coords.st = vec2(0.0, 0.0);

    // For odd numbered vertices.
    if((gl_VertexID % 2) == 1) {
        // Add the width_offset.
        position += width_offset;
        // With the 's' texture coordinate is 1.0.
        pass_text_coords.s = 1.0;
        }

    // For the vertex numbered two & three.
    if(gl_VertexID > 1) {
        // Add the height offset.
        position += height_offset;
        // With the 't' texture coordinate being 1.0.
        pass_text_coords.t = 1.0;
        }

    // Emit final position of the vertex.
    gl_Position = CameraMatrix * vec4(position, 1.0f);
}
