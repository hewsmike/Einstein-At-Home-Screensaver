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

#ifndef ORTHO_NORMAL_PLATFORM_H_
#define ORTHO_NORMAL_PLATFORM_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Cartesian orthonormal vector set
 *
 * TODO Include testing and if necessary adjustment to maintain mutual
 * orthogonality of the three axes ?
 *
 * \author Mike Hewson\n
 */

class OrthoNormalPlatform {
   public:
      /**
       * \brief Constructor
       */
      OrthoNormalPlatform(void);

      /**
       * \brief Destructor
       */
      virtual ~OrthoNormalPlatform();

      /**
       * \brief return the cross vector ( by lazy evaluation )
       *
       * This is obtained by forming the cross product of the 'look' and 'up'
       * vectors in that order. If one is situated at the intersection of these
       * three vectors, looking along the positive direction of the look axis
       * ( from the tail to the head of the 'look' vector ), then the 'cross'
       * vector can be obtained by turning the 'up' vector 90 degrees
       * clockwise.
       */
      Vector3D cross(void) const;

      /**
       * \brief Retrieve the look vector
       */
      Vector3D look(void) const;

      /**
       * \brief Retrieve the up vector
       */
      Vector3D up(void) const;

   protected:
      /**
       * \brief Set the direction of the 'look' vector
       *
       * Protected use only as need to ensure orthogonality of the vector set
       * ie. can't have arbitrary assignment of the 'look' vector.
       */
      void set_look(const Vector3D& n_look);

       /**
       * \brief Set the direction of the 'up' vector
       *
       * Protected use only as need to ensure orthogonality of the vector set
       * ie. can't have arbitrary assignment of the 'up' vector.
       */
      void set_up(const Vector3D& n_up);

      /**
       * \brief Reset the platform in orientation
       */
      virtual void reset(void);

   private:
      /// Fiducial 'look' vector
      static const Vector3D INIT_LOOK;

      /// Fiducial 'up' vector
      static const Vector3D INIT_UP;

      /// The current 'look' vector.
      Vector3D look_dir;

      /// The current 'up' vector.
      Vector3D up_dir;
   };

/**
 * @}
 */

#endif // ORTHO_NORMAL_PLATFORM_H_
