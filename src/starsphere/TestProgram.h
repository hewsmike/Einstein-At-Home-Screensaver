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

#ifndef TEST_PROGRAM_H_
#define TEST_PROGRAM_H_

#include "ogl_utility.h"

#include "Program.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief 
 *
 * \see Program
 *
 * \author Mike Hewson\n
 */

class TestProgram : public Program {
    public :
        /**
         * \brief Constructor.
         *
         * \param vertex_shader : a pointer to an existing VertexShader object,
         *                        assumed to take on the role of an
         *                        OpenGL vertex shader.
         * \param fragment_shader : a pointer to an existing Shader object,
         *                          assumed to take on the role of an
         *                          OpenGL fragment shader.
         * \param adapter : a pointer to an AttributeInputAdapter.
         * \param dispose : one of the shaderDisposition enumerants indicating
         *                  desired fate of supplied shaders after any successful
         *                  linkage.
         */
        TestProgram(VertexShader* vertex_shader,
                	FragmentShader* fragment_shader,
					AttributeInputAdapter* adapter,
					UniformInputAdapter* uniforms,
					Program::shaderDisposition dispose);

        /**
         * \brief Destructor.
         */
        virtual ~TestProgram();

        virtual void frameCallback(void);

    private:
        
  	};

/**
 * @}
 */

#endif /*TEST_PROGRAM_H_*/
