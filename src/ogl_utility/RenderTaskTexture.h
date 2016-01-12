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

#ifndef RENDER_TASK_TEXTURE_H_
#define RENDER_TASK_TEXTURE_H_

#include "ogl_utility.h"

#include "RenderTask.h"
#include "TextureBuffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with an entire rendering task
 * 		  that involves a texture.
 *        That being a specific collection of ogl_utility objects to render an entire vertex
 *        buffer data set, with or without indexing.
 *
 *        The minimum to be provided is :
 *          - requirements for a RenderTask.
 *          - a texture buffer object.
 *
 * \see RenderTask
 * \see TextureBuffer
 *
 * \author Mike Hewson\n
 */

class RenderTaskTexture : public RenderTask {
    public :
		/**
         * \brief Constructor.
         *
         * \param s_group : a RenderTask::shader_group structure that specifies the key parameters
         *                  to construct Shader objects for this rendering task.
         * \param i_group : a RenderTask::index_buffer_group structure that specifies the key parameters
         *                  to possibly construct an IndexBuffer object for this rendering task.
         * \param v_group : a Render::Task vertex_buffer_group structure that specifies the key parameters
         *                  to construct a VertexBuffer object for this rendering task.
         * \param texture_buffer : a pointer to an existing TextureBuffer instance.
         */
        RenderTaskTexture(RenderTask::shader_group s_group,
                   	   	  RenderTask::index_buffer_group i_group,
						  RenderTask::vertex_buffer_group v_group,
						  TextureBuffer* texture_buffer);

        /**
         * \brief Destructor.
         */
        virtual ~RenderTaskTexture();

    private:
        TextureBuffer* m_texture_buffer;
    };

/**
 * @}
 */

#endif /*RENDER_TASK_TEXTURE_H_*/
