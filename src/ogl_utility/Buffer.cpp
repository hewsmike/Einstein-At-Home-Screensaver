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

Buffer::Buffer(GLenum target, GLsizeiptr size, GLenum usage, const GLvoid* data) {
    }

Buffer::~Buffer() {
    // Must call this here in this derived class.
    Buffer::release();
    }

bool Buffer::acquire(void) {
    // Assume failure to acquire.
    bool ret_val = false;

    // Ask OpenGL for a single buffer handle.
    glGenBuffers(1, &ident);

    // Failure to acquire a handle should be FATAL.
    if(ident == OGL_ID::NO_ID) {
        ErrorHandler::record("Buffer::acquire : failure to obtain identifier",
                             ErrorHandler::FATAL);
        }
    else {
        ret_val = true;
        }

    return ret_val;
    }

void Buffer::release(void) {
    // Inform OpenGL that we no longer need this specific buffer handle.
    glDeleteBuffers(1, &ident);
    }

void Buffer::loadBuffer() {
	// This implementation avoids both the poor performance and poor error
	// reporting ( as disclosed in actual practice ) of glMapBuffer().

	// If we have no identifier allocation then get one.
	if(this->ID() == OGL_ID::NO_ID) {
		this->acquire();
		}

	// Bind the buffer ( of 'target' type ) to our identifier.
	glBindBuffer(m_target, this->ID());

	// Allocate space. NULL for third parameter means no data
	// transfer for THIS OpenGL call. See note below.
	glBufferData(m_target, m_size, m_data, m_usage);

	// Unbind the buffer.
	glBindBuffer(m_target, Buffer::NO_ID);
	}
