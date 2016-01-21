/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
 *   hewsmike[AT]iinet.net.au                                              *
 *                                                                         *
 *   This file is part of Einstein@Home.                                   *
 *                                                                         *
 *   Einstein@Home is free software: you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published     *
 *   by the Free Software Foundation, version 2 of the License.            *
 *                                                                         *
 *   Einstein@Home is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "TexturedParallelogram.h"

#include <iostream>

#include "ErrorHandler.h"

const GLuint TexturedParallelogram::VERTEX_COUNT(4);

const std::string TexturedParallelogram::m_vertex_shader("#version 150"
""
"// This is a vertex shader. Creates a parallel sided quadrilateral"
"// area based at lower left corner, with offsets along the sides."
"// Both dimensions of texture coordinates are bound/clamped to"
"// zero and one at extents."
""
"uniform vec3 base_position;"
"uniform vec3 height_offset;"
"uniform vec3 width_offset;"
""
"out vec2 pass_text_coords;"
""
"uniform mat4 CameraMatrix;"
""
"void main()"
"{"
"    // Start at lower left corner."
"    vec3 position = base_position;"
"    // With texture coordinates of zero."
"    pass_text_coords.st = vec2(0.0, 0.0);"
""
"    // For odd numbered vertices."
"    if((gl_VertexID % 2) == 1) {"
"        // Add the width_offset."
"        position += width_offset;"
"        // With the 's' texture coordinate is 1.0."
"        pass_text_coords.s = 1.0;"
"        }"
""
"    // For the vertex numbered two & three."
"    if(gl_VertexID > 1) {"
"        // Add the height offset."
"        position += height_offset;"
"        // With the 't' texture coordinate being 1.0."
"        pass_text_coords.t = 1.0;"
"        }"
""
"    // Emit final position of the vertex."
"    gl_Position = CameraMatrix * vec4(position, 1.0f);"
"}");

const std::string TexturedParallelogram::m_fragment_shader("#version 150"
""
"// This is a fragment shader. It samples/interpolates the"
"// texture object currently bound to GL_TEXTURE0."
""
"// With these texture coordinates ..."
"in vec2 pass_text_coords;"
""
"// ... lookup using this sampler device ..."
"uniform sampler2D color_map;"
""
"// ... to determine the fragment color."
"out vec4 out_color;"
""
"void main()"
"{"
"	out_color = texture(color_map, pass_text_coords.st);"
"}");

TexturedParallelogram::TexturedParallelogram(glm::vec3 position,
	  	  	  	  	  	  	  	  	  	  	 glm::vec3 height_offset,
											 glm::vec3 width_offset,
											 TextureBuffer* texture) :
												 m_position(position),
												 m_height_offset(height_offset),
												 m_width_offset(width_offset){
	if(texture == NULL) {
		ErrorHandler::record("TexturedParallelogram::TexturedParallelogram() : Texture not provided!", ErrorHandler::FATAL);
		}

	m_texture = texture;
	m_render_task = NULL;
	m_configure_flag = false;
    }

TexturedParallelogram::~TexturedParallelogram() {
	if(m_render_task) delete m_render_task;
    }

void TexturedParallelogram::setPosition(glm::vec3 position) {
	m_position = position;
	configureTask();
	}

void TexturedParallelogram::setHeightOffset(glm::vec3 height_offset) {
	m_height_offset = height_offset;
	configureTask();
	}

void TexturedParallelogram::setWidthOffset(glm::vec3 width_offset) {
	m_width_offset = width_offset;
	configureTask();
	}

void TexturedParallelogram::setTexture(TextureBuffer* texture) {
	if(texture == NULL) {
	    ErrorHandler::record("TexturedParallelogram::setTexture() : Texture not provided!", ErrorHandler::FATAL);
		}
	m_texture = texture;
	configureTask();
	}

void TexturedParallelogram::configureTask(void) {
	// Firstly discard any existing rendering task.
	if(m_render_task) delete m_render_task;

	// Construct a shader group structure.
	RenderTask::shader_group s_group = {m_vertex_shader, m_fragment_shader};

	// Create an index buffer group structure, for completeness.
	RenderTask::index_buffer_group i_group = {NULL, 0, 0, 0, 0};

	// Create a vertex buffer group structure, this matters.
	RenderTask::vertex_buffer_group v_group = {NULL,
											   0,
											   VERTEX_COUNT,
											   GL_STATIC_DRAW,
											   VertexBuffer::BY_VERTEX};

	m_render_task = new RenderTask(s_group,i_group, v_group);

	m_render_task->setUniformLoadPoint("base_position", &m_position);

	m_render_task->setUniformLoadPoint("height_offset", &m_height_offset);

	m_render_task->setUniformLoadPoint("width_offset", &m_width_offset);

	m_render_task->acquire();

	m_texture->bind();

	m_configure_flag = true;
	}

void TexturedParallelogram::utilise(void) {
    if(m_configure_flag == false) {
        configureTask();
        }
	m_render_task->utilise(GL_TRIANGLE_STRIP, VERTEX_COUNT);
	}
