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
      enum primaryJustification {START, CENTRE, END, START_AND_END, SPAN};

      enum secondaryJustification {PROXIMAL, MIDDLE, DISTAL};

      enum Axis {HORIZONTAL, VERTICAL};

      /**
       * \brief Constructor
       */
      HUDFlowLayout(HUDContainer* enclosing, Axis axis);

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
      void setPrimaryJustification(primaryJustification just);

      /**
       * \brief Set the justification along the secondary flow axis.
       *
       * \param just : the desired justification
       */
      void setSecondaryJustification(secondaryJustification just);

      /**
       * \brief Get the justification of the primary flow axis.
       *
       * \return the desired justification
       */
      HUDFlowLayout::primaryJustification getPrimaryJustification(void) const;

      /**
       * \brief Get the justification of the secondary flow axis.
       *
       * \return the desired justification
       */
      HUDFlowLayout::secondaryJustification getSecondaryJustification(void) const;

   protected:
      /**
       * \brief Determine the miminum dimensions as per the internal
       *        layout of items within. Enacts HUDContainer interface.
       *
       * \return std::pair composed of ( in order )
       *          - the desired width
       *          - the desired height
       */
      virtual std::pair<GLuint, GLuint> reassessMinimumDimensions(void);

      /**
       * \brief Determine and set the base positions of each item.
       *        Enacts HUDContainer interface.
       */
      virtual void allocateItemBases(void);

   private:
      /* brief Determine how many of, and how wide, whitespace gaps as
       *       per the justification setting.
       */
      void setGaps(void);

      GLuint gap_count;
      GLuint total_white_space;
      GLuint start_offset;
      GLuint item_gap;

      Axis ax;
      primaryJustification primary_just;
      secondaryJustification secondary_just;
   };

/**
 * @}
 */

#endif /* HUD_FLOW_LAYOUT_H_ */
