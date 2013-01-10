/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#include "OrthoNormalPlatform.h"

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"

// NOTE we want to have these class invariants :
//    'up' , 'look' and 'cross' vectors form a right handed
//    orthonormal Cartesian set

// The initial stance puts one looking along the positive x-axis with the
// z-axis being 'up', thus implying the positive y-axis is to the left of
// view. So our 'cross' vector is initially anti-parallel to the y-axis.
const Vector3D OrthoNormalPlatform::INITIAL_LOOK(+1.0f, 0.0f, 0.0f);
const Vector3D OrthoNormalPlatform::INITIAL_UP(0.0f, 0.0f, 1.0f);

OrthoNormalPlatform::OrthoNormalPlatform(void) {
    reset();
    }

OrthoNormalPlatform::~OrthoNormalPlatform() {
    }

Vector3D OrthoNormalPlatform::cross(void) const {
    // Lazy evaluated as per current values of 'look' and 'up' vectors.
    return look_dir * up_dir;
    }

Vector3D OrthoNormalPlatform::look(void) const {
    return look_dir;
    }

Vector3D OrthoNormalPlatform::up(void) const {
    return up_dir;
    }

void OrthoNormalPlatform::setOrientation(Vector3D new_look,
                                         Vector3D new_up) {
    // Ensure unit lengths.
    look_dir = new_look.unit();
    up_dir = new_up.unit();

    /// TODO - for check and correct to orthogonality.
    }

void OrthoNormalPlatform::reset(void) {
    // This will set 'look' and 'up' as orthogonal according
    // to the definitions of INITIAL_LOOK and INITIAL_UP.
    setOrientation(OrthoNormalPlatform::INITIAL_LOOK,
                   OrthoNormalPlatform::INITIAL_UP);
    }
