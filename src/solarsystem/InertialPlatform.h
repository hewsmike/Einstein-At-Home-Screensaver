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

#ifndef INERTIAL_PLATFORM_H_
#define INERTIAL_PLATFORM_H_

#include "TranslatablePlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Unaccelerated Cartesian translatable orthonormal vector set
 *
 * This class comprises velocity state data, accessors and mutators thereof.
 *
 * \author Mike Hewson\n
 */

class InertialPlatform : public TranslatablePlatform {
   public:
      /**
       * \brief Constructor
       */
      InertialPlatform(void);

      /**
       * \brief Destructor
       */
      virtual ~InertialPlatform();

      /**
       * \brief Get the velocity
       */
      Vector3D velocity(void) const;

      /**
       * \brief Set the velocity
       */
      void set_velocity(const Vector3D& vc);

   protected:
      /**
       * \brief Reset the platform in velocity, position and rotation
       */
      virtual void reset(void);
      
      /**
       * \brief Evolve platform one unit step in time
       */
      virtual void step(void);
      
   private:
      /// Fiducial velocity.
      static const Vector3D INIT_VEL;

      /// Current velocity in space.
      Vector3D vel;
   };
   
/**
 * @}
 */

#endif // INERTIAL_PLATFORM_H_
