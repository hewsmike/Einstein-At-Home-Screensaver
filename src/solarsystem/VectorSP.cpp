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

#include "VectorSP.h"

VectorSP::VectorSP() {
   }

VectorSP::VectorSP(vec_t ra, vec_t dec, vec_t rd) {
   x_comp = rd * COS(dec) * COS(ra);
   y_comp = rd * COS(dec) * SIN(ra);
   z_comp = rd * SIN(dec);
   }

VectorSP::~VectorSP() {
   }
