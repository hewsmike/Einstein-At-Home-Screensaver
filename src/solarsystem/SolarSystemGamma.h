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

#ifndef SOLARSYSTEM_GAMMA_H_
#define SOLARSYSTEM_GAMMA_H_

#include "SimulationGamma.h"
#include "SolarSystem.h"
#include "EinsteinRadioAdapter.h"

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

/**
 * \brief Specialized rendering engine for the gamma-ray pulsar searches
 *
 * This class comprises the specialized parts of the SolarSystem rendering engine.
 * The main differences stem from the fact that most science runs differ in their
 * search configuration and parameters. Thus the parameters exposed by the HUD
 * (head-up display) are positioned and rendered here. For the time being the
 * "BOINC Statistics" are top-aligned to the "Search Parameters", hence they're
 * also positioned and rendered here.
 *
 */
class SolarSystemGamma : public SolarSystem {
    public:
       /// Default contructor
       SolarSystemGamma();

       /// Destructor
       virtual ~SolarSystemGamma();

       /**
        * \brief This method is called to initialize the engine
        *
        * As this method overrides its parent's implementation, it calls SolarSystem::initialize()
        * first in order to "add" the specialized parts afterwards.
        *
        * \param width The current width of the display surface
        * \param height The current height of the display surface
        * \param font A pointer to a Resource object containing TTF font faces for text rendering
        * \param recycle This flag indicates whether we initialize (FALSE) or reinitialize (TRUE) the context
        */
       virtual void initialize(const int width, const int height, const Resource *font, const bool recycle = false);

       /**
        * \brief This method is called when the windowing system encounters a window resize event
        *
        * As this method overrides its parent's implementation, it calls SolarSystem::resize()
        * first in order to "add" the specialized parts afterwards.
        *
        * \param width The new width of the display surface
        * \param height The new height of the display surface
        */
       void resize(const int width, const int height);

       /**
        * \brief This method is called when the BOINC client information should be updated
        *
        * This method implements AbstractGraphicsEngine::refreshBOINCInformation() and calls
        * SolarSystem::refreshLocalBOINCInformation() first and "adds" the sepcialized
        * parts afterwards.
        *
        * \see AbstractGraphicsEngine::refreshBOINCInformation()
        * \see SolarSystem::refreshLocalBOINCInformation()
        */
       void refreshBOINCInformation(void);

    protected:
        virtual Simulation& simulationInstance(void);

    private:
        /// Specialized BOINC client adapter instance for information retrieval
        EinsteinRadioAdapter m_EinsteinAdapter;

        /// The 3D scene objects to be rendered.
        SimulationGamma sim;
    };

/**
 * @}
 */

#endif /*SOLARSYSTEM_GAMMA_H_*/
