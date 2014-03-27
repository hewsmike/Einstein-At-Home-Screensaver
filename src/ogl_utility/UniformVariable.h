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

#ifndef UNIFORM_VARIABLE_H_
#define UNIFORM_VARIABLE_H_

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This virtual interface declares public methods to deal with OpenGL
 *        ES 2.0 uniform variables.
 *
 * \author Mike Hewson\n
 */

class UniformVariable {
    public :
        /**
         * \brief Constructor.
         *
         * \param name : name of the uniform variable as used within shader.
         * \param shader_type :  one of GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
         */
        UniformVariable(const std::string& name,
                        GLenum shader_type);

        /**
         * \brief Destructor.
         */
        virtual ~UniformVariable();

    private:
        /// The variable's name.
        std::string m_name;

        /// The shader type which contains it.
        GLenum m_shader_type;
    };

/**
 * @}
 */

#endif /*UNIFORM_VARIABLE_H_*/
