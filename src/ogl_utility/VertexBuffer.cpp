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
#include <sstream>

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
    }

VertexBuffer::~VertexBuffer() {
    Buffer::release();
    }

GLuint VertexBuffer::size(void) const {
    return m_vertex_count;
    }

void VertexBuffer::acquire_ID(GLuint* handle) const {
	OGL_DEBUG(glGenBuffers(1, handle));
	}

void VertexBuffer::release_ID(GLuint* handle) const {
	OGL_DEBUG(glDeleteBuffers(1, handle));
    }

void VertexBuffer::loadBuffer(void) const {
	OGL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, this->ID()));

    // Allocate space and transfer the data.
	OGL_DEBUG(glBufferData(GL_ARRAY_BUFFER, m_size, this->data(), m_usage));

    // Unbind the buffer.
    OGL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID));

    }

void VertexBuffer::bind(void) {
    // Ensure resource acquisition first.
    this->acquire();

	// Bind the given buffer object to pipeline state.
	OGL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, this->ID()));

	// Enable fetching for all supplied vertex attribute indices,
	// these corresponding to 'location' definitions within the
	// vertex shader's GLSL code.
	for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
		attrib != m_attribute_specs.end();
		++attrib) {
		OGL_DEBUG(glEnableVertexAttribArray(attrib->a_spec.attrib_index));
		OGL_DEBUG(glVertexAttribPointer(attrib->a_spec.attrib_index,
										attrib->a_spec.multiplicity,
										attrib->a_spec.type,
										attrib->a_spec.normalised,
										attrib->stride,
										attrib->pointer));

		}
	// Unbind the given buffer object from the pipeline state.
	OGL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID));
    }

void VertexBuffer::unbind(void) {
    // Detachment only occurs if mapping was ever completed.
    if(m_attributes_mapped == true) {
        // Disable fetching for all supplied vertex attribute indices.
        for(std::vector<attribute_record>::iterator attrib = m_attribute_specs.begin();
            attrib != m_attribute_specs.end();
            ++attrib) {
        	OGL_DEBUG(glDisableVertexAttribArray(attrib->a_spec.attrib_index));
            }
        // Unbind the given buffer object from pipeline state.
        OGL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID));
        }
    }

VertexBuffer::data_mix VertexBuffer::mix(void) const {
	return m_mix;
	}

