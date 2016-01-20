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

#include "TextString.h"

#include <iostream>

#include "ErrorHandler.h"

TextString::TextString(glm::vec3 position,
	  	  	  	  	   glm::vec3 height_offset,
					   glm::vec3 width_offset,
					   TTF_Font* font,
					   const std::string& text,
					   SDL_Color foreground) :
					      m_position(position),
						  m_height_offset(height_offset),
						  m_width_offset(width_offset),
						  m_font(font),
						  m_text_string(text),
						  m_foreground(foreground) {
	m_texture_buffer = NULL;
    m_textured_parallelogram = NULL;

    m_configure_flag = false;
    }

TextString::~TextString() {
	if(m_texture_buffer) delete m_texture_buffer;
	if(m_textured_parallelogram) delete m_textured_parallelogram;
    }

void TextString::configureTask(void) {
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_text_string.c_str(), m_foreground);

	SDL_PixelFormat format;



	// SDL_Surface* converted = SDL_ConvertSurface(surface, const SDL_PixelFormat* fmt, 0);

//	m_texture_buffer = new TextureBuffer(m_font,
//			  	  	  	  	  	  GLuint bytes,
//								  	  GLsizei width,
//            GLsizei height,
//            GLenum format,
//            GLenum data_type,
//			  GLenum wrap_type_s,
//			  GLenum wrap_type_t,
//            bool mipmaps = true);

	m_textured_parallelogram = new TexturedParallelogram(m_position,
	  	  	  	  	  	 	 	 	 	 	 	 	 	 m_height_offset,
														 m_width_offset,
														 m_texture_buffer);



	}

void TextString::utilise(void) {
	// Ensure it is configured before use.
	if(m_configure_flag == false) {
		configureTask();
		}

	m_textured_parallelogram->utilise();
	}
