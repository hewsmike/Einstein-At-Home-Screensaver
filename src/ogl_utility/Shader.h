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
 * \addtogroup ogl_utility OGL_Utility
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
        enum compileStatus {NEVER_COMPILED,
                            COMPILE_FAILED,
                            COMPILE_SUCCEEDED};

        /**
         * \brief Constructor
         *
         * \param type - one of the following OpenGL ES 2.0 supported shader types :
         *               GL_VERTEX_SHADER
         *               GL_FRAGMENT_SHADER
         *
         * \param source - pointer to the shader's source code.
         */
        Shader_OBJ(GLenum type, const GLchar* source);

        /**
         * \brief Destructor
         */
        virtual ~Shader();

        /**
         * \brief Acquire the shader object's OpenGL resources.
         *
         * \return a boolean indicating success of acquisition
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        virtual bool acquire(void);

        /**
         * \brief Release the shader object's OpenGL resources.
         *
         * \return a boolean indicating success of release
         *              TRUE - resources released without error
         *              FALSE - resources were not released
         */
        virtual bool release(void);

        /**
         * \brief Compile the shader.
         *
         * \return a boolean indicating success of compilation.
         *              TRUE - shader compiled without error
         *              FALSE - shader compilation failed
         */
        bool compile(void);

        /**
         * \brief Query the compilation status.
         *
         * \return an enumerant of type compileStatus indicating the result
         *         of the most recent compilation, if any.
         *              NEVER_COMPILED : has not yet been presented to GLSL compiler
         *              COMPILE_FAILED : has been presented but failed
         *              COMPILE_SUCCEEDED : has been presented and no error occurred
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

        compileStatus isCompiled(void) const;

        /**
         * \brief Query the shader type.
         *
         * \return an enumerant indicating one of the following OpenGL ES 2.0
         *         supported shader types :
         *               GL_VERTEX_SHADER
         *               GL_FRAGMENT_SHADER
         */
        GLenum type(void) const;

    private :
        GLenum shader_type;

        compileStatus compilation_status;
    };

/**
 * @}
 */

#endif /*SHADER_H_*/
