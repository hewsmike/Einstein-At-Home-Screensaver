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

#ifndef OGL_ID_H_
#define OGL_ID_H_

#include "Acquirable.h"
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
 * \author Mike Hewson\n
 */

class OGL_ID : public Acquirable {
    public:
        /// Default initialiser for the identifier.
        static const GLuint NO_ID;

        /**
         * \brief Constructor ( no argument )
         */
        OGL_ID(void);

        /**
         * \brief Destructor.
         */
        virtual ~OGL_ID();

        /**
         * \brief Obtain the OpenGL resource identifier.
         *
         * \return the identifier.
         */
        GLuint ID(void) const;

    protected :
        /**
         * \brief Set the OpenGL resource identifier.
         *
         * \param value : the identifier value
         */
        void set_ID(GLuint value);

        /**
         * \brief Reset the OpenGL resource identifier to a 'NULL' value.
         */
        void reset_ID(void);

    private:
        /// The identifier as allocated by OpenGL.
        GLuint m_ident;
    };

/**
 * @}
 */

#endif /*OGL_ID_H_*/
