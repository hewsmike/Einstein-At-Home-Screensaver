/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#include "AttributeInputAdapter.h"
#include "Bound.h"
#include "IndexBuffer.h"
#include "OGL_ID.h"
#include "VertexBuffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 *  \brief This base class declares public methods to deal with the
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
 * \see AttributeInputAdapter
 * \see Bound
 * \see IndexBuffer
 * \see OGL_ID
 * \see VertexBuffer
 *
 * \author Mike Hewson\n
 */

class VertexFetch : public OGL_ID, public Bound {
    public :
        /**
         * \brief Constructor.
         */
        VertexFetch(void);

        /**
         * \brief Constructor.
         *
         * \param adapter : a valid pointer to an AtrributeInputAdapter.
         * \param vertices : a valid pointer to a VertexBuffer.
         */
        VertexFetch(AttributeInputAdapter* adapter,
                    VertexBuffer* vertices);

        /**
         * \brief Constructor.
         *
         * \param adapter : a valid pointer to an AtrributeInputAdapter.
         * \param vertices : a valid pointer to a VertexBuffer.
         * \param indices : a valid pointer to an IndexBuffer.
         */
        VertexFetch(AttributeInputAdapter* adapter,
                    VertexBuffer* vertices,
                    IndexBuffer* indices);

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
        virtual bool acquire(void);

        /**
         * \brief Release the OpenGL resource.
         */
        virtual void release(void);

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        virtual void bind(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         *
         *      HIGHLY ADVISED to invoke after rendering and before any
         * other pipeline activity, in order to properly reset the vertex
         * fetching state.
         */
        virtual void unbind(void);

        /**
            * \brief Is the underlying vertex array object ( VAO ) bound to the
            *           state machine ?
            *
            * \return a boolean indicating binding
            *              TRUE - VAO is bound
            *              FALSE - VAO is not bound
            */
        virtual bool isBound(void) const;

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
        // These are the possible operating modes.
        enum operatingMode {BARE,
                            VERTICES_ONLY,
                            VERTICES_AND_INDICES};

        /// Indicator of current operating state.
        operatingMode m_operating_mode;

        ///Has this object been configured ?
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
         *           he vertex buffer, based upon any given vertex attribute
         *           specifications and choice of data mixing.
         */
        void prepareAttributeMapping(void);
    };

/**
 * @}
 */

#endif /*VERTEX_FETCH_*/
