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
 * TODO : currently only enacting one each of vertex and fragment shaders. Extend ??
 *
 * \see OGL_ID
 * \see Shader
 *
 * \author Mike Hewson\n
 */

class Program : public OGL_ID {
    public :
        enum linkageState {NEVER_LINKED,
                           LINKAGE_FAILED,
                           LINKAGE_SUCCEEDED};

        enum shaderDisposition {KEEP_ON_GOOD_LINK,
                                RELEASE_ON_GOOD_LINK};

        /**
         * \brief Constructor.
         *
         * \param vertex_shader : reference to an existing Shader object,
         *                        assumed to take on the role of an
         *                        OpenGL ES 2.0 vertex shader.
         * \param fragment_shader : reference to an existing Shader object,
         *                          assumed to take on the role of an
         *                          OpenGL ES 2.0 fragment shader.
         * \param dispose : one of the shaderDisposition enumerants indicating
         *                  desired fate of supplied shaders after any successful
         *                  linkage.
         */
        Program(Shader& vertex_shader,
                Shader& fragment_shader,
                shaderDisposition dispose);

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

        /**
         * \brief Determine if program has been marked for deletion.
         *
         * \return a boolean indicating deletion status
         *              true - shader is marked for deletion
         *              false - shader is NOT marked for deletion
         */
        bool isDeleted(void) const;

        /**
         * \brief Query the linkage status.
         *
         * \return an enumerant of type linkageState indicating the result
         *         of the most recent linking attempt, if any.
         *              NEVER_LINKED : has not yet been presented to GLSL linker
         *              LINKAGE_FAILED : has been presented but failed
         *              LINKAGE_SUCCEEDED : has been presented and no error occurred
         */
        Program::linkageState status(void) const;


    private:
        static const GLint GLSL_LINKAGE_FAILURE;
        static const GLint GLSL_LINKAGE_SUCCESS;

        /**
         * \brief Link the program.
         *
         * \return a boolean indicating success of linkage.
         *              true - program linked without error
         *              false - program linkage failed
         */
        bool link(void);

        // These are merely set during construction, though utilised during acquisition.
        /// The vertex shader reference.
        Shader& m_vertex_shader;

        /// The fragment shader reference.
        Shader& m_fragment_shader;

        /// Indicator of current linkage state.
        linkageState link_status;

        /// Choice of disposition of shader objects.
        shaderDisposition m_dispose;

        /// The linker log for this shader.
        std::string linker_log;
    };

/**
 * @}
 */

#endif /*PROGRAM_H_*/
