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

#include "Star.h"

// Set the coordinate boundary values.
const starpos_t Star::DECLINATION_HIGH(90.0f);
const starpos_t Star::DECLINATION_LOW(-90.0f);
const starpos_t Star::RIGHT_ASCENSION_HIGH(360.0f);
const starpos_t Star::RIGHT_ASCENSION_LOW(0.0f);

Star::Star(starpos_t ra, starpos_t dec, std::string nm) : s_name(nm) {
   // Note endpoint treatment of bounds. Fail construction if out-of-bounds,
   // which is no biggie, as heap allocation not involved. Deserves program
   // exit though as this would indicate an important data error.

   // Valid if within the range = [RIGHT_ASCENSION_LOW, RIGHT_ASCENSION_HIGH)
   // Specifically a right ascension of 360 degrees is NOT valid.
   if((ra < RIGHT_ASCENSION_LOW) || (ra >= RIGHT_ASCENSION_HIGH)) {
      std::stringstream msg;
      msg << "Star::Star() : bad right ascension given : ";
	   msg << "value = ";
	   msg << ra;
	   ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
      }

   // Valid if within the range = [DECLINATION_LOW, DECLINATION_HIGH].
   if((dec < DECLINATION_LOW) || (dec > DECLINATION_HIGH)) {
      std::stringstream msg;
      msg << "Star::Star() : bad declination given : ";
      msg << "value = ";
      msg << dec;
      ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
      }

   // Inputs in range, so can now initialise with given arguments.
	right_asc = ra;
	decl = dec;
   }

Star::~Star() {
   }

starpos_t Star::right_ascension(void) const {
   return right_asc;
   }

starpos_t Star::declination(void) const {
   return decl;
   }

std::string Star::name(void) const {
	return s_name;
	}
