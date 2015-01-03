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
        GLuint size(void) const;

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void bind(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         */
        void unbind(void);

        /**
		 * \brief What is the type of data mixing in this buffer?
		 */
        enum data_mix mix(void) const;

    private:
        /// The number of bytes to be allocated to the buffer.
        GLsizeiptr m_size;

        /// The usage hint.
        GLenum m_usage;

        /// The number of vertices represented.
        GLuint m_vertex_count;

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
    };

/**
* @}
*/

#endif /*VERTEX_BUFFER_H_*/
