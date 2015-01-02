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
                         GLuint indices,
                         GLenum usage,
                         GLenum index_type) :
                Buffer(buffer_data),
                m_usage(usage) {
    // Ensure strictly positive index count.
    if(indices > 0) {
        m_indices = indices;
        }
    else {
        ErrorHandler::record("IndexBuffer::IndexBuffer() : Strictly positive index count required.",
                             ErrorHandler::FATAL);
        }

    // Initially buffer size is nil.
    m_size = 0;

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    m_index_type = index_type;
    switch(index_type) {
        case GL_UNSIGNED_BYTE:
            m_size = indices * sizeof(GLubyte);
            break;
        case GL_UNSIGNED_SHORT:
            m_size = indices * sizeof(GLushort);
            break;
        case GL_UNSIGNED_INT:
            m_size = indices * sizeof(GLuint);
            break;
        default:
            ErrorHandler::record("IndexBuffer::IndexBuffer() : Bad index type provided.",
                                 ErrorHandler::FATAL);
            break;
        }
    }

IndexBuffer::~IndexBuffer() {
    Buffer::release();
    }

void IndexBuffer::acquire_ID(GLuint* handle) const {
	OGL_DEBUG(glGenBuffers(1, handle));
	}

void IndexBuffer::release_ID(GLuint* handle) const {
	OGL_DEBUG(glDeleteBuffers(1, handle));
    }

void IndexBuffer::loadBuffer(void) const {
    // Bind this buffer to the specified target.
    OGL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID()));

    // Allocate space and transfer the data.
    std::cout << "IndexBuffer::loadBuffer() : index buffer data is (pre-load) : " << std::endl;

        for(unsigned int index = 0; index < m_indices; ++index) {
        	std::cout << "\t value = " << *(static_cast<const GLuint*>(this->data()) + index) << std::endl;
        	}
    OGL_DEBUG(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, this->data(), m_usage));

    void* m_data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,  GL_READ_ONLY);

    std::cout << "IndexBuffer::loadBuffer() : index buffer data is (post-load) : " << std::endl;

    for(unsigned int index = 0; index < m_indices; ++index) {
    	std::cout << "\t value = " << *(static_cast<GLuint*>(m_data) + index) << std::endl;
    	}

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    // Unbind the buffer.
    OGL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_ID::NO_ID));
    }

void IndexBuffer::attach(void) {
    // Ensure resource acquisition first.
    this->acquire();
    // Bind the given buffer object to pipeline state.
    OGL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID()));
    }

void IndexBuffer::detach(void) {
    // Unbind the given buffer object from pipeline state.
	OGL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_ID::NO_ID));
    }

GLenum IndexBuffer::indexType(void) const {
	return m_index_type;
	}
