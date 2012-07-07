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

#ifndef STAR_H_
#define STAR_H_

#include <string>

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/// Underlying type for astronomical data.
typedef vec_t starpos_t;

/**
 * \brief %Solarsystem Star data type.
 *
 *      This base class comprises the astronomical data relating to a specific
 * star, from which other celestial object classes may be derived. Here 'star'
 * generically means any object in the ( distant ) sky, that is deemed to be
 * 'sitting on' the celestial sphere. There is only one constructor, there being
 * no point in having default 'nul' coordinate values, but nameless is OK.
 * There is also little point in allowing malleable values ie. for this purpose,
 * do stars move and change names? So no mutators either.
 *
 * \see OrdStar
 * \see Pulsar
 * \see Supernova
 *
 * \author Mike Hewson\n
 */

class Star {
    public:
        /**
         * \brief Constructor, giving fully specified member values
         *
         * \param ra - right ascension of the star
         *
         * \param dec - declination of the star
         *
         * \param nm - the name of the star
         */
        Star(starpos_t ra, starpos_t dec, const std::string& nm);

        /**
         * \brief Destructor
         */
        virtual ~Star();

        /**
         * \brief Obtain the right ascension value
         *
         * \return The right ascension value
         */
        starpos_t right_ascension(void) const;

        /**
         * \brief Obtain the declination value
         *
         * \return The declination value
         */
        starpos_t declination(void) const;

        /**
         * \brief Obtain the name
         *
         * \return The name
         */
        std::string name(void) const;

    private:
        /// Bounds for right ascension and declination values.
        static const starpos_t DECLINATION_HIGH;
        static const starpos_t DECLINATION_LOW;
        static const starpos_t RIGHT_ASCENSION_HIGH;
        static const starpos_t RIGHT_ASCENSION_LOW;

        /// Position coordinate on celestial sphere in decimal degrees,
        /// from 1st point of Aries ( zero of celestial longitude ) and
        /// increasing towards the east. It's actually in Pisces now .....
        starpos_t right_asc;

        /// Position coordinate on celestial sphere in decimal degrees,
        /// above ( +ve ) and below ( -ve ) the celestial equator ( zero of
        /// celestial latitude ).
        starpos_t decl;

        /// The name of the star.
        std::string s_name;
    };

/**
 * @}
 */

#endif /*STAR_H_*/
