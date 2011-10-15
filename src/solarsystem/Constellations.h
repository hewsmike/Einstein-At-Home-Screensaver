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

#ifndef CONSTELLATIONS_H_
#define CONSTELLATIONS_H_

#include <iostream>
#include <string>
#include <vector>

#include "Buffer_OBJ.h"
#include "Constellation.h"
#include "DisplayList.h"
#include "ErrorHandler.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "VectorSP.h"
#include "Vector3D.h"

class Constellations : public Renderable {
	private:
      static const GLfloat OFFSET;
		static const GLfloat LINK_WIDTH;
		static const GLushort LINK_STIPPLE_PATTERN;
		static const GLint LINK_STIPPLE_FACTOR;
		static const GLfloat LINK_RGB_RED;
		static const GLfloat LINK_RGB_GREEN;
		static const GLfloat LINK_RGB_BLUE;

		static const GLfloat MAG_SIZE;

		static const GLuint INDICES_PER_LINK;
		static const GLuint COLORS_PER_VERTEX;
		static const GLuint COORDS_PER_VERTEX;
		static const GLuint BYTE_STRIDE_PER_VERTEX;

      /// Store constellation star data here
      std::vector<Constellation> cons_list;

      /// At what radius from the origin do the constellations lie?
      vec_t c_rad;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_points;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_indices;

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

      /// This is set during object construction, by summing the number
      /// of stars within each constellation.
      GLuint tot_stars;

      /// This is set during object construction, by summing the number
      /// of links within each constellation.
      GLuint tot_links;

      void loadVertexBuffer(void);

      void loadIndexBuffer(void);

      static void class_color(OrdStar::spectral_type spectral_class, GLfloat* colors);

	public:
		Constellations(vec_t rad);

      /**
       * \brief Destructor
       */
		~Constellations();
   };

#endif // CONSTELLATIONS_H_
