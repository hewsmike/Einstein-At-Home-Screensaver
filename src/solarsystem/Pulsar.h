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
#ifndef PULSAR_H_
#define PULSAR_H_

#include "Star.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Pulsar data type.
 *
 *      This class comprises the astronomical data relating to a specific pulsar.
 * It stores the catalog origin, or the deemed 'discoverer' of it.
 *
 * \see Star
 *
 * TODO - maybe add a member for other pulsar associated data, say spin
 *        frequency?
 *
 * \author Mike Hewson\n
 */

class Pulsar : public Star {
    public:
        /// Identifiers of pulsar catalogue source.
        enum pulsar_source {DUNNO,          // Don't know.
                            ATNF,           // ATNF catalog
                            PALFA,          // Pulsar ALFA.
                            FERMILAT,       // The GR detector on the Fermi satellite.
                            PMPS};          // Deep Multi-Beam ( Parkes ).

        /**
         * \brief Constructor, giving fully specified member values.
         *
         * \param ra - right ascension of the pulsar
         *
         * \param dec - declination of the pulsar
         *
         * \param nm - the name of the pulsar
         *
         * \param sc - the catalog/source of the pulsar
         */
        Pulsar(starpos_t ra, starpos_t dec, std::string nm, pulsar_source sc);

        /**
         * \brief Destructor
         */
        virtual ~Pulsar();

        /**
         * \brief Obtain the catalogue
         *
         * \return The catalog source
         */
        pulsar_source source(void) const;

    private:
        /// The catalog from which information was derived.
        pulsar_source p_source;
    };

/**
 * @}
 */

#endif /*PULSAR_H_*/
