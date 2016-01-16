#version 150

// This is a fragment shader. It samples/interpolates the texture object
// currently bound to GL_TEXTURE0
  
uniform sampler2D color_map;  
  
in vec2 pass_text_coords;

out vec4 out_color;

void main()
{
	out_color = texture(color_map, pass_text_coords.st);
}
