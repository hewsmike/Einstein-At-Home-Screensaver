/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

Buffer_OBJ::Buffer_OBJ() {
   }

Buffer_OBJ::~Buffer_OBJ() {
   // Must call this here in this derived class.
   release();
   }

void Buffer_OBJ::acquire(void) {
   // Ask OpenGL to assign a buffer object.
   glGenBuffers(1, &ident);
   }

void Buffer_OBJ::release(void) {
   // Ask OpenGL to release the buffer object.
   glDeleteBuffers(1, &ident);
   }
