/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#include "framework.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        identifiers.
 *
 *      OpenGL display lists, buffer objects and texture objects have
 * a common functionality for which this class is a wrapper of. The
 * detailed acquisition and release of OpenGL resources is to be provided
 * in subclasses. NOTE CAREFULLY that a derived class destructor MUST
 * call release() !!!
 *
 * \author Mike Hewson\n
 */

class OGL_ID {
    public:
        /// Default initialiser for the identifier.
        static const GLuint NO_ID;

        /**
         * \brief Constructor ( no argument )
         */
        OGL_ID(void);

        /**
         * \brief Destructor - any derived class destructor must call release()
         */
        virtual ~OGL_ID();

        /**
         * \brief Obtains the OpenGL resource.
         */
        virtual void acquire(void) = 0;

        /**
         * \brief Releases the OpenGL resource.
         */
        virtual void release(void) = 0;

        /**
         * \brief Obtain the OpenGL resource identifier.
         *
         * \return the identifier.
         */
        GLuint ID(void) const;

    protected:
        /// The identifier as allocated by OpenGL.
        GLuint ident;
    };

/**
 * @}
 */

#endif /*OGL_ID_H_*/
