/***************************************************************************
 *   Copyright (C) 2015 by Mike Hewson                                     *
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

#include "RenderTask.h"

#include <sstream>

std::string RenderTask::m_transform_name("");
glm::mat4* RenderTask::m_transform(NULL);

RenderTask::RenderTask(RenderTask::shader_group s_group,
        			   RenderTask::index_buffer_group i_group,
		               RenderTask::vertex_buffer_group v_group) {
    //
    m_vertex_shader = new VertexShader(s_group.vert_shader_source);
    m_frag_shader = new FragmentShader(s_group.frag_shader_source);
    m_attrib_adapt = new AttributeInputAdapter();
    m_program = new Program(m_vertex_shader, m_frag_shader, m_attrib_adapt);

    // Is a vertex buffer being used ?
    m_vertex_buffer = NULL;
    if(v_group.buffer_data != NULL) {
    	m_vertex_buffer = new VertexBuffer(v_group.buffer_data, v_group.bytes, v_group.vertices, v_group.usage, v_group.mix);
    	m_vertex_buffer->acquire();
    	}

    // Is an index buffer being used ?
    m_index_buffer = NULL;
    if(i_group.buffer_data != NULL) {
        m_index_buffer = new IndexBuffer(i_group.buffer_data, i_group.bytes, i_group.indices, i_group.usage, i_group.index_type);
        m_index_buffer->acquire();
        }

    if((m_vertex_buffer == NULL)) {
    	m_vertex_fetch = new VertexFetch();
    	}
    else {
    	if(m_index_buffer == NULL) {
    		m_vertex_fetch = new VertexFetch(m_attrib_adapt, m_vertex_buffer);
    		}
    	else {
    		m_vertex_fetch = new VertexFetch(m_attrib_adapt, m_vertex_buffer, m_index_buffer);
    		}
    	}

    m_pipeline = new Pipeline(m_program, m_vertex_fetch);
    }

RenderTask::~RenderTask() {
    if(m_attrib_adapt) delete m_attrib_adapt;
    if(m_frag_shader) delete m_frag_shader;
    if(m_index_buffer) delete m_index_buffer;
    if(m_pipeline) delete m_pipeline;
    if(m_program) delete m_program;
    if(m_vertex_buffer) delete m_vertex_buffer;
    if(m_vertex_fetch) delete m_vertex_fetch;
    if(m_vertex_shader) delete m_vertex_shader;
    }

void RenderTask::addSpecification(const AttributeInputAdapter::attribute_spec spec) {
    // Pass on to the underlying adapter.
    m_attrib_adapt->addSpecification(spec);
    }

void RenderTask::setUniformLoadPoint(std::string u_name, GLvoid* source) {
    // Pass on to the underlying program object.
	m_program->setUniformLoadPoint(u_name, source);
    }

void RenderTask::utilise(GLenum primitive, GLsizei count) {
	// Pass on to the underlying pipeline object.
	m_pipeline->utilise(primitive, count);
    }

void RenderTask::acquire(void) {
	if(m_vertex_buffer != NULL) {
		m_vertex_buffer->acquire();
		}

    if(m_index_buffer != NULL) {
    	m_index_buffer->acquire();
    	}

    m_program->acquire();

    if(m_program->status() != Program::LINKAGE_SUCCEEDED) {
    	std::stringstream linking_log;
    	linking_log << "RenderTask::acquire() : program did not link !!" << std::endl
					<< "Linker log follows :\n"
					<< "------------------------------------------------------\n"
					<< m_program->linkageLog()
					<< "------------------------------------------------------"
					<< std::endl;
    	ErrorHandler::record(linking_log.str(), ErrorHandler::INFORM);
    	}

    // Pass on to the underlying program object.
    m_program->setUniformLoadPoint(m_transform_name, m_transform);
    }

void RenderTask::setTransform(const std::string& name, glm::mat4* matrix) {
	m_transform_name = name;
	m_transform = matrix;
	}
