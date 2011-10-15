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

#include "OrthoPlatform.h"

// NOTE we want to have these class invariants :
//    'up' , 'look' and 'cross' vectors form a right handed
//    orthonormal Cartesian set

// The initial stance puts one looking down the x-axis
// with the z-axis being 'up'.
const Vector3D OrthoPlatform::INIT_LOOK(-1.0f, 0.0f, 0.0f);
const Vector3D OrthoPlatform::INIT_UP(0.0f, 0.0f, 1.0f);

OrthoPlatform::OrthoPlatform() :	look_dir(OrthoPlatform::INIT_LOOK),
								         up_dir(OrthoPlatform::INIT_UP) {
	}

OrthoPlatform::~OrthoPlatform() {
   }

// Accessors.
Vector3D OrthoPlatform::cross(void) const {
   // Form their cross product, carefully noting the order of operands,
   // This will be a unit vector perpendicular to both 'look' and 'up'
   // in a right-handed sense. Hint : looking along the 'look' vector
   // from it's tail towards it's head, the 'up' vector can rotate into
   // 'cross' by a 90 degrees clockwise turn.
   return look_dir * up_dir;
   }

Vector3D OrthoPlatform::look(void) const {
   return look_dir;
   }

Vector3D OrthoPlatform::up(void) const {
   return up_dir;
   }

void OrthoPlatform::set_look(const Vector3D& n_look) {
   look_dir = n_look;
   }

void OrthoPlatform::set_up(const Vector3D& n_up) {
   up_dir = n_up;
   }

// Mutators.
/**
 * Both the 'up' and 'look' directions change with this manouevre.
 */
void OrthoPlatform::pitch_down(vec_t angle) {
   // Here's the trig for the rotation.
   Vector3D new_up = cos(angle)*up() + sin(angle)*look();
   Vector3D new_look = cos(angle)*look() - sin(angle)*up();

   // Normalise and store 'up' and 'look' before you go.
   set_up(new_up.unit());
   set_look(new_look.unit());
   }

/**
 * Both the 'up' and 'look' directions change with this manouevre.
 */
void OrthoPlatform::pitch_up(vec_t angle) {
   // Here's the trig for the rotation.
   Vector3D new_up = cos(angle)*up() - sin(angle)*look();
   Vector3D new_look = cos(angle)*look() + sin(angle)*up();

   // Normalise and store 'up' and 'look' before you go.
   set_up(new_up.unit());
   set_look(new_look.unit());
   }

/**
 * Both the 'up' and 'cross' directions changes with this manouevre.
 */
void OrthoPlatform::roll_right(vec_t angle) {
   Vector3D new_up = cos(angle)*up() + sin(angle)*cross();

   // Normalise and store 'up' before you go.
   set_up(new_up.unit());
   }

/**
 * Both the 'up' and 'cross' directions changes with this manouevre.
 */
void OrthoPlatform::roll_left(vec_t angle) {
   Vector3D new_up = cos(angle)*up() - sin(angle)*cross();

   // Normalise and store 'up' before you go.
   set_up(new_up.unit());
   }

/**
 * Both the 'look' and 'cross' directions changes with this manouevre.
 */
void OrthoPlatform::yaw_left(vec_t angle) {
   Vector3D new_look = cos(angle)*look() - sin(angle)*cross();

   // Normalise and store 'up' before you go.
   set_look(new_look.unit());
   }

/**
 * Both the 'look' and 'cross' directions changes with this manouevre.
 */
void OrthoPlatform::yaw_right(vec_t angle) {
   Vector3D new_look = cos(angle)*look() + sin(angle)*cross();

   // Normalise and store 'up' before you go.
   set_look(new_look.unit());
   }

void OrthoPlatform::reset(void) {
   look_dir = OrthoPlatform::INIT_LOOK;
	up_dir = OrthoPlatform::INIT_UP;
	}

void OrthoPlatform::show_invariants(void) const {
   // Get the normalised vector in the 'up' direction.
   Vector3D old_up = up();

   // Get the normalised vector in the 'look' direction.
   Vector3D old_look = look();

   vec_t ortho = old_up^old_look;
   std::cout << "\tortho = " << ortho << std::endl;

   vec_t look_mag = old_look^old_look;
   std::cout << "\tlook_mag = " << look_mag << std::endl;

   vec_t up_mag = old_up^old_up;
   std::cout << "\tup_mag = " << up_mag << std::endl;
   }
