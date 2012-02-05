/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
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

#include "OrthoNormalPlatform.h"

// NOTE we want to have these class invariants :
//    'up' , 'look' and 'cross' vectors form a right handed
//    orthonormal Cartesian set

// The initial stance puts one looking along the positive x-axis with the z-axis
// being 'up', thus implying the positive y-axis is to the left of view.
const Vector3D OrthoNormalPlatform::INIT_LOOK(+1.0f, 0.0f, 0.0f);
const Vector3D OrthoNormalPlatform::INIT_UP(0.0f, 0.0f, 1.0f);

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

void OrthoNormalPlatform::set_look(const Vector3D& n_look) {
   look_dir = n_look;
   }

void OrthoNormalPlatform::set_up(const Vector3D& n_up) {
   up_dir = n_up;
   }

void OrthoNormalPlatform::reset(void) {
   // This will set 'look' and 'up' as orthogonal according to the
   // definitions of INIT_LOOK and INIT_UP.
   look_dir = INIT_LOOK;
	up_dir = INIT_UP;
	}
