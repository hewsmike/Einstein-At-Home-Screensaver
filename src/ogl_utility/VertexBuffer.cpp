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

VertexBuffer::VertexBuffer(GLenum target,
                           GLsizeiptr size,
                           GLenum usage,
                           const GLvoid* buffer_data) :
                Buffer(buffer_data) {
    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((target == GL_ARRAY_BUFFER) ||
       (target == GL_ELEMENT_ARRAY_BUFFER)) {
        m_target = target;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : Bad target type provided.",
                             ErrorHandler::FATAL);
        }

    // Ensure strictly positive buffer size.
    if(size > 0) {
        m_size = size;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : Strictly positive buffer size required.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((usage == GL_STREAM_DRAW) ||
       (usage == GL_STATIC_DRAW) ||
       (usage == GL_DYNAMIC_DRAW)) {
        m_usage = usage;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : Bad usage type provided.",
                             ErrorHandler::FATAL);
        }
    }

VertexBuffer::~VertexBuffer() {
    Buffer::release();
    }

GLenum VertexBuffer::target(void) const {
    return m_target;
    }

GLenum VertexBuffer::usage(void) const {
    return m_usage;
    }

GLuint VertexBuffer::acquire_ID(GLuint* handle) const {
    glGenBuffers(1, handle);
    }

GLuint VertexBuffer::release_ID(GLuint* handle) const {
    glDeleteBuffers(1, handle);
    }

void VertexBuffer::loadBuffer(void) const {
    // Bind this buffer to the specified target.
    glBindBuffer(m_target, this->ID());

    // Allocate space and transfer the data.
    glBufferData(m_target, m_size, m_data, m_usage);

    // Unbind the buffer.
    glBindBuffer(m_target, Buffer::NO_ID);
    }