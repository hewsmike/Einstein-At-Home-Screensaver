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

VertexBuffer::VertexBuffer(const GLvoid* buffer_data,
                           GLsizeiptr size,
                           GLenum usage) :
                Buffer(buffer_data) {
    // Ensure strictly positive buffer size.
    if(size > 0) {
        m_size = size;
        }
    else {
        ErrorHandler::record("VertexBuffer::VertexBuffer() : Strictly positive buffer size required.",
                             ErrorHandler::FATAL);
        }

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
    }

void VertexBuffer::release_ID(GLuint* handle) const {
    glDeleteBuffers(1, handle);
    }

void VertexBuffer::loadBuffer(void) const {
    // Bind this buffer to the specified target.
    glBindBuffer(GL_ARRAY_BUFFER, this->ID());

    // Allocate space and transfer the data.
    glBufferData(GL_ARRAY_BUFFER, m_size, this->data(), m_usage);

    // Unbind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
    }

void VertexBuffer::attach(void) {
    // Attachment only occurs if all preparations are complete.
    if(m_attributes_mapped == true) {
        // Enable fetching for all supplied vertex attribute indices,
        // these corresponding to 'location' definitions within the
        // vertex shader's GLSL code.
//        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
//            attrib != m_attribute_specs.end();
//            ++attrib) {
//            glEnableVertexAttribArray(attrib->a_spec.index);
//            glVertexAttribPointer(attrib->index,
//                                  attrib->a_spec.size,
//                                  attrib->a_spec.type,
//                                  attrib->a_spec.normalised,
//                                  attrib->stride,
//                                  attrib->pointer);
//            }
        // Bind the given buffer object to pipeline state.
        glBindBuffer(GL_ARRAY_BUFFER, this->ID());
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
//        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
//            attrib != m_attribute_specs.end();
//            ++attrib) {
//            glDisableVertexAttribArray(attrib->a_spec.index);
//            }
        // Unbind the given buffer object from pipeline state.
        glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
        }
    }

void VertexBuffer::addAttributeDescription(attribute_spec specification) {
    // May only enter new attribute descriptions if mapping has
    // not yet been performed.
    if(m_attributes_mapped == false) {
        attribute_record record;
        record.a_spec = specification;

        // The length in bytes of an attribute is it's number
        // of ( identically sized ) components times the size of
        // a component.
        record.length = record.a_spec.size;
        switch (record.a_spec.type) {
            case GL_BYTE:
                record.length *= sizeof(GL_BYTE);
                break;
            case GL_UNSIGNED_BYTE:
                record.length *= sizeof(GL_UNSIGNED_BYTE);
                break;
            case GL_SHORT:
                record.length *= sizeof(GL_SHORT);
                break;
            case GL_UNSIGNED_SHORT:
                record.length *= sizeof(GL_UNSIGNED_SHORT);
                break;
            case GL_FIXED:
                record.length *= sizeof(GL_FIXED);
                break;
            case GL_FLOAT:
                record.length *= sizeof(GL_FLOAT);
                break;
            default:
                ErrorHandler::record("BufferVertexFetch::addVertexAttribute() : Bad switch case ( default )",
                                     ErrorHandler::FATAL);
                break;
            }

        // Add this attribute's length to the sum of same.
        m_attribute_length_sum += record.length;

        // These must/will be calculated once all attributes have been described.
        record.stride = 0;
        record.pointer = 0;

        // Insert into the attributes record.
        m_attribute_specs.push_back(record);
        }
    else {
        ErrorHandler::record("VertexBuffer::addAttributeDescription() : attempt to add attribute after mapping completed",
                              ErrorHandler::WARN);
        }
    }

void VertexBuffer::prepareAttributeMapping(void) {
    // Can only map attributes if it has never been done before in the lifetime
    // a BufferVertexFetch instance ie. this method is only ever performed
    // at most ONCE.
    if(m_attributes_mapped == false) {
        // Go through and examine all the given attribute specifications.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
            if(m_mix == BY_VERTEX) {
                attrib->stride = m_attribute_length_sum;
                attrib->pointer = 0;
                }
            if(m_mix == BY_ATTRIBUTE) {
                attrib->stride = 0;
                attrib->pointer = 0;
                }
            }

        // Mark attribute mapping as completed.
        m_attributes_mapped = true;
        }
    else {
        ErrorHandler::record("VertexBuffer::prepareAttributeMapping() : called more than once",
                              ErrorHandler::WARN);
        }
    }
