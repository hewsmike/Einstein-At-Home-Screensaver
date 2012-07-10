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

#include "AcceleratedPlatform.h"

// The initial acceleration is none in either translation or rotation.
const Vector3D AcceleratedPlatform::INITIAL_LINEAR_ACCELERATION(Vector3D::NULLV);
const vec_t AcceleratedPlatform::NULL_RATE(0.0f);

AcceleratedPlatform::AcceleratedPlatform(void) : acc(INITIAL_LINEAR_ACCELERATION) {
    }

AcceleratedPlatform::~AcceleratedPlatform() {
    }

Vector3D AcceleratedPlatform::linearAcceleration(void) const {
    return acc;
    }

void AcceleratedPlatform::setLinearAcceleration(const Vector3D& acceleration) {
    acc = acceleration;
    }

void AcceleratedPlatform::setPitchRate(vec_t rate) {
    pitch_rate = rate;
    }

void AcceleratedPlatform::setRollRate(vec_t rate) {
    roll_rate = rate;
    }

void AcceleratedPlatform::setYawRate(vec_t rate) {
    yaw_rate = rate;
    }

CameraState AcceleratedPlatform::viewState(void) const {
    return CameraState(PositionPlatform::position(),
                       OrthoNormalPlatform::look(),
                       OrthoNormalPlatform::up());
    }

void AcceleratedPlatform::setViewState(const CameraState& cam) {
    PositionPlatform::setPosition(cam.position());
    OrthoNormalPlatform::setOrientation(cam.focus(), cam.orientation());
    }

vec_t AcceleratedPlatform::pitchRate(void) const {
    return pitch_rate;
    }

vec_t AcceleratedPlatform::rollRate(void) const {
    return roll_rate;
    }

vec_t AcceleratedPlatform::yawRate(void) const {
    return yaw_rate;
    }

void AcceleratedPlatform::reset(void) {
    // Not only reset to a choice of initial acceleration ...
    setLinearAcceleration(INITIAL_LINEAR_ACCELERATION);

    // But also reset the velocity, position.
    VelocityPlatform::reset();

    // ... and in rotation rates and orientation.
    setPitchRate(NULL_RATE);
    setRollRate(NULL_RATE);
    setYawRate(NULL_RATE);
    OrthoNormalPlatform::reset();
    }

void AcceleratedPlatform::step(void) {
    // Evolve in velocity as per current linear acceleration.
    VelocityPlatform::setVelocity(VelocityPlatform::velocity() + acc);

    // Call parent class to further state evolution in translation.
    VelocityPlatform::step();

    // Confession here : while these operations don't actually commute,
    // you have to pick some ordering to implement and I chose
    // alphabetical. Commutation is approached in the limit of tiny
    // rates. But hey, don't use this code to land the Space Shuttle ! :-)
    pitch(pitch_rate);
    roll(roll_rate);
    yaw(yaw_rate);
    }
