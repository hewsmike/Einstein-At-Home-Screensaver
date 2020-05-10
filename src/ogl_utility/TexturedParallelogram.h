/***************************************************************************
 *   Copyright (C) 2020 by Mike Hewson                                     *
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
 * \brief This class models a four vertex convex parallel side
 *        planar area onto which a given texture is applied and
 *        to be placed using orthographic projection.
 *
 * \see TextureBuffer
 *
 * \author Mike Hewson\n
 */

class TexturedParallelogram : public Configurable {
    public :
        /**
         * \brief Constructor for 3D perspective rendering
         *
         * \param position - a vector to the point on the lower left corner
         *                   of the parallelogram.
         * \param width_offset - a vector directed to the lower right corner
         *                       of the parallelogram, from the lower left.
         * \param height_offset - a vector directed to the upper left corner
         *                        of the parallelogram, from the lower left.
         * \param t_group - a RenderTask::texture_buffer_group structure instance.
         */
        TexturedParallelogram(glm::vec3 position,
                              glm::vec3 width_offset,
                              glm::vec3 height_offset,
                              RenderTask::texture_buffer_group t_group);

        /**
         * \brief Constructor for 2D orthographic rendering
         *
         * \param position - a vector to the point on the lower left corner
         *                   of the parallelogram.
         * \param width_offset - a vector directed to the lower right corner
         *                       of the parallelogram, from the lower left.
         * \param height_offset - a vector directed to the upper left corner
         *                        of the parallelogram, from the lower left.
         * \param t_group - a RenderTask::texture_buffer_group structure instance.
         */
        TexturedParallelogram(glm::vec2 position,
                              glm::vec2 width_offset,
                              glm::vec2 height_offset,
                              RenderTask::texture_buffer_group t_group);

        /**
         * \brief Destructor
         */
        virtual ~TexturedParallelogram();

        /**
         * \brief Get the lower left parallelogram corner.
         *
         * \return the lower left parallelogram corner.
         */
        glm::vec3 position(void);

        /**
         * \brief Get the 'vertical' offset from the lower left parallelogram corner.
         *
         * \return the offset from lower left parallelogram corner.
         */
        glm::vec3 heightOffset(void);

        /**
         * \brief Get the 'horizontal' offset from the lower left parallelogram corner.
         *
         * \return the offset from lower left parallelogram corner.
         */
        glm::vec3 widthOffset(void);

        /**
         * \brief Gets the flag idicating whether it renders in 3D space or on the HUD.
         *
         * \return the flag indicating true or false.
         */
        bool isThreeDimensional(void);

        /**
         * \brief Actually configure any underlying object(s).
         *
         * \return a boolean indicating success of configuration
         *              - true, the configuration as successful.
         *              - false, the configuration was not successful.
         */
        virtual bool configure(void);

        /**
         * \brief Invoke rendering activity.
         */
        void utilise(void);

   private :
        static const GLuint VERTEX_COUNT;

        /// The shader code for this type of texturing.
        static const std::string m_vertex_shader_3D;
        static const std::string m_fragment_shader;

        /// Positioning vectors for vertices.
        glm::vec3 m_position;
        glm::vec3 m_height_offset;
        glm::vec3 m_width_offset;

        // Whether the parallelogram is rendered in 3D or flat on HUD.
        bool is3D;

        // The texture buffer details.
        RenderTask::texture_buffer_group m_t_group;

        /// The rendering task to enact the texturing.
        RenderTask* m_render_task;
    };

/**
 * @}
 */

#endif /*TEXTURED_PARALLELOGRAM_H_*/
