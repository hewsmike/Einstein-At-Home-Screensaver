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

#include "SpinPlatform.h"

const vec_t SpinPlatform::NULL_RATE(0.0f);

SpinPlatform::SpinPlatform(void) {
   reset();
   }

SpinPlatform::~SpinPlatform() {
   }

void SpinPlatform::pitch(vec_t angle) {
   // Here's the trig for the rotation.
   Vector3D new_up = cos(angle)*up() - sin(angle)*look();
   Vector3D new_look = cos(angle)*look() + sin(angle)*up();

   // Normalise and store 'up' and 'look' before you go.
   set_up(new_up.unit());
   set_look(new_look.unit());
   }

void SpinPlatform::roll(vec_t angle) {
   Vector3D new_up = cos(angle)*up() - sin(angle)*cross();

   // Normalise and store 'up' before you go.
   set_up(new_up.unit());
   }

void SpinPlatform::yaw(vec_t angle) {
   Vector3D new_look = cos(angle)*look() - sin(angle)*cross();

   // Normalise and store 'look' before you go.
   set_look(new_look.unit());
   }

void SpinPlatform::reset(void) {
   pitch_rate = NULL_RATE;
   roll_rate = NULL_RATE;
   yaw_rate = NULL_RATE;

   OrthoNormalPlatform::reset();
   }

void SpinPlatform::step(void) {
   // Confession here : these operations don't actually commute, but you have to pick
   // some ordering. Commutation is approached in the limit of tiny rates. But hey,
   // don't use this code to land the Space Shuttle ! :-)
   pitch(pitch_rate);
   roll(roll_rate);
   yaw(yaw_rate);
   }
