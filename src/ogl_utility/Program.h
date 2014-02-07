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

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "OGL_ID.h"
#include "Shader.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        program objects. It's a wrapper.
 *
 * \see OGL_ID
 * \see Shader
 *
 * \author Mike Hewson\n
 */

class Program : public OGL_ID {
    public :
        /**
         * \brief Constructor.
         *
         * \param vertex_shader : reference to an existing Shader object,
         *                        assumed to take on the role of an
         *                        OpenGL ES 2.0 vertex shader.
         * \param fragment_shader : reference to an existing Shader object,
         *                          assumed to take on the role of an
         *                          OpenGL ES 2.0 fragment shader.
         */
        Program(const Shader& vertex_shader, const Shader& fragment_shader);

        /**
         * \brief Destructor.
         */
        virtual ~Program();

        /**
         * \brief Obtains the program object resources.
         *
         * \return a boolean indicating success of acquisition
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        bool acquire(void);

        /**
         * \brief Releases the program object resources.
         */
        void release(void);

    private:
        // These are merely set during construction, though utilised during acquisition.
        /// The vertex shader reference.
        const Shader& m_vertex_shader;

        /// The fragment shader reference.
        const Shader& m_fragment_shader;
    };

/**
 * @}
 */

#endif /*PROGRAM_H_*/
