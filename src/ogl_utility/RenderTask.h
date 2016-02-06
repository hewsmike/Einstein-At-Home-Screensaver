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

#include <string>

#include "AttributeInputAdapter.h"
#include "FragmentShader.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Program.h"
#include "TextureBuffer.h"
#include "Uniform.h"
#include "VertexBuffer.h"
#include "VertexFetch.h"
#include "VertexShader.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class declares public methods to deal with an entire
 *        rendering task, such that activation will cause rendering to
 *        occur within the current OpenGL context. Cases :
 *
 * MINIMUM : provide GLSL source code for both vertex and fragment
 * shaders. This is the scenario where vertex attributes are generated
 * and handled entirely by shader code. A shader_group structure
 * with valid contents must be provided.
 *
 * MINUMUM + TEXTURE : case MINIMUM plus provision of texture data
 * to be accessed/interpolated by a fragment shader.
 *
 * MINUMUM + VERTICES : case MINIMUM with provision of vertex
 * attributes to be accessed by a vertex shader.
 *
 * MINUMUM + VERTICES + TEXTURE : case MINIMUM + VERTICES with provision
 * of texture data to be accessed/interpolated by a fragment shader.
 *
 * MINUMUM + VERTICES + INDICES : case MINIMUM + VERTICES with provision
 * of indices into the vertex attribute data in order to select a
 * subset of it. NB it makes no sense to provide indicial data without
 * vertex attribute data.
 *
 * MINUMUM + VERTICES + INDICES + TEXTURE : case MINIMUM + VERTICES +
 * INDICES with provision of a texture data to be accessed/interpolated
 * by a fragment shader.
 *
 * Usage :
 * 		- must ALWAYS provide a shader_group structure.
 *      - provide any other structures suitable per the desired case.
 *      - call a constructor corresponding the desired case,
 *        using the appropriate structures as arguments.
 *      - provide Uniform instances to specify client code loading points.
 *      - if required ie. when using a buffer of vertex attributes, provide
 *        attribute specifications.
 *
 * \see AttributeInputAdapter
 * \see FragmentShader
 * \see IndexBuffer
 * \see Pipeline
 * \see Program
 * \see TextureBuffer
 * \see Uniform
 * \see VertexBuffer
 * \see VertexFetch
 * \see VertexShader
 *
 * \author Mike Hewson\n
 */

class RenderTask {
    public :
		struct shader_group {
			const std::string& vert_shader_source;
			const std::string& frag_shader_source;
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

        struct texture_buffer_group {
            const GLvoid* texture_data;
        	GLuint bytes;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum data_type;
			GLenum wrap_type_s;
			GLenum wrap_type_t;
            bool mipmaps;
            };

        /**
         * \brief Constructor. Shaders only provided. Corresponds to MINIMUM
         *                     case as described.
         *
         * \param s_group : a shader_group structure that specifies the key parameters
         *                  to construct Shader objects for this rendering task.
         */
        //RenderTask(RenderTask::shader_group s_group, Uniform& transform);

        /**
         * \brief Constructor.
         *
         * \param s_group : a shader_group structure that specifies the key parameters
         *                  to construct Shader objects for this rendering task.
         * \param i_group : an index_buffer_group structure that specifies the key parameters
         *                  to possibly construct an IndexBuffer object for this rendering task.
         * \param v_group : a vertex_buffer_group structure that specifies the key parameters
         *                  to construct a VertexBuffer object for this rendering task.
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
         *
         * \param spec : an attribute specification as defined in the AttributeInputAdapter class.
         */
        void addSpecification(const AttributeInputAdapter::attribute_spec spec);

        /**
         * \brief Create a correspondence between a uniform variable, as known
         *        by an OpenGL program object, and a position within client code.
         *        DO NOT set the vertex shader's uniform transform matrix name
         *        and load point by this method.
         * \param uniform : a reference to a Uniform variable.
         */
        void setUniform(Uniform& uniform);

        /**
         * \brief utilise this task ie. trigger rendering as per setup.
         */
        void utilise(GLenum primitive, GLsizei count);

        /**
         * \brief Acquire the OpenGL state resources for this task.
         */
        void acquire(void);

        static void setTransform(Uniform& transform);

        static Uniform getTransfrom(void);

    private:
        static std::string m_transform_name;
        static GLvoid* m_transform_matrix;
        static bool m_transform_set;

        AttributeInputAdapter* m_attrib_adapt;
        FragmentShader* m_frag_shader;
        IndexBuffer* m_index_buffer;
        Pipeline* m_pipeline;
        Program* m_program;
        TextureBuffer* m_texture;
        Uniform m_transform;
        VertexBuffer* m_vertex_buffer;
        VertexFetch* m_vertex_fetch;
        VertexShader* m_vertex_shader;
    };

/**
 * @}
 */

#endif /*RENDER_TASK_H_*/
