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

#ifndef VERTEX_FETCH_
#define VERTEX_FETCH_

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This base class declares public methods to deal with the
 *        OpenGL ES 2.0 pipeline vertex fetch functionality.
 *
 *      As such, this is the 'bare' case where no vertex attributes are
 * supplied to the pipeline ie. any vertex attributes must be generated
 * by a vertex shader.
 *
 * \author Mike Hewson\n
 */

class VertexFetch
    public :
        /**
         * \brief Constructor.
         */
        VertexFetch(void);

        /**
         * \brief Destructor.
         */
        virtual ~VertexFetch();

        /**
         * \brief Perform any data binding to the pipeline input.
         *
         * \return a boolean indicating success of attachment
         *              true - attachment achieved without error
         *              false - attachment was not achieved
         */
        virtual bool attach(void);

        /**
         * \brief Detach any data binding from the pipeline input.
         */
        virtual void detach(void);

        /**
         * \brief Trigger pipeline activity.
         *
         * \param primitive : one of the OpenGL ES 2.0 primitives
         *          GL_POINTS
         *          GL_LINE_STRIP
         *          GL_LINE_LOOP
         *          GL_LINES
         *          GL_TRIANGLE_STRIP
         *          GL_TRIANGLE_FAN
         *          GL_TRIANGLES
         * \param count : how many times to invoke the vertex shader.
         */
        virtual void trigger(GLenum primitive, GLsizei count);
    };

/**
 * @}
 */

#endif /*VERTEX_FETCH_*/
