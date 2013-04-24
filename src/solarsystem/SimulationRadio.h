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

#ifndef SIMULATION_RADIO_H_
#define SIMULATION_RADIO_H_

#include "Simulation.h"

#include "EinsteinRadioAdapter.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Derived container/manager of scene renderable elements
 * specific to the radio pulsar variant of workunit.
 *
 * \see Simulation
 *
 * \author Mike Hewson\n
 */

class SimulationRadio : public Simulation {
    public:
        /**
         * \brief Constructor
         */
        SimulationRadio(BOINCClientAdapter* boinc_adapter, EinsteinRadioAdapter* adapter);

        /**
         * \brief Destructor
         */
        virtual ~SimulationRadio();

    protected:
        // Satisfy Simulation interface methods.
        void includeLogo(HUDFlowLayout* container);

        void includeSearchInformation(HUDLogoCycle* container);

        void renderObservatories(void);

    private:
        EinsteinRadioAdapter* ER_adapter;
    };

/**
 * @}
 */

#endif /* SIMULATION_RADIO_H_*/
