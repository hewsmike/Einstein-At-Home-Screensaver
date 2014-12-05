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

bool OGL_ID::isNull(void) const {
    return (ID() ? false : true);
    }

id_type OGL_ID::type(bool emit) const {
    id_type ret_val;
    // Lazy evaluation by inquiry of OpenGL state machine.
    temp_ID = ID();

    if(temp_ID == OGL_ID::NO_ID) {
        ret_val = OGL_ID::NULL_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : NULL_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsBuffer(temp_ID)) {
        ret_val = OGL_ID::BUFFER_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : BUFFER_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsFrameBuffer(temp_ID)) {
        ret_val = OGL_ID::FRAME_BUFFER_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : FRAME_BUFFER_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsProgram(temp_ID)) {
        ret_val = OGL_ID::PROGRAM_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : PROGRAM_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsRenderBuffer(temp_ID)) {
        ret_val = OGL_ID::RENDER_BUFFER_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : RENDER_BUFFER_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsShader(temp_ID)) {
        ret_val = OGL_ID::SHADER_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : SHADER_ID" , ErrorHandler::INFORM);
            }
        }
    else if(glIsTexture(temp_ID)) {
        ret_val = OGL_ID::TEXTURE_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : TEXTURE_ID" , ErrorHandler::INFORM);
            }
        }
    else {
        ret_val = OGL_ID::INVALID_ID;
        if(emit){
            ErrorHandler("OGL_ID::type() : INVALID_ID" , ErrorHandler::INFORM);
            }
        }

    return ret_val;
    }
