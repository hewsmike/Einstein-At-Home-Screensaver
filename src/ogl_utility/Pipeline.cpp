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

#include "Pipeline.h"

#include <iostream>

Pipeline::Pipeline(Program* program, VertexFetch* vertex_fetch, TextureBuffer* texture) :
                   m_texture_buffer(texture) {
    // Check Program pointer validity.
    if(program == NULL) {
        ErrorHandler::record("Pipeline::Pipeline() : No program provided.", ErrorHandler::FATAL);
        }
    m_program = program;

    // Check VertexFetch pointer validity.
    if(vertex_fetch == NULL) {
        ErrorHandler::record("Pipeline::Pipeline() : No vertex fetch provided.", ErrorHandler::INFORM);
        }
    m_vertex_fetch = vertex_fetch;
    }


Pipeline::~Pipeline() {
    }

void Pipeline::trigger(GLenum primitive, GLsizei count) {
    // Link program if not done.
    if(m_program->status() == Program::NEVER_LINKED) {
        ErrorHandler::record("Pipeline::trigger() : program needs acquisition ...", ErrorHandler::INFORM);
        m_program->configure();
        }

    // Need to acquire Vertexfetch if not already.
    if(m_vertex_fetch != NULL) {
        m_vertex_fetch->acquire();
        }

    // If you have a texture, but it is not acquired, then do so.
    if((m_texture_buffer != NULL) && (!m_texture_buffer->isAcquired())) {
        m_texture_buffer->acquire();
        }

    // Only if the program was successfully linked.
    if(m_program->status() == Program::LINKAGE_SUCCEEDED) {
        m_program-> bind();

        m_vertex_fetch->bind();

        // If texturing then bind.
        if(m_texture_buffer != NULL) {
            m_texture_buffer->bind();
            }

        m_program->frameCallBack();

        m_vertex_fetch->trigger(primitive, count);

        // If texturing then unbind.
        if(m_texture_buffer != NULL) {
            m_texture_buffer->unbind();
            }

        m_vertex_fetch->unbind();

        m_program->unbind();
        }
    else {
        ErrorHandler::record("Pipeline::trigger() : Program did not link !", ErrorHandler::FATAL);
        }
    }
