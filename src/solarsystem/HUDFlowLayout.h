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
 *      Either a horizontal or vertical axis of flow is chosen upon
 * construction, which indicates the direction along which to place
 * any items later added. Thus for a horizontal flow layout any added
 * items are placed from left to right in the order of their insertion
 * ie. the most recently added item is placed to the right side of
 * existing items. Similiarly the vertical flow layout has the items
 * placed from top to bottom in the order of their insertion ie. the
 * most recently added item is placed below existing items.
 *
 *      For a given direction of flow then there are several choices of
 * policy to determine the distribution of spacing ( or justification )
 * between the items :
 *          - START : places all items to the left side in the case
 *            of horizontal flow, or the top side in the case of
 *            vertical flow, with any remaining space placed to
 *            the right or below respectively.
 *          - CENTRE : within rounding, the items are placed together
 *            in the centre of the container, with equal space allotted
 *            to left and right sides in the case of horizontal flow,
 *            or equally above and below in the case of vertical flow.
 *          - END : similiar to START except item placement is to the
 *            right side for horizontal flow, and at the bottom in the
 *            case of vertical flow, with any extra space being allocated
 *            to the left and above respectively.
 *          - START_AND_END : all extra space is allocated in the gaps
 *            between items, equally within rounding, and no space at
 *            either extreme of the container. If only one item present
 *            then behaves like a START case.
 *          - SPAN : similiar to START_AND_END except some gap, equal to
 *            within rounding, at the extreme margins. If only one item
 *            present then behaves like a CENTRE case.
 *
 *      This feature has not yet been implemented. The distribution
 * of spacing ( or justification ) along the axis orthogonal to the
 * primary flow has these policies :
 *          - PROXIMAL : whereby items abut the upper edge of the
 *            container in the case of horizontal flow, or the left
 *            edge of the container in the case of vertical flow.
 *          - MIDDLE : items have, within rounding, equal spacing above
 *            and below in the case of horizontal flow, or to the left
 *            and right in the case of vertical flow.
 *          - DISTAL : similiar to PROXIMAL, but items abut the lower
 *            container edge for horizontal flow, and the right edge
 *            for vertical flow.
 *
 * \see HUDContainer
 * \see HUDContent
 *
 * \author Mike Hewson\n
 */

class HUDFlowLayout : public HUDContainer {
    public:
        /// Enumerants to specify the mode of justification
        /// along the primary or 'flow' axis.
        enum primaryJustification {START, CENTRE, END, START_AND_END, SPAN};

        /// Enumerants to specify the mode of justification
        /// along the secondary axis.
        enum secondaryJustification {PROXIMAL, MIDDLE, DISTAL};

        /// Enumerant to specify the flow axis.
        enum Axis {HORIZONTAL, VERTICAL};

        /// Enumerant to specify which end of the axis to load into.
        enum load {FIRST, LAST};

        /**
         * \brief Constructor
         */
        HUDFlowLayout(HUDContainer* enclosing, Axis axis, HUDContainer::Mode mode);

        /**
         * \brief Destructor
         */
        virtual ~HUDFlowLayout();

        /**
         * \brief Add content to this container, which maybe another container.
         *
         * \param content : a pointer to the desired content
         */
        void addContent(HUDItem* item);

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

        /**
         * \brief Set which end of the primary axis to load into.
         *
         * \param the desired end
         */
        void setLoad(load end);

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
         *         whitespace gaps as per the justification setting.
         */
        void setPrimaryAxisGaps(void);

        GLuint setSecondaryAxisGaps(secondaryJustification just, GLuint gap_total);

        GLuint gap_count;
        GLuint total_white_space;
        GLint start_offset;
        GLint item_gap;

        /// The axis and justification settings.
        Axis ax;
        primaryJustification primary_just;
        secondaryJustification secondary_just;
        load load_dir;
    };

/**
 * @}
 */

#endif /* HUD_FLOW_LAYOUT_H_ */
