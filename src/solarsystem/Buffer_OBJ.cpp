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

//#ifdef WIN32_GLEXT_LINKS
#include "OpenGLExts.h"
#include <iostream>
///#endif

Buffer_OBJ::Buffer_OBJ() {
   }

Buffer_OBJ::~Buffer_OBJ() {
   // Must call this here in this derived class.
   release();
   }

void Buffer_OBJ::acquire(void) {
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glGenBuffers(1, &ident);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLGenBuffers(1, &ident);
#endif
   }

void Buffer_OBJ::release(void) {
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glDeleteBuffers(1, &ident);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLDeleteBuffers(1, &ident);
#endif
   }
