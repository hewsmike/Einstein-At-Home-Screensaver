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

#include "Texture_OBJ.h"

#include <iostream>

#include "ErrorHandler.h"

Texture_OBJ::Texture_OBJ(void) {
    }

Texture_OBJ::~Texture_OBJ() {
    // Must call this here in this derived class.
    release();
    }

void Texture_OBJ::acquire(void) {
    // Ask OpenGL to assign a texture object.
    glGenTextures(1, &ident);

    std::cout << "Texture_OBJ::acquire() : check for OpenGL error - ident = " << ident<< std::endl;
    ErrorHandler::check_OpenGL_Error();
    std::cout << "Texture_OBJ::acquire() : OpenGL error checked\n" << std::endl;
    }

void Texture_OBJ::release(void) {
    // Ask OpenGL to release the texture object.
    glDeleteTextures(1, &ident);
    std::cout << "Texture_OBJ::release() : check for OpenGL error - ident = " << ident << std::endl;
    ErrorHandler::check_OpenGL_Error();
    std::cout << "Texture_OBJ::release() : OpenGL error checked\n" << std::endl;
    }
