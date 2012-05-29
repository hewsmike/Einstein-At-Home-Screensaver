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

#include "Traverse.h"

#include "Curve.h"
#include "ErrorHandler.h"

Traverse::Traverse(const CameraState& first,
                   const CameraState& second) {
	addWayPoint(first);
	addWayPoint(second);

	current_path = 0;
   }

Traverse::~Traverse() {
	}

void Traverse::addWayPoint(const CameraState& cam) {
   cam_states.push_back(cam);
   }

Path Traverse::getFirstPath(void) {
	current_path = 0;

   return makePath();
   }

Path Traverse::getNextPath(void) {
   return makePath();
	}

Path Traverse::makePath() {
	unsigned int first_index = current_path;
	unsigned int second_index = (current_path + 1) % cam_states.size();

	Curve pos(cam_states.at(first_index).position(), cam_states.at(second_index).position());
   Curve focus(cam_states.at(first_index).focus(), cam_states.at(second_index).focus());
   Curve orient(cam_states.at(first_index).orientation(), cam_states.at(second_index).orientation());

   current_path = (current_path + 1) % cam_states.size();
   return Path(pos, focus, orient);
   }
