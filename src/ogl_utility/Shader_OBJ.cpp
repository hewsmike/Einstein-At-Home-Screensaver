/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include "Shader_OBJ.h"

#include <sstream>

#include "ErrorHandler.h"
#include "Resource.h"
#include "ResourceFactory.h"

Shader_OBJ::Shader_OBJ(const string resource_identifier, GLenum shader_type) {
    // Remember the shader type.
    type = shader_type;

    // Assume failure.
    state = Shader_OBJ::INVALID;

    // Get a shader identifier.
    acquire();

    // Instantiate a ResourceFactory in order to obtain the
    // text of the Shader_OBJ.
    ResourceFactory factory;

    Resource* txt = factory.createInstance(resource_identifier);
    if(txt != NULL) {
        const vector<unsigned char>* contents = txt.data();
        glShaderSource(ident, contents.size(),
                       txt->data().const GLchar **string, NULL);

        const vector<unsigned char>* contents = txt.data();
        for(unsigned int index = 0; index < contents->sid ze(); ++index) {
            text += (*contents)[index];
            }
        state = Shader_OBJ::EXISTS;
        }
    else {
        std::stringstream msg;
        msg << "Shader_OBJ::Shader_OBJ() : no resource to construct Shader_OBJ available "
            << "for resource identifier = '" << resource_identifier << "'";
        ErrorHandler::record(msg, ErrorHandler::WARN);
        }

    // Now try and compile it.


    }

Shader_OBJ::~Shader_OBJ() {
    // Must call this here in this derived class.
    release();
    }

Shader_OBJ::status Shader_OBJ::getStatus(void) const {
    return state;
    }

void Shader_OBJ::acquire(void) {
    // Ask OpenGL for a single shader handle.
    ident = glCreateShader(type);

    // Failure to acquire a handle should be FATAL.
    if(ident == OGL_ID::NO_ID) {
        ErrorHandler::record("Shader_OBJ::acquire : failure to obtain identifier",
                             ErrorHandler::FATAL);
        }
    }

void Shader_OBJ::release(void) {
    // Inform OpenGL that we no longer need this specific shader handle.
    glDeleteShader(ident);

    // And set the identifier as unused for OpenGL.
    ident = OGL_ID::NO_ID;
    }
