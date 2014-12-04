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

#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "ogl_utility.h"

#include "Buffer.h"

#include <vector>

/**
* \addtogroup ogl_utility OGL_Utility
* @{
*/

/**
* \brief This interface declares public methods to deal with OpenGL ES 2.0
*        vertex buffer objects.
*
*    This is the case where vertex attributes are supplied to the pipeline
* sequentially from a buffer. Assumes data is stored on a per vertex basis.
* The following sequence is strongly advised :
*       - by sufficient calls to addAttributeDescription(), provide
*         information about ALL attributes within the provided buffer,
*         in the order that they are stored within the buffer.
*       - use attach(), to make buffer current for the pipeline.
*       - use detach(), when rendering is complete.
*
* \see Buffer
* \see Pipeline
*
* \author Mike Hewson\n
*/

class VertexBuffer : public Buffer {
    public :
        // Overall structure of data buffer :
        //      BY_VERTEX - primary key is vertex, secondary is attribute,
        //                  if you like 'interleaved' or 'array of structures'.
        //      BY_ATTRIBUTE - primary key is attribute, secondary is vertex,
        //                     if you like 'non-interleaved' or 'structure of arrays'.
        enum data_mix {BY_VERTEX,
                       BY_ATTRIBUTE};

        struct attribute_spec {
            // The index of the attribute in the vertex fetching.
            GLuint attrib_index;
            // Number of components for this attribute.
            GLint multiplicity;
            // The data type of each component of this attribute.
            // Valid values are :
            // GL_BYTE
            // GL_UNSIGNED_BYTE
            // GL_SHORT
            // GL_UNSIGNED_SHORT
            // GL_FIXED
            // GL_FLOAT
            GLenum type;
            // For integer data types only, whether values are normalised
            // with respect to the type range.
            // GL_TRUE signed integers map to [-1, +1]
            // unsigned integers map to [0, +1]
            // GL_FALSE no normalisation, directly map to floats.
            GLboolean normalised;
            };

        /**
         * \brief Constructor. Will fail fatally for the application if one or
         *        more of the following applies :
         *          - the data pointer is NULL ( base class enforced ).
         *          - vertices is not strictly positive.
         *          - usage type is incorrect for OpenGL ES 2.x
         *
         * \param data : pointer to the data to be stored.
         * \param vertices : number of vertices.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         * \param mix : one of the data_mix enumerants.
         */
        VertexBuffer(const GLvoid* buffer_data,
                     GLuint vertices,
                     GLenum usage,
                     data_mix mix);

        /**
         * \brief Destructor.
         */
        virtual ~VertexBuffer();

        /**
         * \brief The number of vertices represented in this buffer.
         */
        GLuint vertexCount(void) const;

        /**
         * \brief Add an attribute description.
         *
         * CRUCIAL : By sufficient calls to this method, present ALL
         * attributes IN THEIR SEQUENCE of storage within the given buffer.
         * ( The index specified within the 'attribute_spec' structure is not
         * relevant to this ).
         *
         * \param specification : structure describing the attribute data.
         */
        void addAttributeDescription(attribute_spec specification);

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void attach(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         */
        void detach(void);

    private:
        /// The number of bytes to be allocated to the buffer.
        GLsizeiptr m_size;

        /// The usage hint.
        GLenum m_usage;

        /// The number of vertices represented.
        GLuint m_vertex_count;

        /// Flag indicating if buffer attribute mapping has been done.
        bool m_attributes_mapped;

        /// The total length in bytes of all the attributes.
        GLuint m_attribute_length_sum;

        /// The manner of data interleaving.
        data_mix m_mix;

        /**
         * \brief Get an OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        virtual void acquire_ID(GLuint* handle) const;

        /**
         * \brief Release to pool the OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        virtual void release_ID(GLuint* handle) const;

        /**
         * \brief Populate the buffer with vertex data.
         */
        virtual void loadBuffer(void) const;

        struct attribute_record {attribute_spec a_spec;     // An attribute specification.
                                 GLuint length;             // The byte length of this attribute (how many x how long).
                                 GLsizei stride;            // The byte gap between this attribute type in the buffer.
                                 GLvoid* pointer;           // The byte offset of the FIRST of this attribute in the buffer.
                                 };

        // Storage for all the attribute specifications.
        std::vector<attribute_record> m_attribute_specs;

        /**
         * \brief Create full/detailed mapping of attribute positions within
         * the buffer, based upon any given vertex attribute
         * specifications and choice of data mixing.
         */
        void prepareAttributeMapping(void);
    };

/**
* @}
*/

#endif /*VERTEX_BUFFER_H_*/
