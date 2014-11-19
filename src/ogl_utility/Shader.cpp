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

#include <iostream>

#include "Shader.h"

#include "ErrorHandler.h"

const GLint Shader::GLSL_COMPILE_FAILURE(0);
const GLint Shader::GLSL_COMPILE_SUCCESS(1);

Shader::Shader(GLenum type, const std::string source) {
    // Check and store valid shader type.
    switch(type) {
        case GL_VERTEX_SHADER:
        case GL_FRAGMENT_SHADER:
            shader_type = type;
            break;
        default:
            // Should not arrive here.
            ErrorHandler::record("Shader::Shader() : bad switch case ( default )!", ErrorHandler::FATAL);
            break;
        }

    // Retrieve and store shader source code, if provided.
    if(source.size() != 0) {
        shader_source = source;
        }

    // Initially uncompiled.
    comp_status = Shader::NEVER_COMPILED;
    }

Shader::~Shader() {
    Shader::release();
    }

bool Shader::acquire(void) {
    // Assume failure to acquire.
    bool ret_val = false;

    // Clear the compilation log.
    compile_log = "";

    // Get an OpenGL handle for this shader object, if needed.
    if(this->ID() == OGL_ID::NO_ID) {
        // Get an OpenGL handle for this shader object.
        set_ID(glCreateShader(shader_type));

        // If that handle acquisition failed the we have no other option ...
        if(this->ID() == OGL_ID::NO_ID)  {
            ErrorHandler::record("Shader::acquire() : OpenGL handle acquisition failure !",
                                 ErrorHandler::FATAL);
            }
        }

    // Only manipulate this object if not currently marked for deletion.
    if(this->isDeleted() == false) {
        // Load the source code into the shader object.
        glShaderSource(this->ID(), 1, StringHelper(shader_source), NULL);

        // Only compile if never attempted.
        if(comp_status == Shader::NEVER_COMPILED) {
            ret_val = compile();

            // Examine the result.
            if(ret_val == true ) {
                comp_status = COMPILE_SUCCEEDED;
                ErrorHandler::record("Shader::acquire() : success of GLSL compile !",
                                     ErrorHandler::WARN);
                }
            else {
                comp_status = COMPILE_FAILED;
                ErrorHandler::record("Shader::acquire() : failure to GLSL compile !",
                                     ErrorHandler::WARN);
                }

            // Populate the compilation log ie. retrieve compiler error output.
            // Copy to an std::string via temporary character array to avoid
            // const semantic difficulties on the std::string c_str() method.
            GLint log_len;
            glGetShaderiv(this->ID(), GL_INFO_LOG_LENGTH, &log_len);
            // Use extra character to account for null character terminator ( documentation unclear ).
            GLchar* temp_log = new GLchar[log_len+1];
            GLsizei returned_log_len;
            glGetShaderInfoLog(this->ID(), log_len+1, &returned_log_len, temp_log);

            // Account for null character terminator ( documentation unclear ).
            temp_log[log_len] = '\0';
            compile_log = temp_log;

            // Dispose of the temporary character array.
            delete[] temp_log;
            }
        }

    return ret_val;
    }

void Shader::release(void) {
    // Inform OpenGL that we no longer need this specific shader handle.
    glDeleteShader(this->ID());
    // Set our handle store to safe value.
    set_ID(OGL_ID::NO_ID);
    // Reset compilation status.
    comp_status = Shader::NEVER_COMPILED;
    }

bool Shader::isDeleted(void) const {
    // Assume not marked for deletion.
    bool ret_val = false;

    // Deletion status only applies to a valid handle.
    if(this->ID() != OGL_ID::NO_ID) {
        // Lazy evaluation through inquiry to OpenGL context.
        GLint d_status;
        glGetShaderiv(this->ID(), GL_DELETE_STATUS, &d_status);
        if(d_status == GL_TRUE) {
            // It is marked for deletion.
            ret_val = true;
            }
        }

    return ret_val;
    }

bool Shader::compile(void) {
    // Assume compile failure
    bool ret_val = false;

    // Only compile a valid handle.
    if(this->ID() != OGL_ID::NO_ID) {
        // Present shader's GLSL code to the GLSL compiler.
        glCompileShader(this->ID());

        /// TODO - remove after full testing.
        // Check here proximally pro-tem.
        ErrorHandler::check_OpenGL_Error();

        // See if the compilation was a success.
        GLint c_status;
        glGetShaderiv(this->ID(), GL_COMPILE_STATUS, &c_status);
        if(c_status == GL_TRUE) {
            ret_val = true;
            }
        }

    return ret_val;
    }

Shader::compilationState Shader::status(void) const {
    return comp_status;
    }

GLenum Shader::type(void) const {
    return shader_type;
    }

const std::string& Shader::source(void) const {
    return shader_source;
    }

const std::string& Shader::compileLog(void) const {
    return compile_log;
    }
