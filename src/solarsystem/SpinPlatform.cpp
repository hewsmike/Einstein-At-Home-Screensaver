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
    // Stop all rotations.
    setPitchRate(NULL_RATE);
    setRollRate(NULL_RATE);
    setYawRate(NULL_RATE);

    // Re-orient to a standard.
    OrthoNormalPlatform::reset();
    }

void SpinPlatform::step(void) {
    // Confession here : while these operations don't actually commute, you
    // have to pick some ordering to implement. I chose alphabetical.
    // Commutation is approached in the limit of tiny rates. But hey, don't
    // use this code to land the Space Shuttle ! :-)
    pitch(pitch_rate);
    roll(roll_rate);
    yaw(yaw_rate);
    }

void SpinPlatform::setPitchRate(vec_t rate) {
    pitch_rate = rate;
    }

void SpinPlatform::setRollRate(vec_t rate) {
    roll_rate = rate;
    }

void SpinPlatform::setYawRate(vec_t rate){
    yaw_rate = rate;
    }
