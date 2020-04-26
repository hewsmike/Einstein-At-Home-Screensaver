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
    // A null texture pointer will just be stored. Trigger() will handle nullity.
    // Check Program pointer validity.
    if(program == NULL) {
        // Got to have a Program involved.
        ErrorHandler::record("Pipeline::Pipeline() : No program provided.", ErrorHandler::FATAL);
        }
    m_program = program;

    // Check VertexFetch pointer validity.
    if(vertex_fetch == NULL) {
        // Curiously you can have a Pipeline without vertex fetching,
        // which is useful for vertex shader generated position, color
        // & texture coordinate data.
        ErrorHandler::record("Pipeline::Pipeline() : No vertex fetch provided.", ErrorHandler::FATAL);
        }
    m_vertex_fetch = vertex_fetch;
    }


Pipeline::~Pipeline() {
    }

void Pipeline::trigger(GLenum primitive, GLsizei count) {
    // Link program etc if not already done.
    if(m_program->status() == Program::NEVER_LINKED) {
        ErrorHandler::record("Pipeline::trigger() : program needs configuring ...", ErrorHandler::INFORM);
        m_program->configure();
        }

    // Need to configure Vertexfetch if not already.
    if((m_vertex_fetch != NULL) && (!m_vertex_fetch->isConfigured())) {
        ErrorHandler::record("Pipeline::trigger() : vertex fetch needs configuring ...", ErrorHandler::INFORM);
        m_vertex_fetch->configure();
        }

    // If you have a texture, but it is not acquired, then do so.
    if((m_texture_buffer != NULL) && (!m_texture_buffer->isAcquired())) {
        m_texture_buffer->acquire();
        }

    // Only if the program was successfully linked.
    if(m_program->status() == Program::LINKAGE_SUCCEEDED) {
        // All this binding and unbinding is explicit so the
        // state machine defaults need not be relied upon.
        m_program-> bind();
        m_vertex_fetch->bind();

        // If texturing then bind.
        if(m_texture_buffer != NULL) {
            m_texture_buffer->bind();
            }

        // This will load uniform data from client
        // space into the vertex shader, once per
        // rendered frame.
        m_program->frameCallBack();

        // Finally start some rendering pipeline activity.
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
