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

#ifndef SIMULATION_GAMMA_H_
#define SIMULATION_GAMMA_H_

#include "Simulation.h"

#include "EinsteinRadioAdapter.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Derived container/manager of scene renderable elements
 * specific to the gamma pulsar variant of workunit.
 *
 * \see Simulation
 *
 * \author Mike Hewson\n
 */

class SimulationGamma : public Simulation {
    public:
        /**
         * \brief Constructor
         */
        SimulationGamma(BOINCClientAdapter* boinc_adapter, EinsteinRadioAdapter* adapter);

        /**
         * \brief Destructor
         */
        virtual ~SimulationGamma();

    protected:
        // Satisfy Simulation interface methods.
        void includeLogo(HUDFlowLayout* container);

        void includeSearchInformation(HUDLogoCycle* container);

        void renderObservatories(void);

    private:
        /// TODO Make a gamma adapter class etc ....
        EinsteinRadioAdapter* EG_adapter;
    };

/**
 * @}
 */

#endif /* SIMULATION_GAMMA_H_*/

