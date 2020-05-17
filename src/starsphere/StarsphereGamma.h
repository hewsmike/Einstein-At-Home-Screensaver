/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   Copyright (C) 2020 by Mike Hewson                                     *
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

#ifndef STARSPHERE_GAMMA_H_
#define STARSPHERE_GAMMA_H_

#include <string>

#include "Starsphere.h"
#include "EinsteinGammaAdapter.h"
#include "TexturedParallelogram.h"

using namespace std;

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief Specialized rendering engine for the "Fermi LAT Search" science run
 *
 * This class comprises the specialized parts of the Starsphere rendering engine.
 * The main differences stem from the fact that most science runs differ in their
 * search configuration and parameters. Thus the parameters exposed by the HUD
 * (head-up display) are positioned and rendered here.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class StarsphereGamma : public Starsphere {
    public:
        /// Default contructor
        StarsphereGamma();

        /// Destructor
        virtual ~StarsphereGamma();

        /**
         * \brief This method is called to initialize the engine
         *
         * As this method overrides its parent's implementation, it calls Starsphere::initialize()
         * first in order to "add" the specialized parts afterwards.
         *
         * \param width The current width of the display surface
         * \param height The current height of the display surface
         * \param font A pointer to a Resource object containing TTF font faces for text rendering
         */
        virtual void initialize(const int width, const int height, const Resource* font);

        /**
         * \brief This method is called when the windowing system encounters a window resize event
         *
         * As this method overrides its parent's implementation, it calls Starsphere::resize()
         * first in order to "add" the specialized parts afterwards.
         *
         * \param width The new width of the display surface
         * \param height The new height of the display surface
         */
        void resize(const int width, const int height);

        /**
         * \brief This method is called when the windowing system encounters a window render event
         *
         * As this method overrides its parent's implementation, it calls Starsphere::render()
         * first in order to "add" the specialized parts afterwards.
         *
         * \param timeOfDay The time of day
         */
        virtual void render(const double timeOfDay);

        /**
         * \brief This method is called when the BOINC client information should be updated
         *
         * This method implements AbstractGraphicsEngine::refreshBOINCInformation() and calls
         * Starsphere::refreshLocalBOINCInformation() first and "adds" the specialized
         * parts afterwards.
         *
         * \see AbstractGraphicsEngine::refreshBOINCInformation()
         * \see Starsphere::refreshLocalBOINCInformation()
         */
        void refreshBOINCInformation();

    private:
        TexturedParallelogram* m_logo;

        /**
         * \brief Prepare science run specific logo
         *
         */
        void prepareLogo();

        /**
         * \brief Prepare science run specific search information
         *
         */
        void prepareSearchInformation();

        ///
        TexturedParallelogram* m_right_ascension_info;
        TexturedParallelogram* m_declination_info;
        TexturedParallelogram* m_percent_done_info;
        TexturedParallelogram* m_cpu_time_info;

        /// Active render quality setting
        BOINCClientAdapter::GraphicsQualitySetting m_QualitySetting;

        /// Specialized BOINC client adapter instance for information retrieval
        EinsteinGammaAdapter m_EinsteinAdapter;

        /// Formatted string copy of the current WU's search parameter "Right-Ascension" (degrees)
        string m_WUSkyPosRightAscension;

        /// Formatted string copy of the current WU's search parameter "Declination" (degrees)
        string m_WUSkyPosDeclination;

        /// Formatted string copy of the current WU's search parameter "Percent done"
        string m_WUPercentDone;

        /// Formatted string copy of the current WU's search parameter "CPU Time"
        string m_WUCPUTime;
    };

/**
 * @}
 */

#endif /*STARSPHERE_GAMMA_H_*/