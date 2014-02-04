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

#include<string>

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
        enum compilationState {NEVER_COMPILED,
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
        Shader(GLenum type, const GLchar* source);

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
         * \brief Determine if shader has been marked for deletion.
         *
         * \return a boolean indicating deletion status
         *              TRUE - shader is marked for deletion
         *              FALSE - shader is NOT marked for deletion
         */
        bool isDeleted(void);

        /**
         * \brief Query the compilation status.
         *
         * \return an enumerant of type compilationState indicating the result
         *         of the most recent compilation, if any.
         *              NEVER_COMPILED : has not yet been presented to GLSL compiler
         *              COMPILE_FAILED : has been presented but failed
         *              COMPILE_SUCCEEDED : has been presented and no error occurred
         */
        compilationState status(void) const;

        /**
         * \brief Query the shader type.
         *
         * \return an enumerant indicating one of the following OpenGL ES 2.0
         *         supported shader types :
         *               GL_VERTEX_SHADER
         *               GL_FRAGMENT_SHADER
         */
        GLenum type(void) const;

        /**
         * \brief Obtain a copy of this shader's source code.
         *
         * \return A string reference to the source code.
         */
        const std::string& source(void) const;

        /**
         * \brief Obtain a copy of this shader's compile log.
         *
         * \return A string reference to the compile log code. This may be empty.
         */
        const std::string& compileLog(void) const;

    private :
        static const GLint GLSL_COMPILE_FAILURE;
        static const GLint GLSL_COMPILE_SUCCESS;

        /**
         * \brief Compile the shader.
         *
         * \return a boolean indicating success of compilation.
         *              TRUE - shader compiled without error
         *              FALSE - shader compilation failed
         */
        bool compile(void);

        /// One of the OpenGL ES 2.0 supported shader types.
        GLenum shader_type;

        /// Indicator of current compilation state.
        compilationState comp_status;

        /// The shader's GLSL source code.
        std::string shader_source;

        /// The compilation log for this shader.
        std::string compile_log;
    };

/**
 * @}
 */

#endif /*SHADER_H_*/
