/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#include "Bindable.h"
#include "Configurable.h"
#include "ogl_utility.h"
#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This virtual interface declares public methods to deal with OpenGL
 *        buffer objects. Such objects are NOT C++ language objects in client
 *        code, but those generated by the OpenGL API on request and then
 *        manipulated by the relevant identifier/handle using OpenGL API calls.
 *
 *    It's a generic wrapper, with further typing/behaviour being managed in
 * detail in derived classes.
 *
 * \see Bindable
 * \see IndexBuffer
 * \see OGL_ID
 * \see TextureBuffer
 * \see VertexBuffer
 *
 * \author Mike Hewson\n
 */

class Buffer : public OGL_ID, public Bindable, public Configurable {
    public :
        /**
         * \brief Constructor.
         *
         * \param buffer_data : pointer to the data to be copied from client
         *                      memory and stored in an underlying OpenGL
         *                      buffer type. As this is in client memory it
         *                      ought persist at least until OpenGL resource
         *                      acquisition has occurred . Will fail fatally
         *                      if NULL supplied.
         * \param bytes : the number of bytes of data. This must be a strictly
         *                positive ( non-zero ) integer.
         */
        Buffer(const GLvoid* buffer_data, GLuint bytes);

        /**
         * \brief Destructor.
         */
        virtual ~Buffer();

        /**
         * \brief Actually configure any underlying object(s).
         *
         * \return a boolean indicating success of configuration
         *              - true, the configuration as successful.
         *              - false, the configuration was not successful.
         */
        virtual bool configure(void);

        /**
         * \brief The number of bytes stored within the buffer.
         *
         * \return The size of the buffer in bytes.
         */
        GLuint size(void) const;

    protected :
        /**
         * \brief Obtain the pointer to immutable byte granular data.
         *        By construction this is guaranteed to be non-NULL.
         *        HOWEVER as it refers to a location in client memory
         *        it OUGHT be valid for the lifetime of an object of this
         *        class. Failing that then at least until state machine
         *        resources have been acquired and data copied to same
         *        ( and assuming no re-acquistion is later desired ).
         *
         * \return pointer to client memory location of buffer data.
         */
        const GLvoid* data(void) const;

    private:
        /// The number of bytes to be allocated to the buffer.
        GLuint m_size;

        /// A valid pointer to untyped data in client memory. Note other
        /// comments regarding persistence.
        const GLvoid* m_data;

        /**
         * \brief Populate the buffer with data. This is the where a valid
         *        client side pointer is required.
         */
        virtual void loadBuffer(void) = 0;
    };

/**
 * @}
 */

#endif /*BUFFER_H_*/
