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

#include "InertialPlatform.h"

// The initial velocity is motionless.
const Vector3D InertialPlatform::INIT_VEL(Vector3D::NULLV);

InertialPlatform::InertialPlatform(const Vector3D& velocity = InertialPlatform::INIT_VEL) :
                                       vel(velocity) {
	}

InertialPlatform::~InertialPlatform() {
    }

Vector3D InertialPlatform::velocity(void) const {
    return vel;
    }

void InertialPlatform::set_velocity(const Vector3D& velocity) {
    vel = velocity;
    }

void InertialPlatform::reset(void) {
    // Not only reset to a choice of initial velocity ...
    set_velocity(InertialPlatform::INIT_VEL);

    // ... but also reset the position too.
    TranslatablePlatform::reset();
    }

void InertialPlatform::step(void) {
    // Evolve in position as per current velocity.
    TranslatablePlatform::set_position(TranslatablePlatform::position() + vel);
    }
