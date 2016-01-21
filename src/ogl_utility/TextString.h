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

#ifndef TEXT_STRING_H_
#define TEXT_STRING_H_

#include "ogl_utility.h"

#include "TexturedParallelogram.h"

#include <string>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class models a character string displayed by texturing
 * 		  onto a parallelogram.
 *
 * \see TexturedParallelogram
 *
 * \author Mike Hewson\n
 */

class TextString {
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
		 * \param font - a pointer to an existing TTF_Font to render with.
		 * \param text - an std::string containing the text message to render.
		 * \param foreground - an SDL_Color specification for the foreground.
         */
		TextString(glm::vec3 position,
				   glm::vec3 height_offset,
				   glm::vec3 width_offset,
				   TTF_Font* font,
				   const std::string& text,
				   SDL_Color foreground);

        /**
         * \brief Destructor
         */
        virtual ~TextString();

        /**
         * \brief Invoke rendering activity.
         */
        void utilise(void);

    private :
        /// Positioning vectors for vertices.
        glm::vec3 m_position;
        glm::vec3 m_height_offset;
        glm::vec3 m_width_offset;

        /// The text string to be displayed.
        const std::string& m_text_string;

        /// The color for the text foreground.
        SDL_Color m_foreground;

        /// The font type to use.
        TTF_Font* m_font;

        /// The texture to be applied.
        TextureBuffer* m_texture_buffer;

        /// The parallelogram to be used for rendering.
        TexturedParallelogram* m_textured_parallelogram;

        ///Has this object been configured ?
        bool m_configure_flag;

        /// Create the rendering task.
        void configureTask(void);
    };

/**
 * @}
 */

#endif /*TEXT_STRING_H_*/
