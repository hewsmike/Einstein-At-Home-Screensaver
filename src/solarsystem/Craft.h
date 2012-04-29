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

#ifndef CRAFT_H_
#define CRAFT_H_

#include "AcceleratedPlatform.h"
#include "SolarSystemGlobals.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Simulates the kinetic and dynamic behaviours of a spaceship.
 *
 * \author Mike Hewson\n
 */

class Craft {
   public:
      Craft();
      ~Craft();

      void manouevre(SolarSystemGlobals::movements mov);

      const AcceleratedPlatform& get_platform() const;

      void step(GLfloat dayOfYear);

   private:
      /// Global bounds for craft position
      static const vec_t MAX_RANGE;
      static const vec_t MIN_EARTH_RANGE;
      static const vec_t MIN_SUN_RANGE;

      static const vec_t START_RADIUS;
      static const Vector3D START_POSITION;
      static const Vector3D START_LOOKING;

      /// Top speed of craft in whatever nett direction
      static const vec_t MAX_SPEED;

      /// Rates for about centre of mass craft rotations
      static const vec_t PITCH_RATE_INC;
      static const vec_t ROLL_RATE_INC;
      static const vec_t YAW_RATE_INC;
      static const vec_t RATE_FUDGE;

      /// Speed setting post 'bounce' off objects
      static const vec_t REBOUND_SPEED;

      /// Speed change steps for the aft-to-forward craft axis
      static const vec_t SPEED_DEC;
      static const vec_t SPEED_INC;

      /// Speed change step for the left-to-right craft axis
      static const vec_t LATERAL_THRUST_RATE;

      /// Speed change step for the floor-to-ceiling craft axis
      static const vec_t VERTICAL_THRUST_RATE;

      AcceleratedPlatform state;

      void go_home(void);
      void nose_down();
      void nose_up();
      void roll_left();
      void roll_right();
      void stop(void);
      void reverse_thrust(void);
      void forward_thrust(void);
      void yaw_left();
      void yaw_right();
      void right_thrust(void);
      void left_thrust(void);
      void null_rotation(void);
      void up_thrust(void);
      void down_thrust(void);

      void vector_thrust(Vector3D thrust);
   };

/**
 * @}
 */

#endif // CRAFT_H_
