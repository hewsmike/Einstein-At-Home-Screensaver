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

#include "VertexBuffer.h"

#include "ErrorHandler.h"

#include <iostream>

VertexBuffer::VertexBuffer(const GLvoid* buffer_data,
                           GLuint vertices,
                           GLenum usage,
                           data_mix mix) :
                Buffer(buffer_data),
                m_mix(mix) {
    // Ensure strictly positive vertex count.
    if(vertices > 0) {
        m_vertex_count = vertices;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Strictly positive vertex count required.",
                             ErrorHandler::FATAL);
        }

    // Initially buffer size is nil.
    m_size = 0;

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((usage == GL_STREAM_DRAW) ||
       (usage == GL_STATIC_DRAW) ||
       (usage == GL_DYNAMIC_DRAW)) {
        m_usage = usage;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Bad usage type provided.",
                             ErrorHandler::FATAL);
        }

    // Initially the total sum of attribute lengths is zero.
    m_attribute_length_sum = 0;

    // Initially attributes are not mapped.
    m_attributes_mapped = false;
    }

VertexBuffer::~VertexBuffer() {
    Buffer::release();
    }

GLuint VertexBuffer::vertexCount(void) const {
    return m_vertex_count;
    }

void VertexBuffer::acquire_ID(GLuint* handle) const {
    glGenBuffers(1, handle);
    OGL_DEBUG
    }

void VertexBuffer::release_ID(GLuint* handle) const {
    glDeleteBuffers(1, handle);
    OGL_DEBUG
    }

void VertexBuffer::loadBuffer(void) const {
#ifndef NDEBUG
    ErrorHandler::check_OpenGL_Error(__FILE__, __LINE__);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, this->ID());
    OGL_DEBUG
    ErrorHandler::check_OpenGL_Error(__FILE__, __LINE__);

    // Allocate space and transfer the data.
    std::cout << "VertexBuffer::loadBuffer() m_size = "
              << m_size
              << std::endl;
    std::cout << "VertexBuffer::loadBuffer() this->data() = "
              << this->data()
              << std::endl;
    glBufferData(GL_ARRAY_BUFFER, m_size, this->data(), m_usage);
    OGL_DEBUG

    // Unbind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
    OGL_DEBUG
    }

void VertexBuffer::attach(void) {
    // Attachment only occurs if all preparations are complete.
    if(m_attributes_mapped == true) {
        // Ensure resource acquisition first.
        this->acquire();

        // Bind the given buffer object to pipeline state.
        std::cout << "VertexBuffer::attach() binding the buffer object "
                  << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, this->ID());
        OGL_DEBUG

        // Enable fetching for all supplied vertex attribute indices,
        // these corresponding to 'location' definitions within the
        // vertex shader's GLSL code.
//        std::cout << "m_attribute_specs->size() = " << m_attribute_specs.size() << std::endl;
//        std::cout << "GL_FLOAT = " << GL_FLOAT << std::endl;
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
//            std::cout << "attrib->a_spec.attrib_index = " << attrib->a_spec.attrib_index << std::endl;
//            std::cout << "\tattrib->a_spec.multiplicity = " << attrib->a_spec.multiplicity << std::endl;
//            std::cout << "\tattrib->a_spec.type = " << attrib->a_spec.type << std::endl;
//            std::cout << "\tattrib->a_spec.normalised = " << attrib->a_spec.normalised << std::endl;
//            std::cout << "\tattrib->stride = " << attrib->stride << std::endl;
//            std::cout << "\tattrib->pointer = " << attrib->pointer << std::endl;

            glVertexAttribPointer(attrib->a_spec.attrib_index,
                                  attrib->a_spec.multiplicity,
                                  attrib->a_spec.type,
                                  attrib->a_spec.normalised,
                                  attrib->stride,
                                  attrib->pointer);
            OGL_DEBUG
            glEnableVertexAttribArray(attrib->a_spec.attrib_index);
            OGL_DEBUG
            }
        }
    else {
        // Mapping has not yet occurred. Do it.
        prepareAttributeMapping();

        // RECURSION !! :-)
        this->attach();
        }
    }

void VertexBuffer::detach(void) {
    // Detachment only occurs if mapping was ever completed.
    if(m_attributes_mapped == true) {
        // Disable fetching for all supplied vertex attribute indices.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
            glDisableVertexAttribArray(attrib->a_spec.attrib_index);
            OGL_DEBUG
            }
        // Unbind the given buffer object from pipeline state.
        glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
        OGL_DEBUG
        }
    }

void VertexBuffer::addAttributeDescription(attribute_spec specification) {
    // May only enter new attribute descriptions if mapping has
    // not yet been performed.
    if(m_attributes_mapped == false) {
        attribute_record record;
        record.a_spec = specification;
        record.a_spec.attrib_index = specification.attrib_index;

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
            }

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
    else {
        ErrorHandler::record("VertexBuffer::addAttributeDescription() : attempt to add attribute after mapping completed",
                              ErrorHandler::FATAL);
        }
    }

void VertexBuffer::prepareAttributeMapping(void) {
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
            switch(m_mix) {
                case BY_VERTEX :
                    attrib->stride = m_attribute_length_sum;
                    attrib->pointer = reinterpret_cast<GLvoid*>(interleave_progressive_offset);
                    break;
                case BY_ATTRIBUTE :
                    attrib->stride = attrib->length;
                    attrib->pointer = reinterpret_cast<GLvoid*>(non_interleave_progressive_offset);
                    break;
                default:
                    // Should never get here.
                    ErrorHandler::record("VertexBuffer::prepareAttributeMapping() : bad switch case ( default ) !",
                                  ErrorHandler::FATAL);
                }
                interleave_progressive_offset += attrib->length;

                non_interleave_progressive_offset += attrib->length * m_vertex_count;
            }

        // Mark attribute mapping as completed.
        m_attributes_mapped = true;
        }
    else {
        ErrorHandler::record("VertexBuffer::prepareAttributeMapping() : called more than once",
                              ErrorHandler::FATAL);
        }
    }
