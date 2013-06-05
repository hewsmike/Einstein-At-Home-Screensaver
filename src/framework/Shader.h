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

#ifndef SHADER_H_
#define SHADER_H_

#include <string>

class Shader {
    public:
        /// Enumerant to define stages of shader handling.
        ///     INVALID - shader has no contents ( invalid resource identifier provided ).
        ///     EXIXTS - contents have been loaded from a Resource.
        ///     COMPILED - shader has compiled satisfactorily.
        enum status {INVALID, EXISTS, COMPILED};

        enum context {BACKWARD, FORWARD};

        /**
         * Constructor
         *
         * \param a resource identifier, so that a ResourceFactory may instantiate.
         *
         * \param an enmerant indicating type of OpenGL context being used.
         */
        Shader(const string resource_identifier, Shader::context ogl_context);

        /**
         * Destructor
         */
        ~Shader();

        /**
         * Obtain the text contents of the shader.
         *
         * \return - an std::string reference representing the text
         */
        const std::string& contents(void) const;

        /**
         * Obtain the status of this shader.
         *
         * \return - one of the status enumerants as above.
         */
        Shader::status getStatus(void) const;

    private:
        /// Strings representing shader headers per OpenGL context type.
        static const std::string BACKWARD_SHADER_HEADER;
        static const std::string FORWARD_SHADER_HEADER;

        /// Stage of this shader.
        Shader::status state;

        /// OpenGL context this shader will run under.
        Shader::context ogl_context_type;

        /// Shader contents.
        std::string text;
    }

#endif /*SHADER_H_*/
