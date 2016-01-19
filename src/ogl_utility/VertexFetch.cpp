/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#include "VertexFetch.h"

#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

VertexFetch::VertexFetch(void) {
	m_adapter = NULL;
	m_vertices = NULL;
	m_indices = NULL;
	m_operating_mode = BARE;
	m_configure_flag = false;
	m_attribute_length_sum = 0;
}

VertexFetch::VertexFetch(AttributeInputAdapter* adapter, VertexBuffer* vertices, IndexBuffer* indices) :
						 m_adapter(adapter),
                         m_vertices(vertices),
                         m_indices(indices) {
	// Check to see that an AttributeInputAdapter was provided.
	if(m_adapter == NULL) {
		ErrorHandler::record("VertexFetch::VertexFetch() : NULL AttributeInputAdapter pointer provided !",
	 						 ErrorHandler::FATAL);
		}

	// Check to see that at least a vertex buffer was provided.
	if(m_vertices == NULL) {
		ErrorHandler::record("VertexFetch::VertexFetch() : NULL VertexBuffer pointer provided !",
		                     ErrorHandler::FATAL);
		}

	m_operating_mode = VERTICES_ONLY;

	if(m_indices != NULL) {
		m_operating_mode = VERTICES_AND_INDICES;
		}

	// Initially the total sum of attribute lengths is zero.
	m_attribute_length_sum = 0;

	m_configure_flag = false;
    }

VertexFetch::~VertexFetch() {
	}

bool VertexFetch::acquire(void) {
	// Assume failure.
	bool ret_val = false;

	// Check and maybe acquire handle if we don't already have one.
	if(this->ID() == OGL_ID::NO_ID) {
		// Ask OpenGL for a single VAO handle.
		GLuint temp = 0;
		glGenVertexArrays(1, &temp);
		set_ID(temp);

		// Failure to acquire a handle should be FATAL.
		if(this->ID() == OGL_ID::NO_ID) {
		ErrorHandler::record("VertexFetch::acquire() : failure to obtain identifier",
							 ErrorHandler::FATAL);
			}
		ret_val = true;
		}
	return ret_val;
	}

void VertexFetch::release(void) {
	// Inform OpenGL that we no longer need this specific VAO handle.
	GLuint temp = this->ID();
	glDeleteVertexArrays(1, &temp);

	// Reset our handle store to safe value.
	set_ID(OGL_ID::NO_ID);
	}

void VertexFetch::bind(void) {
    // Ensure configuration has occurred.
	if(m_configure_flag == false) {
		/// TODO - failure mode path for configure fail.
		configure();
	    }

	// Bind any existing index buffer, if not already.
	if(m_operating_mode == VERTICES_AND_INDICES) {
	    if(m_indices.isBound() == false) {
            m_indices->bind();
            }
		}

	glBindVertexArray(this->ID());
	}

void VertexFetch::unbind(void) {
	glBindVertexArray(OGL_ID::NO_ID);
    }

bool VertexFetch::isBound(void) const {
    // Assume failure.
    bool ret_flag = false;

    // Dynamically obtain the current VAO name.
    GLint vao_name;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao_name);

    // Is it this object's ID ?
    if(vao_name == GLint(this->ID())) {
        // Yes, so this VAO is bound.
        ret_flag = true;
        }

	return ret_flag;
	}

