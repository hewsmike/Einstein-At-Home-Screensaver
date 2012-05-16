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

#include "ErrorHandler.h"

Shape::Shape(void) : begin(), end() {
	}

Shape::Shape(Vector3D start, Vector3D finish) : begin(start), end(finish) {
   }

Shape::~Shape() {
	}

void Shape::setStart(Vector3D start) {
	begin = start;
	}

void Shape::setFinish(Vector3D finish) {
	end = finish;
	}

Vector3D Shape::value(float rho) const {
	// Bounds check/clamp on rho
	if(rho < 0.0f) {
		rho = 0.0f;
		}

	if(rho > 1.0f) {
		rho = 1.0f;
		}

	// Simple linear interpolation.
	return begin + (end - begin)*rho;
	}
