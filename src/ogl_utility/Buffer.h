/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

/// Not a pretty solution for a purist, but it gets the pointer type right.
/// Used to represent an offset into an OpenGL buffer ( server side ) object.
#define BUFFER_OFFSET(bytes)  ((GLubyte*)NULL + (bytes))

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        buffer objects. It's a wrapper.
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class Buffer : public OGL_ID {
    public :
        /**
         * \brief Constructor.
         *
         * \param target : one of GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
         * \param size : number of bytes to allocate
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW
         * \param data : pointer to the data to be stored, but
         *               NOTE CAREFULLY : the data is only accessed when acquire() is called.
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
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        bool acquire(void);

        /**
         * \brief Releases the buffer object resources.
         */
        void release(void);

    private:
        // These are merely set during construction, though utilised during acquisition.
        /// The buffer target type ( binding point ).
        GLenum m_target;

        /// The number of bytes to be allocated to the buffer.
        GLsizeiptr m_size;

        /// The usage hint.
        GLenum m_usage;

        /// A pointer to untyped data.
        const GLvoid* m_data;

        /**
         * \brief Write data to the buffer with the characteristics given at construction.
         */
        void loadBuffer(void);
    };

/**
 * @}
 */

#endif /*BUFFER_H_*/
