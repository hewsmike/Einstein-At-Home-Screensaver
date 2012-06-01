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

const float Path::LAMBDA_LOWER_BOUND(0.0f);
const float Path::LAMBDA_UPPER_BOUND(1.0f);

Path::Path(const Curve& pos, const Curve& view, const Curve& orient) :
			  where(pos),
			  look_at(view),
			  up_dir(orient) {
   }

Path::Path(void) : where(), look_at(), up_dir() {
   }

Path::~Path() {
	}

float Path::curveLength(Path::component comp) const {
	float ret_val = 0.0f;

	switch (comp) {
		case Path::POSITION :
			ret_val = where.length();
			break;
		case Path::FOCUS :
			ret_val = look_at.length();
			break;
		case Path::ORIENTATION:
			ret_val = up_dir.length();
			break;
		default :
			// Ought not get here !!
			std::string msg = "Path::curveLength() - bad switch case reached (default)";
			ErrorHandler::record(msg, ErrorHandler::FATAL);
			break;
		}

	return ret_val;
	}

CameraState Path::value(float lambda) const {
   // Clamp lambda
	if(lambda < LAMBDA_LOWER_BOUND) {
	   lambda = LAMBDA_LOWER_BOUND;
	   }
	if(lambda > LAMBDA_UPPER_BOUND) {
		lambda = LAMBDA_UPPER_BOUND;
		}

	return CameraState(where.value(lambda), look_at.value(lambda), up_dir.value(lambda));
	}
