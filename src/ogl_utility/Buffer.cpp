/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

Buffer::Buffer(GLenum target, GLsizeiptr size, GLenum usage, const GLvoid* data) :
                m_target(target), m_size(size), m_usage(usage), m_data(data) {
    }

Buffer::~Buffer() {
    // Must call this here in this derived class.
    Buffer::release();
    }

bool Buffer::acquire(void) {
    // Assume failure to acquire.
    bool ret_val = false;

    // Ask OpenGL for a single buffer handle.
    GLuint temp;
    glGenBuffers(1, &temp);
    set_ID(temp);

    // Failure to acquire a handle should be FATAL.
    if(this->ID() == OGL_ID::NO_ID) {
        ErrorHandler::record("Buffer::acquire() : failure to obtain identifier",
                             ErrorHandler::FATAL);
        }
    else {
        // Use the handle and load data.
        loadBuffer();
        ret_val = true;
        }

    return ret_val;
    }

void Buffer::release(void) {
    // Inform OpenGL that we no longer need this specific buffer handle.
    GLuint temp = this->ID();
    glDeleteBuffers(1, &temp);
    // Set our handle store to safe value.
    set_ID(OGL_ID::NO_ID);
    }

void Buffer::loadBuffer() {
	// Bind the buffer ( of 'target' type ) to our identifier.
	glBindBuffer(m_target, this->ID());

	// Allocate space and transfer any data.
	glBufferData(m_target, m_size, m_data, m_usage);

	// Unbind the buffer.
	glBindBuffer(m_target, Buffer::NO_ID);
	}

GLenum Buffer::target(void) const {
    return m_target;
    }
