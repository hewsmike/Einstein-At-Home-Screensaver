/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

#include <oglft/OGLFT.h>

#include "AbstractGraphicsEngine.h"
#include "SSGObserver.h"
#include "Resource.h"

// SIN and COS take arguments in DEGREES
// TODO - Make these static constants.
#define PI 3.14159265
#define PI2 (2*PI)
// TODO - Make these static ( inline ? ) functions.
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

/// Convenience type for OGLFT font specification
typedef OGLFT::TranslucentTexture OGLFT_ft;

/**
 * \brief %SolarSystem Helper class with global definitions and useful static
 *        functions that I didn't put elsewhere. Didn't want to call
 *        it 'general miscellaneous', but of course may refactor in future. :-)
 */

class SolarSystemGlobals {
    public:
        // Levels of rendering quality.
        enum render_quality {RENDER_LOWEST, RENDER_MEDIUM, RENDER_HIGHEST};

        // Arbitrary distance units here.
        // How far away is the distant sky, plus sizes of the globes.
        static const GLuint CELESTIAL_SPHERE_RADIUS;
        static const GLuint EARTH_RADIUS;
        static const GLuint SUN_RADIUS;

        /// Useful degree measures of arc.
        static const GLfloat FULL_CIRCLE_DEG;
        static const GLfloat HALF_CIRCLE_DEG;
        static const GLfloat QUARTER_CIRCLE_DEG;
        static const GLfloat MINUTES_PER_HOUR;
        static const GLfloat SECONDS_PER_MINUTE;
        static const GLfloat MINUTES_PER_DEGREE;
        static const GLfloat DEGREES_PER_HOUR;

        /// Minimum accepted screen/window dimensions
        static const GLuint MIN_SCREEN_WIDTH;
        static const GLuint MIN_SCREEN_HEIGHT;

        /**
         * \brief Constructor.
         */
        SolarSystemGlobals();

        /**
         * \brief Destructor.
         */
        ~SolarSystemGlobals();

        /**
         * \brief Obtain the current render level setting
         *
         * \return : the current render level setting
         */
        static render_quality getRenderLevel(void);

        /**
         * \brief Register an SSGObserver object to be informed
         *        when the render quality is changes
         *
         * \param observer - pointer to the SSGObserver object to register
         */
        static void registerRenderQualityObserver(SSGObserver* observer);

        /**
         * \brief Unregister an SSGObserver object for callback
         *        when there is a change in render quality
         *
         * \param observer - pointer to the SSGObserver object to unregister
         */
        static void unRegisterRenderQualityObserver(SSGObserver* observer);

        /**
         * \brief Set the current render level setting
         *
         * \param rq - the desired render level setting
         */
        static void setRenderLevel(SolarSystemGlobals::render_quality rq);

    private:
        /// If needed, what is the preferred OpenGL compatibility mode?
        static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MAJOR;
        static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MINOR;

        /// How many color channels if an alpha channel included.
        static const GLuint ALPHA_CHANNEL;

        /// How many color channels if an alpha channel NOT included.
        static const GLuint NO_ALPHA_CHANNEL;

        /// The current quality level.
        static SolarSystemGlobals::render_quality qual;

        class ObsComp {
            public:
                bool operator()(SSGObserver* s1, SSGObserver* s2) {
                    if(s1 < s2) {
                        return true;
                        }
                    else {
                        return false;
                        }
                    }
            };

        /// The set of observers to inform of any
        /// change to the rendering quality setting.
        static std::set<SSGObserver*, ObsComp> quality_observers;
    };

/**
 * @}
 */

#endif /*SOLARSYSTEM_GLOBALS_H_*/
