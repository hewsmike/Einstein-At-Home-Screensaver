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

#ifndef SOLARSYSTEM_H_
#define SOLARSYSTEM_H_

#include "framework.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <util.h>

#include <oglft/OGLFT.h>

#include "AbstractGraphicsEngine.h"
#include "Craft.h"
#include "RenderQualityObserver.h"
#include "Simulation.h"

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

/**
 * \brief %SolarSystem rendering engine for \b Einstein\@Home
 *
 * This class comprises the generic parts of the %SolarSystem rendering engine.
 * %SolarSystem. Furthermore a head-up display (HUD) shows relevant BOINC
 * statistics as well as information on the current workunit (WU) being processed.
 *
 * Note: all science run specific parts are implemented in specialized subclasses
 * of this engine.
 *
 */
class SolarSystem : public AbstractGraphicsEngine, public RenderQualityObserver {
    public:
        // How many times the celestial sphere radius do we look at?
        static const int FAR_LOOK_RATIO;

        /// Destructor
        virtual ~SolarSystem();

        /**
         * \brief This method is called to initialize the engine
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
         * \param width The new width of the display surface
         * \param height The new height of the display surface
         */
        virtual void resize(const int width, const int height);

        /**
         * \brief This method renders one frame of the animation
         *
         */
        void render(const double timeOfDay);

        /**
         * \brief Event handler for mouse button events
         *
         * \param positionX The mouse position's x-coordinate
         * \param positionY The mouse position's y-coordinate
         * \param buttonPressed The mouse button pressed
         */
        void mouseButtonEvent(const int positionX,
                              const int positionY,
                              const AbstractGraphicsEngine::MouseButton buttonPressed);

        /**
         * \brief Event handler for mouse move events
         *
         * \param deltaX The relative mouse movement in the x-direction
         * \param deltaY The relative mouse movement in the y-direction
         * \param buttonPressed The mouse button pressed
         */
        void mouseMoveEvent(const int deltaX,
                            const int deltaY,
                            const AbstractGraphicsEngine::MouseButton buttonPressed);

        /**
         * \brief Event handler for key press events
         *
         * \param keyPressed The key pressed
         */
        void keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed);

        /**
         * \brief Receives any rendering quality level change from the
         *        static SolarSystemGlobals instance.
         *
         *      Satisfies RenderQualityObserver interface.
         */
        virtual void renderUpdate(void);

    protected:
        /**
         * \brief Constructor
         *
         * The constructor is protected since this is an abstract class. It takes
         * as an argument the name of the shared memory area which is propagated
         * to the BOINC client adapter instance (during construction).
         *
         * \param sharedMemoryIdentifier The identifier of the shared memory area
         *
         * \see BOINCClientAdapter::BOINCClientAdapter()
         */
        SolarSystem(string sharedMemoryIdentifier);

        virtual void refreshLocalBOINCInformation(void);

        virtual Simulation& simulationInstance(void) = 0;

        /// Current window width (x-resolution)
        int m_CurrentWidth;

        /// Current window height (y-resolution)
        int m_CurrentHeight;

        /// Current window aspect ration
        float aspect;

        /// Font resource instance
        const Resource* spaceFontResource;

        /// Font texture instances for in-simulation rendering
        OGLFT_ft* skygridFont;
        OGLFT_ft* earthgridFont;
        OGLFT_ft* constellationFont;
        OGLFT_ft* HUDFont;

    private:
        void changeRenderQuality(void);

        bool renderUpdateFlag;

        /// Overall rendering parameters.
        static const GLdouble FOV_ANGLE;
        static const GLdouble NEAR_CLIP;
        static const GLdouble FAR_CLIP;
        static const int FAR_LOOK_DISTANCE;
    };

/**
 * @}
 */

#endif /*SOLARSYSTEM_H_*/
