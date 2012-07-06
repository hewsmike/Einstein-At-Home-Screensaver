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

#include "Curve.h"

#include "ErrorHandler.h"

// This will default to NULL vectors from the Vector3D class,
// that is from the origin to the origin.
Curve::Curve(void) : begin(), end() {
    }

Curve::Curve(const Vector3D& start, const Vector3D& finish) :
             begin(start), end(finish) {
    }

Curve::~Curve() {
	}

Vector3D Curve::value(float rho) const {
	// Simple linear interpolation.
	return begin + (end - begin)*rho;
	}

float Curve::length(void) const {
	return (end - begin).len();
	}
