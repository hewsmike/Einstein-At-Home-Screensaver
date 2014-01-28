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

#include "OGL_ID.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        shader objects.
 *
 *      It's a wrapper class to especially manage acquisition and
 * release of OpenGL resources.
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class Shader : public OGL_ID {
    public :
        enum shaderType {VERTEX, FRAGMENT};
        /**
         * \brief Constructor
         */
        Shader(shaderType type);

        /**
         * \brief Destructor
         */
        virtual ~Shader();

        /**
         * \brief Acquire the shader object's OpenGL resources.
         */
        virtual void acquire(void);

        /**
         * \brief Release the shader object's OpenGL resources.
         */
        virtual void release(void);

        /**
         * \brief Release the shader object's OpenGL resources.
         */
        shaderType getType(void) const;

    void loadSource(const GLchar* source);

    const std::string& getSource(void) const;

    bool compile(void);

    bool isCompileOK(void) const;

    const std::string& getCompileLog(void) const;

    const std::string& getGLSLversion(void) const;

  private:

    GLuint m_id;
    shaderType m_type;

    bool m_compile_flag;

    std::string m_GLSL_version;
    std::string m_compile_log;
    std::string m_source;
    };

/**
 * @}
 */

#endif /*SHADER_H_*/
