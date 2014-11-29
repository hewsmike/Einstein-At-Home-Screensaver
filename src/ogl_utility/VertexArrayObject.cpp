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

#include "VertexArrayObject.h"

#include "ErrorHandler.h"

VertexArrayObject::VertexArrayObject(void) {
    }

VertexArrayObject::~VertexArrayObject() {
    VertexArrayObject::release();
    }

bool VertexArrayObject::acquire(void) {
    // Get an OpenGL handle for this shader object, if needed.
    if(this->ID() == OGL_ID::NO_ID) {
        GLuint temp;
        glGenVertexArrays(1, &temp);
        this->set_ID(temp);

        // If that handle acquisition failed the we have no other option ...
        if(this->ID() == OGL_ID::NO_ID)  {
            ErrorHandler::record("Shader::acquire() : OpenGL handle acquisition failure !",
                              ErrorHandler::FATAL);
            }
        }

    return true;
    }

void VertexArrayObject::release(void) {
    GLuint temp = this->ID();
    // Inform OpenGL that we no longer need this specific VertexArrayObject handle.
    glDeleteVertexArrays(1, &temp);
    // Set our handle store to safe value.
    set_ID(OGL_ID::NO_ID);
    }
