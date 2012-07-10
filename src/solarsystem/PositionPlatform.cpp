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

#include "PositionPlatform.h"

// The initial stance is at the (global) origin.
const Vector3D PositionPlatform::INITIAL_POSITION(Vector3D::NULLV);

PositionPlatform::PositionPlatform(void) {
    }

PositionPlatform::PositionPlatform(const Vector3D& position = PositionPlatform::INITIAL_POSITION) :
                                      pos(position) {
    }

PositionPlatform::~PositionPlatform() {
    }

Vector3D PositionPlatform::position(void) const {
    return pos;
    }

void PositionPlatform::setPosition(const Vector3D& position) {
    pos = position;
    }

void PositionPlatform::reset(void) {
    // Reset to a choice of initial position.
    setPosition(PositionPlatform::INITIAL_POSITION);
    }
