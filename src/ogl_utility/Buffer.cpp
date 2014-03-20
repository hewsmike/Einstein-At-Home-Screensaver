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

#include "Buffer.h"
#include "ErrorHandler.h"

Buffer::Buffer(GLenum target, GLsizeiptr size, GLenum usage, const GLvoid* data) {
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

    // Ensure an actual data source was provided.
    if(data != NULL) {
        m_data = data;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : NULL data pointer provided.",
                             ErrorHandler::FATAL);
        }

    // Initially nothing acquired.
    acquire_flag = false;
    }

Buffer::~Buffer() {
    // Must call this here in this derived class.
    Buffer::release();
    }

bool Buffer::acquire(void) {
    // Check and maybe acquire handle if we don't already have one.
    if(this->ID() == OGL_ID::NO_ID) {
        // Ask OpenGL for a single buffer handle.
        GLuint temp;
        glGenBuffers(1, &temp);
        set_ID(temp);

        // Failure to acquire a handle should be FATAL.
        if(this->ID() == OGL_ID::NO_ID) {
            ErrorHandler::record("Buffer::acquire() : failure to obtain identifier",
                                 ErrorHandler::FATAL);
            }
        }

    // Only load if not already done.
    if(acquire_flag == false) {
        // Use the handle and load data.
        loadBuffer();
        // Denote that acquisition succeeded.
        acquire_flag = true;
        }

    return acquire_flag;
    }

void Buffer::release(void) {
    // Inform OpenGL that we no longer need this specific buffer handle.
    GLuint temp = this->ID();
    glDeleteBuffers(1, &temp);
    // Reset our handle store to safe value.
    set_ID(OGL_ID::NO_ID);

    // Reset acquisition status.
    acquire_flag = false;
    }

GLenum Buffer::target(void) const {
    return m_target;
    }

GLenum Buffer::usage(void) const {
    return m_usage;
    }

void Buffer::loadBuffer(void) const {
	// Bind this buffer to the specified target.
	glBindBuffer(m_target, this->ID());

	// Allocate space and transfer the data.
	glBufferData(m_target, m_size, m_data, m_usage);

	// Unbind the buffer.
	glBindBuffer(m_target, Buffer::NO_ID);
	}
