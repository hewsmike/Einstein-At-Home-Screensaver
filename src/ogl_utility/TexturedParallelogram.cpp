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
#include "TransformGlobals.h"

const GLuint TexturedParallelogram::VERTEX_COUNT(4);

/// NB Especially the newline character '\n' at the end of each line which
/// is required to avoid incorrect string length and termination,
/// which would otherwise be exposed when the shaders are compiled.

const std::string TexturedParallelogram::m_vertex_shader_3D("#version 150\n"
"\n"
"// This is a vertex shader. Creates a parallel sided quadrilateral\n"
"// area based at lower left corner, with offsets along the sides.\n"
"// Both dimensions of texture coordinates are bound/clamped to\n"
"// zero and one at extents.\n"
"\n"
"uniform vec3 base_position;\n"
"uniform vec3 height_offset;\n"
"uniform vec3 width_offset;\n"
"uniform mat4 CameraMatrix;\n"
"\n"
"out vec2 pass_text_coords;\n"
"\n"
"void main()\n"
"{\n"
"    // Start at lower left corner.\n"
"    vec2 position = base_position.xy;\n"
"    // With texture coordinates of zero.\n"
"    pass_text_coords.st = vec2(0.0, 0.0);\n"
"\n"
"    // For odd numbered vertices.\n"
"    if((gl_VertexID % 2) == 1) {\n"
"        // Add the width_offset.\n"
"        position += width_offset.xy;\n"
"        // With the 's' texture coordinate is 1.0.\n"
"        pass_text_coords.s = 1.0;\n"
"        }\n"
"\n"
"    // For the vertex numbered two & three.\n"
"    if(gl_VertexID > 1) {\n"
"        // Add the height offset.\n"
"        position += height_offset.xy;\n"
"        // With the 't' texture coordinate being 1.0.\n"
"        pass_text_coords.t = 1.0;\n"
"        }\n"
"\n"
"    // Emit final position of the vertex.\n"
"    gl_Position = CameraMatrix * vec4(position, 0.0f, 1.0f);\n"
"}\n");

const std::string TexturedParallelogram::m_vertex_shader_2D("#version 150\n"
"\n"
"// This is a vertex shader. Creates a parallel sided quadrilateral\n"
"// area based at lower left corner, with offsets along the sides.\n"
"// Both dimensions of texture coordinates are bound/clamped to\n"
"// zero and one at extents.\n"
""
"uniform vec3 base_position;\n"
"uniform vec3 height_offset;\n"
"uniform vec3 width_offset;\n"
"uniform mat4 HUDMatrix;\n"
"\n"
"out vec2 pass_text_coords;\n"
"\n"
"void main()\n"
"{\n"
"    // Start at lower left corner.\n"
"    vec2 position = base_position.xy;\n"
"    // With texture coordinates of zero.\n"
"    pass_text_coords.st = vec2(0.0, 0.0);\n"
"\n"
"    // For odd numbered vertices.\n"
"    if((gl_VertexID % 2) == 1) {\n"
"        // Add the width_offset.\n"
"        position += width_offset.xy;\n"
"        // With the 's' texture coordinate is 1.0.\n"
"        pass_text_coords.s = 1.0;\n"
"        }\n"
"\n"
"    // For the vertex numbered two & three.\n"
"    if(gl_VertexID > 1) {\n"
"        // Add the height offset.\n"
"        position += height_offset.xy;\n"
"        // With the 't' texture coordinate being 1.0.\n"
"        pass_text_coords.t = 1.0;\n"
"        }\n"
"\n"
"    // Emit final position of the vertex.\n"
"    gl_Position = HUDMatrix * vec4(position, 0.0f, 1.0f);\n"
"}\n");

const std::string TexturedParallelogram::m_fragment_shader("#version 150\n"
"\n"
"// This is a fragment shader. It samples/interpolates the\n"
"// texture object currently bound to GL_TEXTURE0.\n"
"\n"
"// With these texture coordinates ...\n"
"in vec2 pass_text_coords;\n"
"\n"
"// ... lookup using this sampler device ...\n"
"uniform sampler2D color_map;\n"
"\n"
"// ... to determine the fragment color.\n"
"out vec4 out_color;\n"
"void main()\n"
"{\n"
"    out_color = texture(color_map, pass_text_coords.st);\n"
"}\n");

TexturedParallelogram::TexturedParallelogram(glm::vec3 position,
                                             glm::vec3 height_offset,
                                             glm::vec3 width_offset,
											 RenderTask::texture_buffer_group t_group) :
                                                 m_position(position),
                                                 m_height_offset(height_offset),
                                                 m_width_offset(width_offset) {
    m_render_mode = VOLUME;
    m_t_group = t_group;
    m_render_task = NULL;
    m_configure_flag = false;
    }

TexturedParallelogram::TexturedParallelogram(glm::vec2 position,
                                                   glm::vec2 height_offset,
                                             glm::vec2 width_offset,
											 RenderTask::texture_buffer_group t_group) :
                       TexturedParallelogram::TexturedParallelogram(glm::vec3(position.x, position.y, 0),
                                                                    glm::vec3(height_offset.x, height_offset.y, 0),
                                                                    glm::vec3(width_offset.x, width_offset.y, 0),
                                                                    t_group) {
    m_render_mode = FLAT;
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

void TexturedParallelogram::setTexture(RenderTask::texture_buffer_group t_group) {
    if(t_group.texture_data == NULL) {
        ErrorHandler::record("TexturedParallelogram::setTexture() : Texture not provided!", ErrorHandler::FATAL);
        }
    m_t_group = t_group;
    configureTask();
    }

void TexturedParallelogram::configureTask(void) {
    // Firstly discard any existing rendering task.
    if(m_render_task != NULL) delete m_render_task;

    // Construct a shader group structure.
    RenderTask::shader_group s_group;
    s_group.frag_shader_source = m_fragment_shader;

    if(m_render_mode == FLAT) {
        s_group.vert_shader_source = m_vertex_shader_2D;
        }
    else {
        s_group.vert_shader_source = m_vertex_shader_3D;
        }

    m_render_task = new RenderTask(s_group, m_t_group);

    if(m_render_mode == FLAT) {
        m_render_task->setUniform("HUDMatrix", TransformGlobals::getOrthographicTransformMatrix());
        }
    else {
        m_render_task->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());
        }

    m_render_task->setUniform("base_position", &m_position);

    m_render_task->setUniform("height_offset", &m_height_offset);

    m_render_task->setUniform("width_offset", &m_width_offset);

    m_render_task->acquire();

    m_configure_flag = true;
    }

void TexturedParallelogram::utilise(void) {
    if(m_configure_flag == false) {
        configureTask();
        }
    m_render_task->utilise(GL_TRIANGLE_STRIP, VERTEX_COUNT);
    }
