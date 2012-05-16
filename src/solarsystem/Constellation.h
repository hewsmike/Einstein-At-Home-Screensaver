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

#ifndef CONSTELLATION_H_
#define CONSTELLATION_H_

#include <string>
#include <utility>
#include <vector>

#include "OrdStar.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class holds star and linkage data for a constellation
 *
 * A constellation is a named grouping of stars with an additional pattern of lines
 * like 'join the dots' to hint at the appearance of something indicated by the
 * constellations name. I recommend that all star additions are made to a given
 * constellation prior to linkages being defined for any of them : as when a link is added
 * bounds checking occurs on the contents of the star store AT THE TIME THE LINK IS
 * ADDED. This is to prevent references to not-yet ( or ever ) entered stars.
 *
 * \author Mike Hewson\n
 */

class Constellation {
   public:
      /**
       * \brief Constructor
       *
       * \param nm : the name of the constellation
       */
      Constellation(const std::string nm);

      /**
       * \brief Destructor
       */
      ~Constellation();

      /**
       * \brief Add a star to the constellation
       *
       * The order in which stars are added is important for later reference
       * to them when linkages are defined. Each star has a zero based index
       * assigned. The first star entered has index 0, the second index 1 ... etc
       *
       * @param star : the star to add
       */
      void add_star(OrdStar star);

      /**
       * \brief Add a link between two stars
       *
       * Bounds checking occurs which discovers
       *    - attempts to link when no stars are in the constellation
       *    - attempts to link to a non-existent star index
       *    - attempts to link a star to itself
       *
       * @param first : the index of the first star
       *
       * @param second : the index of the second star
       */
      void add_link(unsigned int first, unsigned int second);

      /**
       * \brief Retrieve the name of the constellation
       */
      const std::string& name(void) const;

      /**
       * \brief Retrieve a reference to an immutable listing of the stars
       */
      const std::vector<OrdStar>& stars(void) const;

      /**
       * \brief Retrieve a reference to an immutable listing of the star linkages
       */
      const std::vector< std::pair<unsigned int, unsigned int> >& links(void) const;

      /**
       * \brief Retrieve the number of stars in the constellation
       */
      GLuint total_stars(void) const;

      /**
       * \brief Retrieve the number of linkages defined for the constellation
       */
      GLuint total_links(void) const;

      /**
       * \brief Retrieve the coordinates as a right-ascension/declination pair
       *        of the centroid of the stars in this constellation.
       */
      std::pair<GLfloat, GLfloat> centre(void);

   private:
      bool first_quadrant;
      bool fourth_quadrant;
      bool middle_quadrants;

      /// Flag to indicate if right ascension values in constellation span
      /// across a right ascension of zero. Important for calculating the
      /// centroid and other metrics.

      bool crosses_first_point_Aries;

      starpos_t ra_centre;
      starpos_t dec_centre;

      bool centre_evaluated;

      /**
       * \brief Helper function to detect invalid star indices
       */
      bool isValidIndex(unsigned int) const;

      std::string cons_name;

      /// The stars in this constellation.
      std::vector<OrdStar> star_list;

      /// Which star is connected to which.
      std::vector< std::pair<unsigned int, unsigned int> > link_list;
   };

#endif // CONSTELLATION_H_
