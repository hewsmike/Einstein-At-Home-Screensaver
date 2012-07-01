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

#ifndef SPHERE_H_
#define SPHERE_H_

#include "framework.h"

#include <vector>

#include <oglft/OGLFT.h>

#include "SolarSystemGlobals.h"
#include "Vector3D.h"
#include "VectorSP.h"
#include "Vertex.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class creates OpenGL vertex data for a spherical shape.
 *
 * The sphere is approximated polygonally by a mesh of points at
 * specific latitude and longitude values. Stacks refers to how many
 * distinct values of latitude ( counting each pole as one ) are desired,
 * and slices as to how many distinct values of longitude. I recommend an odd
 * number of stacks so that one will have points on the equator. The
 * radius being the common distance of all vertices from the origin.
 * Staggering refers to whether/not the longitude values b/w consecutive
 * latitudes are identical ( non-staggered ) or mid-way ( staggered ).
 * Stitching refers to potential usage with OpenGL texturing. It is also
 * recommended that the following call be made prior to subsequent texturing
 * upon the vertices :
 *
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 *
 * on account of the behaviour of the texture coordinate values at the prime
 * meridian.
 *
 * \author Mike Hewson\n
 */

class Sphere {
   public:
      /// Minimal sphere parameters.
      static const GLfloat MIN_RADIUS;
      static const GLuint MIN_SLICES;
      static const GLuint MIN_STACKS;

      /**
       * \brief Constructor
       *
       * \param radius : the radius
       *
       * \param slices : the number of longitudinal meridians
       *
       * \param stacks : the number of latitudinal bands
       *
       * \param stagger : whether or not longitude values stagger
       *
       * \param stitch : whether or not we create 'stitch' vertices
       */
      Sphere(vec_t radius, GLuint slices, GLuint stacks, bool stagger, bool stitch);

      /**
       * \brief Destructor
       */
      ~Sphere();

      /**
       * \brief Read only access to vertex listing
       */
      const std::vector<Vertex>& vertices(void) const;

      /**
       * \brief Read only access to slice vertices index listing
       */
      const std::vector<std::vector<GLuint> >& sliceIndices(void) const;

      /**
       * \brief Read only access to stack vertices index listing
       */
      const std::vector<std::vector<GLuint> >& stackIndices(void) const;

      /**
       * \brief Obtain the radius
       */
      GLfloat radius(void) const;

      /**
       * \brief Obtain the slice number
       */
      GLuint slices(void) const;

      /**
       * \brief Obtain the stack number
       */
      GLuint stacks(void) const;

      /**
       * \brief Obtain the stagger setting
       */
      bool isStaggered(void) const;

      /**
       * \brief Obtain the stitch setting
       */
      bool isStitched(void) const;

   private:
      /// Radius of sphere.
      vec_t rad;

      /// Number of longitudinal values.
      GLuint num_slices;

      /// Number of latitudinal values ( counting each pole as one band )
      GLuint num_stacks;

      /// Whether we stagger the vertices or not.
      bool stagger_flag;

      /// Whether we insert stitch vertices or not.
      bool stitch_flag;

      /// Listing of vertices.
      std::vector<Vertex> verts;

      /// Listing of vertex indices by slices.
      std::vector<std::vector<GLuint> > slice_indices;

      /// Listing of vertex indices by stacks.
      std::vector<std::vector<GLuint> > stack_indices;

      /**
       * \brief Vertex creator
       */
      void makeVertices(void);

      /**
       * \brief Stack index creator
       */
      void makeStackIndices(void);

      /**
       * \brief Slice index creator
       */
      void makeSliceIndices(void);
   };

/**
 * @}
 */

#endif // SPHERE_H_
