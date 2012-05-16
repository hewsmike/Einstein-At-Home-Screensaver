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

#include <cmath>

#include "ErrorHandler.h"

Path::Path(void) : where(), look_at(), up_dir (), et(Path::CLAMPED) {
	}

Path::Path(const Shape& pos, const Shape& view, const Shape& orient, Path::evolve_type e_type) :
			  where(pos),
			  look_at(view),
			  up_dir(orient),
			  et(e_type) {
   }

Path::~Path() {
	}

Vector3D Path::value(float lambda, param_type pt) const {
	Vector3D ret_val;

	float rho = lambda;

	switch (et) {
		case Path::CLAMPED :
			if(lambda < 0.0f) {
				rho = 0.0f;
				}
			if(lambda > 1.0f) {
				rho = 1.0f;
				}
			break;
		case Path::CYCLIC :
			rho = lambda - floor(lambda);
			break;
		default:
			// Ought not get here !!
			ErrorHandler::record("Path::value() - bad switch case reached (default) for tp", ErrorHandler::FATAL);
			break;
		}

	switch (pt) {
		case Path::POSITION :
			ret_val = where.value(rho);
			break;
		case Path::FOCUS :
			ret_val = look_at.value(rho);
			break;
		case Path::ORIENTATION :
			ret_val = up_dir.value(rho);
			break;
		default:
			// Ought not get here !!
		 	ErrorHandler::record("Path::value() - bad switch case reached (default) for pt", ErrorHandler::FATAL);
			break;
		}

	return ret_val;
	}

void Path::set(const Shape& curve, param_type pt) {
	switch (pt) {
		case Path::POSITION :
			where = curve;
			break;
		case Path::FOCUS :
			look_at = curve;
			break;
		case Path::ORIENTATION :
			up_dir = curve;
			break;
		default:
			// Ought not get here !!
		 	ErrorHandler::record("Path::set() - bad switch case reached (default) for pt", ErrorHandler::FATAL);
			break;
		}
	}
