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

#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include "ogl_utility.h"

#include "Buffer.h"

/**
* \addtogroup ogl_utility OGL_Utility
* @{
*/

/**
* \brief This interface declares public methods to deal with OpenGL ES 2.0
*        index buffer objects.
*
* \see Buffer
*
* \author Mike Hewson\n
*/

class IndexBuffer : public Buffer {
    public :
        /**
         * \brief Constructor. Will fail fatally for the application if one or
         *        more of the following applies :
         *          - the data pointer is NULL ( base class enforced ).
         *          - indices is not strictly positive.
         *          - usage type is incorrect for OpenGL ES 2.x
         *          - index type is incorrect for OpenGL ES 2.x
         *
         * \param data : pointer to the data to be stored.
         * \param indices : number of indices.
         * \param usage : one of GL_STREAM_DRAW, GL_STATIC_DRAW or GL_DYNAMIC_DRAW.
         * \param index_type : of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNISGNED_INT.
         */
        IndexBuffer(const GLvoid* buffer_data,
                    GLuint indices,
                    GLenum usage,
                    GLenum index_type);

        /**
         * \brief Destructor.
         */
        virtual ~IndexBuffer();

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

        /// The number of indices in the buffer.
        GLuint m_indices;

        /// The usage hint.
        GLenum m_usage;

        /// The index data type.
        GLenum m_index_type;

        /**
         * \brief Populate the buffer with index data.
         */
        virtual void loadBuffer(void) const;
    };

/**
* @}
*/

#endif /*INDEX_BUFFER_H_*/
