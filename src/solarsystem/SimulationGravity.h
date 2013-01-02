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

#ifndef SIMULATION_GRAVITY_H_
#define SIMULATION_GRAVITY_H_

#include "Simulation.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Derived container/manager of scene renderable elements
 * specific to the gravitational wave variant of workunit.
 *
 * \see Simulation
 *
 * \author Mike Hewson\n
 */

class SimulationGravity : public Simulation {
    public:
        /**
         * \brief Constructor
         */
        SimulationGravity(void);

        /**
         * \brief Destructor
         */
        virtual ~SimulationGravity();

    private:
    };

/**
 * @}
 */

#endif /* SIMULATION_GRAVITY_H_*/

