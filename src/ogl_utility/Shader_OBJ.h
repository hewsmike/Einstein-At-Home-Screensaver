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

#ifndef SHADER_OBJ_H_
#define SHADER_OBJ_H_

#include <string>

class Shader_OBJ {
    public:
        /// Enumerant to define stages of Shader_OBJ handling.
        ///     INVALID - Shader_OBJ has no contents ( invalid resource identifier provided ).
        ///     EXIXTS - contents have been loaded from a Resource.
        ///     COMPILED - Shader_OBJ has compiled satisfactorily.
        enum status {INVALID, EXISTS, COMPILED};

        enum context {BACKWARD, FORWARD};

        /**
         * Constructor
         *
         * \param a resource identifier, so that a ResourceFactory may instantiate.
         *
         * \param an enmerant indicating type of OpenGL context being used.
         */
        Shader_OBJ(const string resource_identifier, Shader_OBJ::context ogl_context);

        /**
         * Destructor
         */
        ~Shader_OBJ();

        /**
         * Obtain the text contents of the Shader_OBJ.
         *
         * \return - an std::string reference representing the text
         */
        const std::string& contents(void) const;

        /**
         * Obtain the status of this Shader_OBJ.
         *
         * \return - one of the status enumerants as above.
         */
        Shader_OBJ::status getStatus(void) const;

    private:
        /// Strings representing Shader_OBJ headers per OpenGL context type.
        static const std::string BACKWARD_SHADER_HEADER;
        static const std::string FORWARD_SHADER_HEADER;

        /// Stage of this Shader_OBJ.
        Shader_OBJ::status state;

        /// OpenGL context this Shader_OBJ will run under.
        Shader_OBJ::context ogl_context_type;

        /// Shader_OBJ contents.
        std::string text;
    }

#endif /*SHADER_OBJ_H_*/
