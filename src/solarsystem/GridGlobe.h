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

#include <cstdlib>
#include <iostream>
#include <string>

#include <oglft/OGLFT.h>

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Vector3D.h"
#include "VectorSP.h"
#include "Vertex.h"

class GridGlobe : public Renderable {
   public:
		GridGlobe(vec_t rad, GLuint slices, GLuint stacks);

		virtual ~GridGlobe();

   private:
      /// The displayed size and color parameters of ordinary grid lines
		static const GLfloat GRID_LINE_WIDTH;
  	   static const GLfloat GRID_RED;
	   static const GLfloat GRID_GREEN;
	   static const GLfloat GRID_BLUE;

      static const GLfloat PRIME_MERIDIAN_LINE_WIDTH;
      static const GLfloat PRIME_MERIDIAN_RED;
      static const GLfloat PRIME_MERIDIAN_GREEN;
      static const GLfloat PRIME_MERIDIAN_BLUE;

      static const GLfloat CELESTIAL_EQUATOR_LINE_WIDTH;
      static const GLfloat CELESTIAL_EQUATOR_RED;
      static const GLfloat CELESTIAL_EQUATOR_GREEN;
      static const GLfloat CELESTIAL_EQUATOR_BLUE;

      static const GLuint ARRAY_START;
 	   static const GLsizei ARRAY_STRIDE;
 	   static const GLuint BYTE_OFFSET;
      static const GLuint COORDS_PER_VERTEX;
      static const GLuint VERTICES_PER_LINK;

      static const bool STAGGERING;
      static const bool STITCHING;

      static const GLfloat TEXT_RATIO;
      static const GLfloat TEXT_OTHER_RATIO;
      static const GLfloat TEXT_OFFSET;
      static const GLfloat TEXT_UNITS_RATIO;

      Sphere* sp;

      vec_t radius;
      GLuint slices;
      GLuint stacks;

      bool equator;

      GLuint grid_links;

      GLuint prime_meridian_links;

      GLuint celestial_equator_links;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_points;

      /// The OpenGL buffer object wrapper for grid vertex indices.
      Buffer_OBJ buff_obj_grid_links;

      /// The OpenGL buffer object wrapper for grid vertex indices.
      Buffer_OBJ buff_obj_prime_meridian_links;

      /// The OpenGL buffer object wrapper for grid vertex indices.
      Buffer_OBJ buff_obj_celestial_equator_links;

      void loadVertexBuffer(void);

      void loadGridIndexBuffer(void);

      void loadPrimeMeridianIndexBuffer(void);

      void loadCelestialEquatorIndexBuffer(void);

      void createMarkerLists(void);

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

      GLuint hour_glyph;

      GLuint degree_glyph;

      std::vector<std::vector<GLuint> > marker_lists;
   };

#endif // GRID_GLOBE_H_
