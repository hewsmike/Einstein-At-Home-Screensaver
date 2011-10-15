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

#include "Sphere.h"

/// Minimal sphere has radius one, two poles, and
/// an equatorial equilateral triangle.
const GLuint Sphere::MIN_SLICES(3);
const GLuint Sphere::MIN_STACKS(3);
const GLfloat Sphere::MIN_RADIUS(1.0f);

Sphere::Sphere(vec_t radius  = Sphere::MIN_RADIUS,
               GLuint slices = Sphere::MIN_SLICES,
               GLuint stacks = Sphere::MIN_STACKS,
               Sphere::stagger stag = Sphere::NO_STAGGER)
               : rad(radius), num_slices(slices),
                 num_stacks(stacks), st(stag) {
   /// Some bounds checking.
   if((rad < Sphere::MIN_RADIUS) || (num_slices < Sphere::MIN_SLICES) ||
      (num_stacks < Sphere::MIN_STACKS)) {
      /// FATAL if lousy arguments ....
      std::string msg = "Sphere::Sphere() - bad construction arguments : ";
      msg += "radius = ";
      msg += rad;
      msg += " ( minimum = ";
      msg += Sphere::MIN_RADIUS;
      msg += " )";
      msg += "\t slices = ";
      msg += num_slices;
      msg += " ( minimum = ";
      msg += Sphere::MIN_SLICES;
      msg += " )";
      msg += "\tstacks = ";
      msg += num_stacks;
      msg += " ( minimum = ";
      msg += Sphere::MIN_STACKS;
      msg += " )";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   /// The jump in latitude between each stack.
   GLfloat stack_step = SolarSystemGlobals::HALF_CIRCLE_DEG/(num_stacks - 1);

   /// The jump in longitude between each slice.
   GLfloat slice_step = SolarSystemGlobals::FULL_CIRCLE_DEG/num_slices;

   /// By what amount, if any, do we stagger alternate stacks with
   /// respect to longitudinal values ?
   GLfloat long_stag_step = (st == Sphere::NO_STAGGER) ? 0.0f : slice_step/2.0f;

   /// The jump in the 'horizontal' texture coordinate
   /// between adjacent vertices at a given latitude.
   GLfloat s_step = 1.0f/num_slices;

   /// The jump in the 'vertical' texture coordinate
   /// between adjacent vertices at a given longitude.
   GLfloat t_step = 1.0f/(num_stacks - 1);

   /// By what amount, if any, do we stagger alternate stacks with
   /// respect to horizontal texture values ?
   GLfloat text_stag_step = (st == Sphere::NO_STAGGER) ? 0.0f : s_step/2.0f;

   // Now can make the vertex list.
   for(unsigned int stack = 0; stack < num_stacks; stack++) {
      /// The latitude of points at this stack, noting that the north pole is set as
      /// positive 90 degrees or one quarter of a full circle. For every
      /// latitude band below the north pole subtract the step in latitude.
      vec_t latit = SolarSystemGlobals::QUARTER_CIRCLE_DEG - stack*stack_step;

      unsigned int slice_count = num_slices;
      /// If it's the first or the last stack then you're at a pole.
      if((stack == 0) || (stack == (num_stacks - 1))) {
         slice_count = 1;
         }

      /// The 'vertical' texture coordinate for this value of latitude.
      GLfloat lat_text = stack * t_step;

      /// A series of vertices for each longitude, but an extra one
      /// for the initial longitude with a texture coordinate of 1.0f
      for(unsigned int slice = 0; slice < slice_count; slice++) {
         /// By construction ( with a radius of 1 ) then 'point' and
         /// thus 'normal' will be unit vectors.

         /// The longitude for this point, perhaps staggered and
         /// allowing modulo 360 degrees.
         vec_t longit = (slice * slice_step + long_stag_step * (stack % 2));

         if(longit >= SolarSystemGlobals::FULL_CIRCLE_DEG) {
            longit -= SolarSystemGlobals::FULL_CIRCLE_DEG;
            }

         /// Make a vector to the point at radius one with those longitude
         /// and latitude values, in spherical polar terms.
         VectorSP point = VectorSP(longit, latit, 1);

         /// Make a vector to that same point, Cartesian coordinate equivalents.
         Vector3D normal = Vector3D(point.x(), point.y(), point.z());

         /// Now project that normal vector out to the desired radius.
         /// Evidently I'm setting the normal to a point upon the surface of
         /// the sphere in line with the radial vector from the origin to said
         /// point where the normal lies.
         /// This is appropriate for a sphere ..... :-)
         Vector3D position = Vector3D(rad*point.x(), rad*point.y(), rad*point.z());

         /// The 'horizontal' texture coordinate for this value of longitude,
         /// also possibly staggered and maybe giving a value > 1.0f.
         GLfloat long_text = slice * s_step + text_stag_step * (stack % 2);

         /// Create a vertex with the given position, normal and texture coordinates.
         Vertex current = Vertex(position,
                                 normal,
                                 std::pair<GLfloat, GLfloat>(long_text, lat_text));

         /// Save that vertex to our listing.
         verts.push_back(current);
         }
      }
  	}

Sphere::~Sphere() {
	}

const std::vector<Vertex>& Sphere::vertices(void) const {
   return verts;
   }

GLfloat Sphere::radius(void) const {
   return rad;
   }

GLuint Sphere::slices(void) const {
   return num_slices;
   }

GLuint Sphere::stacks(void) const {
   return num_stacks;
   }

bool Sphere::isStaggered(void) const {
   return (st == Sphere::NO_STAGGER) ? false : true;
   }
