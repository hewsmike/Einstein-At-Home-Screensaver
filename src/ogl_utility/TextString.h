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
        /// The rendering style.
        /// SOLID :     Quick and dirty, tranparent background & opaque
        ///             foreground. No surrounding parallelogram apparent.
        /// SHADED :    Not as fast as SOLID, pixels graded from background to
        ///             foreground. Text appears as within a visible
        ///             parallellogram shape.
        /// BLENDED :   Slowest of all but best quality. Alpha blending
        ///             dithers and anti-aliases adjacent pixels. No
        ///             surrounding parallelogram apparent.
        enum render_style {SOLID,
                           SHADED,
                           BLENDED};

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
		 * \param text - an ASCIIZ string containing the text message to render.
		 * \param text_style - one of the the render_style enumerants
		 * \param foreground - an SDL_Color specification for the foreground.
		 * \param background - an SDL_Color specification for the background.
		 *                     But this is ONLY RELEVANT for the SHADED
		 *                     rendering style, and the value is ignored for
		 *                     either SOLID or BLENDED. It has a default
		 *                     argument of {0, 0, 0, 0} ie. transparent
		 *                     black if not provided.
		 */
		TextString(glm::vec3 position,
				   glm::vec3 height_offset,
				   glm::vec3 width_offset,
				   TTF_Font* font,
				   const char* text,
				   TextString::render_style text_style,
				   SDL_Color foreground,
				   SDL_Color background = {0, 0, 0, 0});

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

        /// The font type to use.
        TTF_Font* m_font;

        /// The text string to be displayed.
        const char* m_text_string;

        /// The choice of rendering style.
        TextString::render_style m_text_style;

        /// The color for the text foreground.
        SDL_Color m_foreground;

        /// The color for the text background.
        SDL_Color m_background;

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
