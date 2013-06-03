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

#ifndef SOLARSYSTEM_GLOBALS_H_
#define SOLARSYSTEM_GLOBALS_H_

#include "framework.h"

#include <string>
#include <set>
#include <utility>

#include <oglft/OGLFT.h>

#include "AbstractGraphicsEngine.h"
#include "RenderQualityObserver.h"
#include "Resource.h"
#include "WindowManager.h"

// SIN and COS take arguments in DEGREES
/// TODO - Make these static constants.
#define PI 3.14159265
#define PI2 (2*PI)
/// TODO - Make these into ( static ? inline ? ) functions.
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

/// Convenience type for OGLFT font specification
typedef OGLFT::MonochromeTexture OGLFT_ft;

/**
 * \brief %SolarSystem Helper class with global definitions and useful static
 *        functions that I didn't put elsewhere. Didn't want to call
 *        it 'general miscellaneous', but of course may refactor in future. :-)
 */

class SolarSystemGlobals {
    public:
        /// Enumerants for the scene elements
        enum content {AXES,
                      CONSTELLATIONS,
                      EARTH,
                      EARTH_GRID,
                      SUN,
                      SKY_GRID,
                      PULSARS,
                      SUPERNOVAE,
                      HUDOVER,
                      AUTOPILOT,
                      TARGET_RETICLE,
                      RA_UNITS,
                      HELP_HUD};

        // Levels of rendering quality.
        enum render_quality {RENDER_LOWEST, RENDER_MEDIUM, RENDER_HIGHEST};

        // Arbitrary distance units here.
        // How far away is the distant sky, plus sizes of the globes.
        static const GLfloat CELESTIAL_SPHERE_RADIUS;
        static const GLfloat EARTH_RADIUS;
        static const GLfloat SUN_RADIUS;

        /// Useful degree measures of arc.
        static const GLfloat FULL_CIRCLE_DEG;
        static const GLfloat HALF_CIRCLE_DEG;
        static const GLfloat QUARTER_CIRCLE_DEG;
        static const GLfloat MINUTES_PER_HOUR;
        static const GLfloat HOURS_PER_DAY;
        static const GLfloat SECONDS_PER_MINUTE;
        static const GLfloat MINUTES_PER_DEGREE;
        static const GLfloat DEGREES_PER_HOUR;

        /// Minimum accepted screen/window dimensions
        static const GLuint MIN_SCREEN_WIDTH;
        static const GLuint MIN_SCREEN_HEIGHT;

        virtual ~SolarSystemGlobals();

        /**
         * \brief Obtain the current render level setting
         *
         * \return : the current render level setting
         */
        static render_quality getRenderLevel(void);

        /**
         * \brief Register a RenderQualityObserver object to be informed
         *        when the render quality is changes
         *
         * \param observer - pointer to the RenderQualityObserver object to register
         */
        static void registerRenderQualityObserver(RenderQualityObserver* observer);

        /**
         * \brief Unregister a RenderQualityObserver object for callback
         *        when there is a change in render quality
         *
         * \param observer - pointer to the RenderQualityObserver object to unregister
         */
        static void unRegisterRenderQualityObserver(RenderQualityObserver* observer);

        /**
         * \brief Set the current render level setting
         *
         * \param rq - the desired render level setting
         */
        static void setRenderLevel(SolarSystemGlobals::render_quality rq);

        /**
         * \brief Record the current display mode.
         *
         * \param one of the displaymode enumerants as above.
         */
        static void setDisplayMode(WindowManager::displaymode mode);

        /**
         * \brief Obtain the current display mode.
         *
         * \return one of the displaymode enumerants as above.
         */
        static WindowManager::displaymode getDisplayMode(void);

    private:
        /// Prevent direct construction, copying and assignment.
        SolarSystemGlobals();
        SolarSystemGlobals(const SolarSystemGlobals& other);
        SolarSystemGlobals& operator=(const SolarSystemGlobals& other);

        /// If needed, what is the preferred OpenGL compatibility mode?
        static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MAJOR;
        static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MINOR;

        /// How many color channels if an alpha channel included.
        static const GLuint ALPHA_CHANNEL;

        /// How many color channels if an alpha channel NOT included.
        static const GLuint NO_ALPHA_CHANNEL;

        /// The current quality level.
        static SolarSystemGlobals::render_quality qual;

        /// The current display mode.
        static WindowManager::displaymode operating_mode;

        /// The set of observers to inform of any
        /// change to the rendering quality setting.
        static std::set<RenderQualityObserver*> quality_observers;

        /// The current window dimensions.
        static GLuint width;
        static GLuint height;
    };

/**
 * @}
 */

#endif /*SOLARSYSTEM_GLOBALS_H_*/
