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
#include <sstream>

#include "ErrorHandler.h"

TextString::TextString(glm::vec3 position,
	  	  	  	  	   glm::vec3 height_offset,
					   glm::vec3 width_offset,
					   TTF_Font* font,
					   const char* text,
					   TextString::render_style text_style,
                       SDL_Color foreground,
				       SDL_Color background) :
					      m_position(position),
						  m_height_offset(height_offset),
						  m_width_offset(width_offset),
						  m_font(font),
						  m_text_string(text),
                          m_text_style(text_style),
						  m_foreground(foreground),
						  m_background(background) {
	m_texture_buffer = NULL;
    m_textured_parallelogram = NULL;
    m_configure_flag = false;
    m_mode = TexturedParallelogram::VOLUME;
    }

TextString::TextString(glm::vec2 position,
	  	  	  	  	   glm::vec2 height_offset,
					   glm::vec2 width_offset,
					   TTF_Font* font,
					   const char* text,
					   TextString::render_style text_style,
                       SDL_Color foreground,
				       SDL_Color background) :
			TextString::TextString(glm::vec3(position.x, position.y, 0),
								   glm::vec3(height_offset.x, height_offset.y, 0),
                                   glm::vec3(width_offset.x, width_offset.y, 0),
                                   font,
			                       text,
			                       text_style,
                                   foreground,
			                       background) {
	m_mode = TexturedParallelogram::FLAT;
	}

TextString::~TextString() {
	if(m_texture_buffer) delete m_texture_buffer;
	if(m_textured_parallelogram) delete m_textured_parallelogram;
    }

void TextString::configureTask(void) {
    // Get SDL_ttf to construct an SDL_Surface with the embedded glyph patterns.
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_text_string, m_foreground);
	if(surface == NULL) {
        ErrorHandler::record("TextString::configureTask() : NULL return for RenderText !", ErrorHandler::FATAL);
        }

    // Convert that surface to another with the RGBA format @ 8 bits per color plus alpha.
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    if(converted == NULL) {
        ErrorHandler::record("TextString::configureTask() : NULL return for ConvertSurfaceFormat !", ErrorHandler::FATAL);
        }

    // Create a TextureBuffer object
    m_texture_buffer = new TextureBuffer(converted->pixels,
                                         GLuint(converted->pitch) * GLuint(converted->h),
                                         GLsizei(converted->w),
                                         GLsizei(converted->h),
                                         GL_RGBA,
                                         GL_UNSIGNED_BYTE,
                                         GL_CLAMP,
                                         GL_CLAMP,
                                         true);

    m_texture_buffer->acquire();

    m_textured_parallelogram = new TexturedParallelogram(m_position,
	  	  	  	  	  	 	 	 	 	 	 	 	 	 m_height_offset,
														 m_width_offset,
														 m_texture_buffer);

    // With the SDL_ttf library the caller must free the surface when done.
    SDL_FreeSurface(surface);
    SDL_FreeSurface(converted);

    m_configure_flag = true;
    }

void TextString::utilise(void) {
	// Ensure it is configured before use.
	if(m_configure_flag == false) {
		configureTask();
		}

	m_textured_parallelogram->utilise();
	}
