#version 150

// This is a fragment shader. Two texture coords with sampling from
// texture object currently bound.
in vec2 pass_text_coords;

out vec4 out_color;

uniform sampler2D colorMap;

void main()
{
	out_color = texture(colorMap, vVaryingTexCoords.st);
}
