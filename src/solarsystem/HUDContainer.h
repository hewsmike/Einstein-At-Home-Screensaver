/***************************************************************************
 * Copyright (C) 2013 by Mike Hewson                                       *
 * hewsmike[AT]iinet.net.au                                                *
 *                                                                         *
 * This file is part of Einstein@Home.                                     *
 *                                                                         *
 * Einstein@Home is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published       *
 * by the Free Software Foundation, version 2 of the License.              *
 *                                                                         *
 * Einstein@Home is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef HUD_CONTAINER_H_
#define HUD_CONTAINER_H_

#include <map>
#include <utility>

#include "HUDItem.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Base container type for display of HUD content.
 *
 *      The key design feature here is that a HUDContainer may be one/both of
 * container and contained. That is, any HUDContainer object may contain any
 * HUDItem, thus specifically another HUDContainer object. Circular inclusion
 * gives undefined behaviour ie. stick to directed acyclic graphs. What is
 * stored are in fact pointers to HUDItems.
 *      Note that it is not the  responsibility of this container to manage
 * heap allocation of contained items.
 *      Any contained object(s) will be activated for rendering when the
 * container itself is activated ( but only those present within at such time
 * of activation ).
 *      For a given direction of primary axis there are several choices of
 * policy to determine the distribution of spacing ( or justification )
 * between the items :
 *          - START : places all items to the left side in the case
 *            of horizontal primary axis, or the top side in the case of
 *            vertical primary axis, with any remaining space placed to
 *            the right or below respectively.
 *          - CENTRE : within rounding, the items are placed together
 *            in the centre of the container, with equal space allotted
 *            to left and right sides in the case of horizontal primary axis,
 *            or equally above and below in the case of vertical primary axis.
 *          - END : similiar to START except item placement is to the
 *            right side for horizontal primary axis, and at the bottom in the
 *            case of vertical primary axis, with any extra space being allocated
 *            to the left and above respectively.
 *          - START_AND_END : all extra space is allocated in the gaps
 *            between items, equally within rounding, and no space at
 *            either extreme of the container. If only one item present
 *            then behaves like a START case.
 *          - SPAN : similiar to START_AND_END except some gap, equal to
 *            within rounding, at the extreme margins. If only one item
 *            present then behaves like a CENTRE case.
 *          The distribution of spacing ( or justification ) along the axis
 * orthogonal to the primary axis has these policies :
 *          - PROXIMAL : whereby items abut the upper edge of the
 *            container in the case of horizontal primary axis, or the left
 *            edge of the container in the case of vertical primary axis.
 *          - MIDDLE : items have, within rounding, equal spacing above
 *            and below in the case of horizontal primary axis, or to the left
 *            and right in the case of vertical primary axis.
 *          - DISTAL : similiar to PROXIMAL, but items abut the lower
 *            container edge for horizontal primary axis, and the right edge
 *            for vertical primary axis.
 *      It is the responsibility of any subclass to actually enact the above
 * placement semantics via allocateItemBases(). This class only accesses and
 * mutates the justification settings.
 *
 * \see HUDItem
 *
 * \author Mike Hewson\n
 */

class HUDContainer : public HUDItem {
    public:
        /// Enumerants to specify the mode of justification
        /// along the primary axis.
        enum primaryJustification {START, CENTRE, END, START_AND_END, SPAN};

        /// Enumerants to specify the mode of justification
        /// along the secondary axis.
        enum secondaryJustification {PROXIMAL, MIDDLE, DISTAL};

        /// Enumerant to specify the primary axis on screen.
        enum Axis {HORIZONTAL, VERTICAL};

        /**
         * \brief Constructor
         */
        HUDContainer(HUDContainer::Axis axis = HUDContainer::HORIZONTAL);

        /**
         * \brief Destructor
         */
        virtual ~HUDContainer();

        /**
         * \brief Resize this container, at or above it's minimum dimensions
         *
         * \param newWidth : the desired width
         * \param newHeight : the desired height
         *
         * \return bool : whether the resize occured
         * - true if a resize was allowed
         * - false if below minimum was requested
         */
        virtual bool requestResize(GLuint newWidth, GLuint newHeight);

        /**
         * \brief Set the lower left corner window coordinates
         *
         * \param newHorz - the desired horizontal coordinate
         * \param newVert - the desired vertical coordinate
         */
        virtual void reBase(GLuint newHorz, GLuint newVert);

        /**
         * \brief Re-evaluate this container's sizing and contained items'
         * positions. Triggered by a change within the container.
         */
        void adjust(void);

        /**
         * \brief Empty the underlying container.
         */
        void erase(void);

        /**
         * \brief Obtain the count of contained items
         *
         * \return the count
         */
        GLuint itemCount(void) const;

        /**
         * \brief Query whether anything is contained within
         *
         * \return boolean : indicating true if empty, false otherwise
         */
        bool isEmpty(void) const;

        /**
         * \brief Get a pointer to a contained item
         *
         * \param handle : the identifier used when the item was added
         *
         * \return HUDItem* : a pointer to the item ( NULL if not present )
         */
        HUDItem* getItem(int handle);

        /**
         * \brief Add a HUDItem to this container.
         *
         * IF the given pointer was not NULL, then
         * - the item was inserted, and
         * - any existing item with the same handle was removed, and
         * - this container's minimum dimensions were adjusted
         *
         * \param handle : an identifier for the item
         * \param obj : the item's pointer
         */
        void addItem(int handle, HUDItem* item);

        /**
         * \brief Set the justification along the primary axis.
         *
         *      Triggers a recalculation of the positions of enclosed items.
         *
         * \param just : the desired justification
         */
        void setPrimaryJustification(primaryJustification just);

        /**
         * \brief Set the justification along the secondary axis.
         *
         *      Triggers a recalculation of the positions of enclosed items.
         *
         * \param just : the desired justification
         */
        void setSecondaryJustification(secondaryJustification just);

        /**
         * \brief Get the justification of the primary flow axis.
         *
         * \return the desired justification
         */
        HUDContainer::primaryJustification getPrimaryJustification(void) const;

        /**
         * \brief Get the justification of the secondary flow axis.
         *
         * \return the desired justification
         */
        HUDContainer::secondaryJustification getSecondaryJustification(void) const;

    protected:
        std::map<int, HUDItem*>& getMap(void);

        HUDContainer::Axis getAxis(void) const;

        /**
         * \brief Re-assess the minimal width and height.
         *
         * Pure virtual as this depends upon the container's
         * internal layout, and a subclass must enact this.
         *
         * \return std::pair composed of ( in order )
         * - the minimum width
         * - the minimum height
         */
        virtual std::pair<GLuint, GLuint> reassessMinimumDimensions(void) = 0;

        /**
         * \brief Determine and set the base positions of each item.
         *
         * Pure virtual as only a subclass will know how to do this.
         */
        virtual void allocateItemBases(void) = 0;

        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// Associative array b/w item pointers and integral labels.
        std::map<int, HUDItem*> container;

        /// The axis and justification settings.
        Axis ax;
        primaryJustification primary_just;
        secondaryJustification secondary_just;
    };

/**
* @}
*/

#endif /* HUD_CONTAINER_H_ */
