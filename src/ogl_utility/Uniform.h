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

#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "ogl_utility.h"

#include <string>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class represents a GLSL shader uniform variable and it's
 *        correspondence to a client code load point.
 *
 * \author Mike Hewson\n
 */

class Uniform {
    public :
        /**
         * \brief Constructor
         *
         * \param uniform_name - the name of the uniform variable as
         * 						 declared within a shader
         * \param load_point - the untyped client code location to retrieve
         * 					   the value from.
         */
        Uniform(const std::string& uniform_name, GLvoid* load_point);

        /**
		 * \brief Copy Constructor
		 *
		 * \param other - the name of the Uniform instance.
		 */
		Uniform(const Uniform& other);

		/**
		 * \brief Copy Assignment Operator
		 *
		 * \param other - the name of the Uniform instance.
		 */
		Uniform& operator=(const Uniform& other);

        /**
         * \brief Destructor
         */
        virtual ~Uniform();

        /**
         * \brief Return the uniform variable's name as declared within
         *        a shader.
         *
         * \return An std::string representing the name.
         */
        std::string name(void) const;

        /**
         * \brief Return the uniform variable's loading
         *        point within client code.
         *
         * \return An untyped pointer to client code.
         */
        GLvoid* loadPoint(void) const;

        /**
         * \brief Return the uniform variable's type name.
         *
         * \return An std::string representing the type's name. The
         *         value "UNKNOWN" may be returned if not an OpenGL 3.3
         *         recognised type.
         */
        static std::string type_name(GLenum type);

    private :
        /// The name of the variable as known within a shader.
        std::string m_name;

        /// The client code load point.
        GLvoid* m_load_point;
    };

/**
 * @}
 */

#endif /*UNIFORM_H_*/
