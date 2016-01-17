#version 150

// This is a fragment shader. Two texture coords with
// sampling from currently bound texture object.

// For the current fragment with these texture coordinates ...
in vec2 pass_text_coords ...

// ... produce a fragment color ...
out vec4 out_color;

// ... derived from the texture using this sampler device.
uniform sampler2D colorMap;

void main()
{
	out_color = texture(colorMap, vVaryingTexCoords.st);
}
