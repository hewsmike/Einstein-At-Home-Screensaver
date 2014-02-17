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

BufferVertexFetch::BufferVertexFetch(data_mix mix) :
                        m_mix(mix) {
    }

BufferVertexFetch::~BufferVertexFetch() {
    }

void BufferVertexFetch::attach(const Buffer& vertex_buffer,
                               GLuint vertex_count) {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->ID());
    m_vertex_count = vertex_count;
    }

void BufferVertexFetch::detach(void) {
    for(std::<attribute_record>::iterator attrib = m_attribute_specs.begin();
        attrib != m_attribute_specs.end();
        ++attrib) {
        glDisableVertexAttributeArray(attrib->first);
        }

    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
    }

void BufferVertexFetch::addVertexAttribute(GLuint shader_index,
                                           attribute_spec specification) {
    attribute_record record;
    record.a_spec = specification;
    record.stride = 0;
    record.pointer = 0;

    // Insert into the attributes record.
    m_attribute_specs[index] = record;

    // Recalculate the attribute positioning within the buffer.
    mapAttributes();
    }

void BufferVertexFetch::mapAttributes(void) {
    for(std::<attribute_record>::iterator attrib = m_attribute_specs.begin();
        attrib != m_attribute_specs.end();
        ++attrib) {

        }
    }
