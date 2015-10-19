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

#include "ErrorHandler.h"

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

    // Remember the buffer size in bytes.
    m_size = bytes;

    // Initially nothing acquired.
    acquire_flag = false;

    // NOr anyhting bound.
    bound_flag = false.
    }

Buffer::~Buffer() {
    }

bool Buffer::isAcquired(void) const {
    return acquire_flag;
    }

bool Buffer::isBound(void) const {
    return bound_flag;
    }

const GLvoid* Buffer::data(void) const {
    return m_data;
    }

void Buffer::setBoundState(bool state) {
    bound_flag = state;
    }

void Buffer::setAcquisitionState(bool state) {
    acquire_flag = state;
    }
