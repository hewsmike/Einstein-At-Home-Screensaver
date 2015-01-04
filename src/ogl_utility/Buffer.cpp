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

#include <iostream>

Buffer::Buffer(const GLvoid* buffer_data, GLuint bytes) {
    // Ensure an actual data source was provided.
    if(buffer_data != NULL) {
        m_data = buffer_data;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : NULL data pointer provided.",
                             ErrorHandler::FATAL);
        }

    // Remember the buffer size in bytes.
    m_size = bytes;

    // Initially nothing acquired.
    acquire_flag = false;
    }

Buffer::~Buffer() {
    }

bool Buffer::acquire(void) {
	// Check and maybe acquire handle if we don't already have one.
    if(this->ID() == OGL_ID::NO_ID) {
        // Ask OpenGL for a single buffer handle.
        GLuint temp = 0;
        acquire_ID(&temp);
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
    	/// TODO - check return value to see if load did succeed !
        loadBuffer();
        // Denote that acquisition succeeded.
        acquire_flag = true;
        }

    return acquire_flag;
    }

void Buffer::release(void) {
    // Inform OpenGL that we no longer need this specific buffer handle.
	GLuint temp = this->ID();
    release_ID(&temp);

    // Reset our handle store to safe value.
    set_ID(OGL_ID::NO_ID);

    // Reset acquisition status.
    acquire_flag = false;
    }

const GLvoid* Buffer::data(void) const {
    return m_data;
    }

GLuint Buffer::size(void) const {
    return m_size;
    }
