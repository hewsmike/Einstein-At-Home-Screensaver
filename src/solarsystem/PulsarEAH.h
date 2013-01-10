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

#ifndef PULSAR_EAH_H_
#define PULSAR_EAH_H_

#include "Pulsar.h"

#include <string>

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
 * include the re-discovery of known pulsars.
 *
 * \author Mike Hewson\n
 */

class PulsarEAH : public Pulsar {
    public:
        /**
         * \brief Constructor, giving fully specified member values.
         *
         * \param right_asc - right ascension of the pulsar
         *
         * \param decl - declination of the pulsar
         *
         * \param name - the name of the pulsar
         *
         * \param src - the catalog/source of the pulsar
         *
         * \param period - the period in milliseconds
         *
         * \param disp_mod - the dispersion modulus
         *
         * \param dist - the distance in kiloparsecs
         *
         * \param disc - the E@H volunteers who helped discover
         */
        PulsarEAH(starpos_t right_asc,
                  starpos_t decl,
                  std::string name,
                  pulsar_source src,
                  float period,
                  float disp_mod,
                  float dist,
                  const std::string& disc);

        /**
         * \brief Destructor
         */
        virtual ~PulsarEAH();

        unsigned int raHours(void) const;

        unsigned int raMinutes(void) const;

        float period(void) const;

        float dispersionModulus(void) const;

        float distance(void) const;

        std::string discoverers(void) const;

    private:
        unsigned int ra_hours;
        unsigned int ra_minutes;

        /// The period of the pulsar in milliseconds.
        double pulse_period;

        /// The dispersion modulus of the pulsar.
        double dispersion_modulus;

        /// The distance in kiloparsecs.
        float kp_distance;

        /// The list of associated discoverers ie. Einstein At Home
        /// contributors upon whose machines the relevant WU's
        /// were done.
        std::string volunteers;
    };

/**
 * @}
 */

#endif /*PULSAR_EAH_H_*/
