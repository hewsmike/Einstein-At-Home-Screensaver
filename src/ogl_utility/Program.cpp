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

#include <iostream>
#include <sstream>

const int Program::UNIFORM_NAME_BUFFER_SIZE(50);

Program::Program(VertexShader* vertex_shader,
                 FragmentShader* fragment_shader,
				 AttributeInputAdapter* adapter,
				 shaderDisposition dispose) :
                    m_vertex_shader(vertex_shader),
                    m_fragment_shader(fragment_shader),
					m_adapter(adapter),
					m_dispose(dispose){
    // Initially unlinked.
    link_status = Program::NEVER_LINKED;
    }

Program::~Program() {
    Program::release();
    }

void Program::use(void) const {
	glUseProgram(this->ID());
	}

void Program::stopUse(void)const {
	glUseProgram(OGL_ID::NO_ID);
	}

bool Program::acquire(void) {
    // Assume failure.
    bool ret_val = false;

    // Provided a prior link attempt has not occurred.
    if(link_status == Program::NEVER_LINKED) {
        // Clear the linker log.
        linker_log = "";

        // Attaching and linking shaders only occurs if both have compiled
        // successfully and neither have been marked for deletion ( which
        // includes the possibility that they may have been deleted ).
        if((m_vertex_shader->isDeleted() == false ) &&
           (m_fragment_shader->isDeleted() == false )) {
            // No deletions marked, so compile if needed.
            if(m_vertex_shader->status() == Shader::NEVER_COMPILED) {
                m_vertex_shader->acquire();
                }

            if(m_fragment_shader->status() == Shader::NEVER_COMPILED) {
                m_fragment_shader->acquire();
                }

            // Attach only if both pass muster.
            if((m_vertex_shader->status() == Shader::COMPILE_SUCCEEDED) &&
               (m_fragment_shader->status() == Shader::COMPILE_SUCCEEDED)) {

            	// Only get a handle if none already.
				if(this->ID() == OGL_ID::NO_ID) {
					// Get an OpenGL handle for this program object.
					GLuint temp = glCreateProgram();
					set_ID(temp);
					// If that handle acquisition failed the we have no other option ...
					if(this->ID() == OGL_ID::NO_ID)  {
						ErrorHandler::record("Program::acquire() : OpenGL handle acquisition failure !",
											 ErrorHandler::FATAL);
						}
					}

                // The shaders have compiled without error, and are
                // not marked for deletion, so attach them.
            	std::stringstream vshader_msg;
            	vshader_msg << "Program::acquire() : attaching vertex shader with ID = "
            				<< m_vertex_shader->ID()
							<< " to program with ID = "
							<< this->ID();
            	ErrorHandler::record(vshader_msg.str(), ErrorHandler::INFORM);
            	glAttachShader(this->ID(), m_vertex_shader->ID());

            	std::stringstream fshader_msg;
            	fshader_msg << "Program::acquire() : attaching fragment shader with ID = "
            	            << m_fragment_shader->ID()
							<< " to program with ID = "
							<< this->ID();
            	ErrorHandler::record(fshader_msg.str(), ErrorHandler::INFORM);
            	glAttachShader(this->ID(), m_fragment_shader->ID());

                // Link program and check for success.
                if(link() == true) {
                    // If this behaviour previously selected, then release the shaders.
                    /// TODO - enable this after other testing.
                    if(m_dispose == Program::DELETE_ON_GOOD_LINK) {
                        m_vertex_shader->release();
                        m_fragment_shader->release();
                        }
                    ret_val = true;
                    }
                else {
                    link_status = Program::LINKAGE_FAILED;
                    ErrorHandler::record("Program::acquire() : failure to GLSL link !",
                                         ErrorHandler::WARN);
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
    for(GLuint index = 0; index < m_adapter->size(); ++index) {
    	AttributeInputAdapter::attribute_spec temp_spec;
        if(m_adapter->getAttributeSpecAt(index, temp_spec)) {
			std::stringstream attrib_msg("");
			attrib_msg << "Program::link() : For program with ID = "
					   << this->ID()
					   << ", binding an attribute at index "
					   << temp_spec.attrib_index
					   << " with the name '"
					   << temp_spec.name
					   << "'";
			ErrorHandler::record(attrib_msg.str(), ErrorHandler::INFORM);
			glBindAttribLocation(this->ID(), temp_spec.attrib_index, temp_spec.name.c_str());
        	}
        else {
        	ErrorHandler::record("Program::link() : attempt made to access out of range attribute index !",
        	                     ErrorHandler::WARN);
        	}
        }

    // Attempt to link the program.
    glLinkProgram(this->ID());

    // Check for linkage success.
    GLint link_state = GL_FALSE;
    glGetProgramiv(this->ID(), GL_LINK_STATUS, &link_state);
    if(link_state == GL_TRUE) {
    	link_status = Program::LINKAGE_SUCCEEDED;
    	mapUniforms();
        ret_val = true;
        }

    return ret_val;
    }

const std::string& Program::linkageLog(void) const {
    return linker_log;
    }

bool Program::setUniformLoadPoint(std::string u_name, GLvoid* source) {
	uniform_data current;
	current.m_load_point = source;

	std::map<std::string, Program::uniform_data>::iterator pos = uniforms.find(u_name);
	if(pos == uniforms.end()) {
		uniforms.insert(std::make_pair(std::string(u_name), current));
		}
	else {
		pos->second = current;
		}

    return true;
	}

bool Program::mapUniforms(void) {
    // Assume failure.
    bool ret_val = false;

    // Only proceed if linkage was attempted and succeeded.
    if(this->status() == LINKAGE_SUCCEEDED) {
        // How many uniforms does this program entity have ?
        GLuint temp;
        glGetProgramiv(this->ID(), GL_ACTIVE_UNIFORMS, (GLint*)(&temp));

        // Retrieve their names and locations.
        for(GLuint index = 0; index < temp; ++index) {
            GLchar u_name[UNIFORM_NAME_BUFFER_SIZE];
            // Paranoia.
            u_name[UNIFORM_NAME_BUFFER_SIZE - 1] = '\0';
            GLuint written_length;
            GLuint uniform_size;
            GLenum uniform_type;

            uniform_data current;

            glGetActiveUniform(this->ID(),
                               index,
                               UNIFORM_NAME_BUFFER_SIZE - 1,
                               (GLint*)(&written_length),
                               (GLint*)&uniform_size,
                               &uniform_type,
                               u_name);

            current.m_location = (GLint)(index);
            current.m_type = uniform_type;

            std::map<std::string, Program::uniform_data>::iterator pos = uniforms.find(u_name);
			if(pos != uniforms.end()) {
				pos->second.m_type = uniform_type;
				pos->second.m_location = index;
				}
			else {
				uniforms.insert(std::make_pair(std::string(u_name), current));
				}
            }

        ret_val = true;
        }

    return ret_val;
    }

Program::uniform_data Program::getUniform(std::string u_name) {
	std::map<std::string, Program::uniform_data>::iterator pos = uniforms.find(u_name);
	return pos->second;
	}

bool Program::loadUniform(Program::uniform_data current) {
	// Assume success.
	bool ret_val = true;

	switch(current.m_type) {
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(current.m_location, 1, false, static_cast<const GLfloat*>(current.m_load_point));
			break;
		default:
			ErrorHandler::record("Program::loadUniform() : bad switch case ( default ).", ErrorHandler::WARN);
			ret_val = false;
			break;
		}

	return ret_val;
	}

std::string Program::checkUniform(GLenum type) {
	std::string ret_val("");
	switch(type) {
		case GL_FLOAT:
			ret_val = "GL_FLOAT";
			break;
		case GL_FLOAT_VEC2:
			ret_val = "GL_FLOAT_VEC2";
			break;
		case GL_FLOAT_VEC3:
			ret_val = "GL_FLOAT_VEC3";
			break;
		case GL_FLOAT_VEC4:
			ret_val = "GL_FLOAT_VEC4";
			break;
		case GL_DOUBLE:
			ret_val = "GL_DOUBLE";
			break;
		case GL_DOUBLE_VEC2:
			ret_val = "GL_DOUBLE_VEC2";
			break;
		case GL_DOUBLE_VEC3:
			ret_val = "GL_DOUBLE_VEC3";
			break;
		case GL_DOUBLE_VEC4:
			ret_val = "GL_DOUBLE_VEC24";
			break;
		case GL_INT:
			ret_val = "GL_INT";
			break;
		case GL_INT_VEC2:
			ret_val = "GL_INT_VEC2";
			break;
		case GL_INT_VEC3:
			ret_val = "GL_INT_VEC3";
			break;
		case GL_INT_VEC4:
			ret_val = "GL_INT_VEC4";
			break;
		case GL_UNSIGNED_INT:
			ret_val = "GL_UNSIGNED_INT";
			break;
		case GL_UNSIGNED_INT_VEC2:
			ret_val = "UNSIGNED_INT_VEC2";
			break;
		case GL_UNSIGNED_INT_VEC3:
			ret_val = "UNSIGNED_INT_VEC3";
			break;
		case GL_UNSIGNED_INT_VEC4:
			ret_val = "UNSIGNED_INT_VEC4";
			break;
		case GL_BOOL:
			ret_val = "GL_BOOL";
			break;
		case GL_BOOL_VEC2:
			ret_val = "GL_BOOL_VEC2";
			break;
		case GL_BOOL_VEC3:
			ret_val = "GL_BOOL_VEC3";
			break;
		case GL_BOOL_VEC4:
			ret_val = "GL_BOOL_VEC4";
			break;
		case GL_FLOAT_MAT2:
			ret_val = "GL_FLOAT_MAT2";
			break;
		case GL_FLOAT_MAT3:
			ret_val = "GL_FLOAT_MAT3";
			break;
		case GL_FLOAT_MAT4:
			ret_val = "GL_FLOAT_MAT4";
			break;
		case GL_FLOAT_MAT2x3:
			ret_val = "GL_FLOAT_MAT2x3";
			break;
		case GL_FLOAT_MAT2x4:
			ret_val = "GL_FLOAT_MAT2x4";
			break;
		case GL_FLOAT_MAT3x2:
			ret_val = "GL_FLOAT_MAT3x2";
			break;
		case GL_FLOAT_MAT3x4:
			ret_val = "GL_FLOAT_MAT3x4";
			break;
		case GL_FLOAT_MAT4x2:
			ret_val = "GL_FLOAT_MAT4x2";
			break;
		case GL_FLOAT_MAT4x3:
			ret_val = "GL_FLOAT_MAT4x3";
			break;
		case GL_DOUBLE_MAT2:
			ret_val = "GL_DOUBLE_MAT2";
			break;
		case GL_DOUBLE_MAT3:
			ret_val = "GL_DOUBLE_MAT3";
			break;
		case GL_DOUBLE_MAT4:
			ret_val = "GL_DOUBLE_MAT4";
			break;
		case GL_DOUBLE_MAT2x3:
			ret_val = "GL_DOUBLE_MAT2x3";
			break;
		case GL_DOUBLE_MAT2x4:
			ret_val = "GL_DOUBLE_MAT2x4";
			break;
		case GL_DOUBLE_MAT3x2:
			ret_val = "GL_DOUBLE_MAT3x2";
			break;
		case GL_DOUBLE_MAT3x4:
			ret_val = "GL_DOUBLE_MAT3x4";
			break;
		case GL_DOUBLE_MAT4x2:
			ret_val = "GL_DOUBLE_MAT4x2";
			break;
		case GL_DOUBLE_MAT4x3:
			ret_val = "GL_DOUBLE_MAT4x3";
			break;
		case GL_SAMPLER_1D:
			ret_val = "GL_SAMPLER_1D";
			break;
		case GL_SAMPLER_2D:
			ret_val = "GL_SAMPLER_2D";
			break;
		case GL_SAMPLER_3D:
			ret_val = "SAMPLER_3D";
			break;
		case GL_SAMPLER_CUBE:
			ret_val = "GL_SAMPLER_CUBE";
			break;
		case GL_SAMPLER_1D_SHADOW:
			ret_val = "GL_SAMPLER_1D_SHADOW";
			break;
		case GL_SAMPLER_2D_SHADOW:
			ret_val = "GL_SAMPLER_2D_SHADOW";
			break;
		case GL_SAMPLER_1D_ARRAY:
			ret_val = "GL_SAMPLER_1D_ARRAY";
			break;
		case GL_SAMPLER_2D_ARRAY:
			ret_val = "GL_SAMPLER_2D_ARRAY";
			break;
		case GL_SAMPLER_1D_ARRAY_SHADOW:
			ret_val = "GL_SAMPLER_1D_ARRAY_SHADOW";
			break;
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			ret_val = "GL_SAMPLER_2D_ARRAY_SHADOW";
			break;
		case GL_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_SAMPLER_CUBE_SHADOW:
			ret_val = "GL_SAMPLER_CUBE_SHADOW";
			break;
		case GL_SAMPLER_BUFFER:
			ret_val = "GL_SAMPLER_BUFFER";
			break;
		case GL_SAMPLER_2D_RECT:
			ret_val = "GL_SAMPLER_2D_RECT";
			break;
		case GL_SAMPLER_2D_RECT_SHADOW:
			ret_val = "GL_SAMPLER_2D_RECT_SHADOW";
			break;
		case GL_INT_SAMPLER_1D:
			ret_val = "GL_INT_SAMPLER_1D";
			break;
		case GL_INT_SAMPLER_2D:
			ret_val = "GL_INT_SAMPLER_2D";
			break;
		case GL_INT_SAMPLER_3D:
			ret_val = "GL_INT_SAMPLER_3D";
			break;
		case GL_INT_SAMPLER_CUBE:
			ret_val = "GL_INT_SAMPLER_CUBE";
			break;
		case GL_INT_SAMPLER_1D_ARRAY:
			ret_val = "GL_INT_SAMPLER_1D_ARRAY";
			break;
		case GL_INT_SAMPLER_2D_ARRAY:
			ret_val = "GL_INT_SAMPLER_2D_ARRAY";
			break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_INT_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_INT_SAMPLER_2D_MULTISMAPLE_ARRAY";
			break;
		case GL_INT_SAMPLER_BUFFER:
			ret_val = "GL_INT_SAMPLER_BUFFER";
			break;
		case GL_INT_SAMPLER_2D_RECT:
			ret_val = "GL_INT_SAMPLER_2D_RECT";
			break;
		case GL_UNSIGNED_INT_SAMPLER_1D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_1D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_3D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_3D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_CUBE";
			break;
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
			ret_val = "GL_DOUBLE_UNSIGNED_INT_SAMPLER_2D_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_BUFFER";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
			break;
		case GL_IMAGE_1D:
			ret_val = "GL_IMAGE_1D";
			break;
		case GL_IMAGE_2D:
			ret_val = "GL_IMAGE_2D";
			break;
		case GL_IMAGE_3D:
			ret_val = "GL_IMAGE_3D";
			break;
		case GL_IMAGE_2D_RECT:
			ret_val = "GL_IMAGE_2D_RECT";
			break;
		case GL_IMAGE_CUBE:
			ret_val = "GL_IMAGE_CUBE";
			break;
		case GL_IMAGE_BUFFER:
			ret_val = "GL_IMAGE_BUFFER";
			break;
		case GL_IMAGE_1D_ARRAY:
			ret_val = "GL_IMAGE_1D_ARRAY";
			break;
		case GL_IMAGE_2D_ARRAY:
			ret_val = "GL_IMAGE_2D_ARRAY";
			break;
		case GL_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_INT_IMAGE_1D:
			ret_val = "GL_INT_IMAGE_1D";
			break;
		case GL_INT_IMAGE_2D:
			ret_val = "GL_INT_IMAGE_2D";
			break;
		case GL_INT_IMAGE_3D:
			ret_val = "GL_INT_IMAGE_3D";
			break;
		case GL_INT_IMAGE_2D_RECT:
			ret_val = "GL_INT_IMAGE_2D_RECT";
			break;
		case GL_INT_IMAGE_CUBE:
			ret_val = "GL_INT_IMAGE_CUBE";
			break;
		case GL_INT_IMAGE_BUFFER:
			ret_val = "GL_INT_IMAGE_BUFFER";
			break;
		case GL_INT_IMAGE_1D_ARRAY:
			ret_val = "GL_INT_IMAGE_1D_ARRAY";
			break;
		case GL_INT_IMAGE_2D_ARRAY:
			ret_val = "GL_INT_IMAGE_2D_ARRAY";
			break;
		case GL_INT_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_INT_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_1D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_1D";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D";
			break;
		case GL_UNSIGNED_INT_IMAGE_3D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_3D";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_RECT:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_RECT";
			break;
		case GL_UNSIGNED_INT_IMAGE_CUBE:
			ret_val = "GL_UNSIGNED_INT_IMAGE_CUBE";
			break;
		case GL_UNSIGNED_INT_IMAGE_BUFFER:
			ret_val = "GL_UNSIGNED_INT_IMAGE_BUFFER";
			break;
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_1D_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_ATOMIC_COUNTER:
			ret_val = "UNSIGNED_INT_ATOMIC_COUNTER";
			break;
		default:
			std::stringstream chk_uni_msg;
			chk_uni_msg << "Program::checkUniform() : unknown type ( default case ) = "
						<< type;
			ErrorHandler::record(chk_uni_msg.str(), ErrorHandler::WARN);
			ret_val = "UNKNOWN";
			break;
		}

	return ret_val;
	}

void Program::frameCallback(void) {
	// this->loadUniform("RotationMatrix");
	for(std::map<std::string, uniform_data>::const_iterator uniform = uniforms.begin();
        uniform != uniforms.end();
        ++uniform) {
        this->loadUniform(uniform->second);
        }
	}
