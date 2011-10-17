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

#ifndef VERTEX_H_
#define VERTEX_H_

#include <utility>

#include "SDL_opengl.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates data relevant to an OpenGL vertex.
 *
 * It contains a position in 3D space, a normal vector of 3 components
 * and a pair of coordinate values for 2D texture mapping.
 *
 * \author Mike Hewson\n
 */

class Vertex {
   public:
      /**
       * \brief Constructor ( fully qualified )
       *
       * \param ps The position of the vertex
       *
       * \param nm The normal to the vertex
       *
       * \param tc The texture coordinates of the vertex
       */
      Vertex(const Vector3D& ps, const Vector3D& nm,
             const std::pair<GLfloat, GLfloat>& tc);

		/**
       * \brief Destructor
       */
      ~Vertex();

      /**
       * \brief Obtains the position of the vertex
       */
      const Vector3D& position(void) const;

      /**
       * \brief Obtains the normal to the vertex
       */
      const Vector3D& normal(void) const;

      /**
       * \brief Obtains texture coordinates of the vertex
       */
      const std::pair<GLfloat, GLfloat>& texture_co_ords(void) const;
      
   private:
      /// Position in 3D space.
      Vector3D pos;

      /// Normal vector.
      Vector3D norm;

      /// Coordinate value pair for 2D texturing.
      std::pair<GLfloat, GLfloat> t_cds;
	};

#endif // VERTEX_H_
