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


#ifndef ORDSTAR_H
#define ORDSTAR_H

#include <string>

#include "Star.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Ordinary star data type.
 *
 *      This class comprises the astronomical data relating to a specifically
 * deemed not-especially-notable star. The visual magnitude and color
 * are stored.
 *
 * \see Star
 *
 * \author Mike Hewson\n
 */

class OrdStar : public Star {
    public:
        /// Identifiers of star spectral class.
        enum spectral_type {SPEC_A, SPEC_B, SPEC_F, SPEC_G, SPEC_K, SPEC_M, SPEC_O};

        /**
         * \brief Constructor
         *
         * \param ra - right ascension of the star
         *
         * \param dec - declination of the star
         *
         * \param mag - the visual magnitude ( brightness ) of the star
         *
         * \param spect - the spectral type ( color ) of the star
         *
         * \param nm - the name of the star
         */
        OrdStar(starpos_t ra, starpos_t dec, GLfloat mag,
                spectral_type spect, std::string nm);

        /**
         * \brief Destructor
         */
        ~OrdStar();

        /**
         * \brief Obtain the magnitude ( brightness )
         *
         * \return The magnitude
         */
        GLfloat magnitude(void) const;

        /**
         * \brief Obtain the spectral class ( color )
         *
         * \return The spectral class
         */
        spectral_type spectral_class(void) const;

    private:
        /// The star's brightness.
        GLfloat mag;

        /// The star's color.
        spectral_type spect_class;
    };

/**
 * @}
 */

#endif // ORDSTAR_H
