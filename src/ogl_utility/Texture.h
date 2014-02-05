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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        texture objects.
 *
 *      It's a wrapper class to especially manage acquisition and
 * release of OpenGL resources.
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class Texture : public OGL_ID {
    public :
        /**
         * \brief Constructor
         */
        Texture(void);

        /**
         * \brief Destructor
         */
        virtual ~Texture();

        /**
         * \brief Acquire the texture object's OpenGL resources.
         */
        virtual bool acquire(void);

        /**
         * \brief Release the texture object's OpenGL resources.
         */
        virtual void release(void);

    private :

        /**
         * \brief Write data to the texture with the characteristics given at construction.
         */
        void loadTexture(void);
    };

/**
 * @}
 */

#endif /*TEXTURE_H_*/
