/***************************************************************************
 *   Copyright (C) 2020 by Mike Hewson                                     *
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

const std::string TexturedParallelogram::m_vertex_shader_3D("#version 330\n"
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
"    vec3 position = base_position.xyz;\n"
"    // With texture coordinates of zero.\n"
"    pass_text_coords.st = vec2(0.0, 0.0);\n"
"\n"
"    // For second vertex.\n"
"    if(gl_VertexID == 1) {\n"
"        // Add the width_offset.\n"
"        position.x += width_offset.x;\n"
"        position.y += width_offset.y;\n"
"        position.z += width_offset.z;\n"
"        // With the 's' texture coordinate is 1.0.\n"
"        pass_text_coords.s = 1.0;\n"
"        }\n"
"\n"
"    // For third vertex.\n"
"    if(gl_VertexID == 2) {\n"
"        // Add the height & width_offset.\n"
"        position.x += width_offset.x;\n"
"        position.x += height_offset.x;\n"
"        position.y += width_offset.y;\n"
"        position.y += height_offset.y;\n"
"        position.z += width_offset.z;\n"
"        position.z += height_offset.z;\n"
"        // With both texture coordinates 1.0.\n"
"        pass_text_coords.s = 1.0;\n"
"        pass_text_coords.t = 1.0;\n"
"        }\n"
"\n"
"    // For the fourth/last vertex.\n"
"    if(gl_VertexID == 3) {\n"
"        // Add the height offset.\n"
"        position.x += height_offset.x;\n"
"        position.y += height_offset.y;\n"
"        position.z += height_offset.z;\n"
"        // With the 't' texture coordinate being 1.0.\n"
"        pass_text_coords.t = 1.0;\n"
"        }\n"
"\n"
"    // Emit final position of the vertex.\n"
"    // Note the scale is unity.\n"
"    gl_Position = CameraMatrix * vec4(position, 1.0f);\n"
"}\n");

const std::string TexturedParallelogram::m_fragment_shader("#version 330\n"
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
"    out_color = vec4(out_color.r, out_color.g, out_color.b, out_color.a);\n"
"}\n");

TexturedParallelogram::TexturedParallelogram(glm::vec3 position,
                                             glm::vec3 width_offset,
                                             glm::vec3 height_offset,
                                             RenderTask::texture_buffer_group t_group) :
                                                 m_position(position),
                                                 m_width_offset(width_offset),
                                                 m_height_offset(height_offset),
                                                 is3D(true) {
    // Check for nullity of texture data pointer.
    if(t_group.texture_data == NULL) {
        ErrorHandler::record("TexturedParallelogram::TexturedParallelogram() : Texture not provided!", ErrorHandler::FATAL);
        }
    m_t_group = t_group;

    // I'm not going to check for 'sensible' choices of position and offsets.
    m_render_task = NULL;
    this->setConfigurationState(false);
    }

TexturedParallelogram::TexturedParallelogram(glm::vec2 position,
                                             glm::vec2 width_offset,
                                             glm::vec2 height_offset,
                                             RenderTask::texture_buffer_group t_group) :
                                                 m_position(position, 0.0f),
                                                 m_width_offset(width_offset, 0.0f),
                                                 m_height_offset(height_offset, 0.0f),
                                                 is3D(false) {
    // Check for nullity of texture data pointer.
    if(t_group.texture_data == NULL) {
        ErrorHandler::record("TexturedParallelogram::TexturedParallelogram() : Texture not provided!", ErrorHandler::FATAL);
        }
    m_t_group = t_group;

    // I'm not going to check for 'sensible' choices of position and offsets.
    m_render_task = NULL;
    this->setConfigurationState(false);
    }

TexturedParallelogram::~TexturedParallelogram() {
    if(m_render_task) delete m_render_task;
    }

glm::vec3 TexturedParallelogram::position(void) {
    return m_position;
    }

glm::vec3 TexturedParallelogram::heightOffset(void) {
    return m_height_offset;
    }

glm::vec3 TexturedParallelogram::widthOffset(void) {
    return m_width_offset;
    }

bool TexturedParallelogram::isThreeDimensional(void) {
    return is3D;
    }

bool TexturedParallelogram::configure(void) {
    // Construct a shader group structure.
    // Use strings defined at top of file.
    RenderTask::shader_group m_s_group;
    m_s_group.frag_shader_source = m_fragment_shader;
    m_s_group.vert_shader_source = m_vertex_shader_3D;

    // Construct a render task.
    m_render_task = new RenderTask(m_s_group, m_t_group);
    if(is3D == false) {
        m_render_task->setUniform("CameraMatrix", TransformGlobals::getOrthographicTransformMatrix());
        }
    else {
        m_render_task->setUniform("CameraMatrix", TransformGlobals::getPerspectiveTransformMatrix());
        }
    m_render_task->setUniform("base_position", &m_position);
    m_render_task->setUniform("height_offset", &m_height_offset);
    m_render_task->setUniform("width_offset", &m_width_offset);

    // Acquire OpenGL assets.
    m_render_task->acquire();

    this->setConfigurationState(true);
    return true;
    }

void TexturedParallelogram::utilise(void) {
    if(!this->isConfigured()) {
        configure();
        }
    // Render two triangles, four vertices in all.
    m_render_task->render(GL_TRIANGLE_FAN, VERTEX_COUNT);
    }
