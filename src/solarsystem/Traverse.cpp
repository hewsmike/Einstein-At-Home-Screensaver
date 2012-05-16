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

#include "Shape.h"
#include "Traverse.h"

#include "ErrorHandler.h"

Traverse::Traverse(const CameraState& first, const CameraState& second) {
	this->addWayPoint(first);
	this->addWayPoint(second);
   }

Traverse::~Traverse() {
	}

void Traverse::addWayPoint(const CameraState& cam) {
	cam_states.push_back(cam);
	}

bool Traverse::getSegment(Path& pt, unsigned int seq) const {
	// Assume failure.
	bool ret_val = false;

	if(seq < this->numberOfSegments()) {
		pt.set(Shape(cam_states.at(seq).position(), cam_states.at(seq + 1).position()), Path::POSITION);
		pt.set(Shape(cam_states.at(seq).focus(), cam_states.at(seq + 1).focus()), Path::FOCUS);
		pt.set(Shape(cam_states.at(seq).orientation(), cam_states.at(seq + 1).orientation()), Path::ORIENTATION);
		ret_val = true;
		}

	return ret_val;
	}

unsigned int Traverse::numberOfSegments(void) const {
	return this->numberOfWayPoints() - 1;
	}

unsigned int Traverse::numberOfWayPoints(void) const {
	return cam_states.size();
	}
