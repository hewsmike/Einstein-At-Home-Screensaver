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

#include <algorithm>
#include <sstream>

#include "Constellation.h"

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"

Constellation::Constellation(const std::string nm) : cons_name(nm) {
   crosses_first_point_Aries = false;
   first_quadrant = false;
   fourth_quadrant = false;
   centre_evaluated = false;
   }

Constellation::~Constellation(){
   }

const std::string& Constellation::name(void) const {
   return cons_name;
   }

void Constellation::add_link(unsigned int first, unsigned int second) {
   // Bounds checking on both indices.

   // TODO ( maybe ) - check for an already entered link, keeping in mind
   // that (a,b) is really the same as (b,a) ?

   // Only add the links if they both refer to actual stars
   // currently in the listing
   if(isValidIndex(first) && isValidIndex(second)) {
      // Are the stars different ?
      if(first != second) {
         // Yes, so put the linkage in.
         // But magnitude ordering the indices in the link might
         // be useful later in later development.
         unsigned int link_min = std::min(first, second);
         unsigned int link_max = std::max(first, second);
         link_list.push_back(std::pair<unsigned int, unsigned int>(link_min, link_max));
         }
      else {
         // No, it's the same star
         std::stringstream msg;
         msg << "Constellation::add_link() - attempt to link a star to itself : ";
         msg << "\tindex = ";
         msg << first;
         ErrorHandler::record(msg.str(), ErrorHandler::WARN);
         }
      }
   else {
      // One or more lousy star indices given.
      std::stringstream msg;
      msg << "Constellation::add_link() - bad star index : ";
      msg << "\tfirst index = ";
      msg << first;
      msg << "\tsecond index = ";
      msg << second;
      ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
      }
   }

bool Constellation::isValidIndex(unsigned int index) const {
   // Assume success.
   bool ret_val = true;

   // The the number of stars in the constellation.
   unsigned int cons_size = star_list.size();

   if(cons_size == 0) {
      // Whoops! No stars yet in the constellation.
      std::stringstream msg;
      msg << "Constellation::isValidIndex() - no stars in constellation!";
      ErrorHandler::record(msg.str(), ErrorHandler::WARN);
      ret_val = false;
      }
   else {
      // One or more stars in the constellation.
      if(index > cons_size - 1) {
         // But out of range star index.
         std::stringstream msg;
         msg << "Constellation::isValidIndex() - star index out of range : ";
         msg << "\tindex = ";
         msg << index;
         ErrorHandler::record(msg.str(), ErrorHandler::WARN);
         ret_val = false;
         }
      }

   return ret_val;
   }

void Constellation::add_star(OrdStar star) {
   // By adding a new star then previous centroid evaluation no longer holds.
   centre_evaluated = false;

   static const GLfloat FIRST_QUAD_UPPER(90);
   static const GLfloat FOURTH_QUAD_LOWER(270);

   // As stars are added, keep track of whether any have their right
   // ascension values within quadrants either side of The First Point
   // of Aries, or the other quadrants.
   starpos_t right_ascension = star.right_ascension();

   if(right_ascension < FIRST_QUAD_UPPER) {
         first_quadrant = true;
      }
   else {
      if(right_ascension > FOURTH_QUAD_LOWER) {
         fourth_quadrant = true;
         }
      else {
         middle_quadrants = true;
         }
      }

   // If there are right ascension values in the arcs [0,90) AND (270,360),
   // but NOT in the arc [90,270], then the constellation must be spanning
   // across the first point of Aries.
   if(first_quadrant && fourth_quadrant && !middle_quadrants) {
      crosses_first_point_Aries = true;
      }

   star_list.push_back(star);
   }

const std::vector<OrdStar>& Constellation::stars(void) const {
   return star_list;
   }

const std::vector< std::pair<unsigned int, unsigned int> >& Constellation::links(void) const {
   return link_list;
   }

GLuint Constellation::total_stars(void) const {
   return star_list.size();
   }

GLuint Constellation::total_links(void) const {
   return link_list.size();
   }

std::pair<GLfloat, GLfloat> Constellation::centre(void) {
   // Lazy evaluated on first request !
   std::pair<GLfloat, GLfloat> ret_val(0.0f, 0.0f);

   // Make sure we have a list of stars to average over, and
   // that it hasn't already been done.
   if((star_list.size() > 0) && (centre_evaluated == false)) {
      starpos_t ra_total = 0;
      starpos_t dec_total = 0;

      // Iterate through the list.
      for(std::vector<OrdStar>::const_iterator st = star_list.begin();
          st != star_list.end();
          ++st) {
         starpos_t ra = st->right_ascension();

         // If this constellation spans the prime meridian, and
         // the current right ascension is on the 'eastern side'
         // of it then add a full circle's worth of angle to it.
         if(crosses_first_point_Aries && (ra < SolarSystemGlobals::HALF_CIRCLE_DEG)) {
            ra += SolarSystemGlobals::FULL_CIRCLE_DEG;
            }

         ra_total += ra;
         dec_total += st->declination();
         }

      // Average right ascension.
      ra_centre = ra_total/star_list.size();

      // Adjust right ascension average, if need be
      if(crosses_first_point_Aries && (ra_centre > SolarSystemGlobals::FULL_CIRCLE_DEG)) {
         ra_centre -= SolarSystemGlobals::FULL_CIRCLE_DEG;
         }

      // Average declination.
      dec_centre = dec_total/star_list.size();
      centre_evaluated = true;
      }

   ret_val.first = ra_centre;
   ret_val.second = dec_centre;
   return ret_val;
   }
