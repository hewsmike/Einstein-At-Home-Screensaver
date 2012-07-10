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

#include "VelocityPlatform.h"

// The initial velocity is motionless.
const Vector3D VelocityPlatform::INITIAL_VELOCITY(Vector3D::NULLV);

VelocityPlatform::VelocityPlatform(void) {
    }

VelocityPlatform::VelocityPlatform(const Vector3D& velocity = VelocityPlatform::INITIAL_VELOCITY) :
                                       vel(velocity) {
    }

VelocityPlatform::~VelocityPlatform() {
    }

Vector3D VelocityPlatform::velocity(void) const {
    return vel;
    }

void VelocityPlatform::setVelocity(const Vector3D& velocity) {
    vel = velocity;
    }

void VelocityPlatform::reset(void) {
    // Not only reset to a choice of initial velocity ...
    setVelocity(VelocityPlatform::INITIAL_VELOCITY);

    // ... but also reset the position too.
    PositionPlatform::reset();
    }

void VelocityPlatform::step(void) {
    // Evolve in position as per current velocity.
    PositionPlatform::setPosition(PositionPlatform::position() + vel);
    }
