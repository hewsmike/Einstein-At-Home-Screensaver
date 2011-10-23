/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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
#ifndef EARTH_H_
#define EARTH_H_

#include <iostream>
#include <vector>

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Sphere.h"
#include "SolarSystemGlobals.h"
#include "Texture_OBJ.h"

class Earth : public Renderable {
   public:
      // How big is the Earth ?
      static const GLuint EARTH_RADIUS;

      Earth(void);

		virtual ~Earth();

	private:
      static const GLfloat EARTH_GRID_RADIUS;
      static const GLuint EARTH_GRID_SLICES;
      static const GLuint EARTH_GRID_STACKS;
      static const char* TEXTURE_FILE_NAME;

      static const GLuint POINT_COORDS_PER_VERTEX;
      static const GLuint NORM_COORDS_PER_VERTEX;
      static const GLuint TEXTURE_COORDS_PER_VERTEX;
      static const GLuint ELEMENTS_PER_VERTEX;

      static const GLuint VERTICES_PER_LATITUDE;

      static const GLuint ARRAY_START;
 	   static const GLsizei ARRAY_STRIDE;

      static const GLfloat GREENWICH_TEXTURE_OFFSET;

      /// The buffer object for the northern polar cap.
 	   Buffer_OBJ north_cap_indices;

      /// The buffer object for waist rendering.
      Buffer_OBJ waist_buffer;

      /// The buffer object for the southern polar cap.
      Buffer_OBJ south_cap_indices;

      Sphere sp;
      SDL_Surface* surface;

      GLenum texture_format;
      GLint nOfColors;

      Buffer_OBJ buff_obj_points;

      Texture_OBJ texture;

      bool loadImage(void);
      void loadTexture(void);

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

      void loadVertexBuffer(void);
      void vertex2buffer(const Vertex& vert, vec_t* buffer) const;

      void loadIndexBuffers(void);
      void loadNorthBuffer(void);
      void loadWaistBuffer(void);
      void loadSouthBuffer(void);
	};

#endif // EARTH_H_
