/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "ogl_utility.h"

#include "Program.h"
#include "TextureBuffer.h"
#include "VertexFetch.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with Pipeline
 *        objects. This encompasses a given pipeline state ie. a specified
 *        OpenGL program with it's vertex fetch capability, and possibly
 *        a texture.
 *
 * \see Program
 * \see TextureBuffer
 * \see VertexFetch
 *
 * \author Mike Hewson\n
 */

class Pipeline {
    public :
        /**
         * \brief Constructor.
         *
         * \param program : pointer to an existing Program object.
         * \param vertex_fetch : pointer to an existing VertexFetch object.
         * \param texture : pointer to an existing TextureBuffer object, which
         *                  may be NULL ( default ) if no texturing is desired.
         */
        Pipeline(Program* program,
                 VertexFetch* vertex_fetch,
                 TextureBuffer* texture = NULL);

        /**
         * \brief Destructor.
         */
        virtual ~Pipeline();

        /**
         * \brief Trigger pipeline activity. It is the responsibility
         *        of the calling routine to ensure that parameter choices
         *        do not cause an over-run of the supplied buffer. NB
         *        GL_QUADS is not available with OpenGL 3.x and above.
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

    private:
        /// The Program reference.
        Program* m_program;

        /// The VertexFetch reference.
        VertexFetch* m_vertex_fetch;

        /// The TextureBuffer reference.
        TextureBuffer* m_texture_buffer;
    };

/**
 * @}
 */

#endif /*PIPELINE_H_*/
