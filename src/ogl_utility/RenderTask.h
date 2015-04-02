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

#ifndef RENDER_TASK_H_
#define RENDER_TASK_H_

#include "ogl_utility.h"

#include <map>
#include <string>

#include "AttributeInputAdapter.h"
#include "FragmentShader.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "VertexFetch.h"
#include "VertexShader.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with an entire rendering task,
 *        that being a specific collection of ogl_utility objects to render an entire vertex
 *        buffer data set.
 *
 * \see AttributeInputAdapter
 * \see FragmentShader
 * \see IndexBuffer
 * \see Pipeline
 * \see Program
 * \see VertexBuffer
 * \see VertexFetch
 * \see VertexShader
 *
 * \author Mike Hewson\n
 */

class RenderTask {
    public :
		struct shader_group {
			const std::string frag_shader_source;
			const std::string vert_shader_source;
			Program::shaderDisposition disposition;
			};

		struct index_buffer_group {
			const GLvoid* buffer_data;
			GLuint bytes;
			GLuint indices;
			GLenum usage;
			GLenum index_type;
			};

        struct vertex_buffer_group {
			const GLvoid* buffer_data;
			GLuint bytes;
			GLuint vertices;
			GLenum usage;
			VertexBuffer::data_mix mix;
        	};

        /**
         * \brief Constructor.
         *
         * \param vertex_shader : a pointer to an existing VertexShader object,
         *                        assumed to take on the role of an
         *                        OpenGL vertex shader.
         * \param fragment_shader : a pointer to an existing Shader object,
         *                          assumed to take on the role of an
         *                          OpenGL fragment shader.
         * \param adapter : a pointer to an AttributeInputAdapter.
         * \param dispose : one of the shaderDisposition enumerants indicating
         *                  desired fate of supplied shaders after any successful
         *                  linkage.
         */
        RenderTask(RenderTask::shader_group s_group,
                   RenderTask::index_buffer_group i_group,
        		   RenderTask::vertex_buffer_group v_group);

        /**
         * \brief Destructor.
         */
        virtual ~RenderTask();

        /**
         * \brief Add another correspondence between vertex buffer and the vertex shader.
         */
        void addSpecification(const AttributeInputAdapter::attribute_spec& spec);

        /**
         * \brief Create a correspondence between a uniform variable, as known
         *        by an OpenGL program object, and a position within client code.
         * \param u_name : the name of the uniform variable.
         * \param source : an untyped pointer to client code where the value
         *                 may be uploaded from.
         */
        void setUniformLoadPoint(std::string u_name, GLvoid* source);

        void utilise(GLenum primitive, GLsizei count);

        void acquire(void);

    private:

        AttributeInputAdapter* m_attrib_adapt;
        FragmentShader* m_frag_shader;
        IndexBuffer* m_index_buffer;
        Pipeline* m_pipeline;
        Program* m_program;
        VertexBuffer* m_vertex_buffer;
        VertexFetch* m_vertex_fetch;
        VertexShader* m_vertex_shader;

  };

/**
 * @}
 */

#endif /*RENDER_TASK_H_*/
