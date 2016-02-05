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
         * \param type - the uniform variable's OpenGL type enumeration
         * \param uniform_name - the name of the uniform variable
         * \param load_point - the location to load the value from.
         */
        Uniform(GLenum type, const std::string& uniform_name, GLvoid* load_point);

        /**
         * \brief Destructor
         */
        virtual ~Uniform();

        /**
         * \brief Return the OpenGL enumerant representing the uniform
         *        variable's type.
         *
         * \return The OpenGL enumerant.
         */
        GLenum type(void) const;

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
        std::string type_name(void) const;

        /**
         * \brief Return the uniform variable's program index.
         *
         * \return An integer representing it's program index.
         */
        GLuint index(void) const;

        /**
         * \brief Load the value of the uniform variable into the
         *        currently bound OpenGL program
         *
         * \return A boolean indicating success of the operation
         *              - false : the load failed
         *              - true : the load succeeded
         */
        bool load(void) const;

    private :
        /// Set the program index of the variable.
        void m_set_index(GLint index);

        /// The OpenGL enumerant represeting the uniform variable type.
        GLenum m_type;

        /// The name of the variable as known within a shader.
        std::string m_name;

        /// The client code load point.
        GLvoid* m_load_point;

        /// The index of the uniform variable within it's OpenGL program.
        GLint m_program_index;
	};

/**
 * @}
 */

#endif /*UNIFORM_H_*/
