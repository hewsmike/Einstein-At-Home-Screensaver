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

#ifndef BUFFER_OBJ_H_
#define BUFFER_OBJ_H_

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

class Buffer_OBJ : public OGL_ID {
    public :
        /**
         * \brief Constructor.
         *
         * \param target : one of GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
         * \param siz
         */
        Buffer_OBJ(GLenum target,
                  GLsizeiptr size,
                  const GLvoid * data,
                  GLenum usage);

        /**
         * \brief Destructor.
         */
        virtual ~Buffer_OBJ();

        /**
         * \brief Obtains the buffer object resources.
         */
        virtual void acquire(void);

        /**
         * \brief Releases the buffer object resources.
         */
        virtual void release(void);

        /**
         * \brief Write data to the buffer with the given characteristics.
         *
         * 	    It is the caller's responsibility to ensure that the given
         * parameters are sensible. So beware the combination of enumerants,
         * the validity of the data pointer and the size. If no buffer
         * identifier has been allocated by OpenGL then one is obtained.
         *
         * \param target - one of the accepted OpenGL enumerants for buffer
         *                 objects, typically either GL_ARRAY_BUFFER or
         *                 GL_ELEMENT_ARRAY_BUFFER.
         * \param usage - one of the accepted OpenGL enumerants, say
         *                GL_STATIC_DRAW.
         * \param size - the amount of data ( counted in bytes ) to transfer
         *               to the server buffer.
         * \param data - a pointer to the data to be transferred.
         */
        void loadBuffer(GLenum  target, GLenum  usage,
                        GLsizeiptr size, const GLvoid* data);
    };

/**
 * @}
 */

#endif /*BUFFER_OBJ_H_*/
