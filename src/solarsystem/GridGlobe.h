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

#ifndef GRID_GLOBE_H_
#define GRID_GLOBE_H_

#include <iostream>

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Vertex.h"

class GridGlobe : public Renderable {
   private:
      /// The displayed size and color parameters of grid lines
		static const GLfloat LINE_WIDTH;
  	   static const GLfloat RGB_RED;
	   static const GLfloat RGB_GREEN;
	   static const GLfloat RGB_BLUE;

      static const GLuint ARRAY_START;
 	   static const GLsizei ARRAY_STRIDE;
 	   static const GLuint BYTE_OFFSET;
      static const GLuint COORDS_PER_VERTEX;
      static const GLuint VERTICES_PER_LINK;

      int total_links;

      Sphere sp;

      unsigned int slices;
      unsigned int stacks;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_points;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_links;

      void loadVertexBuffer(void);

      void loadIndexBuffer(void);

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

	public:
		GridGlobe(vec_t rad, GLuint slices, GLuint stacks);

		virtual ~GridGlobe();
	};

#endif // GRID_GLOBE_H_
