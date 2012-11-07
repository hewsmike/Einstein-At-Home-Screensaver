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
 *      The essential reason for this class is that on Windows platforms
 * the OpenGL context is destroyed, and a new one created, when a window
 * is resized. Thus for some objects who haved acquired persistent OpenGL
 * server assets eg. a buffer object on the video card memory, these are lost
 * and need to be re-acquired. While this a superclass of the Renderable type,
 * not all Renderable derived types use persistent OpenGL server assets, and
 * thus only some will need to be informed of a change in window dimensions.
 *      The default behaviour on resize is to do nothing, and may be over-ridden
 * by a virtual function in a derived class to suit. This ought be the class
 * that provides the actual definitions of prepare/render/release as per the
 * Renderable interface, or at least a class derived from such.
 *      For instance the HUDImage class has a non-trivial content for the
 * resizeUpdate() member on Windows machines ( by conditional compilation ).
 *      Of course one could do other things upon a resize signal, regardless
 * of the target operating system.
 *
 * \see HUDImage
 * \see Renderable
 * \see SolarSystemGlobals
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
         * this behaviour to suit.
         */
        virtual void resizeUpdate(GLuint width, GLuint height);

    private:
        bool attach_flag;
    };

/**
 * @}
 */

#endif // RESIZE_OBSERVER_H_
