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

#include "ogl_utility.h"

#include <map>

#include "AttributeInputAdapter.h"
#include "FragmentShader.h"
#include "OGL_ID.h"
#include "VertexShader.h"

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
 * \see FragmentShader
 * \see VertexShader
 *
 * \author Mike Hewson\n
 */

class Program : public OGL_ID {
    public :
        enum linkageState {NEVER_LINKED,
                           LINKAGE_FAILED,
                           LINKAGE_SUCCEEDED};

        // NB DELETE_ON_GOOD_LINK means the OpenGL state machine marks an OpenGL shader
        // object for deletion. This does not refer to any client source code entity.
        enum shaderDisposition {KEEP_ON_GOOD_LINK,
                                DELETE_ON_GOOD_LINK};

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
        Program(VertexShader* vertex_shader,
                FragmentShader* fragment_shader,
				AttributeInputAdapter* adapter,
				shaderDisposition dispose);

        /**
         * \brief Destructor.
         */
        virtual ~Program();

        /**
		 * \brief Use the underlying OpenGL program object.
		 */
		void use(void) const;

		/**
		 * \brief Don't use the underlying OpenGL program object.
		 */
		void stopUse(void) const;

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
         * \brief Determine if program has been MARKED for deletion.
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

        /**
         * \brief Obtain a reference to this program's linkage log.
         *
         * \return A string reference to the linker log. This may be empty
         *         in the instance that linking was a complete success.
         */
        const std::string& linkageLog(void) const;

        /**
		 * \brief Invoke the callback function with the GLuint argument being the
		 * 		  (underlying) OpenGL program entity's OpenGL identifier.
		 */
        void frameCallBack(void);

        /**
         * \brief Create a correspondence between a uniform variable, as known
         *        by an OpenGL program object, and a position within client code.
         * \param u_name : the name of the uniform variable.
         * \param source : an untyped pointer to client code where the value
         *                 may be uploaded from.
         */
        void setUniformLoadPoint(std::string u_name, GLvoid* source);

    private:

        // Data structure containing the relevant parameters for
        // a uniform variable.
        struct uniform_data {
        	// The uniform variable type as known to OpenGL.
        	GLenum m_type;

        	// Untyped pointer to location of persistent data in client space
        	// which will be used to refresh the in-program value.
        	GLvoid* m_load_point;

        	// The location of the uniform within the OpenGL program object.
        	GLint m_location;
        	};

        /**
         * \brief Obtain the uniform_data instance corresponding to the given
         */
        uniform_data getUniform(std::string u_name);

        bool loadUniform(Program::uniform_data current);

        /**
         * \brief Link the program.
         *
         * \return a boolean indicating success of linkage.
         *              true - program linked without error
         *              false - program linkage failed
         */
        bool link(void);

        /**
         * \brief For uniform variables, determine the correspondence between
         *        it's name ( as declared within an attached shader ) and
         *        it's location in the program object. This ought only be done
         *        if the program has been successfully linked, if not this
         *        method will return a false value.
         *
         * \return a boolean indicating success.
         *              true - map is valid.
         *              false - map is not valid ( program wasn't linked
         *                      successfully ).
         */
        bool mapUniforms(void);

        std::string checkUniform(GLenum type);

        // These are merely set during construction, though utilised during acquisition.
        /// The vertex shader reference.
        VertexShader* m_vertex_shader;

        /// The fragment shader reference.
        FragmentShader* m_fragment_shader;

        /// The AttributeInputAdapter reference.
        AttributeInputAdapter* m_adapter;

        /// Indicator of current linkage state.
        linkageState link_status;

        /// Choice of disposition of shader objects.
        shaderDisposition m_dispose;

        /// The linker log for this shader.
        std::string linker_log;

        /// Map to track the active uniform variables within the OpenGL
        /// program entity. Active means those declared and used within
        /// the shader code. Inactive uniforms are those that are declared
        /// but not otherwise usefully referenced ie. the OpenGL compiler
        /// may ( and probably will ) ignore them and thus discard any
        /// reference to them.
        typedef std::map<std::string, Program::uniform_data> uniformMap;
        uniformMap uniforms;
  };

/**
 * @}
 */

#endif /*PROGRAM_H_*/
