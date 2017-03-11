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

#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLvoid* buffer_data,
                         GLuint bytes,
                         GLuint indices,
                         GLenum usage,
                         GLenum index_type) :
                Buffer(buffer_data, bytes),
                m_usage(usage) {
    // Ensure strictly positive index count.
    if(indices > 0) {
        m_indices = indices;
        }
    else {
        ErrorHandler::record("IndexBuffer::IndexBuffer() : Strictly positive index count required.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL acceptable parameter types.
    if((usage == GL_STREAM_DRAW) ||
       (usage == GL_STATIC_DRAW) ||
       (usage == GL_DYNAMIC_DRAW)) {
        m_usage = usage;
        }
    else {
        ErrorHandler::record("IndexBuffer::IndexBuffer() : Bad usage type provided.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL acceptable parameter types.
    switch(index_type) {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_INT:
            m_index_type = index_type;
            break;
        default:
            ErrorHandler::record("IndexBuffer::IndexBuffer() : Bad index type provided.",
                                 ErrorHandler::FATAL);
            break;
        }
    }

IndexBuffer::~IndexBuffer() {
    this->release();
    }

bool IndexBuffer::acquire(void) {
    GLuint handle;
    glGenBuffers(1, &handle);
    this->set_ID(handle);
    this->setAcquisitionState(true);

    this->bind();
    this->loadBuffer();
    this->unbind();

    return true;
    }

void IndexBuffer::release(void) {
    GLuint handle(this->ID());
    glDeleteBuffers(1, &handle);
    this->reset_ID();
    this->setAcquisitionState(false);
    }

void IndexBuffer::bind(void) {
    // Ensure resource acquisition first.
    if(this->isAcquired() == false) {
        // Ensure resource acquisition first.
        this->acquire();
        }

    // Bind the given buffer object to pipeline state.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID());
    }

void IndexBuffer::unbind(void) {
    // Unbind the given buffer object from pipeline state.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_ID::NO_ID);
    }

bool IndexBuffer::isBound(void) const {
    // Assume failure.
    bool ret_val = false;

    // Discover which element array buffer, if any, is bound to the OpenGL state.
    GLuint temp;

    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*) &temp);

    if((this->ID() == temp) && (this->ID() != OGL_ID::NO_ID)) {
        ret_val = true;
        }

    return ret_val;
    }

GLenum IndexBuffer::indexType(void) const {
    return m_index_type;
    }

void IndexBuffer::loadBuffer(void) {
    // Allocate space and transfer the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size(), this->data(), m_usage);
    }
