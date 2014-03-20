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

#include "BufferVertexFetch.h"

#include "ErrorHandler.h"

BufferVertexFetch::BufferVertexFetch(const Buffer* vertex_buffer,
                                     data_mix mix,
                                     GLuint vertex_count) :
                                        m_vert_buffer(vertex_buffer),
                                        m_mix(mix),
                                        m_vertex_count(vertex_count) {
    if(vertex_count == 0) {
        ErrorHandler::record("BufferVertexFetch::BufferVertexFetch() : vertex count is ZERO !", ErrorHandler::FATAL);
        }
    if(vertex_buffer == NULL) {
        ErrorHandler::record("BufferVertexFetch::BufferVertexFetch() : Buffer pointer is NULL !", ErrorHandler::FATAL);
        }

    // Initially the total sum of attribute lengths is zero.
    m_attribute_length_sum = 0;

    // This is only relevant for BY_ATTRIBUTE Buffer structure.
    // Initially the offset into the Buffer of the first attribute is zero.
    attribute_base_accum = 0;

    // Initially attributes are not mapped.
    m_attributes_mapped = false;
    }

BufferVertexFetch::~BufferVertexFetch() {
    }

void BufferVertexFetch::attach(void) {
    // Attachment only occurs if all preparations are complete.
    if(m_attributes_mapped == true) {
        // Enable fetching for all supplied vertex attribute indices,
        // these corresponding to 'location' definitions within the
        // vertex shader's GLSL code.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
            glEnableVertexAttribArray(attrib->a_spec.index);
            glVertexAttribPointer(attrib->a_spec.index,
                                  attrib->a_spec.size,
                                  attrib->a_spec.type,
                                  attrib->a_spec.normalised,
                                  attrib->stride,
                                  attrib->pointer);
            }
        // Bind the given buffer object to pipeline state.
        glBindBuffer(GL_ARRAY_BUFFER, m_vert_buffer->ID());
        }
    else {
        // Mapping has not yet occurred. Do it.
        prepareAttributeMapping();

        // RECURSION !! :-)
        this->attach();
        }
    }

void BufferVertexFetch::trigger(GLenum primitive, GLsizei count) {
    // Pipeline triggering only occurs if mapping was ever completed.
    if(m_attributes_mapped == true) {
        // Provokes vertex shader activity for count invocations.
        glDrawArrays(primitive, 0, count);
        }
	}

void BufferVertexFetch::detach(void) {
    // Detachment only occurs if mapping was ever completed.
    if(m_attributes_mapped == true) {
        // Disable fetching for all supplied vertex attribute indices.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
            glDisableVertexAttribArray(attrib->a_spec.index);
            }
        }
    // Unbind the given buffer object from pipeline state.
    VertexFetch::detach();
    }

void BufferVertexFetch::addAttributeDescription(attrib_spec specification) {
    // May only enter new attribute descriptions if mapping has
    // not yet been performed.
    if(m_attributes_mapped == false) {
        attribute_record record;
        record.a_spec = specification;

        // The length in bytes of an attribute is it's number
        // of ( identically sized ) components times the
        // the size of a component.
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

        if(m_mix == BY_VERTEX) {
            // Start at first vertex ie. offset into buffer of zero.
            record.pointer = 0;
            }
        if(m_mix == BY_ATTRIBUTE) {
            //
            record.pointer = attribute_base_accum;
            // With attribute mixing type of buffer structure, then
            // we offset by blocks of attribute data.
            attribute_base_accum += record.length * m_vertex_count;
            }

        // Insert into the attributes record.
        m_attribute_specs.push_back(record);
        }
    else {
        ErrorHandler::record("BufferVertexFetch::addAttributeDescription() : attempt to add attribute after mapping completed",
                              ErrorHandler::WARN);
        }
    }

void BufferVertexFetch::prepareAttributeMapping(void) {
    // Can only map attributes if it has never been done before in the lifetime
    // a BufferVertexFetch instance ie. this method is only ever performed
    // at most ONCE.
    if(m_attributes_mapped == false) {
        // Go through and examine all the given attribute specifications.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
            /// TODO - collapse these cases on the assumption of tight packing ??
            if(m_mix == BY_VERTEX) {
                // Each vertex entry's length is the total
                // of all attributes. Alternatively one could use
                // zero here as a declaration that the data is
                // tightly packed ie. OpenGL should work out the
                // stepping distance by itself.
                attrib->stride = m_attribute_length_sum;
                }
            if(m_mix == BY_ATTRIBUTE) {
                attrib->stride = 0;
                }
            }

        // Mark attribute mapping as completed.
        m_attributes_mapped = true;
        }
    else {
        ErrorHandler::record("BufferVertexFetch::prepareAttributeMapping() : called more than once",
                              ErrorHandler::WARN);
        }
    }