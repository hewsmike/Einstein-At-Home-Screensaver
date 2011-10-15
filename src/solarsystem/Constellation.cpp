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

Constellation::Constellation(const std::string nm) : cons_name(nm), t_stars(0), t_links(0) {
	}

Constellation::~Constellation(){
	}

const std::string& Constellation::name(void) const {
   return cons_name;
   }

void Constellation::add_link(unsigned int first, unsigned int second) {
   link_list.push_back(std::pair<unsigned int, unsigned int>(first, second));
   t_links++;
   }

void Constellation::add_star(OrdStar star) {
	star_list.push_back(star);
	t_stars++;
	}

const std::vector<OrdStar>& Constellation::stars(void) const {
   return star_list;
   }

const std::vector< std::pair<unsigned int, unsigned int> >& Constellation::links(void) const {
   return link_list;
   }

GLuint Constellation::total_stars(void) const {
   return t_stars;
   }

GLuint Constellation::total_links(void) const {
   return t_links;
   }
