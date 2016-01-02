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

#ifndef VERTEX_FETCH_ATTRIBUTES
#define VERTEX_FETCH_ATTRIBUTES

#include "ogl_utility.h"

#include "framework.h"

#include "AttributeInputAdapter.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFetch.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This base class declares public methods to deal with the
 *        OpenGL pipeline vertex fetch functionality ie.
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

class VertexFetchAttributes : public VertexFetch {
    public :
		/**
         * \brief Constructor.
         *
         * \param adapter : a pointer to an AtrributeInputAdapter. This must be non-NULL.
         * \param vertices : a pointer to a VertexBuffer. This must be non-NULL.
         * \param indices : a pointer to an IndexBuffer. This may be NULL
         *                  if no indices are to be used, and defaults to
         *                  NULL if not provided.
         */
        VertexFetchAttributes(AttributeInputAdapter* adapter,
        					  VertexBuffer* vertices,
        					  IndexBuffer* indices = NULL);

        /**
         * \brief Destructor.
         */
        virtual ~VertexFetchAttributes();

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void bind(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         *
         *      HIGHLY ADVISED to invoke after rendering and before any
         * other pipeline activity, in order to properly reset the vertex
         * fetching state.
         */
        void unbind(void);

        /**
         * \brief Trigger pipeline activity. Attachment occurs automatically
         *        if not already performed.
         *
         * \param primitive : one of the OpenGL primitives
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

    private :
        bool m_configure_flag;

        // The input adapter pointer.
        AttributeInputAdapter* m_adapter;

        // The given Buffer pointers.
        VertexBuffer* m_vertices;
        IndexBuffer* m_indices;

        /// The total length in bytes of all the attributes.
		GLuint m_attribute_length_sum;

		bool configure(void);

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

#endif /*VERTEX_FETCH_ATTRIBUTES*/
