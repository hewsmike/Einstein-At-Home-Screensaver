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

#ifndef FRAGMENT_SHADER_H_
#define FRAGMENT_SHADER_H_

#include "ogl_utility.h"

#include "Shader.h"

#include <string>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        fragment shader objects.
 *
 *      It's a wrapper class to especially manage acquisition and
 * release of OpenGL resources.
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class FragmentShader : public Shader {
    public :
        /**
         * \brief Constructor
         *
         * \param source - pointer to the shader's SINGLE source code C-string.
         */
        FragmentShader(const std::string& source);

        /**
         * \brief Destructor
         */
        virtual ~FragmentShader();

    private :

    };

/**
 * @}
 */

#endif /*FRAGMENT_SHADER_H_*/
