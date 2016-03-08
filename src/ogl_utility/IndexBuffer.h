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

#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include "ogl_utility.h"

#include "Buffer.h"

/**
* \addtogroup ogl_utility OGL_Utility
* @{
*/

/**
* \brief This interface declares public methods to deal with OpenGL
*        index buffer objects.
*
*    This is the case where vertex attributes are supplied to the pipeline
* input end from a linear buffer, with a buffer of this type supplying
* indices into those.
*
* \see Buffer
* \see RenderTask
* \see VertexBuffer
*
* \author Mike Hewson\n
*/

class IndexBuffer : public Buffer {
    public :
        /**
         * \brief Constructor. Will fail fatally for the application if one or
         *        more of the following applies :
         *          - the data pointer is NULL ( base class enforced ).
         *          - the buffer size in bytes is zero ( base class enforced ).
         *          - number of indices is not strictly positive.
         *          - usage type is incorrect.
         *          - index type is incorrect.
         *
         * \param data : pointer to the data to be stored.
         * \param bytes : the number of bytes of data.
         * \param indices : number of indices.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         * \param index_type : one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT ( default ).
         */
        IndexBuffer(const GLvoid* buffer_data,
                    GLuint bytes,
                    GLuint indices,
                    GLenum usage,
                    GLenum index_type = GL_UNSIGNED_INT);

        /**
         * \brief Destructor.
         */
        virtual ~IndexBuffer();

        /**
         * \brief Obtains the resources.
         *
         * \return a boolean indicating success of acquisition
         *              true - resources acquired without error
         *              false - resources were not acquired
         */
        virtual bool acquire(void);

        /**
         * \brief Releases the object resources.
         */
        virtual void release(void);

        /**
         * \brief Perform any binding to the OpenGL pipeline.
         */
        virtual void bind(void);

        /**
         * \brief Remove any binding to the OpenGL pipeline.
         */
        virtual void unbind(void);

        /**
         * \brief The binding state of any underlying OpenGL objects
         *        to the OpenGL state machine. This is a
         *        dynamic inquiry.
         *
         * \return a boolean indicating binding state :
         *          true - the object is bound
         *          false - the object is not bound
         */
        virtual bool isBound(void) const;

        /*
         * \brief The type of index used.
         */
        GLenum indexType(void) const;

    private:
        /// The number of indices in the buffer.
        GLuint m_indices;

        /// The usage hint.
        GLenum m_usage;

        /// The index data type.
        GLenum m_index_type;

        /**
         * \brief Populate the buffer with index data.
         */
        virtual void loadBuffer(void);
    };

/**
* @}
*/

#endif /*INDEX_BUFFER_H_*/
