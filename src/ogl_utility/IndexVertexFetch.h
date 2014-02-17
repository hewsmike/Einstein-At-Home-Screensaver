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

#ifndef INDEX_VERTEX_FETCH_
#define INDEX_VERTEX_FETCH_

#include "Buffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class declares public methods to deal with the OpenGL ES 2.0
 *        pipeline vertex fetch functionality using buffers.
 *
 *      This is the case where vertex attributes are supplied to the pipeline
 * sequentially from a buffer.
 *
 * \author Mike Hewson\n
 */

class IndexVertexFetch
    public :
        /**
         * \brief Constructor.
         */
        IndexVertexFetch(void);

        /**
         * \brief Destructor.
         */
        virtual ~IndexVertexFetch();

        /**
         * \brief Perform any data binding to the pipeline input.
         *
         * \param vertex_buffer : the ogl_utility buffer object containing
         *                        the vertex data.
         * \param index_buffer : the ogl_utility buffer object containing
         *                       the indices into the vertex_buffer.
         */
        void attach(const Buffer& vertex_buffer,
                    const Buffer& index_buffer);

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
        void trigger(GLenum primitive, GLsizei count);
    };

/**
 * @}
 */

#endif /*INDEX_VERTEX_FETCH_*/

