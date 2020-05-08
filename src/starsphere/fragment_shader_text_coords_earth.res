#version 330

// This is a fragment shader. It samples/interpolates the
// texture object currently bound to GL_TEXTURE0. The
// fragment is brightened or dimmed according to a
// float passed from the vertex shader.

// With these texture coordinates ...
in vec2 TexCoord;
in float day;

// ... lookup using this sampler device ...
uniform sampler2D color_map;

// ... to determine the fragment color.
out vec4 out_color;

void main() {
    float contrast_factor = 0.07;
    out_color = texture(color_map, TexCoord.st);
    if(day > 0) {
        out_color = (1.0 + contrast_factor*day) * out_color;
        // Check to see if the color channels
        // are beyond the maximum of 1.0 in
        // which case set them equal to 1.0
        if(out_color.r > 1.0) {
            out_color.r = 1.0;
            }
        if(out_color.g > 1.0) {
            out_color.g = 1.0;
            }
        if(out_color.b > 1.0) {
            out_color.b = 1.0;
            }
        }
    else {
        // No need to test for lower bound.
        out_color = (1 - 6.0 * contrast_factor) * out_color;
        }

    // Opaque fragment regardless.
    out_color.a = 1.0;
    }
