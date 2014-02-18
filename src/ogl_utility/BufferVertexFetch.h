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

#ifndef BUFFER_VERTEX_FETCH_
#define BUFFER_VERTEX_FETCH_

#include "Buffer.h"
#include "VertexFetch.h"

#include <vector>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class declares public methods to deal with the OpenGL ES 2.0
 *        pipeline vertex fetch functionality using server side buffers.
 *
 *      This is the case where vertex attributes are supplied to the pipeline
 * sequentially from a buffer. The following sequence is strongly advised :
 *      - by sufficient calls to addAttributeDescription(), provide
 *        information about ALL attributes within the provided buffer,
 *        in the order that they are stored within the buffer.
 *      - use attach(), to make buffer current for the pipeline.
 *      - use trigger(), to get rendering activity.
 *      - use detach(), when rendering is complete.
 *
 *      Note that the buffer must be populated elsewhere by data to
 * match the provided attribute specifications and data mixing.
 *
 * \author Mike Hewson\n
 */

class BufferVertexFetch : public VertexFetch {
    public :
        enum data_mix {BY_VERTEX,
                       BY_ATTRIBUTE};

        struct attrib_spec {
            // The attribute's index within the vertex shader.
            GLuint index;
            // Number of components for this attribute.
            GLint size;
            // The data type of each component of this attribute.
            // Valid values are :
            //      GL_BYTE
		    //      GL_UNSIGNED_BYTE
		    //      GL_SHORT
		    //      GL_UNSIGNED_SHORT
		    //      GL_FIXED
            //      GL_FLOAT
            GLenum type;
            // For integer data types only, whether values are normalised
            // with respect to the type range.
            //      GL_TRUE signed integers map to [-1, +1]
            //              unsigned integers map to [0, +1]
            //      GL_FALSE no normalisation, directly map to floats.
            GLboolean normalised;
            };

        /**
         * \brief Constructor.
         *
         * \param vertex_buffer : pointer to the ogl_utility Buffer object
         *                        containing the vertex data. A NON-NULL pointer.
         * \param mix : the type of assumed mixing of data within the Buffer
         *                  BY_VERTEX, ie. per vertex storage
         *                  BY_ATTRIBUTE, ie. per attribute storage
         * \param vertex_count : how many vertices are represented by
         *                       the buffer object. A NON-ZERO positive integer.
         */
        BufferVertexFetch(const Buffer* vertex_buffer,
                          data_mix mix,
                          GLuint vertex_count);

        /**
         * \brief Destructor.
         */
        virtual ~BufferVertexFetch();

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void attach(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         *
         *      Must be invoked after rendering using the provided buffer,
         * and before any other pipeline activity, in order to properly reset
         * the the vertex fetching state.
         */
        void detach(void);

        /**
         * \brief Add an attribute description.
         *
         *      CRUCIAL : By sufficient calls to this method, present ALL
         * attributes IN THEIR SEQUENCE of storage within the given buffer.
         * ( The index specified within the 'attribute_spec' structure is not
         * relevant to this ).
         *
         * \param specification : structure describing the attribute data.
         */
        void addAttributeDescription(attrib_spec specification);

    private :
        // Pointer to a provided buffer.
        const Buffer* m_vert_buffer;

        // How the data is mixed within the buffer.
        data_mix m_mix;

        // How many vertices are represented within the buffer.
        GLuint m_vertex_count;

        // Flag indicating if buffer attribute mapping has been done.
        bool m_attributes_mapped;

        GLuint m_attribute_length_sum;

        struct attribute_record {attrib_spec a_spec;        // An attribute specification.
                                 GLuint length;             // The byte length of this attribute.
                                 GLsizei stride;            // The byte gap between this attribute type in the buffer.
                                 GLvoid* pointer;           // The byte offset of the FIRST of this attribute in the buffer.
                                 };

        // Storage for all the attribute specifications, sorted
        // by shader index.
        std::vector<attribute_record> m_attribute_specs;

        /**
         * \brief Create full/detailed mapping of attribute positions within
         *        the buffer, based upon any given vertex attribute
         *        specifications and choice of data mixing.
         */
        void prepareAttributeMapping(void);
    };

/**
 * @}
 */

#endif /*BUFFER_VERTEX_FETCH_*/
