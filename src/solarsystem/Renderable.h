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

#include "SolarSystemGlobals.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares all mandatory public methods to be provided by
 * any given OpenGL Renderable object. One can compose a given scene element
 * with several Renderable objects. The intent of encapsulating is to ensure
 * proper acquisition and release of OpenGL resources. If the existing global
 * render quality setting is altered then current resources are released,
 * new resources are acquired as per needs and rendering continues with said
 * resources. This class does not deal with any specific OpenGL mechanisms.
 *
 * \author Mike Hewson\n
 */

class Renderable {
   public:
      // Visibility ie. to be rendered at all?
      enum activity_state {INACTIVE, ACTIVE};

      /// Virtual destructor
      virtual ~Renderable();

      /**
       * \brief Invoke display of the object within the scene according to it's
       *        activation state.
       */
      void draw(void);

      /**
       * \brief Mark object as to be shown
       */
      void activate(void);

      /**
       * \brief Mark object as to NOT be shown
       */
      void inactivate(void);

      /**
       * \brief Retrieves the activation state
       *
       * \return The activation state
       */
      Renderable::activity_state is_activated() const;

      /**
       * \brief Cycles the activation state of the object
       */
      virtual void cycleActivation(void);

      /**
       * \brief Retrieves the rendering quality level
       */
      SolarSystemGlobals::render_quality render_level(void);

      /**
       * \brief Sets the rendering quality level
       */
      void set_render_level(SolarSystemGlobals::render_quality rq);

      /**
       * \brief Sets the font. ABSOLUTE need to set the font BEFORE
       * activation or you won't see any text!
       */
      void setFont(OGLFT_ft* a_font);

      /**
       * \brief Gets the font.
       */
      OGLFT_ft* getFont(void) const;

   protected:
      /**
       * \brief Constructor
       *
       * The constructor is protected since this is an abstract class.
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
      /// Indicates whether object is to be shown at all.
      activity_state activity;

      /// Current choice of rendering quality.
      SolarSystemGlobals::render_quality quality;

      /// The font to use for any rendering.
      OGLFT_ft* myFont;
   };

/**
 * @}
 */

#endif /*RENDERABLE_H_*/
