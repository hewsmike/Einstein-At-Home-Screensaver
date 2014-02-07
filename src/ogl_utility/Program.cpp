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

#include "Program.h"

#include "ErrorHandler.h"

Program::Program(const Shader& vertex_shader, const Shader& fragment_shader) :
                    m_vertex_shader(vertex_shader),
                    m_fragment_shader(fragment_shader) {
    }

Program::~Program() {
    Program::release();
    }

bool Program::acquire(void) {
    // Assume failure.
    bool ret_val = false;

    // Get an OpenGl handle for this program object.
    OGL_ID::ident = glCreateProgram();
    // If that handle acquisition failed the we have no other option ...
    if(OGL_ID::ident == OGL_ID::NO_ID)  {
        ErrorHandler::record("Program::acquire() : OpenGL handle acquisition failure !", ErrorHandler::FATAL);
        }

    return ret_val;
    }

void Program::release(void) {
    // Inform OpenGL that we no longer need this specific program handle.
    glDeleteProgram(ident);
    // Set our handle store to safe value.
    ident = OGL_ID::NO_ID;
    }
