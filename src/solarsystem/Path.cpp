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

#include "Path.h"

#include "ErrorHandler.h"

Path::Path(const Curve& pos, const Curve& view, const Curve& orient) :
			  where(pos),
			  look_at(view),
			  up_dir(orient) {
   }

Path::~Path() {
	}

CameraState Path::value(float lambda) const {
   // Clamp lambda to [0.0, 1.0]
	if(lambda < 0.0f) {
	   lambda = 0.0f;
	   }
	if(lambda > 1.0f) {
		lambda = 1.0f;
		}

	return CameraState(where.value(lambda), look_at.value(lambda), up_dir.value(lambda));
	}
