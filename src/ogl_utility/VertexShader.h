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

#ifndef VERTEX_SHADER_H_
#define VERTEX_SHADER_H_

#include "OGL_ID.h"
#include "Shader.h"

#include <string>
#include <vector>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        vertex shader objects.
 *
 *      It's a wrapper class to especially manage acquisition and
 * release of OpenGL resources.
 *
 * \see Shader
 *
 * \author Mike Hewson\n
 */

class VertexShader : public Shader {
    public :
        /**
         * \brief Constructor
         *
         * \param source - pointer to the shader's SINGLE source code C-string.
         */
        VertexShader(const std::string source, std::vector<std::pair<GLuint, std::string> >& attribs);

        /**
         * \brief Destructor
         */
        virtual ~VertexShader();

        GLuint attribCount(void) const;

        std::pair<GLuint, std::string> getAttrib(GLuint index) const;

    private :
        std::vector<std::pair<GLuint, std::string> > m_attribs;
    };

/**
 * @}
 */

#endif /*VERTEX_SHADER_H_*/
