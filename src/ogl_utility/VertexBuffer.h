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

#include "Buffer.h"

/**
* \addtogroup ogl_utility OGL_Utility
* @{
*/

/**
* \brief This interface declares public methods to deal with OpenGL ES 2.0
*        vertex buffer objects.
*
* \see Buffer
*
* \author Mike Hewson\n
*/

class VertexBuffer : public Buffer {
    public :
        /**
         * \brief Constructor. Will fail fatally for the application if one or
         *        more of the following applies :
         *          - the data pointer is NULL ( base class enforced ).
         *          - size is not strictly positive.
         *          - usage type is incorrect for OpenGL ES 2.x
         *
         * \param data : pointer to the data to be stored.
         * \param size : number of bytes to allocate.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         */
        VertexBuffer(const GLvoid* buffer_data,
                     GLsizeiptr size,
                     GLenum usage);

        /**
         * \brief Destructor.
         */
        virtual ~VertexBuffer();

    private:
        /// The number of bytes to be allocated to the buffer.
        GLsizeiptr m_size;

        /// The usage hint.
        GLenum m_usage;

        /**
         * \brief Get an OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        GLuint acquire_ID(GLuint* handle) const;

        /**
         * \brief Release to pool the OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        GLuint release_ID(GLuint* handle) const;

         /**
         * \brief Populate the buffer with vertex data.
         */
        void loadBuffer(void) const;
    };

/**
* @}
*/

#endif /*VERTEX_BUFFER_H_*/
