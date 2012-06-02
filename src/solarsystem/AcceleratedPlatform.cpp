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

// The initial acceleration is none.
const Vector3D AcceleratedPlatform::INIT_ACC(Vector3D::NULLV);

AcceleratedPlatform::AcceleratedPlatform(void) : acc(INIT_ACC) {
	}

AcceleratedPlatform::~AcceleratedPlatform() {
   }

Vector3D AcceleratedPlatform::linear_acceleration(void) const {
   return acc;
   }

void AcceleratedPlatform::set_linear_acceleration(const Vector3D& ac) {
   acc = ac;
   }

void AcceleratedPlatform::set_pitch_rate(vec_t rate) {
   SpinPlatform::pitch_rate = rate;
   }

void AcceleratedPlatform::set_roll_rate(vec_t rate) {
   SpinPlatform::roll_rate = rate;
   }

void AcceleratedPlatform::set_yaw_rate(vec_t rate) {
   SpinPlatform::yaw_rate = rate;
   }

CameraState AcceleratedPlatform::getViewState(void) const {
	return CameraState(this->position(),
							 this->look(),
							 this->up());
	}

void AcceleratedPlatform::setViewState(const CameraState& cam) {
	this->set_position(cam.position());
	this->set_look(cam.focus());
	this->set_up(cam.orientation());
	}

vec_t AcceleratedPlatform::get_pitch_rate(void) {
   return SpinPlatform::pitch_rate;
   }

vec_t AcceleratedPlatform::get_roll_rate(void) {
   return SpinPlatform::roll_rate;
   }

vec_t AcceleratedPlatform::get_yaw_rate(void) {
   return SpinPlatform::yaw_rate;
   }

void AcceleratedPlatform::reset(void) {
   // Not only reset to a choice of initial acceleration ...
   set_linear_acceleration(INIT_ACC);

   // ... but also reset the velocity, position.
   InertialPlatform::reset();

   // ... and in rotation and orientation.
   SpinPlatform::reset();
   }

void AcceleratedPlatform::step(void) {
   // Evolve in velocity as per current linear acceleration.
   InertialPlatform::set_velocity(InertialPlatform::velocity() + acc);

   // Call parent class to further state evolution in translation.
   InertialPlatform::step();

   // Call parent class to further state evolution in rotation.
   SpinPlatform::step();
   }
