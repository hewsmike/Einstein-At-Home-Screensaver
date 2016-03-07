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

#ifndef TEXTURED_PARALLELOGRAM_H_
#define TEXTURED_PARALLELOGRAM_H_

#include "ogl_utility.h"

#include "RenderTask.h"

#include <string>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class models a four vertex convex parallel
 *           sided planar area onto which a given texture is applied.
 *
 * \see TextureBuffer
 *
 * \author Mike Hewson\n
 */

class TexturedParallelogram {
    public :
        /// The rendering mode.
        /// FLAT :  render onto the frustum near face.
        /// VOLUME : render into the world space volume.
        enum render_mode{FLAT, VOLUME};

        /**
         * \brief Constructor for 3D perspective rendering
         *
         * \param position - a vector to the point on the lower left corner
         *                   of the parallelogram.
         * \param height_offset - a vector directed to the upper left corner
         *                        of the parallelogram, from the lower left.
         * \param width_offset - a vector directed to the lower right corner
         *                       of the parallelogram, from the lower left.
         * \param t_group - a RenderTask::texture_buffer_group structure instance.
         */
        TexturedParallelogram(glm::vec3 position,
                              glm::vec3 height_offset,
                              glm::vec3 width_offset,
                              RenderTask::texture_buffer_group t_group);

        /**
         * \brief Constructor for 2D orthographic rendering
         *
         * \param position - a vector to the point on the lower left corner
         *                   of the parallelogram.
         * \param height_offset - a vector directed to the upper left corner
         *                        of the parallelogram, from the lower left.
         * \param width_offset - a vector directed to the lower right corner
         *                       of the parallelogram, from the lower left.
         * \param t_group - a RenderTask::texture_buffer_group structure instance.
         */
        TexturedParallelogram(glm::vec2 position,
                              glm::vec2 height_offset,
                              glm::vec2 width_offset,
							  RenderTask::texture_buffer_group t_group);

        /**
         * \brief Destructor
         */
        virtual ~TexturedParallelogram();

        /**
         * \brief Set the lower left parallelogram corner.
         *
         * \param position - the lower left parallelogram corner.
         */
        void setPosition(glm::vec3 position);

        /**
         * \brief Set the 'vertical' offset from the lower left parallelogram corner.
         *
         * \param height_offset - the offset from lower left parallelogram corner.
         */
        void setHeightOffset(glm::vec3 height_offset);

        /**
         * \brief Set the 'horizontal' offset from the lower left parallelogram corner.
         *
         * \param width_offset - the offset from lower left parallelogram corner.
         */
        void setWidthOffset(glm::vec3 width_offset);

        /**
         * \brief Set the texture to be applied to the parallelogram.
         *
         * \param t_group - a RenderTask::texture_buffer_group structure instance.
         */
        void setTexture(RenderTask::texture_buffer_group t_group);

        /**
         * \brief Invoke rendering activity.
         */
        void utilise(void);

    private :
        static const GLuint VERTEX_COUNT;

        /// The shader code for this type of texturing.
        static const std::string m_vertex_shader_2D;
        static const std::string m_vertex_shader_3D;
        static const std::string m_fragment_shader;

        /// Flag to indicate if this object has been configured.
        bool m_configure_flag;

        /// Positioning vectors for vertices.
        glm::vec3 m_position;
        glm::vec3 m_height_offset;
        glm::vec3 m_width_offset;

        RenderTask::texture_buffer_group m_t_group;

        TexturedParallelogram::render_mode m_render_mode;

        /// The rendering task to enact the texturing.
        RenderTask* m_render_task;

        /// Create the rendering task.
        void configureTask(void);
    };

/**
 * @}
 */

#endif /*TEXTURED_PARALLELOGRAM_H_*/
