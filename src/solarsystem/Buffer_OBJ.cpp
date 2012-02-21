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

#include "ErrorHandler.h"

#ifdef WIN32_GLEXT_LINKS
genBuffers_Func Buffer_OBJ::genBuffersLink(NULL);
deleteBuffers_Func Buffer_OBJ::deleteBuffersLink(NULL);
bindBuffer_Func Buffer_OBJ::bindBufferLink(NULL);
bufferData_Func Buffer_OBJ::bufferDataLink(NULL);
mapBuffer_Func Buffer_OBJ::mapBufferLink(NULL);
unMapBuffer_Func Buffer_OBJ::unMapBufferLink(NULL);
#endif

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
