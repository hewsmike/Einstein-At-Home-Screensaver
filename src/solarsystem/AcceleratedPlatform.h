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

#ifndef ACCELERATED_PLATFORM_H_
#define ACCELERATED_PLATFORM_H_

#include "CameraState.h"
#include "InertialPlatform.h"
#include "SpinPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Accelerated Cartesian translatable orthonormal vector set
 *
 * This class comprises acceleration state data, accessors and mutators thereof.
 * HOWEVER this doesn't model angular acceleration ( around centre of mass ),
 * only linear ( of centre of mass ). A constant angular rotation rate is
 * already an acceleration ... :-)
 *
 * \author Mike Hewson\n
 */

class AcceleratedPlatform : public InertialPlatform, public SpinPlatform {
   public:
      /**
       * \brief Constructor
       */
      AcceleratedPlatform(void);

      /**
       * \brief Destructor
       */
      virtual ~AcceleratedPlatform();

      /**
       * \brief Get the acceleration
       */
      Vector3D linear_acceleration(void) const;

      /**
       * \brief Set the acceleration
       */
      void set_linear_acceleration(const Vector3D& ac);

      void set_pitch_rate(vec_t rate);

      void set_roll_rate(vec_t rate);

      void set_yaw_rate(vec_t rate);

      CameraState getViewState(void) const;

      void setViewState(const CameraState& cam);

      vec_t get_pitch_rate(void);

      vec_t get_roll_rate(void);

      vec_t get_yaw_rate(void);

      /**
       * \brief Reset the platform in acceleration, velocity, position
       * and rotation
       */
      virtual void reset(void);

      /**
       * \brief Evolve platform one unit step in time
       */
      virtual void step(void);

   private:
      /// Fiducial acceleration.
      static const Vector3D INIT_ACC;

      // Current acceleration in space.
      Vector3D acc;
   };

/**
 * @}
 */

#endif // ACCELERATED_PLATFORM_H_
