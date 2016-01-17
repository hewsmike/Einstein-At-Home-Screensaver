#version 150

// This is a fragment shader. It samples/interpolates the
// texture object currently bound to GL_TEXTURE0.
  
// With these texture coordinates ...
in vec2 pass_text_coords;  
  
// ... lookup using this sampler device ...  
uniform sampler2D color_map;  
  
// ... to determine the fragment color.
out vec4 out_color;

void main()
{
	out_color = texture(color_map, pass_text_coords.st);
}
