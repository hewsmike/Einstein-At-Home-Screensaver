/***************************************************************************
 *   Copyright (C) 2014 by Mike Hewson                                     *
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

#include "Pipeline.h"


Pipeline::Pipeline(Program& program, VertexFetch& vertex_fetch) :
                    m_program(program),
                    m_vertex_fetch(vertex_fetch) {
    }

Pipeline::~Pipeline() {
    }

void Pipeline::utilise(GLenum primitive, GLsizei count) {
    // Only if the program was successfully linked.
    if(m_program.status() == Program::LINKAGE_SUCCEEDED) {
        // Check to see what, if any, program is currently active.
        GLint current_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        // If any active program is not the one pertaining
        // to this Pipeline object then make it so.
        if(current_program != m_program.ID()) {
            glUseProgram(m_program.ID());
            }

        // Dynamic cast to determine most derived type of the
        // VertexFetch object provided at construction.
        VertexFetch* p_bvf;
        p_bvf = dynamic_cast<VertexFetch*>(&m_vertex_fetch);
        if(p_bvf != NULL) {
            // So we have actually a BufferVertexFetch object.
            // Is it the same as the one currently bound to
            // the GL_ARRAY_BUFFER_BINDING target?
            GLint current_array_buffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_array_buffer);
            // If no active array buffer then make the one for this
            // Pipeline object the currently active one.
            if(current_array_buffer != OGL_ID::NO_ID) {
                glUseProgram(m_program.ID());
                }
            }

        m_vertex_fetch.trigger(primitive, count);
        }
    }
