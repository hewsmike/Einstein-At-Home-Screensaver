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
#ifndef HUD_FLOW_LAYOUT_H_
#define HUD_FLOW_LAYOUT_H_

#include "HUDContainer.h"
#include "HUDContent.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class enacts a container type having a 'flow' layout.
 *
 *      Thus for a horizontal flow layout any added items are placed from
 * left to right in the order of their insertion ie. the most recently added
 * item is placed to the right side of existing items. Similiarly the vertical
 * flow layout has the items placed from top to bottom in the order of their
 * insertion ie. the most recently added item is placed below existing items.
 *
 * \see HUDContainer
 * \see HUDContent
 *
 * \author Mike Hewson\n
 */

class HUDFlowLayout : public HUDContainer {
    public:
        /**
         * \brief Constructor
         */
        HUDFlowLayout(Axis axis);

        /**
         * \brief Destructor
         */
        virtual ~HUDFlowLayout();

        /**
         * \brief Add content to this container, which maybe another container.
         *
         * \param content : a pointer to the desired content
         */
        virtual void addItem(HUDItem* item);

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
        /** brief Determine how many of, and how wide are, the
         *        whitespace gaps as per the justification setting.
         */
        void setPrimaryAxisGaps(void);

        GLuint primary_axis_gap_count;
        GLuint primary_axis_total_white_space;
        GLint primary_axis_start_offset;
        GLint primary_axis_item_gap;
    };

/**
 * @}
 */

#endif /* HUD_FLOW_LAYOUT_H_ */
