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

#include "VertexFetch.h"

#include <iostream>
#include <sstream>

VertexFetch::VertexFetch(){
	m_bound_flag = false;
    }

VertexFetch::~VertexFetch() {
	release();
	}

bool VertexFetch::acquire(void) {
	// Assume failure.
	bool ret_val = false;

	// Check and maybe acquire handle if we don't already have one.
	if(this->ID() == OGL_ID::NO_ID) {
		// Ask OpenGL for a single VAO handle.
		GLuint temp = 0;
		glGenVertexArrays(1, &temp);
		set_ID(temp);

		// Failure to acquire a handle should be FATAL.
		if(this->ID() == OGL_ID::NO_ID) {
			ErrorHandler::record("VertexFetch::acquire() : failure to obtain identifier",
								 ErrorHandler::FATAL);
			}
		ret_val = true;
		}

	return ret_val;
    }

void VertexFetch::release(void) {
	// Inform OpenGL that we no longer need this specific VAO handle.
	GLuint temp = this->ID();
	glDeleteVertexArrays(1, &temp);

	// Reset our handle store to safe value.
	set_ID(OGL_ID::NO_ID);
    }

void VertexFetch::bind(void) {
	glBindVertexArray(this->ID());

	m_bound_flag = true;
	}

void VertexFetch::unbind(void) {
	glBindVertexArray(OGL_ID::NO_ID);

	// Reset attachment state.
    m_bound_flag = false;
    }

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
    // If VAO is not bound then do so.
    if(m_bound_flag == false) {
        ErrorHandler::record("VertexFetch::trigger() : VAO not attached !",
                              ErrorHandler::WARN);
        this->bind();
        }

    // Provokes vertex shader activity for count invocations.
    glDrawArrays(primitive, 0, count);
	}

bool VertexFetch::isBound(void) const {
	return m_bound_flag;
	}
