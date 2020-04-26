#version 330

// This is a fragment shader. It samples/interpolates the
// texture object currently bound to GL_TEXTURE0.

// in vec3 pass_color;
// With these texture coordinates ...
in vec2 TexCoord;

// ... lookup using this sampler device ...
uniform sampler2D color_map;

// ... to determine the fragment color.
out vec4 out_color;

void main()
{
	out_color = texture(color_map, TexCoord.st);
	// out_color = vec4(this_color, 1.0f);
}
