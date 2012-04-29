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

// Default/minimal sphere has radius one, two poles, and
// an equatorial equilateral triangle.
// This implementation REQUIRES the following constants as is.
// Plagues, Armageddon and Nasal Daemons to visit upon anyone
// who dares alter them .... :-)
const GLfloat Sphere::MIN_RADIUS(1.0f);
const GLuint Sphere::MIN_SLICES(3);
const GLuint Sphere::MIN_STACKS(3);

// Default is neither staggered nor stitched.
Sphere::Sphere(vec_t radius = MIN_RADIUS,
               GLuint slices = MIN_SLICES,
               GLuint stacks = MIN_STACKS,
               bool stagger = false,
               bool stitch = false)
               : rad(radius),
                 num_slices(slices),
                 num_stacks(stacks),
                 stagger_flag(stagger),
                 stitch_flag(stitch) {
   // Bounds check and force minimum values ... without report.
   if(rad < MIN_RADIUS) {
      rad = MIN_RADIUS;
      }

   if(num_slices < MIN_SLICES) {
      num_slices = MIN_SLICES;
      }

   if(num_stacks < MIN_STACKS) {
      num_stacks = MIN_STACKS;
      }

   makeVertices();
   makeStackIndices();
   makeSliceIndices();
   }

Sphere::~Sphere() {
	}

const std::vector<Vertex>& Sphere::vertices(void) const {
   return verts;
   }

const std::vector<std::vector<GLuint> >& Sphere::sliceIndices(void) const {
   return slice_indices;
   }

const std::vector<std::vector<GLuint> >& Sphere::stackIndices(void) const {
   return stack_indices;
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
   return stagger_flag;
   }

bool Sphere::isStitched(void) const {
   return stitch_flag;
   }

void Sphere::makeVertices(void) {
   // The first and last stacks are entirely consist of the poles by
   // themselves. Creation starts from the north pole stack, and for each
   // stack begin from it's 'prime' meridian and increase eastwards.

   // The degree jump in latitude between each stack.
   GLfloat stack_step = SolarSystemGlobals::HALF_CIRCLE_DEG/(num_stacks - 1);

   // The degree jump in longitude between each slice.
   GLfloat slice_step = SolarSystemGlobals::FULL_CIRCLE_DEG/num_slices;

   // By what degree amount, if any, do we stagger alternate
   // stacks with respect to longitudinal values ?
   GLfloat long_stag_step = stagger_flag ? (slice_step/2.0f) : 0.0f;

   // The jump in the longitudinal texture coordinate values
   // between adjacent vertices at a given latitude.
   GLfloat s_step = 1.0f/num_slices;

   // The jump in the latitudinal texture coordinate values
   // between adjacent vertices at a given longitude.
   GLfloat t_step = 1.0f/(num_stacks - 1);

   // By what amount, if any, do we stagger alternate stacks with
   // respect to longitudinal texture values ?
   GLfloat text_stag_step = stagger_flag ? (s_step/2.0f) : 0.0f;

   // Now can make the vertex listing. We do this by latitude,
   // and by each longitude within a latitude stack.
   for(GLuint stack = 0; stack < num_stacks; ++stack) {
      // The latitude of points in this stack, noting that the north pole is
      // set as positive 90 degrees or one quarter of a full circle. For every
      // latitude band below the north pole subtract the step in latitude.
      vec_t latit = SolarSystemGlobals::QUARTER_CIRCLE_DEG - stack * stack_step;

      GLuint stack_vertex_count = num_slices + (stitch_flag ? 1 : 0);

      // If it's a pole stack, then there's only one point in it
      // and no stitching regardless of stitch setting.
      if((stack == 0) ||
         (stack == num_stacks - 1)) {
         stack_vertex_count = 1;
         }

      // The latitudinal texture coordinate value for this stack.
      GLfloat lat_text_coord = stack * t_step;

      // Now the vertices for each longitude.
      for(GLuint slice = 0; slice < stack_vertex_count; ++slice) {
         // The longitude for this point, perhaps
         // staggering even-indexed stacks.
         vec_t longit = slice * slice_step + long_stag_step * (stack % 2);

         // Allowing for modulo 360 degrees.
         if(longit >= SolarSystemGlobals::FULL_CIRCLE_DEG) {
            longit -= SolarSystemGlobals::FULL_CIRCLE_DEG;
            }

         // Make a unit vector in the direction
         // of the given longitude and latitude.
         VectorSP normal = VectorSP(longit, latit, 1);

         // Now project that normal vector out to the desired radius.
         Vector3D position = rad * normal;

         // The longitudinal texture coordinate for this slice,
         // also possibly staggered on even-indexed stacks.
         // NB This value may exceed 1.0 in which case other code
         // will need to invoke :
         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
         // to get the wrap looking right.
         GLfloat long_text_coord = slice * s_step + text_stag_step * (stack % 2);

         // Create a vertex with the given position, normal and texture coordinates.
         Vertex current = Vertex(position, normal,
                                 std::pair<GLfloat,
                                 GLfloat>(long_text_coord, lat_text_coord));

         /// Save that vertex to our listing.
         verts.push_back(current);
         }
      }
   }

void Sphere::makeStackIndices(void) {
   // Holder for indices in a given stack.
   std::vector<GLuint> current_stack;

   // For the first stack, just enter the north pole index in our holder
   current_stack.push_back(0);
   // and copy/store the holder for this stack.
   stack_indices.push_back(current_stack);

   // Adjust count of stack indices for presence of any stitch ( used in subclasses )
   GLuint indices_per_stack = num_slices + (stitch_flag ? 1 : 0);

   // For neither the first nor the last stack ie. non-polar points
   for(GLuint stack = 1; stack < num_stacks - 1; ++stack) {
      // Empty the index holder.
      current_stack.clear();
      // Put all the longitude indices in ....
      for(GLuint slice = 0; slice < indices_per_stack; ++slice) {
         current_stack.push_back(1 +                                 // move to first non-polar stack, at prime meridian
                                 (stack - 1) * indices_per_stack +   // then jump to current stack, still at prime meridian
                                 slice);                             // and now move around in longitude
         }
      // and copy/store the holder for this stack.
      stack_indices.push_back(current_stack);
      }

   // Empty the index holder.
   current_stack.clear();

   // For the last stack, just push the south pole index,
   current_stack.push_back((num_stacks - 2) * indices_per_stack + 1);

   // and copy/store the holder for this stack.
   stack_indices.push_back(current_stack);
   }

void Sphere::makeSliceIndices(void) {
   // Holder for indices in a given slice.
   std::vector<GLuint> current_slice;

   // Adjust count of stack indices for presence of any stitch ( used in subclasses ).
   // We don't insert any 'stitch slice', but need to adjust for it's presence.
   GLuint indices_per_stack = num_slices + (stitch_flag ? 1 : 0);

   for(GLuint slice = 0; slice < num_slices; ++slice) {
      // Pop in the north pole index.
      current_slice.push_back(0);
      // Put the indices in for non-polar points.
      for(GLuint stack = 1; stack < num_stacks - 1; ++stack) {
         current_slice.push_back(1 +                                 // move to first non-polar stack, at prime meridian
                                 (stack - 1) * indices_per_stack +   // then jump to current stack, still at prime meridian
                                 slice);                             // and now move around in longitude
         }
      // Pop in the south pole index.
      current_slice.push_back(verts.size() - 1);
      // and copy/store the holder for this stack.
      slice_indices.push_back(current_slice);
      // Empty the index holder.
      current_slice.clear();
      }
   }
