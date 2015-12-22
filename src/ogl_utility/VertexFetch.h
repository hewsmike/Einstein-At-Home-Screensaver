/***************************************************************************
 *   Copyright (C) 2015 by Mike Hewson                                     *
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

#ifndef VERTEX_FETCH_
#define VERTEX_FETCH_

#include "ogl_utility.h"

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This base class declares public methods to deal with the
 *        OpenGL pipeline vertex fetch functionality ie.
 *        this wraps Vertex Array Objects (VAO's).
 *
 *  Use this class when all vertex attributes are assumed to be supplied
 *  within a vertex shader.
 *
 * \see Bound
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class VertexFetch : public OGL_ID, public Bound {
    public :
		/**
         * \brief Constructor.
         */
        VertexFetch();

        /**
         * \brief Destructor.
         */
        virtual ~VertexFetch();

        /**
		 * \brief Obtain the OpenGL resource.
		 *
		 * \return a boolean indicating success of acquisition
		 *              TRUE - resources acquired without error
		 *              FALSE - resources were not acquired
		 */
		bool acquire(void);

		/**
		 * \brief Release the OpenGL resource.
		 */
		void release(void);

        /**
         * \brief Perform any data binding to the pipeline input.
         */
        void bind(void);

        /**
         * \brief Remove any data binding to the pipeline input.
         *
         *      HIGHLY ADVISED to invoke after rendering and before any
         * other pipeline activity, in order to properly reset the vertex
         * fetching state.
         */
        void unbind(void);

        /**
         * \brief Trigger pipeline activity. Attachment occurs automatically
         *        if not already performed.
         *
         * \param primitive : one of the OpenGL primitives
         *          GL_POINTS
         *          GL_LINE_STRIP
         *          GL_LINE_LOOP
         *          GL_LINES
         *          GL_TRIANGLE_STRIP
         *          GL_TRIANGLE_FAN
         *          GL_TRIANGLES
         * \param count : how many times to invoke the vertex shader.
         */
        void trigger(GLenum primitive, GLsizei count);

        /**
		 * \brief Is the underlying vertex array object ( VAO ) bound to the
		 * 	      state machine ?
		 *
		 * \return a boolean indicating binding
		 *              TRUE - VAO is bound
		 *              FALSE - VAO is not bound
		 */
        bool isBound(void) const;

    private :
    };

/**
 * @}
 */

#endif /*VERTEX_FETCH_*/
