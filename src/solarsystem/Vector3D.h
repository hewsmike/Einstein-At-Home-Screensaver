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

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <cmath>
#include <iostream>
#include <string>

#include "SDL_opengl.h"
#include "SolarSystemGlobals.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/// Underlying type for vector representation
typedef GLfloat vec_t;

/**
 * \brief %Solarsystem Three dimensional vector data type.
 *
 * This class comprises an orthogonal basis co-ordinate triplet, with
 * standard Euclidean 3D vectorial behaviours. Specifically a right
 * handed system where the x-axis rotates into the y-axis by a 90
 * degree anticlockwise turn around the positive z-axis.
 *
 * \author Mike Hewson\n
 */

class Vector3D {
   private:
      /// Initialisers for the null vector.
      static const GLfloat NULL_LENGTH;
      static const GLfloat NULL_X;
      static const GLfloat NULL_Y;
      static const GLfloat NULL_Z;

   protected:
      /// The x co-ordinate.
      vec_t x_comp;

      /// The y co-ordinate.
      vec_t y_comp;

      /// The z co-ordinate.
      vec_t z_comp;

   public:
      /**
       * \brief Constructor, no arguments, gives the null vector
       */
      Vector3D();

      /**
       * \brief Constructor, giving fully specified member values
       * in Cartesian co-ordinates.
       *
       * \param xc the x co-ordinate
       *
       * \param yc the y co-ordinate
       *
       * \param zc the z co-ordinate
       */
		Vector3D(vec_t xc, vec_t yc, vec_t zc);

      /**
       * \brief Copy constructor
       *
       * \param ov the other vector
       */
      Vector3D(const Vector3D& ov);

		/**
       * \brief Destructor
       */
      virtual ~Vector3D();

      /**
       * \brief Obtain magnitude or length of the vector
       *
       * \return The length of the vector
       */
      vec_t len(void);

      /**
       * \brief Obtain a unit vector in the same direction as the vector
       *
       * \return The unit vector
       */
      Vector3D unit(void);

      /**
       * \brief Determine if the vector is null
       *
       * \return A boolean indicating if it is null
       */
      bool isNullVector(void);

      /**
       * \brief Obtain the x-component
       *
       * \return The x-component
       */
      vec_t x(void) const;

      /**
       * \brief Obtain the y-component
       *
       * \return The y-component
       */
      vec_t y(void) const;

      /**
       * \brief Obtain the z-component
       *
       * \return The z-component
       */
      vec_t z(void) const;

      /**
       * \brief Set the x-component
       *
       * \param The x-component
       */
      void set_x(vec_t xc);

      /**
       * \brief Set the y-component
       *
       * \param The y-component
       */
      void set_y(vec_t yc);

      /**
       * \brief Set the z-component
       *
       * \param The z-component
       */
      void set_z(vec_t zc);
	};

// Non member operators. Probably simpler to do than static methods ....

/**
 * \brief Vector summation, linear addition per component
 *
 * \param first - one vector
 *
 * \param second - another vector
 *
 * \return the resultant vector
 */
Vector3D operator+(const Vector3D& first, const Vector3D& second);

/**
 * \brief Vector subtraction, linear subtraction per component
 *
 * \param first - one vector
 *
 * \param second - another vector
 *
 * \return the resultant vector
 */
Vector3D operator-(const Vector3D& first, const Vector3D& second);

/**
 * \brief Prefix unary minus ie. flip the sign of each component
 *
 * \param vec - the vector
 *
 * \return the resultant vector
 */
Vector3D operator-(const Vector3D& vec);

/**
 * \brief Vector cross product, right handed and non-commutative
 *
 * \param first - one vector
 *
 * \param second - another vector
 *
 * \return the resultant vector
 */
Vector3D operator*(const Vector3D& first, const Vector3D& second);

/**
 * \brief Prefix scalar multiple ie. per component real number multiplier
 *
 * \param factor - the real multiplier
 *
 * \param vec - the vector
 *
 * \return the resultant vector
 */
Vector3D operator*(vec_t factor, const Vector3D& vec);

/**
 * \brief Postfix scalar multiple ie. per component real number multiplier
 *
 * \param vec - the vector
 *
 * \param factor - the real multiplier
 *
 * \return the resultant vector
 */
Vector3D operator*(const Vector3D& vec, vec_t factor);

/**
 * \brief Vector inner product ie. linear sum of per component multiplication
 *
 * \param first - one vector
 *
 * \param second - another vector
 *
 * \return the resultant real number
 */
vec_t operator^(const Vector3D& first, const Vector3D& second);

/**
 * @}
 */

#endif // VECTOR3D_H_
