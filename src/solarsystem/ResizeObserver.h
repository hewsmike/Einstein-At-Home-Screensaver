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
#ifndef RESIZE_OBSERVER_H_
#define RESIZE_OBSERVER_H_

#include "framework.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A virtual base class for resize observers of the static
 *        SolarSystemGlobals object.
 *
 * \author Mike Hewson\n
 */

class ResizeObserver {
    public:
        /**
         * \brief Constructor
         */
        ResizeObserver();

        /**
         * \brief Destructor
         */
        virtual ~ResizeObserver();

        /**
         * \brief Attach to the static SolarSystemGlobals instance.
         */
        void attachResize(void);

        /**
         * \brief Detach from the static SolarSystemGlobals instance.
         */
        void detachResize(void);

        /**
         * \brief Query attachment to the static SolarSystemGlobals instance.
         *
         * \return a boolean indicating - true, if attached
         *                              - false, if not attached
         */
        bool isAttachedResize(void) const;

        /**
         * \brief Callback target for the static SolarSystemGlobals instance.
         *
         *      This routine is called when there is a change in the window
         * size. Default is to do nothing, but a derived class may re-define
         * this behaviour.
         */
        virtual void resizeUpdate(void);

    private:
        bool attach_flag;
    };

/**
 * @}
 */

#endif // RESIZE_OBSERVER_H_
