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

#ifndef BUFFER_H_
#define BUFFER_H_

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL ES 2.0
 *        buffer objects. It's a wrapper. The buffer contents are treated as
 *        byte granular, and further typing is enjoined in other classes.
 *
 * \see OGL_ID
 * \see VertexFetch
 *
 * \author Mike Hewson\n
 */

class Buffer : public OGL_ID {
    public :
        /**
         * \brief Constructor. Will fail fatally for the application if one or
         *        more of the following applies :
         *              - target type is incorrect for OpenGL ES 2.x
         *              - usage type is incorrect for OpenGL ES 2.x
         *              - size is not strictly positive
         *              - the data pointer is NULL.
         *
         * \param target : one of GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.
         * \param size : number of bytes to allocate.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         * \param data : pointer to the data to be stored.
         */
        Buffer(GLenum target, GLsizeiptr size, GLenum usage, const GLvoid* data);

        /**
         * \brief Destructor.
         */
        virtual ~Buffer();

        /**
         * \brief Obtains the buffer object resources.
         *
         * \return a boolean indicating success of acquisition
         *              true - resources acquired without error
         *              false - resources were not acquired
         */
        bool acquire(void);

        /**
         * \brief Releases the buffer object resources.
         */
        void release(void);

        /**
         * \brief Obtains the buffer target type.
         *
         * \return an enumerant indicating one of the allowed OpenGl ES 2.0 types :
         *              GL_ARRAY_BUFFER
         *              GL_ELEMENT_ARRAY_BUFFER
         */
        GLenum target(void) const;

        /**
         * \brief Obtains the buffer usage type.
         *
         * \return an enumerant indicating one of the allowed OpenGl ES 2.0 types :
         *              GL_STREAM_DRAW
         *              GL_STATIC_DRAW
         *              GL_DYNAMIC_DRAW
         */
        GLenum usage(void) const;

    private:
        /// Flag indicating if resources have been acquired.
        bool acquire_flag;

        /// The buffer target type ( binding point ).
        GLenum m_target;

        /// The number of bytes to be allocated to the buffer.
        GLsizeiptr m_size;

        /// The usage hint.
        GLenum m_usage;

        /// A pointer to untyped but immutable data.
        const GLvoid* m_data;

        /**
         * \brief Write data to the buffer with the characteristics given at construction.
         */
        void loadBuffer(void) const;
    };

/**
 * @}
 */

#endif /*BUFFER_H_*/
