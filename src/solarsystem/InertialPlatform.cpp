/***************************************************************************
 *   Copyright (C) 2010 by Mike Hewson                                     *
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

#include "InertialPlatform.h"

// The initial stance at origin with no movement.
const Vector3D InertialPlatform::NULL_VECTOR(0.0f, 0.0f, 0.0f);
const Vector3D InertialPlatform::INIT_POS(InertialPlatform::NULL_VECTOR);
const Vector3D InertialPlatform::INIT_VEL(InertialPlatform::NULL_VECTOR);

InertialPlatform::InertialPlatform() : pos(InertialPlatform::INIT_POS),
                                       vel(InertialPlatform::INIT_VEL) {
	}

InertialPlatform::~InertialPlatform() {
   }

// Accessors.
Vector3D InertialPlatform::position(void) const {
   return pos;
   }

Vector3D InertialPlatform::velocity(void) const {
   return vel;
   }

// Mutators.
void InertialPlatform::set_position(const Vector3D& ps) {
   pos = ps;
   }

void InertialPlatform::set_velocity(const Vector3D& vc) {
   vel = vc;
   }

void InertialPlatform::stop(void) {
   vel = InertialPlatform::NULL_VECTOR;
   }

void InertialPlatform::translate(void) {
   set_position(pos + vel);
   }

void InertialPlatform::reset(void) {
   OrthoPlatform::reset();
   pos = InertialPlatform::INIT_POS;
   vel = InertialPlatform::INIT_VEL;
   }
