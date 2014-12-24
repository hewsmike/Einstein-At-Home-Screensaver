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

#include "VertexFetch.h"

#include "ErrorHandler.h"
#include "OGL_ID.h"

#include <iostream>

VertexFetch::VertexFetch(VertexBuffer* vertices, IndexBuffer* indices, GLenum index_type) :
                          m_vertices(vertices),
                          m_indices(indices),
                          m_index_type(index_type) {
    is_attached = false;
    }

VertexFetch::~VertexFetch() {
    }

void VertexFetch::attach(void) {
    // Bind only existing buffers.
    if(m_vertices != NULL) {
        m_vertices->attach();
        }
    if(m_indices != NULL) {
        m_indices->attach();
        }

    // Indicate that Buffer attachment to targets has been addressed.
    is_attached = true;
    }

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
    this->attach();

    // If buffers are not attached to targets then do so.
    if(is_attached == false) {
        ErrorHandler::record("VertexFetch::trigger() : buffers, if any, not attached, do so !",
                                         ErrorHandler::WARN);
        this->attach();
        }

	// Provokes vertex shader activity for count invocations,
	// buffer use depending upon that which is bound.
	if(m_indices != NULL) {
        // Both GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER targets are bound.
		OGL_DEBUG(glDrawElements(primitive, 0, count, m_indices->data()));
	    }
    else {
        // Either only GL_ARRAY_BUFFER target bound, or none at all.
    	OGL_DEBUG(glDrawArrays(primitive, 0, count));
        }
	this->detach();
    }

void VertexFetch::detach(void) {
    // Ensure that the pipeline vertex fetch
    // stage is not bound to any buffers at all.
    m_vertices->detach();
    m_indices->detach();

    // Reset attachment state.
    is_attached = false;
    }
