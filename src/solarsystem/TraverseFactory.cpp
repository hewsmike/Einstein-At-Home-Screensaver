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

#include "TraverseFactory.h"

#include "Curve.h"
#include "ErrorHandler.h"

TraverseFactory::TraverseFactory(void) {
	}

TraverseFactory::~TraverseFactory() {
	}

Traverse* TraverseFactory::getInstance(const Traversable& trav, const CameraState& cam) {
	// Kick off traverse with the given camera state and the first from our given Traversable object.
	Traverse* trav_pt = new Traverse(cam, trav.getView(0));

	for(int way_point = 1; way_point < trav.numberOfWayPoints(); ++way_point) {
		trav_pt->addWayPoint(trav.getView(way_point));
		}

	return trav_pt;
	}
