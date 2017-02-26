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

VertexFetch::VertexFetch(){
    // Initialisations done of unused pointers.
    m_adapter = NULL;
    m_vertices = NULL;
    m_indices = NULL;

    // Initially the total sum of attribute lengths is zero.
    m_attribute_length_sum = 0;

    // This is the use case enabled for this constructor.
    m_operating_mode = BARE;
    }

VertexFetch::VertexFetch(AttributeInputAdapter* adapter, VertexBuffer* vertices) :
                         m_adapter(adapter),
                         m_vertices(vertices) {
    // Check to see that an AttributeInputAdapter was provided.
    if(m_adapter == NULL) {
        ErrorHandler::record("VertexFetch::VertexFetch() : NULL AttributeInputAdapter pointer provided !",
                             ErrorHandler::FATAL);
        }

    // Check to see that a vertex buffer was provided.
    if(m_vertices == NULL) {
        ErrorHandler::record("VertexFetch::VertexFetch() : NULL VertexBuffer pointer provided !",
                             ErrorHandler::FATAL);
        }

    // No indices here.
    m_indices = NULL;

    // Initially the total sum of attribute lengths is zero.
    m_attribute_length_sum = 0;

    // This is the use case enabled for this constructor.
    m_operating_mode = VERTICES_ONLY;
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

    // Check to see that a vertex buffer was provided.
    if(m_vertices == NULL) {
        ErrorHandler::record("VertexFetch::VertexFetch() : NULL VertexBuffer pointer provided !",
                             ErrorHandler::FATAL);
        }

    // Check to see that an index buffer was provided.
    if(m_indices == NULL) {
        ErrorHandler::record("VertexFetch::VertexFetch() : NULL IndexBuffer pointer provided !",
                             ErrorHandler::FATAL);
        }

    // Initially the total sum of attribute lengths is zero.
    m_attribute_length_sum = 0;

    // This is the use case enabled for this constructor.
    m_operating_mode = VERTICES_AND_INDICES;
    }

VertexFetch::~VertexFetch() {
    release();
    }

bool VertexFetch::acquire(void) {
    // Assume failure.
    bool ret_val = false;

    // Check and maybe acquire handle if we don't already have one.
    if(this->isAcquired() == false) {
        // Ask OpenGL for a single VAO handle.
        GLuint temp = 0;
        glGenVertexArrays(1, &temp);
        OGL_ID::set_ID(temp);

        // Failure to acquire a handle should be FATAL.
        if(this->ID() == OGL_ID::NO_ID) {
            ErrorHandler::record("VertexFetch::acquire() : failure to obtain identifier",
                                 ErrorHandler::FATAL);
            }

        // Acquire vertex resources for the non BARE cases.
        if(m_operating_mode != BARE) {
            m_vertices->acquire();
            }

        // Acquire index resources for the indexing case.
        if(m_operating_mode == VERTICES_AND_INDICES) {
            m_indices->acquire();
            }

        // Record resources as acquired.
        OGL_ID::setAcquisitionState(true);

        ret_val = true;
        }

    return ret_val;
    }

void VertexFetch::release(void) {
    // Inform OpenGL that we no longer need this specific VAO handle.
    GLuint temp = this->ID();
    glDeleteVertexArrays(1, &temp);

    // Reset our handle store to safe value.
    this->reset_ID();
    this->setAcquisitionState(false);
    }

void VertexFetch::bind(void) {
    // Only acquire if not already.
    if(this->isAcquired() == false) {
        // Ensure resource acquisition first.
        this->acquire();
        }

    // Bind this VAO.
    glBindVertexArray(this->ID());

    }

void VertexFetch::unbind(void) {
    // Unbind vertices for the non BARE cases.
    if(m_operating_mode != BARE) {
        m_vertices->unbind();
        }

    // Unbind indices for the indexing case.
    if(m_operating_mode == VERTICES_AND_INDICES) {
        m_indices->unbind();
        }

    // Unbind this VAO.
    glBindVertexArray(OGL_ID::NO_ID);
    }

bool VertexFetch::isBound(void) const {
    // Assume failure.
    bool ret_flag = false;

    // Dynamically obtain the current VAO name.
    GLint vao_name;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao_name);

    // Is it this object's ID ?
    if((vao_name == this->ID()) && (vao_name != OGL_ID::NO_ID)) {
        // Yes, so this VAO is bound.
        ret_flag = true;
        }

    return ret_flag;
    }

bool VertexFetch::configure(void) {
    // Assume success.
    bool ret_val = false;

    if(isConfigured() == false) {
        // Ensure resource acquisition first.
        this->acquire();

        // If a vertex buffers exist then we must correlate
        if(m_operating_mode != BARE) {
            //
            processAttributeDescriptions();

            //
            prepareAttributeMapping();

            // Need to bind the VAO before configuring.
            this->bind();
            // Bind the vertex array buffer - this is global state and should be unbound after configuration.
            m_vertices->bind();
            // Bind the index buffer next. This is not global state and is part of the VAO's state,
            // therefore you have to bind this while the VAO is bound.
            // You must not unbind this when the VAO is bound, as this will change the VAO's state and the index buffer
            // cannot be used for rendering at a later point. Think of this as simply 'telling' the VAO what index buffer to use.
            // m_indices->bind();

            // Enable fetching for all supplied vertex attribute indices,
            // these corresponding to 'location' definitions within the
            // vertex shader's GLSL code.
            for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
                attrib != m_attribute_specs.end();
                ++attrib) {
                glEnableVertexAttribArray(attrib->a_spec.attrib_index);
                // A vertex array buffer must be bound when you call glVertexAttribPointer
                // - it is where the actual linkage between the VAO and the currently bound vertex buffer happens.
                glVertexAttribPointer(attrib->a_spec.attrib_index,
                                      attrib->a_spec.multiplicity,
                                      attrib->a_spec.type,
                                      attrib->a_spec.normalised,
                                      attrib->stride,
                                      attrib->pointer);
            	}
        	}
        	// Unbind the vertex buffer and VAO
        	// You don't need to unbind the vertex buffer for the VAO to work but it is good practice.
        	m_vertices->unbind();
        	this->unbind();

        setConfigurationState(true);
        }

    return ret_val;
    }

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
    //this->bind();

    switch(m_operating_mode) {
        case BARE :
        case VERTICES_ONLY :
            // Either only GL_ARRAY_BUFFER target bound, or none at all.
            glDrawArrays(primitive, 0, count);
            break;
        case VERTICES_AND_INDICES :
            // Both GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER targets are bound.
            glDrawElements(primitive, count, GL_UNSIGNED_INT, 0);
            break;
        default :
            ErrorHandler::record("VertexFetch::trigger() : Bad switch case ( default ) !",
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
