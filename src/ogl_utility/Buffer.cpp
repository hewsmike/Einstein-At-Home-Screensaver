/***************************************************************************
 *   Copyright (C) 2015 by Mike Hewson                                     *
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

Buffer::Buffer(const GLvoid* buffer_data, GLuint bytes) {
    // Ensure an actual data source was provided.
    if(buffer_data != NULL) {
        // Pointer copy only, not deep.
        m_data = buffer_data;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : NULL data pointer provided.",
                             ErrorHandler::FATAL);
        }

    // Ensure the buffer size is strictly positive.
    if(m_size > 0) {
        // Remember the buffer size in bytes.
        m_size = bytes;
        }
    else {
        ErrorHandler::record("Buffer::Buffer() : Invalid data size provided.",
                             ErrorHandler::FATAL);
        }
    }

Buffer::~Buffer() {
    }

bool Buffer::acquire(void) {
    // Only acquire if not already so.
    if(this->isAcquired() == false) {
        // Get a handle from the state machine.
        GLuint handle;
        this->acquire_ID(&handle);
        glGenBuffers(1, &handle);
        // Remember the value for later.
        OGL_ID::set_ID(handle);

        // Copy the data from client side to be stored in the state
        // machine object.
        loadBuffer();

        // Record resources as acquired.
        Buffer::setAcquisitionState(true);
        }

    /// TODO  Oddly we can't determine if the above worked via straight forward API calls.
    return true;
    }

void Buffer::release(void) {
    // Only release if already acquired.
    if(this->isAcquired() == true) {
        GLuint handle = this->ID();
        this->release_ID(&handle);
        // Reset to the null case.
        OGL_ID::set_ID(OGL_ID::NO_ID);
        // Mark as not acquired.
        Buffer::setAcquisitionState(false);
        }
    }

const GLvoid* Buffer::data(void) const {
    return m_data;
    }

GLuint Buffer::size(void) const {
    return m_size;
    }
