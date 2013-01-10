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
#ifndef RENDER_QUALITY_OBSERVER_H_
#define RENDER_QUALITY_OBSERVER_H_

#include "framework.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A virtual base class for observers of the static SolarSystemGlobals
 *        object, that are interested in changes in render quality.
 *
 * \author Mike Hewson\n
 */

class RenderQualityObserver {
    public:
        /**
         * \brief Constructor
         */
        RenderQualityObserver();

        /**
         * \brief Destructor
         */
        virtual ~RenderQualityObserver();

        /**
         * \brief Attach to the static SolarSystemGlobals instance.
         */
        void attachRender(void);

        /**
         * \brief Detach from the static SolarSystemGlobals instance.
         */
        void detachRender(void);

        /**
         * \brief Query attachment to the static SolarSystemGlobals instance.
         *
         * \return a boolean indicating - true, if attached
         *                              - false, if not attached
         */
        bool isAttachedRender(void) const;

        /**
         * \brief Callback target for the static SolarSystemGlobals instance.
         *
         *      This routine is called when there is a change in the render
         * quality setting. Default is to do nothing, but a derived class
         * may re-define this behaviour.
         */
        virtual void renderQualityUpdate(void);

    private:
        bool attach_flag;
    };

/**
 * @}
 */

#endif // RENDER_QUALITY_OBSERVER_H_
