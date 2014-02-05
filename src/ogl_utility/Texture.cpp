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

#include "Texture.h"

#include <iostream>

#include "ErrorHandler.h"

Texture::Texture(void) {
    }

Texture::~Texture() {
    // Must call this here in this derived class.
    release();
    }

bool Texture::acquire(void) {
    // Assume failure to acquire.
    bool ret_val = false;

    // Ask OpenGL to assign a texture object.
    glGenTextures(1, &ident);

    // Failure to acquire a handle should be FATAL.
    if(ident == OGL_ID::NO_ID) {
        ErrorHandler::record("Texture::acquire : failure to obtain identifier",
                             ErrorHandler::FATAL);
        }
    else {
        ret_val = true;
        }

    return ret_val;
    }

void Texture::release(void) {
    // Ask OpenGL to release the texture object.
    glDeleteTextures(1, &ident);
    }
