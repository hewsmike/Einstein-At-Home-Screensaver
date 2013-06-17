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

#include "OGL_ID.h"

#include <string>

class Shader_OBJ {
    public:
        /// Enumerant to define stages of Shader_OBJ handling.
        ///     INVALID - Shader object has no contents
        ///     EXISTS - contents have been loaded from a Resource.
        ///     COMPILED - Shader object has compiled satisfactorily.
        ///     FAILED - Shader object has not compiled satisfactorily.
        enum status {INVALID, EXISTS, COMPILED, FAILED};

        /**
         * Constructor
         *
         * \param a resource identifier, so that a ResourceFactory may instantiate
         *        a byte sequence representing the shader code
         *
         * \param an enumerant of GLenum indicating type of OpenGL shader to construct.
         *          One of :
         *              - GL_VERTEX_SHADER
         *              - GL_FRAGMENT_SHADER
         *              - GL_TESS_CONTROL_SHADER
         *              - GL_TESS_EVALUATION_SHADER
         *              - GL_GEOMETRY_SHADER
         */
        Shader_OBJ(const string resource_identifier, GLenum shader_type);

        /**
         * Destructor
         */
        ~Shader_OBJ();

        /**
         * Obtain the status of this Shader_OBJ.
         *
         * \return - one of the status enumerants as above.
         */
        Shader_OBJ::status getStatus(void) const;

        /**
         * \brief Obtains the shader object resources.
         */
        virtual void acquire(void);

        /**
         * \brief Releases the shader object resources.
         */
        virtual void release(void);

    private:
        /// Stage of this shader object's development.
        Shader_OBJ::status state;

        /// OpenGL shader type this object represents.
        GLenum type;

        /// Shader_OBJ contents.
        std::string text;
    }

#endif /*SHADER_OBJ_H_*/
