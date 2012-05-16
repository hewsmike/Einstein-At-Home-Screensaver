/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#ifndef GRID_GLOBE_H_
#define GRID_GLOBE_H_

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Vector3D.h"

/**
* \addtogroup solarsystem Solarsystem
* @{
*/

/**
* \brief A renderable object of spherical shape with texture, to be
* instantiated in detail for the Earth, the Sun etc
*
* \author Mike Hewson\n
*/

class GridGlobe : public Renderable {
   public:
      enum lineType {MAIN, EQUATOR, PRIME_MERIDIAN};

      enum textFacing{INSIDE, OUTSIDE};

      GridGlobe(vec_t rad, GLuint slices, GLuint stacks, GridGlobe::textFacing tf);

      virtual ~GridGlobe();

      /**
       * \brief Cycles the activation state of the object
       */
      virtual void cycleActivation(void);

      void setLine(lineType type,
                   GLfloat width,
                   GLfloat red,
                   GLfloat green,
                   GLfloat blue);

   private:
      enum state {ALL_OFF, GRID, ALL_ON};

      static const state INITIAL_CYCLE_STATE;

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

      struct Vert {
      	vec_t x_pos;
         vec_t y_pos;
         vec_t z_pos;
         };

      struct Line {
         GLfloat width;
         GLfloat red;
         GLfloat green;
         GLfloat blue;
         };

      Line main;
      Line equator;
      Line prime_meridian;

      Sphere* sp;

      vec_t radius;
      GLuint slices;
      GLuint stacks;

      bool hasEquator;

      bool textInside;

      state current_cycle_state;

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

      /**
       * \brief Clears the OpenGL display lists for the grid marking symbols.
       *
       * Releases OpenGL display list resources associated with the grid markers,
       * and empties the marker_lists STL container. Used when cycling the display
       * components and overall resource release with inactivation.
       */
      void clearMarkerLists(void);

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

/**
* @}
*/

#endif // GRID_GLOBE_H_
