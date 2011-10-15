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

#ifndef CONSTELLATION_H_
#define CONSTELLATION_H_

#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "OrdStar.h"
#include "SolarSystemGlobals.h"

class Constellation {
	private:
      std::string cons_name;

      /// The stars in this constellation.
		std::vector<OrdStar> star_list;

      /// Which star is connected to which.
		std::vector< std::pair<unsigned int, unsigned int> > link_list;
		
		GLuint t_stars;
		
		GLuint t_links;

	public:
      Constellation(const std::string nm);
		~Constellation();

		void add_star(OrdStar star);
		void add_link(unsigned int first, unsigned int second);
		const std::string& name(void) const;
      const std::vector<OrdStar>& stars(void) const;
      const std::vector< std::pair<unsigned int, unsigned int> >& links(void) const;
      GLuint total_stars(void) const;
      GLuint total_links(void) const;
   };

#endif // CONSTELLATION_H_
