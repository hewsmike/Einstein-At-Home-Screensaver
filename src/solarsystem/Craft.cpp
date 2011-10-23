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

#include "Craft.h"

const vec_t Craft::MAX_RANGE(Universe::CELESTIAL_SPHERE_RADIUS * 8);
const vec_t Craft::MIN_EARTH_RANGE(Earth::EARTH_RADIUS*(1.2f));
const vec_t Craft::START_RADIUS(Earth::EARTH_RADIUS*(2.0f));
const vec_t Craft::RETURN_SPEED(5.0f);
const vec_t Craft::MAX_SPEED(50.0f);
const vec_t Craft::SPEED_DEC(2.5f);
const vec_t Craft::SPEED_INC(2.5f);
const vec_t Craft::LATERAL_THRUST_RATE(0.5f);
const vec_t Craft::VERTICAL_THRUST_RATE(0.5f);

const vec_t Craft::PITCH_RATE(PI/360.0f);
const vec_t Craft::ROLL_RATE(PI/360.0f);
const vec_t Craft::YAW_RATE(PI/360.0f);

Craft::Craft() {
   go_home();
  	}

Craft::~Craft() {
	}

const AcceleratedPlatform& Craft::get_platform() const {
	return state;
	}

void Craft::step(void) {
   state.step();
   // Bounds and collision checking ....

   // Too far away from home.
   if(state.position().len() > Craft::MAX_RANGE) {
      // TODO turn him around to point homewards?
      // Give a little nudge to send it back in the direction of home.
      state.set_velocity(-Craft::RETURN_SPEED*state.position().unit());
      std::cout << "Too far away from Earth - nudged back" << std::endl;
      }

   // Too close to home
   if(state.position().len() < Craft::MIN_EARTH_RANGE) {
      // TODO turn him around to point outwards?
      // Give a little nudge to send it back away from home.
      state.set_velocity(+Craft::RETURN_SPEED*state.position().unit());
      std::cout << "Too close to Earth - nudged away" << std::endl;
      }

   // TODO - to close to the Sun
   }

void Craft::forward_thrust(void) {
   // Add to the current velocity vector a fraction of the 'look' vector.
   // That is : thrust is applied along the aft to fore axis.
   vector_thrust(+Craft::SPEED_INC*state.look());
   }

void Craft::go_home(void) {
   // Ought place him initially in good position, stationary.
   // TODO looking at Earth ?
   state.set_position(Vector3D(Craft::START_RADIUS, 0, 3*Craft::START_RADIUS));
   stop();
   }

void Craft::reverse_thrust(void) {
   // Subtract from the current velocity vector a fraction of the 'look' vector.
   // That is : thrust is applied along the fore to aft axis.
   vector_thrust(-Craft::SPEED_DEC*state.look());
   }

void Craft::nose_down() {
   state.set_pitch_rate(-Craft::PITCH_RATE);
   }

void Craft::nose_up() {
   state.set_pitch_rate(Craft::PITCH_RATE);
   }

void Craft::roll_right() {
   state.set_roll_rate(-Craft::ROLL_RATE);
   }

void Craft::roll_left() {
   state.set_roll_rate(Craft::ROLL_RATE);
   }

void Craft::yaw_right() {
   state.set_yaw_rate(-Craft::YAW_RATE);
   }

void Craft::yaw_left() {
   state.set_yaw_rate(Craft::YAW_RATE);
   }

void Craft::null_rotation(void) {
   state.set_pitch_rate(0.0f);
   state.set_roll_rate(0.0f);
   state.set_yaw_rate(0.0f);
   }

void Craft::stop(void) {
   state.set_velocity(Vector3D(0.0f, 0.0f, 0.0f));
   }

void Craft::right_thrust() {
   // Add to the current velocity vector a fraction of the 'cross' vector.
   // That is : thrust is applied along the left to right wing axis.
   vector_thrust(+Craft::LATERAL_THRUST_RATE*state.cross());
   }

void Craft::left_thrust() {
   // Add to the current velocity vector a fraction of the 'cross' vector.
   // That is : thrust is applied along the right to left wing axis.
   vector_thrust(-Craft::LATERAL_THRUST_RATE*state.cross());
   }

void Craft::up_thrust() {
   // Add to the current velocity vector a fraction of the 'up' vector.
   // That is : thrust is applied along the floor to ceiling axis.
   vector_thrust(+Craft::VERTICAL_THRUST_RATE*state.up());
   }

void Craft::down_thrust() {
   // Subtract from the current velocity vector a fraction of the 'up' vector.
   // That is : thrust is applied along the ceiling to floor axis.
   vector_thrust(-Craft::VERTICAL_THRUST_RATE*state.up());
   }

void Craft::vector_thrust(Vector3D thrust) {
   // Apply thrust along given vector.
   Vector3D new_vel = state.velocity() + thrust;

    // How fast are we going? Too fast?
   if(new_vel.len() > Craft::MAX_SPEED) {
      // Yes, so cap speed along the line of the new velocity.
      new_vel = Craft::MAX_SPEED*new_vel.unit();
      std::cout << "Speed capped at " << new_vel.len() << std::endl;
      }

   state.set_velocity(new_vel);
   }
