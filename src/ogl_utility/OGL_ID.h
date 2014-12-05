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

#ifndef OGL_ID_H_
#define OGL_ID_H_

#include "ogl_utility.h"

#include "framework.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        objects that use runtime allocated identifiers.
 *
 *      Many OpenGL types have a common functionality for which this
 * class is a base of. The detailed acquisition and release of OpenGL
 * resources is to be provided in subclasses. NOTE CAREFULLY that a
 * suitable derived class destructor MUST call release() !!!
 *
 * \author Mike Hewson\n
 */

class OGL_ID {
    public:
        /// Enumerants correpsonding to OpenGL ES 2.0 identifer types.
        enum id_type {INVALID_ID,
                      NULL_ID
                      BUFFER_ID,
                      FRAME_BUFFER_ID,
                      PROGRAM_ID,
                      RENDER_BUFFER_ID,
                      SHADER_ID,
                      TEXTURE_ID};

        /// Default initialiser for the identifier.
        static const GLuint NO_ID;

        /**
         * \brief Constructor ( no argument )
         */
        OGL_ID(void);

        /**
         * \brief Destructor - do NOT call release() here, a suitable derived
         *        class destructor MUST call it.
         */
        virtual ~OGL_ID();

        /**
         * \brief Obtain the OpenGL resource.
         *
         * \return a boolean indicating success of acquisition
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        virtual bool acquire(void) = 0;

        /**
         * \brief Release the OpenGL resource.
         *
         * \return a boolean indicating success of release
         *              TRUE - resources released without error
         *              FALSE - resources were not released
         */
        virtual void release(void) = 0;

        /**
         * \brief Obtain the OpenGL resource identifier.
         *
         * \return the identifier.
         */
        GLuint ID(void) const;

        /**
         * \brief Determine if identifier is non-null/zero.
         *
         * \return A boolean : TRUE if the OpenGL identifier is zero
         *                     FALSE if the OpenGL identifier is non-zero.
         */
        bool isNull(void) const;

        /**
         * \brief Obtain the OpenGL resource identifier type.
         *
         * \param emit : If TRUE send message to ErrorHandler::record()
         *               If FALSE no message sent.
         *
         * \return the identifier enumerant as above.
         */
        id_type type(bool emit) const;

    protected :

        /**
         * \brief Set the OpenGL resource identifier.
         *
         * \param value : the identifier value
         */
        void set_ID(GLuint value);

    private:
        /// The identifier as allocated by OpenGL.
        GLuint ident;
    };

/**
 * @}
 */

#endif /*OGL_ID_H_*/
