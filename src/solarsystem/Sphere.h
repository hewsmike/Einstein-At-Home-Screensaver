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

#include <iostream>
#include <utility>
#include <string>
#include <vector>

#include "ErrorHandler.h"
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
 * specific latitude and longitude. Stacks refers to how many
 * distinct values of latitude ( counting each pole as one ),
 * and slices to how many distinct values of longitude. The
 * radius being the common distance of all vertices from the origin.
 *
 * \author Mike Hewson\n
 */

class Sphere {
   public:
      /// Do we stagger the vertex positions b/w stacks?
      enum stagger {NO_STAGGER, STAGGERED};

   private:
      /// Minimal sphere parameters.
      static const GLfloat MIN_RADIUS;
      static const GLuint MIN_SLICES;
      static const GLuint MIN_STACKS;

      /// Radius of sphere.
      vec_t rad;

      /// Number of longitudinal values.
      GLuint num_slices;

      /// Number of latitudinal values ( counting each pole as one band )
      GLuint num_stacks;
      
      /// Whether we stagger the vertices or not.
      stagger st;

      /// Listing of vertices.
      std::vector<Vertex> verts;

	public:
       /**
       * \brief Constructor
       *
       * \param radius : the radius
       *
       * \param slices : the number of longitudinal values
       *
       * \param stacks : the number of latitudinal bands
       */
		Sphere(vec_t radius, GLuint slices, GLuint stacks, Sphere::stagger st);

		/**
       * \brief Destructor
       */
		~Sphere();

      /**
       * \brief Read only access to vertex listing
       */
      const std::vector<Vertex>& vertices(void) const;

      /**
       * \brief Read only access to the radius
       */
      GLfloat radius(void) const;
      
      /**
       * \brief Read only access to the slice number
       */
      GLuint slices(void) const;
      
      /**
       * \brief Read only access to the stack number
       */
      GLuint stacks(void) const;
      
      /**
       * \brief Read only access to the stagger setting
       */
      bool isStaggered(void) const;
	};

#endif // SPHERE_H_
