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

#ifndef STARSPHERE_GRAVITY_H_
#define STARSPHERE_GRAVITY_H_

#include <sstream>

#include "Starsphere.h"
#include "EinsteinGravityAdapter.h"
#include "TexturedParallelogram.h"

// #include "TextString.h"

using namespace std;

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief Specialized rendering engine for the S5R3 science run
 *
 *      This class comprises the specialized parts of the Starsphere rendering
 * engine. The main differences stem from the fact that most science runs differ
 * in their search configuration and parameters. Thus the parameters exposed by
 * the HUD (head-up display) are positioned and rendered here. For the time
 * being the "BOINC Statistics" are right-aligned to the "Search Parameters",
 * hence they're also positioned and rendered here.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class StarsphereGravity : public Starsphere {
    public:
        /// Default constructor
        StarsphereGravity();

        /// Destructor
        virtual ~StarsphereGravity();

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
        virtual void resize(const int width, const int height);

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
         * Starsphere::refreshLocalBOINCInformation() first and "adds" the sepcialized
         * parts afterwards.
         *
         * \see AbstractGraphicsEngine::refreshBOINCInformation()
         * \see Starsphere::refreshLocalBOINCInformation()
         */
        void refreshBOINCInformation();

    private:
        // For scinece run specific logo
        TexturedParallelogram* m_logo;

        /// For celestial sphere target reticle
        TexturedParallelogram* m_target;

        SDL_Surface* m_text_surface;

        /**
         * \brief Render science run specific logo
         *
         * This specific implementation shows the OpenCL logo.
         */
        void prepareLogo(void);

        /**
         * \brief Render science run specific search information
         *
         * For this specific implementation this also includes the "BOINC Statistics"
         * as it is top-aligned to the "Search Information".
         */
        void prepareSearchInformation(void);

        /**
         * \brief Target reticle as per search parameters
         */
        void prepareTargetReticle(void);

        ///
        TexturedParallelogram* m_right_ascension_info;
        TexturedParallelogram* m_declination_info;
        TexturedParallelogram* m_percent_done_info;
        TexturedParallelogram* m_cpu_time_info;

        /// Active render quality setting
        BOINCClientAdapter::GraphicsQualitySetting m_QualitySetting;

        /// Specialized BOINC client adapter instance for information retrieval
        EinsteinGravityAdapter m_EinsteinAdapter;

        /// Local copy of the current WU's search parameter "Right-Ascension" (degrees)
        string m_WUSkyPosRightAscension;

        /// Local copy of the current WU's search parameter "Declination" (degrees)
        string m_WUSkyPosDeclination;

        /// Local copy of the current WU's search parameter "Percent done"
        string m_WUPercentDone;

        /// Local copy of the current WU's search parameter "CPU Time"
        string m_WUCPUTime;
    };

/**
 * @}
 */

#endif /*STARSPHERE_GRAVITY_H_*/
