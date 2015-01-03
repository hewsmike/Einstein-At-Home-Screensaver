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

#include "VertexFetch.h"

#include "ErrorHandler.h"
#include "OGL_ID.h"

#include <iostream>

VertexFetch::VertexFetch(VertexBuffer* vertices, IndexBuffer* indices, AttributeInputAdapter* adapter) :
                          m_vertices(vertices),
                          m_indices(indices),
						  m_adapter(adapter){
	// Check to see that at least a vertex buffer was provided.
	if(m_vertices == NULL) {
		ErrorHandler::record("VertexFetch::VertexFetch() : NULL VertexBuffer pointer provided !",
		                     ErrorHandler::FATAL);
		}

	// Check to see that an AttributeInputAdapter was provided.
	if(m_adapter == NULL) {
		ErrorHandler::record("VertexFetch::VertexFetch() : NULL AttributeInputAdapter pointer provided !",
							 ErrorHandler::FATAL);
		}

	processAttributeDescriptions();

    prepareAttributeMapping();

	m_bound_flag = false;
    }

VertexFetch::~VertexFetch() {
	release();
	}

bool VertexFetch::acquire(void) {
	// Assume failure.
	bool ret_val = false;

	// Check and maybe acquire handle if we don't already have one.
	if(this->ID() == OGL_ID::NO_ID) {
		// Ask OpenGL for a single VAO handle.
		GLuint temp = 0;
		acquire_ID(&temp);
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
	// Inform OpenGL that we no longer need this specific buffer handle.
	GLuint temp = this->ID();
	release_ID(&temp);

	// Reset our handle store to safe value.
	set_ID(OGL_ID::NO_ID);
    }

void VertexFetch::bind(void) {
	// Ensure resource acquisition first.
	this->acquire();

	OGL_DEBUG(glBindVertexArray(this->ID()));

	// Bind only existing buffers.
	if(m_vertices != NULL) {
		m_vertices->bind();
		}
	if(m_indices != NULL) {
		m_indices->bind();
		}

	OGL_DEBUG(glBindVertexArray(OGL_ID::NO_ID));

	// Indicate that Buffer attachment to targets has been addressed.
	m_bound_flag = true;
	}

void VertexFetch::unbind(void) {
    // Ensure that the pipeline vertex fetch
    // stage is not bound to any buffers at all.
    m_vertices->unbind();
    m_indices->unbind();

    // Reset attachment state.
    m_bound_flag = false;
    }

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
    // If buffers are not attached to targets then do so.
    if(m_bound_flag == false) {
        ErrorHandler::record("VertexFetch::trigger() : buffers, if any, not attached, do so !",
                              ErrorHandler::FATAL);
        this->bind();
        }

    OGL_DEBUG(glBindVertexArray(this->ID()));

    // Provokes vertex shader activity for count invocations,
	// buffer use depending upon that which is bound.
	if((m_vertices != NULL) && (m_indices != NULL)) {
        // Both GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER targets are bound.
		OGL_DEBUG(glDrawElements(primitive, count, GL_UNSIGNED_INT, 0));
	    }
    else {
        // Either only GL_ARRAY_BUFFER target bound, or none at all.
    	OGL_DEBUG(glDrawArrays(primitive, 0, count));
        }

	OGL_DEBUG(glBindVertexArray(OGL_ID::NO_ID));
	}

void VertexFetch::acquire_ID(GLuint* handle) const {
	OGL_DEBUG(glGenVertexArrays(1, handle));
	}

void VertexFetch::release_ID(GLuint* handle) const {
	OGL_DEBUG(glDeleteVertexArrays(1, handle));
    }

bool VertexFetch::isBound(void) const {
	return m_bound_flag;
	}

void VertexFetch::processAttributeDescriptions(void) {

    	// Initially the total sum of attribute lengths is zero.
    	m_attribute_length_sum = 0;

        attribute_record record;
        record.a_spec = specification;
        record.a_spec.attrib_index = specification->;
        record.a_spec.normalised = specification.normalised;

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
                ErrorHandler::record("BufferVertexFetch::addVertexAttribute() : Bad switch case ( default )",
                                     ErrorHandler::FATAL);
                break;


        // Add this attribute's length to the sum of same.
        m_attribute_length_sum += record.length;

        // These must be calculated once all attributes have been described.
        record.stride = 0;
        record.pointer = 0;

        // Insert into the attributes record.
        m_attribute_specs.push_back(record);

        // Recalculate total buffer length in bytes.
        m_size = m_vertex_count * m_attribute_length_sum;


    }

void VertexFetch::prepareAttributeMapping(void) {
    // Can only map attributes if it has never been done before in the lifetime
    // a BufferVertexFetch instance ie. this method is only ever performed
    // at most ONCE.
    if(m_attributes_mapped == false) {
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
            map_msg << "VertexBuffer::prepareAttributeMapping() : For attribute with index "
               		<< (attrib->a_spec).attrib_index
               		<< ", the pointer = "
               		<< attrib->pointer
            		<< " and the stride = "
            		<< attrib->stride;
            ErrorHandler::record(map_msg.str(), ErrorHandler::INFORM);

            interleave_progressive_offset += attrib->length;

            non_interleave_progressive_offset += attrib->length * m_vertices->size();
            }

        // Mark attribute mapping as completed.
        m_attributes_mapped = true;
        }
    else {
        ErrorHandler::record("VertexBuffer::prepareAttributeMapping() : called more than once",
                              ErrorHandler::FATAL);
        }
    }

