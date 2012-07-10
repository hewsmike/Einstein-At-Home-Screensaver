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

#include "RotatablePlatform.h"

const vec_t RotatablePlatform::NULL_RATE(0.0f);

RotatablePlatform::RotatablePlatform(void) {
    reset();
    }

RotatablePlatform::~RotatablePlatform() {
    }

void RotatablePlatform::pitch(vec_t angle) {
    // Here's the trig for the rotation.
    Vector3D new_up = cos(angle)*up() - sin(angle)*look();
    Vector3D new_look = cos(angle)*look() + sin(angle)*up();

    // Normalise and store 'up' and 'look' before you go.
    OrthoNormalPlatform::setUp(new_up.unit());
    OrthoNormalPlatform::setLook(new_look.unit());
    }

void RotatablePlatform::roll(vec_t angle) {
    Vector3D new_up = cos(angle)*up() - sin(angle)*cross();

    // Normalise and store 'up' before you go.
    OrthoNormalPlatform::setUp(new_up.unit());
    }

void RotatablePlatform::yaw(vec_t angle) {
    Vector3D new_look = cos(angle)*look() - sin(angle)*cross();

    // Normalise and store 'look' before you go.
    OrthoNormalPlatform::setLook(new_look.unit());
    }
