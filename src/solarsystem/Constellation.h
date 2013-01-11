/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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
 * \brief This class holds star and linkage data for a constellation.
 *
 *      A constellation is a named grouping of stars to be later rendered with
 * a pattern of lines, like a 'join the dots' puzzle, to hint at the appearance
 * of some entity indicated by the constellation's name.
 *
 *      BEWARE : I strongly recommend that ALL star additions are made to a
 * given constellation prior to linkages being defined for any of them : as when
 * a link is added bounds checking occurs on the contents of the star store AT
 * THE TIME THE LINK IS ADDED. This is to prevent references to not-yet ( or
 * ever ) entered stars.
 *
 * \see OrdStar
 *
 * \author Mike Hewson\n
 */

class Constellation {
    public:
        /**
         * \brief Constructor.
         *
         * \param name : the name of the constellation.
         */
        Constellation(const std::string name);

        /**
         * \brief Destructor.
         */
        ~Constellation();

        /**
         * \brief Add a star to the constellation.
         *
         *      The order in which stars are added is important for later reference
         * to them ie. when linkages are defined. Each star has a zero based
         * index assigned. The first star entered has index 0, the second index 1
         * ... etc
         *
         * \param star : the star to add.
         */
        void add_star(OrdStar star);

        /**
         * \brief Add a link between two stars.
         *
         *    Bounds checking occurs which discovers ( making fatal error ) :
         *        - attempts to link when no stars are in the constellation.
         *        - attempts to link to a non-existent star index.
         *        - attempts to link a star to itself.
         *
         * \param first : the index of the first star.
         *
         * \param second : the index of the second star.
         */
        void add_link(unsigned int first, unsigned int second);

        /**
         * \brief Retrieve the name of the constellation.
         *
         * \return The name of the constellation as a string.
         */
        const std::string& name(void) const;

        /**
         * \brief Obtain the listing of the stars.
         *
         * \return a constant reference to the listing.
         */
        const std::vector<OrdStar>& stars(void) const;

        /**
         * \brief Obtain the listing of the star linkages.
         *
         * \return a constant reference to the listing.
         */
        const std::vector<std::pair<unsigned int, unsigned int> >& links(void) const;

        /**
         * \brief Obtain the number of stars in the constellation.
         *
         * \return the number of stars.
         */
        GLuint total_stars(void) const;

        /**
         * \brief Obtain the number of linkages defined for the constellation.
         *
         * \return the nomber of linkages.
         */
        GLuint total_links(void) const;

        /**
         * \brief Obtain the coordinates of the centroid of the stars in this
         *        constellation.
         *
         * \return A right-ascension/declination pair representing the centroid.
         */
        std::pair<GLfloat, GLfloat> centre(void);

        /**
         * \brief Store a description of the constellation.
         *
         * \param description : a ( preferably short ) descriptive string.
         */
        void addToDescription(const std::string& description);

        /**
         * \brief Store a description of the constellation.
         *
         * \param description : a ( preferably short ) descriptive string.
         */
        const std::vector<std::string>& getDescription(void) const;

    private:
        /// Flags in deducing whether or not right ascension values in
        /// constellation span across a right ascension value of zero.
        /// Important for calculating the centroid and other metrics.
        /// Constellation has a star in first quadrant ( RA 0 to 6 hours ).
        bool first_quadrant;
        /// Constellation has a star in fourth quadrant ( RA 18 to 24 hours ).
        bool fourth_quadrant;
        /// Constellation has a star in first quadrant ( RA 6 to 18 hours ).
        bool middle_quadrants;
        /// Does the constellation cross RA of zero ie. has stars in the
        /// first AND fourth quadrants, but NONE in the middle quadrants?
        bool crosses_first_point_Aries;

        /// Right ascension and declination values of the constellation's
        /// centroid.
        starpos_t ra_centre;
        starpos_t dec_centre;

        /// Flag indicating if the centroid has already been ( lazy ) evaluated
        /// for the current set of stars.
        bool centre_evaluated;

        /// The name of the constellation.
        std::string cons_name;

        /// A set of descriptive strings, as provided.
        std::vector<std::string> desc;

        /// The listing of stars in this constellation.
        std::vector<OrdStar> star_list;

        /// Which star is connected to which.
        std::vector<std::pair<unsigned int, unsigned int> > link_list;

        /**
         * \brief Helper function to detect invalid star indices.
         *
         * \param index : a given index to check whether it lies within
         *                current bounds.
         */
        bool isValidIndex(unsigned int index) const;
    };

/**
 * @}
 */

#endif // CONSTELLATION_H_
