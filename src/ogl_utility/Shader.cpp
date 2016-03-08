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

#include "Shader.h"

#include <iostream>
#include <sstream>

const GLint Shader::GLSL_COMPILE_FAILURE(0);
const GLint Shader::GLSL_COMPILE_SUCCESS(1);

Shader::Shader(GLenum type, const std::string& source) {
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
    else {
        ErrorHandler::record("Shader::Shader() : no shader source code provided !", ErrorHandler::FATAL);
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

    // Only manipulate this object if not currently marked for deletion,
    // and a Shader object with no OpengGL handle yet assigned will not
    // be so marked.
    if(this->isDeleted() == false) {
        // Get an OpenGL handle for this shader object, if needed.
        if(this->ID() == OGL_ID::NO_ID) {
            // Get an OpenGL handle for this shader object.
            GLuint temp = glCreateShader(shader_type);
            this->set_ID(temp);

            // If that handle acquisition failed the we have no other option ...
            if(this->ID() == OGL_ID::NO_ID)  {
                ErrorHandler::record("Shader::acquire() : OpenGL handle acquisition failure !",
                                      ErrorHandler::FATAL);
                }
            }

        glShaderSource(this->ID(), 1, StringHelper(shader_source), NULL);

        // Only compile if never attempted.
        if(comp_status == Shader::NEVER_COMPILED) {
            ret_val = compile();
            }
        }

    return ret_val;
    }

void Shader::release(void) {
    // NB This means the OpenGL state machine marks an OpenGL shader object
    // for deletion. This does not refer to any client source code entity.
    glDeleteShader(this->ID());
    // Set our handle store to safe value.
    this->reset_ID();
    // Reset compilation status.
    comp_status = Shader::NEVER_COMPILED;
    }

bool Shader::configure(void) {
	// Assume failure.
	bool ret_val = false;
	if(this->isAcquired() == false) {
		this->acquire();
		}

	ret_val = compile();

	return ret_val;
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
    comp_status = COMPILE_FAILED;

    // Clear the compilation log.
    compile_log = "";

    // Only compile a valid handle.
    if(this->ID() != OGL_ID::NO_ID) {
        // Present shader's GLSL code to the GLSL compiler.
        glCompileShader(this->ID());

        // See if the compilation was a success.
        GLint c_status;
        glGetShaderiv(this->ID(), GL_COMPILE_STATUS, &c_status);
        if(c_status == GL_TRUE) {
            // Compile good.
            comp_status = COMPILE_SUCCEEDED;
            ret_val = true;
            }
        else {
            // Populate the compilation log ie. retrieve compiler error output.
            // Copy to an std::string via temporary character array to avoid
            // const semantic difficulties on the std::string c_str() method.
            // The log length established by this call INCLUDES a null-termination.
            GLint log_len;
            glGetShaderiv(this->ID(), GL_INFO_LOG_LENGTH, &log_len);

            // Strictly positive log length indicates an error log is available
            if(log_len > 0) {
                // Create temporary character array to receive the error log.
                GLchar* temp_log = new GLchar[log_len];

                // The returned log length DOES NOT INCLUDE a null temination.
                GLsizei returned_log_len = 0;
                // Having log-len as the second parameter here limits the write to temp_log
                // ie. stops buffer over-run.
                glGetShaderInfoLog(this->ID(), log_len, &returned_log_len, temp_log);

                // Is the returned error log of expected length ?
                // Bear in mind that we expect a difference of one character for null termination.
                if(log_len != returned_log_len + 1) {
                    std::stringstream error_log_len_msg;
                    error_log_len_msg << "Shader::compile() : error log length = "
                                      << returned_log_len + 1
                                      << "("
                                      << log_len
                                      << " expected)";
                    /// TODO - should this be FATAL ?
                    ErrorHandler::record(error_log_len_msg.str(),
                                         ErrorHandler::WARN);
                    }

                // Paranoia !
                temp_log[log_len -1] = '\0';

                // Store the log.
                compile_log = temp_log;

                // Dispose of the temporary character array.
                delete[] temp_log;
                }
            }
        }

    // Some improved reporting.
    ErrorHandler::message_type info_level = ErrorHandler::INFORM;
    std::stringstream shader_compile_error_msg;
    shader_compile_error_msg << "Shader::compile() : ";

    if(this->type() == GL_VERTEX_SHADER) {
            shader_compile_error_msg << "GL_VERTEX_SHADER";
            }
    if(this->type() == GL_FRAGMENT_SHADER) {
            shader_compile_error_msg << "GL_FRAGMENT_SHADER";
            }

    shader_compile_error_msg << " ( ID = "
                             << this->ID()
                             << " ) did ";

    if(this->status() != Shader::COMPILE_SUCCEEDED) {
        info_level = ErrorHandler::WARN;
        shader_compile_error_msg <<    "not compile !!\n"
                                 << "Compile log is as follows :\n"
                                 << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                                 << this->compileLog()
                                 << "------------------------------------------------------\n"
                                 << "Shader source code is as follows :\n"
                                 << "------------------------------------------------------\n"
                                 << this->source()
                                 << "++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        }
    else {
        shader_compile_error_msg <<    "compile ... ";
        }

    ErrorHandler::record(shader_compile_error_msg.str(), info_level);

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
