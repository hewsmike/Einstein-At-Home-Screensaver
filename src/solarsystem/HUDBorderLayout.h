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

#ifndef HUD_BORDER_LAYOUT_H_
#define HUD_BORDER_LAYOUT_H_

#include "HUDContainer.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates a container type which arranges other
 *        containers in a bordered layout.
 *
 *      It is based upon the Java panel layout of similiar name, whereby
 * four containers of rectangular outline ( named by location as North,
 * South, East and West ) enclose a central screen region. The North panel
 * reaches from the left edge of the client area to the right edge, and
 * from none to many pixels downwards from the top edge. The South panel
 * likewise extends the full width across horizontally but upwards from
 * none to many pixels from the bottom edge. The East and West panels each
 * abut the lower edge of North and the top edge of South, while
 * extending none to several pixels from the right and left client area
 * edges respectively.
 *      These regions have no visible border enacted, but represent
 * alignment guides for items placed within. The panels can each be
 * populated by a single item of HUDContainer type, or not at all : in
 * which case the particular panel is deemed to have no dimensions.
 *      The central screen region enclosed by the panels is un-named and
 * un-addressed, so by default has whatever space remains after
 * allocation to the panels.
 *      Notably the panel dimensions are dynamic, in that the addition or
 * removal or any other resizing of the containers within ( ie. the
 * HUDContainers allocated to North/South/East/West ) causes a
 * re-evaluation of the arrangement of positioning within the entire
 * layout.
 *
 * \see HUDContainer
 *
 * \author Mike Hewson\n
 */

class HUDBorderLayout : public HUDContainer {
    public:
        enum BorderPanel {NORTH, SOUTH, EAST, WEST};

        /**
        * \brief Constructor
        */
        HUDBorderLayout(HUDContainer* enclosing);

        /**
         * \brief Destructor
         */
        virtual ~HUDBorderLayout();

        /**
         * \brief Set the relevant panel to a given container
         *
         * \param panel : the designated panel of the layout
         *
         * \param item : the container that will be placed there
         */
        void setPanel(enum BorderPanel panel, HUDContainer* container);

        /**
         * \brief Obtain the position and dimensions of the centre area
         *
         * \param base : pointer to a GLuint pair for the horizontal and
         *               vertical coordinates respectively of the
         *               lower left corner of the region.
         *
         * \param size : pointer to a GLuint pair for the horizontal
         *               width and vertical height respectively.
         */
        void getCentre(std::pair<GLuint, GLuint>* base,
                       std::pair<GLuint, GLuint>* size) const;

    protected:
        /**
         * \brief Determine the miminum dimensions as per the internal
         *        layout of containers within.
         *
         *      Enacts HUDContainer interface.
         */
        virtual std::pair<GLuint, GLuint> reassessMinimumDimensions(void);

        /**
         * \brief Determine and set the base positions of each item.
         *
         *      Enacts HUDContainer interface.
         */
        virtual void allocateItemBases(void);

    private:
        /**
         * \brief Find the miminum dimensions for a panel.
         *
         * If no container has been allocated for the given panel
         * then the miminum width and height are set to zero.
         *
         * \param w_ptr : pointer to store the minimum width result
         *
         * \param h_ptr : pointer to store the minimum height result
         *
         * \param panel : the layout's area to be queried
         */
        void getPanelMinimumDimensions(GLuint* w_ptr, GLuint* h_ptr,
                                       enum BorderPanel panel);

         /**
         * \brief Determine the sizes and set the base positions of
         *        each panel's container.
         */
        void reassessPanels(void);
    };

/**
 * @}
 */

#endif /* HUD_BORDER_LAYOUT_H_ */
