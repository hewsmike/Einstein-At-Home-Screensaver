/***************************************************************************
 *   Copyright (C) 2010 by Mike Hewson                                     *
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

#include "VectorSPR.h"

// SIN and COS take arguments in DEGREES
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

VectorSPR::VectorSPR() {
   }

VectorSPR::VectorSPR(vec_t ra, vec_t dec, vec_t rd) {
   // Using right-handed spherical polar where positive y-axis
   // is a declination of positive 90 degrees, and positive x-axis
   // is of right ascension of zero, where right ascension increases
   // in a counter-clockwise fashion when looking down from positive
   // y-axis onto the z-x plane ie. to the 'East' if the positive y-axis
   // is 'North'.
   x_comp = rd * COS(dec) * COS(ra);
   z_comp = -rd * COS(dec) * SIN(ra);
   y_comp = rd * SIN(dec);
   }

VectorSPR::~VectorSPR() {
   }
