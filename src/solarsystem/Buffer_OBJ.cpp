/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#include "Buffer_OBJ.h"

#include "ErrorHandler.h"

Buffer_OBJ::Buffer_OBJ() {
   }

Buffer_OBJ::~Buffer_OBJ() {
   // Must call this here in this derived class.
   release();
   }

void Buffer_OBJ::acquire(void) {
   glGenBuffers(1, &ident);
   }

void Buffer_OBJ::release(void) {
   glDeleteBuffers(1, &ident);
   }

void Buffer_OBJ::loadBuffer(GLenum  target, GLenum  usage, GLsizeiptr size, const GLvoid* data) {
	// This implementation avoids both the poor performance
	// and poor error reporting ( found in actual practice )
	// of glMapBuffer().

	// If we have no identifier allocation then get one.
	if(this->ID() == OGL_ID::NO_ID) {
		this->acquire();
		}

	// Bind the buffer ( type ) to our identifier.
	glBindBuffer(target, this->ID());
   
	// Allocate space.
	glBufferData(target, size, NULL, usage);
   
	// Then transfer.
	glBufferSubData(target, 0, size, data);

	// Unbind the buffer.
	glBindBuffer(target, Buffer_OBJ::NO_ID);
	}
