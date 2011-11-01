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

#include <iostream>

#include "Earth.h"
#include "SolarSystemGlobals.h"
#include "AcceleratedPlatform.h"
#include "Universe.h"

class Craft {
   private:
      /// Global bounds for craft position
      static const vec_t MAX_RANGE;
      static const vec_t MIN_EARTH_RANGE;

      static const vec_t START_RADIUS;

      /// Top speed of craft in whatever nett direction
      static const vec_t MAX_SPEED;

      /// Rates for about centre of mass craft rotations
      static const vec_t PITCH_RATE_INC;
      static const vec_t ROLL_RATE_INC;
      static const vec_t YAW_RATE_INC;
      static const vec_t RATE_FUDGE;

      /// Speed setting post 'bounce' off object
      static const vec_t RETURN_SPEED;

      /// Speed change steps for the aft-to-forward craft axis
      static const vec_t SPEED_DEC;
      static const vec_t SPEED_INC;

      /// Speed change step for the left-to-right craft axis
      static const vec_t LATERAL_THRUST_RATE;

      /// Speed change step for the floor-to-ceiling craft axis
      static const vec_t VERTICAL_THRUST_RATE;

      AcceleratedPlatform state;

      void vector_thrust(Vector3D thrust);

   public:
      Craft();
      ~Craft();

      void go_home(void);
      void nose_down();
      void nose_up();
      void roll_left();
      void roll_right();
      void step(void);
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

      const AcceleratedPlatform& get_platform() const;
   };

#endif // CRAFT_H_
