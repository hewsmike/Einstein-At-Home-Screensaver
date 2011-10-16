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

#include "Constellation.h"

Constellation::Constellation(const std::string nm) : cons_name(nm) {
	}

Constellation::~Constellation(){
	}

const std::string& Constellation::name(void) const {
   return cons_name;
   }

void Constellation::add_link(unsigned int first, unsigned int second) {
   // Bounds checking on both indices.

   // TODO ( maybe ) - check for an already entered link, keeping in mind
   // that (1,2) is really the same as (2,1) ?

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
         std::string msg = "Constellation::add_link() - attempt to link a star to itself : ";
         msg += "\tindex = ";
         msg += first;
         ErrorHandler::record(msg, ErrorHandler::WARN);
         }
      }
   else {
      // One or more lousy star indices given.
      std::string msg = "Constellation::add_link() - bad star index : ";
      msg += "\tfirst index = ";
      msg += first;
      msg += "\tsecond index = ";
      msg += second;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }
   }

bool Constellation::isValidIndex(unsigned int index) const {
   // Assume success.
   bool ret_val = true;

   // The the number of stars in the constellation
   unsigned int cons_size = star_list.size();

   if(cons_size == 0) {
      // Whoops! No stars yet in the constellation
      std::string msg = "Constellation::isValidIndex() - no stars in constellation!";
      ErrorHandler::record(msg, ErrorHandler::WARN);
      ret_val = false;
      }
   else {
      // One or more stars in the constellation
      if(index >= cons_size) {
         // But out of range star index
         std::string msg = "Constellation::isValidIndex() - star index out of range : ";
         msg += "\tindex = ";
         msg += index;
         ErrorHandler::record(msg, ErrorHandler::WARN);
         ret_val = false;
         }
      }

   return ret_val;
   }

void Constellation::add_star(OrdStar star) {
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
