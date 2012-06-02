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

Traverse::Traverse(void) {
	current_path_index = 0;
	}

Traverse::~Traverse() {
	}

void Traverse::clear() {
	cam_states.clear();
	current_path_index = 0;
	}

void Traverse::addWayPoint(const LookOut& cam) {
   cam_states.push_back(cam);
   }

unsigned int Traverse::numWayPoints(void) const {
	return cam_states.size();
	}

Path Traverse::getFirstPath(void) {
	Path ret_val;

	if(numWayPoints() > 1) {
		current_path_index = 0;
		ret_val = makePath();
		}

	return ret_val;
   }

Path Traverse::getNextPath(void) {
	Path ret_val;

	if(numWayPoints() > 1) {
		current_path_index = (current_path_index + 1) % cam_states.size();
		ret_val = makePath();
		}

   return ret_val;
	}

Path Traverse::makePath() {
	unsigned int next_path_index = (current_path_index + 1) % cam_states.size();

	Curve pos(cam_states.at(current_path_index).position(), cam_states.at(next_path_index).position());
   Curve focus(cam_states.at(current_path_index).focus(), cam_states.at(next_path_index).focus());
   Curve orient(cam_states.at(current_path_index).orientation(), cam_states.at(next_path_index).orientation());

   return Path(pos, focus, orient);
   }
