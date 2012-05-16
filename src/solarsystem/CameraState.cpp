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

#include "CameraState.h"

#include "ErrorHandler.h"

CameraState::CameraState(const Vector3D& position,
								 const Vector3D& focus,
								 const Vector3D& orientation) :
									where(position),
									look_at(focus),
									up_dir(orientation) {
   }

CameraState::CameraState() : where(),
									  look_at(),
									  up_dir() {
   }

CameraState::~CameraState() {
	}

const Vector3D& CameraState::position(void) const {
	return this->where;
	}

const Vector3D& CameraState::focus(void) const {
	return this->look_at;
	}

const Vector3D& CameraState::orientation(void) const {
	return this->up_dir;
	}
