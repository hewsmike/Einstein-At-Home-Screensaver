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

#include "OGL_ID.h"

#include "ErrorHandler.h"

// An ID with value zero is not returned by OpenGL.
// Thus it may semantically indicate 'no identifier assigned'.
const GLuint OGL_ID::NO_ID(0);

// Make sure you start with an un-assigned identifier,
// lest you later over-write resources!
OGL_ID::OGL_ID() : ident(OGL_ID::NO_ID) {
    }

// Must NOT call release() from here !!
OGL_ID::~OGL_ID() {
    }

GLuint OGL_ID::ID(void) const {
    return ident;
    }

void OGL_ID::set_ID(GLuint value) {
    ident = value;
    }
