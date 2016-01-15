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

#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "ogl_utility.h"

#include "Buffer.h"

/**
* \addtogroup ogl_utility OGL_Utility
* @{
*/

/**
* \brief This interface declares public methods to deal with OpenGL
*        vertex buffer objects.
*
*    This is the case where vertex attributes are supplied to the pipeline
* input end from a linear buffer.
*
* \see Buffer
* \see RenderTask
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
         *          - the buffer size in bytes is zero ( base class enforced ).
         *          - vertices is not strictly positive.
         *          - usage type is incorrect.
         *
         * \param data : pointer to the data to be stored.
         * \param bytes : the number of bytes of data.
         * \param vertices : number of vertices.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         * \param mix : one of the data_mix enumerants.
         */
        VertexBuffer(const GLvoid* buffer_data,
        			 GLuint bytes,
                     GLuint vertices,
                     GLenum usage,
                     data_mix mix);

        /**
         * \brief Destructor.
         */
        virtual ~VertexBuffer();

        /**
         * \brief Perform any binding to the OpenGL pipeline.
         */
        virtual void bind(void);

        /**
         * \brief Remove any binding to the OpenGL pipeline.
         */
        virtual void unbind(void);

        /**
         * \brief The binding state of the underlying OpenGL buffer
         *        object to the OpenGL state machine. This is a
         *        dynamic inquiry.
         *
         * \return a boolean indicating binding state :
         *          true - the object is bound
         *          false - the object is not bound
         */
        virtual bool isBound(void) const;

        /**
		 * \brief What is the usage type for this buffer?
		 */
        GLenum usage(void) const;

        /**
         * \brief The number of vertices represented in this buffer.
         */
        GLuint vertexCount(void) const;

        /**
		 * \brief What is the type of data mixing in this buffer?
		 */
        VertexBuffer::data_mix mix(void) const;

    private :
        /// The usage hint.
        GLenum m_usage;

        /// The number of vertices represented.
        GLuint m_vertex_count;

        /// The manner of data interleaving.
        data_mix m_mix;

        /**
         * \brief Populate the buffer with vertex data.
         */
        virtual void loadBuffer(void) const;

        /**
         * \brief Get an OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void acquire_ID(GLuint* handle);

        /**
         * \brief Release to pool the OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void release_ID(GLuint* handle);

    };

/**
* @}
*/

#endif /*VERTEX_BUFFER_H_*/
