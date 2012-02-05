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

#ifndef HUD_FLOW_LAYOUT_H_
#define HUD_FLOW_LAYOUT_H_

#include <algorithm>
#include <iostream>

#include "ErrorHandler.h"
#include "HUDContainer.h"
#include "HUDContent.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief The container type having a flow layout.
 *
 * \author Mike Hewson\n
 */

class HUDFlowLayout : public HUDContainer {
   public:
      // NB - only first three cases coded for currently !!!
      enum Justification {START, CENTRE, END, START_AND_END, SPAN};

      /**
       * \brief Constructor
       */
      HUDFlowLayout(HUDContainer* enclosing);

      /**
       * \brief Destructor
       */
      virtual ~HUDFlowLayout();

      /**
       * \brief Add renderable content to this container.
       *
       * \param content : a pointer to the desired content
       */
      void addContent(HUDContent* content);

      /**
       * \brief Set the justification along the primary flow axis.
       *
       * \param just : the desired justification
       */
      void setPrimaryJustification(Justification just);

      /**
       * \brief Set the justification along the secondary flow axis.
       *
       * \param just : the desired justification
       */
      void setSecondaryJustification(Justification just);

      /**
       * \brief Get the justification of the primary flow axis.
       *
       * \return the desired justification
       */
      HUDFlowLayout::Justification getPrimaryJustification(void) const;

      /**
       * \brief Get the justification of the secondary flow axis.
       *
       * \return the desired justification
       */
      HUDFlowLayout::Justification getSecondaryJustification(void) const;

   protected:
      /**
       * \brief Determine how many whitespace gaps as per the
       *        justification setting.
       *
       * \param the number of items in the layout.
       *
       * \return the gap count.
       */
      GLuint gapCount(GLuint numItems) const;

   private:
      Justification primary_just;
      Justification secondary_just;
   };

/**
 * @}
 */

#endif /* HUD_FLOW_LAYOUT_H_ */
