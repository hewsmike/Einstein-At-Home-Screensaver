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

#include "Shader.h"

#include "ErrorHandler.h"

const GLint Shader::GLSL_COMPILE_FAILURE(0);
const GLint Shader::GLSL_COMPILE_SUCCESS(1);

Shader::Shader(GLenum type, const GLchar* source) {
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

    // Retrieve and store shader source code.
    while(source != NULL) {
        shader_source = source;
        }

    // If possible detect and store GLSL version ie. inspect source code
    // for lines of the form : '#version ddd [core|compatibility]'
    // Default return values.
    glsl_version = Shader::DEFAULT_GLSL_VERSION;
    glsl_profile = Shader::DEFAULT_GLSL_PROFILE;

    // Search the shader source string for the first occurrence of '#version'.
    /// TODO - use <regex> from BOOST ??

    // Initially uncompiled.
    comp_status = Shader::NEVER_COMPILED;
    }

Shader::~Shader() {
    release();
    }

bool Shader::acquire(void) {
    // Assume failure.
    bool ret_val = FALSE;

    // Empty the compilation log.
    compile_log = "";

    // Get an OpenGl handle for this shader object.
    OGL_ID::ident = glCreateShader(type);
    // If that handle acquisition failed the we have no other option ...
    if(OGL_ID::ident == OGL_ID::NO_ID)  {
        ErrorHandler::record("Shader::acquire() : OpenGL handle acquisition failure !", ErrorHandler::FATAL);
        }

    // Load the source code into the shader object.
    glShaderSource(type, 1, shader_source.c_str(), NULL);

    // Compile it and return the result.
    ret_val = compile();
    if(ret_val == true ) {
        comp_status = COMPILE_SUCCEEDED;
        }
    else {
        comp_status = COMPILE_FAILED;
        // Populate the compilation log ie. retrieve compiler error output.
        // Copy to an std::string via temporary character array to avoid
        // const semantic difficulties on the std::string c_str() method.
        GLint log_len;
        glGetShaderiv(OGL_ID::ident, GL_INFO_LOG_LENGTH, &log_len);
        // Use extra character to account for null character terminator ( documentation unclear ).
        GLchar* temp_log = new GLchar[log_len+1];
        GLsizei returned_log_len;
        glGetShaderInfoLog(OGL_ID::ident, log_len+1, returned_log_len, temp_log);

        /// TODO - remove this after testing
        if(log_len != returned_log_len) {
            ErrorHandler::record("Shader::acquire() : expected and actual log lengths differ.", ErrorHandler::WARN);
            }

        // Account for null character terminator ( documentation unclear ).
        temp_log[log_len] = '\0';
        compile_log = temp_log;

        delete[] temp_log;
        }

    return ret_val;
    }

bool Shader::release(void) {
    OGL_ID::release();
    }

bool Shader::isDeleted(void) {
    // Assume not marked for deletion.
    bool ret_val = false;

    GLint d_status;
    glGetShaderiv(OGL_ID::ident, GL_DELETE_STATUS, &d_status);
    if(d_status == GL_TRUE) {
        // It is marked for deletion.
        ret_val = true;
        }

    return ret_val;
    }

bool Shader::compile(void) {
    // Assume compile failure
    bool ret_val = false;

    glCompileShader(OGL_ID::ident);

    /// TODO - remove after full testing.
    // Check here proximally pro-tem.
    ErrorHandler::check_OpenGL_Error();

    GLint c_status;
    glGetShaderiv(OGL_ID::ident, GL_COMPILE_STATUS, &c_status);
    if(c_status == GL_TRUE) {
        ret_val = true;
        }

    return ret_val;
    }

compilationState Shader::status(void) const {
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
