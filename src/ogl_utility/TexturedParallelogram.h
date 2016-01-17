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
#include "TextureBuffer.h"

#include <string>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class models a four vertex convex parallel
 * 		  sided planar area onto which a given texture is applied.
 *
 * \see TextureBuffer
 *
 * \author Mike Hewson\n
 */

class TexturedParallelogram {
    public :
		/**
         * \brief Constructor
         *
         * \param position - a vector to the point on the lower left corner
         * 					 of the parallelogram.
         * \param height_offset - a vector directed to the upper left corner
         * 						  of the parallelogram, from the lower left.
         * \param width_offset - a vector directed to the lower right corner
         * 						 of the parallelogram, from the lower left.
         * \param texture - a pointer to a TextureBuffer object.
         */
		TexturedParallelogram(glm::vec3 position,
						  	  glm::vec3 height_offset,
							  glm::vec3 width_offset,
							  TextureBuffer* texture);

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
         * \param texture - an existing TextureBuffer to apply to the parallelogram.
         */
        void setTexture(TextureBuffer* texture);

        /**
         * \brief Invoke rendering activity.
         */
        void utilise(void);

    private :
        /// The shader code for this type of texturing.
        static const std::string m_vertex_shader;
        static const std::string m_fragment_shader;

        /// Positioning vectors for vertices.
        glm::vec3 m_position;
        glm::vec3 m_height_offset;
        glm::vec3 m_width_offset;

        /// The texture to be applied.
        TextureBuffer* m_texture;

        /// The rendering task to enact the texturing.
        RenderTask* m_render_task;

        /// Create the rendering task.
        void configureTask(void);
    };

/**
 * @}
 */

#endif /*TEXTURED_PARALLELOGRAM_H_*/