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

#ifndef VERTEX_FETCH_
#define VERTEX_FETCH_

#include "ogl_utility.h"

#include "framework.h"

#include "AttributeInputAdapter.h"
#include "IndexBuffer.h"
#include "OGL_ID.h"
#include "VertexBuffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This base class declares public methods to deal with the
 *        OpenGL ES 2.0 pipeline vertex fetch functionality ie.
 *        this wraps Vertex Array Objects (VAO's).
 *
 *  Common use cases of constructor :
 *        (1) VertexFetch() - vertex and index buffers set to NULL, index type
 *                        set but not used/relevant. Use if a vertex shader will
 *                        provide vertex attributes.
 *        (2) VertexFetch(NON-NULL) - vertex buffer set to the given NON-NULL
 *                                    address, index buffer set to NULL. Use if
 *                                    geometry is straightforward.
 *        (3) VertexFetch(NON-NULLA, NON-NULLB) - vertex buffer set to the
 *                                                given NON-NULLA address, index
 *                                                buffer set to the given
 *                                                NON-NULLB address. Use if a
 *                                                complex geometry needs a
 *                                                selection of vertices.
 *
 * \see Buffer
 *
 * \author Mike Hewson\n
 */

class VertexFetch : public OGL_ID {
    public :
		/**
         * \brief Constructor.
         *
         * \param vertices : a pointer to vertex buffer. This may be NULL
         *                   if a vertex shader is to generate vertex
         *                   attributes, and defaults to NULL if not
         *                   provided.
         * \param indices : a pointer to an index buffer. This may be NULL
         *                  if no indices are to be used, and defaults to
         *                  NULL if not provided.
         * \param adapter : a pointer to a suitable AttributeInputAdapter
         * 					which contains the relevant correspondences b/w
         * 					vertex buffer contents and shader variables.
         * 					This may be NULL if no vertices are to be used,
         * 					and defaults to NULL if not provided.
         */
        VertexFetch(VertexBuffer* vertices = NULL,
                    IndexBuffer* indices = NULL,
					AttributeInputAdapter* adapter = NULL);

        /**
         * \brief Destructor.
         */
        virtual ~VertexFetch();

        /**
		 * \brief Obtain the OpenGL resource.
		 *
		 * \return a boolean indicating success of acquisition
		 *              TRUE - resources acquired without error
		 *              FALSE - resources were not acquired
		 */
		bool acquire(void);

		/**
		 * \brief Release the OpenGL resource.
		 *
		 * \return a boolean indicating success of release
		 *              TRUE - resources released without error
		 *              FALSE - resources were not released
		 */
		void release(void);

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void bind(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         *
         *      Must be invoked after rendering using the provided buffer,
         * and before any other pipeline activity, in order to properly reset
         * the vertex fetching state.
         */
        void unbind(void);

        /**
         * \brief Trigger pipeline activity. Attachment occurs automatically
         *        if not already performed.
         *
         * \param primitive : one of the OpenGL ES 2.0 primitives
         *          GL_POINTS
         *          GL_LINE_STRIP
         *          GL_LINE_LOOP
         *          GL_LINES
         *          GL_TRIANGLE_STRIP
         *          GL_TRIANGLE_FAN
         *          GL_TRIANGLES
         * \param count : how many times to invoke the vertex shader.
         */
        void trigger(GLenum primitive, GLsizei count);

        /**
		 * \brief Is the underlying vertex array object ( VAO ) bound to the
		 * 	      state machine ?
		 *
		 * \return a boolean indicating binding
		 *              TRUE - VAO is bound
		 *              FALSE - VAO is not bound
		 */
        bool isBound(void) const;

    private :
        // Attachment state. For our purposes any NULL buffers are deemed
        // to be always attached. Covers the case of a vertex shader solely
        // providing vertex attributes. A true value here indicates that
        // the issue has been adressed successfully.
        bool m_bound_flag;

        bool m_configure_flag;

        // The given Buffer pointers.
        VertexBuffer* m_vertices;
        IndexBuffer* m_indices;

        AttributeInputAdapter* m_adapter;

        /// The total length in bytes of all the attributes.
		GLuint m_attribute_length_sum;

		bool configure(void);

        /**
		 * \brief Get an OpenGL handle for this vertex array object.
		 *
		 * \param handle : pointer to a handle.
		 */
		virtual void acquire_ID(GLuint* handle) const;

		/**
		 * \brief Release to pool the OpenGL handle for the vertex array object.
		 *
		 * \param handle : pointer to a handle.
		 */
		virtual void release_ID(GLuint* handle) const;

		struct attribute_record {AttributeInputAdapter::attribute_spec a_spec;     // An attribute specification.
								 GLuint length;             // The byte length of this attribute (how many x how long).
								 GLsizei stride;            // The byte gap between this attribute type in the buffer.
								 GLvoid* pointer;           // The byte offset of the FIRST of this attribute in the buffer.
								 };

		// Storage for all the attribute specifications.
		std::vector<attribute_record> m_attribute_specs;

		/*
		 * \brief
		 */
		void processAttributeDescriptions(void);

		/**
		 * \brief Create full/detailed mapping of attribute positions within
		 * 		  he vertex buffer, based upon any given vertex attribute
		 * 		  specifications and choice of data mixing.
		 */
		void prepareAttributeMapping(void);
    };

/**
 * @}
 */

#endif /*VERTEX_FETCH_*/
