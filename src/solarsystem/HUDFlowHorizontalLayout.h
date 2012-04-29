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

#ifndef HUD_FLOW_HORIZONTAL_LAYOUT_H_
#define HUD_FLOW_HORIZONTAL_LAYOUT_H_

#include "HUDFlowLayout.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief The container type having a horizontal flow layout.
 *
 * \author Mike Hewson\n
 */

class HUDFlowHorizontalLayout : public HUDFlowLayout {
   public:
      /**
       * \brief Constructor
       */
      HUDFlowHorizontalLayout(HUDContainer* enclosing);

      /**
       * \brief Destructor
       */
      virtual ~HUDFlowHorizontalLayout();

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
   };

/**
 * @}
 */

#endif /* HUD_FLOW_HORIZONTAL_LAYOUT_H_ */
