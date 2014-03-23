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

#include "ErrorHandler.h"

VertexBuffer::VertexBuffer(const GLvoid* buffer_data,
                           GLsizeiptr size,
                           GLenum usage) :
                Buffer(buffer_data) {
    // Ensure strictly positive buffer size.
    if(size > 0) {
        m_size = size;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Strictly positive buffer size required.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
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

void VertexBuffer::acquire_ID(GLuint* handle) const {
    glGenBuffers(1, handle);
    }

void VertexBuffer::release_ID(GLuint* handle) const {
    glDeleteBuffers(1, handle);
    }

void VertexBuffer::loadBuffer(GLenum target) const {
    // Bind this buffer to the specified target.
    glBindBuffer(target, this->ID());

    // Allocate space and transfer the data.
    glBufferData(target, m_size, this->data(), m_usage);

    // Unbind the buffer.
    glBindBuffer(target, OGL_ID::NO_ID);
    }

void VertexBuffer::loadBuffer(void) const {
    loadBuffer(GL_ARRAY_BUFFER);
    }
