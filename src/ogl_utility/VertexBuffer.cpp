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

#include "VertexBuffer.h"

#include <iostream>
#include <sstream>

VertexBuffer::VertexBuffer(const GLvoid* buffer_data,
						   GLuint bytes,
		                   GLuint vertices,
                           GLenum usage,
                           data_mix mix) :
                Buffer(buffer_data, bytes),
                m_mix(mix) {
    // Ensure strictly positive vertex count.
    if(vertices > 0) {
        m_vertex_count = vertices;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Strictly positive vertex count required.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL acceptable parameter types.
    if((usage == GL_STREAM_DRAW) ||
       (usage == GL_STATIC_DRAW) ||
       (usage == GL_DYNAMIC_DRAW)) {
        m_usage = usage;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Bad usage type provided.",
                             ErrorHandler::FATAL);
        }
    }

VertexBuffer::~VertexBuffer() {
    Buffer::release();
    }

GLuint VertexBuffer::vertexCount(void) const {
    return m_vertex_count;
    }

void VertexBuffer::acquire_ID(GLuint* handle) const {
	glGenBuffers(1, handle);
	}

void VertexBuffer::release_ID(GLuint* handle) const {
	glDeleteBuffers(1, handle);
    }

void VertexBuffer::loadBuffer(void) const {
	glBindBuffer(GL_ARRAY_BUFFER, this->ID());

    // Allocate space and transfer the data.
	glBufferData(GL_ARRAY_BUFFER, this->size(), this->data(), m_usage);

    // Unbind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
	}

void VertexBuffer::bind(void) {
    // Ensure resource acquisition first.
    this->acquire();

	// Bind the given buffer object to pipeline state.
	glBindBuffer(GL_ARRAY_BUFFER, this->ID());
	}

void VertexBuffer::unbind(void) {
    // Unbind the given buffer object from pipeline state.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
	}

VertexBuffer::data_mix VertexBuffer::mix(void) const {
	return m_mix;
	}

bool VertexBuffer::isBound(void) const {
	// Assume failure.
	bool ret_val = false;

	// Discover which array buffer, if any, is bound to the OpenGL state.
	GLuint temp;

	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &temp);

	if((this->ID() == temp) && (this->ID() != OGL_ID::NO_ID)) {
		ret_val = true;
		}

	return ret_val;
	}
