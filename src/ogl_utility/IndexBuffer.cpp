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

#include "IndexBuffer.h"

#include "ErrorHandler.h"

IndexBuffer::IndexBuffer(const GLvoid* buffer_data,
                         GLsizeiptr size,
                         GLenum usage,
                         GLenum index_type) :
                VertexBuffer(buffer_data, size, usage) {
    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((index_type == GL_UNSIGNED_BYTE)
       (index_type == GL_UNSIGNED_SHORT)
       (index_type == GL_UNISGNED_INT)) {
        m_index_type = index_type;
        }
    else {
        ErrorHandler::record("IndexBuffer::IndexBuffer() : Bad index type provided.",
                             ErrorHandler::FATAL);
        }
    }

IndexBuffer::~IndexBuffer() {
    Buffer::release();
    }

void IndexBuffer::loadBuffer(void) const {
    // Bind this buffer to the specified target.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID());

    // Allocate space and transfer the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, m_usage);

    // Unbind the buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL::NO_ID);
    }
