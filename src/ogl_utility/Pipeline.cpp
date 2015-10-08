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

#include <iostream>

Pipeline::Pipeline(Program& program, VertexFetch& vertex_fetch) :
                    m_program(program),
                    m_vertex_fetch(vertex_fetch) {
    }


Pipeline::~Pipeline() {
    }

void Pipeline::utilise(GLenum primitive, GLsizei count) {
	// Link program if not done.
    if(m_program.status() == Program::NEVER_LINKED) {
    	ErrorHandler::record("Pipeline::utilise() : program needs acqusition ...", ErrorHandler::INFORM);
        m_program.acquire();
        }

    // Only if the program was successfully linked.
    if(m_program.status() == Program::LINKAGE_SUCCEEDED) {
    	m_program.use();

    	m_vertex_fetch.bind();

    	m_program.frameCallBack();

        m_vertex_fetch.trigger(primitive, count);

        m_vertex_fetch.unbind();

        m_program.stopUse();
        }
    else {
        /// TODO - Error path if no program link ?
        }
    }
