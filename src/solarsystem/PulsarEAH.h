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

#ifndef PULSAR_EAH_H_
#define PULSAR_EAH_H_

#include "Pulsar.h"

#include <vector>

#include "HUDImage.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Pulsar data type with Einstein At Home co-discovery.
 *
 *      This class comprises the astronomical data relating to a pulsar,
 * newly discovered with the assistance of Einstein At Home. This does not
 * include the re-discovery of prior known pulsars.
 *
 * \author Mike Hewson\n
 */

class PulsarEAH : public Pulsar {
    public:
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
        PulsarEAH(starpos_t ra, starpos_t dec, std::string nm, pulsar_source sc);

        /**
         * \brief Destructor
         */
        virtual ~PulsarEAH();

        const std::vector<std::string>& PulsarEAH::getDescription(void) const;

    private:
        /// The list of associated discoverers ie. Einstein At Home contributors.
        std::vector<std::string> desc;

        std::string attribution_date;

        HUDImage* pulse_profile;

        double pulse_frequency;

        double pulse_period;

        double dispersion_measure;
    };

/**
 * @}
 */

#endif /*PULSAR_EAH_H_*/
