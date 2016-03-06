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

//RenderTask::RenderTask(RenderTask::shader_group s_group) {
//    // Set unused pointers to NULL.
//    m_index_buffer = NULL;
//    m_texture_buffer = NULL;
//    m_vertex_buffer = NULL;
//
//    // Always need to define an attribute adapter, even though not used here.
//    m_attrib_adapt = new AttributeInputAdapter();
//    // Always need a trigger for the pipeline.
//    m_vertex_fetch = new VertexFetch();
//
//    setBaseCase(s_group);
//    }
//
//RenderTask::RenderTask(RenderTask::shader_group s_group,
//                       RenderTask::texture_buffer_group t_group) :
//                           RenderTask(s_group) {
//    if(t_group.texture_data == NULL) {
//        ErrorHandler::record("RenderTask::RenderTask: Texture not provided!", ErrorHandler::FATAL);
//        }
//    m_texture_buffer = new TextureBuffer(t_group.texture_data,
//                                         t_group.bytes,
//                                         t_group.width,
//                                         t_group.height,
//                                         t_group.format,
//                                         t_group.data_type,
//                                         t_group.wrap_type_s,
//                                         t_group.mipmaps);
//    }
//
//RenderTask::RenderTask(RenderTask::shader_group s_group,
//                       RenderTask::vertex_buffer_group v_group) :
//                           RenderTask::RenderTask(s_group) {
//    // Set unused pointers to NULL.
//    m_index_buffer = NULL;
//    m_texture_buffer = NULL;
//
//    // Always need a trigger for the pipeline.
//    m_vertex_fetch = new VertexFetch();
//
//    setBaseCase(s_group);
//
//// VertexFetch(AttributeInputAdapter* adapter,
//                    //VertexBuffer* vertices);
//    if(v_group.buffer_data == NULL) {
//        ErrorHandler::record("RenderTask::RenderTask: Vertex data not provided!", ErrorHandler::FATAL);
//        }
//    if(m_vertex_fetch != NULL) {
//        delete m_vertex_fetch;
//        m_vertex_fetch = new VertexFetch();
//        }
//    m_vertex_buffer = new VertexBuffer(v_group.buffer_data,
//                                       v_group.bytes,
//                                       v_group.vertices,
//                                       v_group.usage,
//                                       v_group.mix);
//    m_vertex_buffer->acquire();
//    }
//
//RenderTask::RenderTask(RenderTask::shader_group s_group,
//                   RenderTask::vertex_buffer_group v_group,
//                   RenderTask::texture_buffer_group t_group) :
//                        RenderTask(s_group, v_group) {
//    if(t_group.texture_data == NULL) {
//        ErrorHandler::record("RenderTask::RenderTask: Texture not provided!", ErrorHandler::FATAL);
//        }
//    m_texture_buffer = new TextureBuffer(t_group.texture_data,
//                                         t_group.bytes,
//                                         t_group.width,
//                                         t_group.height,
//                                         t_group.format,
//                                         t_group.data_type,
//                                         t_group.wrap_type_s,
//                                         t_group.mipmaps);
//    m_texture_buffer->acquire();
//    }

//RenderTask::RenderTask(RenderTask::shader_group s_group,
//                   RenderTask::vertex_buffer_group v_group,
//                   RenderTask::index_buffer_group i_group) :
//                       RenderTask(s_group, v_group) {
//
//                   }

RenderTask::RenderTask(RenderTask::shader_group s_group,
                       RenderTask::vertex_buffer_group v_group,
					   RenderTask::index_buffer_group i_group,
					   RenderTask::texture_buffer_group t_group) {
    //
	m_t_group = t_group;

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

void RenderTask::setUniform(const std::string& uniform_name, GLvoid* load_point) {
    // Pass on to the underlying program object.
    m_program->setUniformLoadPoint(uniform_name, load_point);
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

    if(m_t_group.texture_data != NULL) {
        m_t_group.texture_data->acquire();
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
    }

//void RenderTask::setBaseCase(RenderTask::shader_group s_group) {
//    // Now configure the basic components.
//    // Always need shader code.
//    m_vertex_shader = new VertexShader(s_group.vert_shader_source);
//    m_frag_shader = new FragmentShader(s_group.frag_shader_source);
//    // Always have program entity and a pipleine.
//    m_program = new Program(m_vertex_shader, m_frag_shader, m_attrib_adapt);
//    m_pipeline = new Pipeline(m_program, m_vertex_fetch);
//    }
