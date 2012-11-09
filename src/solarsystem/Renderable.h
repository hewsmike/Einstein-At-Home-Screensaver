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

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "framework.h"

#include <iostream>

#include <oglft/OGLFT.h>

#include "RenderQualityObserver.h"
#include "SolarSystemGlobals.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares all mandatory public methods to be provided by
 * any given OpenGL Renderable object.
 *
 *      One can compose a given scene element with several Renderable objects.
 * The intent of encapsulating is to ensure proper acquisition and release of OpenGL
 * resources. If the existing global render quality setting is altered then current
 * resources are released, new resources are acquired as per needs and rendering
 * continues with said resources. This class does not deal with any specific OpenGL
 * mechanisms.
 *
 *      Having said that, it is IMPERATIVE that any most-derived class
 * containing an actual implementation of release(), call release in it's
 * destructor to ensure release of OpenGL resources.
 *
 *      Addition of show/hide feature which enables control of drawing without
 * loading/unloading of resources. Use if you want persistent resource acquisition
 * but optional rendering. Default upon construction is to show.
 *
 * \author Mike Hewson\n
 */

class Renderable : public RenderQualityObserver {
    public:
        // Visibility ie. to be rendered at all?
        enum activity_state {INACTIVE, ACTIVE};

        /// Virtual destructor
        virtual ~Renderable();

        /**
         * \brief Invoke display of the object within the scene according to
         *        it's activation and show state.
         */
        void draw(void);

        /**
         * \brief Mark object to have resources acquired.
         */
        void activate(void);

        /**
         * \brief Mark object to have resources de-allocated.
         */
        void inactivate(void);

        /**
         * \brief Retrieves the activation state.
         *
         * \return The activation state.
         */
        Renderable::activity_state isActivated() const;

        /**
         * \brief Cycles the activation state of the object.
         */
        virtual void cycleActivation(void);

        /**
         * \brief Allow the object to be shown if activated.
         */
        void show(void);

        /**
         * \brief Allow the object to not be shown even if activated.
         */
        void hide(void);

        /**
         * \brief Toggle the show state of the object.
         */
        void toggleShow(void);

        /**
         * \brief Report on the show state of the object.
         *
         * \return a boolean indicating - true if to be shown
         *                              - false if to be hidden
         */
        bool isShown(void) const;

        /**
         * \brief Receives any rendering quality level change from the
         *        static SolarSystemGlobals instance.
         *
         *      Satisfies RenderQualityObserver interface.
         */
        virtual void renderQualityUpdate(void);

        /**
         * \brief Sets the font.
         *
         *      ABSOLUTE need to set the font BEFORE activation or you won't see any text!
         */
        void setFont(OGLFT_ft* a_font);

        /**
         * \brief Obtain the font.
         */
        OGLFT_ft* getFont(void) const;

    protected:
        /**
         * \brief Constructor.
         *
         *      The constructor is protected since this is an abstract class.
         */
        Renderable();

        /// In subclasses this will provide OpenGL code
        /// to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq) = 0;

        /// In subclasses this will provide OpenGL code
        /// to release any resources used.
        virtual void release(void) = 0;

        /// In subclasses this will provide OpenGL code
        /// to render the object.
        virtual void render(void) = 0;

    private:
        /// I don't want any copying or assignment.
        Renderable(const Renderable&);
        Renderable& operator=(const Renderable&);

        /// Indicates whether object is activated.
        activity_state activity;

        /// Indicates if the object is to be shown, if activated.
        bool show_flag;

        /// Current choice of rendering quality.
        SolarSystemGlobals::render_quality quality;

        /// The font to use for any rendering.
        OGLFT_ft* myFont;
    };

/**
 * @}
 */

#endif /*RENDERABLE_H_*/
