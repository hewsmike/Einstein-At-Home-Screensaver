/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
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

#include "Pulsar.h"

Pulsar::Pulsar(starpos_t ra, starpos_t dec, std::string nm, pulsar_source sc) :
               Star(ra, dec, nm),
               source(sc) {
   // Base class constructor has been called. No point in bounds
   // checking of source ( being an enum that ought pop a
   // compile time error if bad ).
   }

Pulsar::~Pulsar() {
   }

Pulsar::pulsar_source Pulsar::get_source(void) const {
   return source;
   }
