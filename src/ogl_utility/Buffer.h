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
 * \brief This virtual interface declares public methods to deal with OpenGL
 *        ES 2.0 buffer objects.
 *
 *    It's a generic wrapper where the buffer contents are treated as bland and
 * byte granular. Further typing/behaviour is managed in detail in derived classes.
 *
 * \see OGL_ID
 * \see IndexBuffer
 * \see TextureBuffer
 * \see VertexBuffer
 *
 * \author Mike Hewson\n
 */

class Buffer : public OGL_ID {
    public :
        /**
         * \brief Constructor. Will fail fatally if data is NULL.
         *
         * \param data : pointer to the data to be stored.
         */
        Buffer(const GLvoid* buffer_data);

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
         * \brief Obtain a pointer to immutable byte granular data.
         *        By construction this is guaranteed to be non-NULL.
         */
        const GLvoid* data(void) const;

    private:
        /// Flag indicating if resources have been acquired.
        bool acquire_flag;

        /// A pointer to untyped but immutable data.
        const GLvoid* m_data;

        /**
         * \brief Get an OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        virtual void acquire_ID(GLuint* handle) const = 0;

        /**
         * \brief Release to pool the OpenGL handle for the buffer.
         *
         * \param handle : pointer to a handle.
         */
        virtual void release_ID(GLuint* handle) const = 0;

        /**
         * \brief Populate the buffer with data.
         */
        virtual void loadBuffer(void) const = 0;
    };

/**
 * @}
 */

#endif /*BUFFER_H_*/