bool VertexFetch::configure(void) {
	// Assume failure.
	bool ret_val = false;

	// Ensure resource acquisition first.
	this->acquire();

	//
	if(m_operating_mode != BARE) {
		//
		processAttributeDescriptions();

		//
		prepareAttributeMapping();

		if(m_operating_mode != BARE) {
            m_vertices->bind();
            }
        if(m_operating_mode == VERTICES_AND_INDICES) {
            m_indices->bind();
            }

		// Enable fetching for all supplied vertex attribute indices,
    	// these corresponding to 'location' definitions within the
    	// vertex shader's GLSL code.
    	for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
    		attrib != m_attribute_specs.end();
    		++attrib) {
    		glEnableVertexAttribArray(attrib->a_spec.attrib_index);
    		glVertexAttribPointer(attrib->a_spec.attrib_index,
    							  attrib->a_spec.multiplicity,
    							  attrib->a_spec.type,
    							  attrib->a_spec.normalised,
    							  attrib->stride,
    							  attrib->pointer);
    		}
		}
	// Ensure that the pipeline vertex fetch
	// stage is not bound to any buffers at all.
	m_vertices->unbind();

	// NB the index array - if any - is not unbound as that requires persistence.

	this->unbind();

	m_configure_flag = true;

    return ret_val;
	}

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
    // If VAO is not bound then do so.
    if(this->isBound() == false) {
        ErrorHandler::record("VertexFetch::trigger() : VAO not attached !",
                              ErrorHandler::WARN);
        this->bind();
        }

    // Provokes vertex shader activity for count invocations,
	// buffer use depending upon mode of operation.
	switch(m_operating_mode) {
	    case BARE :
	    	ErrorHandler::record("VertexFetch::trigger() : operating mode = BARE",
	    	                     ErrorHandler::INFORM);
            break;
        case VERTICES_ONLY :
        	ErrorHandler::record("VertexFetch::trigger() : operating mode = VERTICES_ONLY",
        		    	         ErrorHandler::INFORM);
            glDrawArrays(primitive, 0, count);
            break;
        case VERTICES_AND_INDICES :
        	ErrorHandler::record("VertexFetch::trigger() : operating mode = VERTICES_AND_INDICES",
        	        		     ErrorHandler::INFORM);
            glDrawElements(primitive, count, GL_UNSIGNED_INT, 0);
            break;
        default :
            ErrorHandler::record("VertexFetch::trigger() : Bad switch case ( default )",
                                 ErrorHandler::FATAL);
            break;
        }
	}

void VertexFetch::processAttributeDescriptions(void) {
	// Access the AttributeInputAdapter and look at each attribute specification.
	for(GLuint index = 0; index < m_adapter->size(); ++index) {
		// Get an attribute specification.
		AttributeInputAdapter::attribute_spec specification;
		m_adapter->getAttributeSpecAt(index, specification);

		// Populate an attribute record.
        attribute_record record;
        record.a_spec = specification;

        // The length in bytes of an attribute is it's number
        // of ( identically sized ) components times the size of
        // a component.
        record.length = record.a_spec.multiplicity;
        switch (record.a_spec.type) {
            case GL_BYTE:
                record.length *= sizeof(GLbyte);
                break;
            case GL_UNSIGNED_BYTE:
                record.length *= sizeof(GLubyte);
                break;
            case GL_SHORT:
                record.length *= sizeof(GLshort);
                break;
            case GL_UNSIGNED_SHORT:
                record.length *= sizeof(GLushort);
                break;
            case GL_FIXED:
                record.length *= sizeof(GLfixed);
                break;
            case GL_FLOAT:
                record.length *= sizeof(GLfloat);
                break;
            default:
                ErrorHandler::record("VertexFetch::processAttributeDescriptions() : Bad switch case ( default )",
                                     ErrorHandler::FATAL);
                break;
        	}

        // Add this attribute's length to the sum of same.
        m_attribute_length_sum += record.length;

        // These must be calculated once all attributes have been described.
        // Initialise now, see prepareAttributeMapping() function.
        record.stride = 0;
        record.pointer = 0;

        // Insert into the attributes record.
        m_attribute_specs.push_back(record);
        }
    }

void VertexFetch::prepareAttributeMapping(void) {
	GLuint interleave_progressive_offset = 0;
	GLuint non_interleave_progressive_offset = 0;
	// Go through and examine all the given attribute specifications.
	for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
		attrib != m_attribute_specs.end();
		++attrib) {
		std::stringstream map_msg;
		switch(m_vertices->mix()) {
			case VertexBuffer::BY_VERTEX :
				attrib->stride = m_attribute_length_sum;
				attrib->pointer = reinterpret_cast<GLvoid*>(interleave_progressive_offset);
				break;
			case VertexBuffer::BY_ATTRIBUTE :
				attrib->stride = attrib->length;
				attrib->pointer = reinterpret_cast<GLvoid*>(non_interleave_progressive_offset);
				break;
			default:
				// Should never get here.
				ErrorHandler::record("VertexBuffer::prepareAttributeMapping() : bad switch case ( default ) !",
							  ErrorHandler::FATAL);
			}
		map_msg << "VertexFetch::prepareAttributeMapping() : For attribute with index "
				<< (attrib->a_spec).attrib_index
				<< ", the pointer = "
				<< attrib->pointer
				<< " and the stride = "
				<< attrib->stride;
		ErrorHandler::record(map_msg.str(), ErrorHandler::INFORM);

		interleave_progressive_offset += attrib->length;

		non_interleave_progressive_offset += attrib->length * m_vertices->size();
		}
    }
