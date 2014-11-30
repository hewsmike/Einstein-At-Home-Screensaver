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

#include "Program.h"

#include "ErrorHandler.h"

const GLint Program::GLSL_LINKAGE_FAILURE(GL_FALSE);
const GLint Program::GLSL_LINKAGE_SUCCESS(GL_TRUE);

Program::Program(VertexShader& vertex_shader,
                 Shader& fragment_shader,
                 shaderDisposition dispose) :
                    m_vertex_shader(vertex_shader),
                    m_fragment_shader(fragment_shader),
                    m_dispose(dispose) {
    // Initially unlinked.
    link_status = Program::NEVER_LINKED;
    }

Program::~Program() {
    Program::release();
    }

bool Program::acquire(void) {
    // Assume failure.
    bool ret_val = false;

    // Provided a prior link attempt has not occurred.
    if(link_status == Program::NEVER_LINKED) {
        // Clear the linker log.
        linker_log = "";

        // Only get a handle if none already.
        if(this->ID() == OGL_ID::NO_ID) {
            // Get an OpenGL handle for this program object.
            set_ID(glCreateProgram());
            // If that handle acquisition failed the we have no other option ...
            if(this->ID() == OGL_ID::NO_ID)  {
                ErrorHandler::record("Program::acquire() : OpenGL handle acquisition failure !",
                                     ErrorHandler::FATAL);
                }
            }

        // Attaching and linking shaders only occurs if both have compiled
        // successfully and neither have been marked for deletion ( which
        // includes the possibility that they may have been deleted ).
        if((m_vertex_shader.isDeleted() == false ) &&
           (m_fragment_shader.isDeleted() == false )) {
            // No deletions marked, so compile if needed.
            if(m_vertex_shader.status() == Shader::NEVER_COMPILED) {
                m_vertex_shader.acquire();
                }

            if(m_fragment_shader.status() == Shader::NEVER_COMPILED) {
                m_fragment_shader.acquire();
                }

            // Attach only if both pass muster.
            if((m_vertex_shader.status() == Shader::COMPILE_SUCCEEDED) &&
               (m_fragment_shader.status() == Shader::COMPILE_SUCCEEDED)) {
                // The shaders have compiled without error, and are
                // not marked for deletion, so attach them.
                glAttachShader(this->ID(), m_vertex_shader.ID());
                glAttachShader(this->ID(), m_fragment_shader.ID());

                // Link program and check for success.
                if(link() == true) {
                    link_status = Program::LINKAGE_SUCCEEDED;
                    // If this behaviour previously selected, then release the shaders.
                    /// TODO - enable this after other testing.
//                    if(m_dispose == Program::RELEASE_ON_GOOD_LINK) {
//                        m_vertex_shader.release();
//                        m_fragment_shader.release();
//                        }
                    ErrorHandler::record("Program::acquire() : success of GLSL link !",
                                         ErrorHandler::WARN);
                    ret_val = true;
                    }
                else {
                    link_status = Program::LINKAGE_FAILED;
                    ErrorHandler::record("Program::acquire() : failure to GLSL link !",
                                         ErrorHandler::WARN);
                    }

                // Populate the linkage log ie. retrieve linker error output.
                // Copy to an std::string via temporary character array to avoid
                // const semantic difficulties on the std::string c_str() method.
                GLint log_len;
                glGetProgramiv(this->ID(), GL_INFO_LOG_LENGTH, &log_len);
                // Use extra character to account for null character terminator ( documentation unclear ).
                GLchar* temp_log = new GLchar[log_len+1];
                GLsizei returned_log_len;
                glGetProgramInfoLog(this->ID(), log_len+1, &returned_log_len, temp_log);

                // Account for null character terminator ( documentation unclear ).
                temp_log[log_len] = '\0';
                linker_log = temp_log;

                // Dispose of the temporary character array.
                delete[] temp_log;
                }
            }
        }

    return ret_val;
    }

void Program::release(void) {
    // Inform OpenGL that we no longer need this specific program handle.
    glDeleteProgram(this->ID());
    // Reset linkage status.
    link_status = Program::NEVER_LINKED;
    // Set our handle store to safe value.
    set_ID(OGL_ID::NO_ID);
    }

bool Program::isDeleted(void) const {
    // Assume not marked for deletion.
    bool ret_val = false;

    // Lazy evaluation through inquiry to OpenGL context.
    GLint d_status;
    glGetProgramiv(this->ID(), GL_DELETE_STATUS, &d_status);
    if(d_status == GL_TRUE) {
        // It is marked for deletion.
        ret_val = true;
        }

    return ret_val;
    }

Program::linkageState Program::status(void) const {
    return link_status;
    }

bool Program::link(void) {
    // Assume linkage failure
    bool ret_val = false;

    // Before link need to access VertexShader attribute indices and variable names
    // in order to bind the attributes to locations.
    for(GLuint index = 0; index < m_vertex_shader.attribCount(); ++index) {
        std::pair<GLuint, const std::string> temp;
        glBindAttribLocation(this->ID(), temp.first, temp.second.c_str());
        }

    // Attempt to link the program.
    glLinkProgram(this->ID());

    // Check for linkage success.
    GLint link_success;
    glGetProgramiv(this->ID(), GL_LINK_STATUS, &link_success);
    if(link_success == GLSL_LINKAGE_SUCCESS) {
        ret_val = true;
        }

    return ret_val;
    }

const std::string& Program::linkageLog(void) const {
    return linker_log;
    }
