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

#ifndef HUD_TEMP_FLOW_LAYOUT_H_
#define HUD_TEMP_FLOW_LAYOUT_H_

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
 * stored is in fact pointers to HUDItems.
 *      The RETAIN option ( default ) on construction is to indicate that it is
 * not the  responsibility of the container to manage heap allocation of
 * contained items.
 *      The DESTROY option on construction is for the benefit of creating
 * containers with temporary heap content that will be deleted from the heap
 * when they are removed from the container. That is, it saves having to do such
 * deletions separately. BEWARE using the destroy option for otherwise 'named'
 * heap items as double deletion ( undefined behaviour ) may be risked.
 *      Any contained object(s) will be activated for rendering when the container itself is activated, but only those
 * present within at such time of activation.
 *
 * \see HUDItem
 *
 * \author Mike Hewson\n
 */

class HUDTempFlowLayout : public HUDContainer {
    public:
         /**
         * \brief Constructor
         */
        HUDTempFlowLayout(Mode mode);

        /**
         * \brief Destructor
         */
        virtual ~HUDTempFlowLayout();

        /**
         * \brief Add a HUDContent item to this container.
         *
         * IF the given pointer was not NULL, then
         * - the item was inserted, and
         * - any existing item with the same handle was removed, and
         * - this container's minimum dimensions were adjusted
         *
         * \param handle : an identifier for the item
         * \param obj : the item's pointer
         */
        void addItem(int handle, HUDContent* item);

        /**
         * \brief Empty the underlying container.
         */
        void erase(void);

    protected:

    private:
    };

/**
* @}
*/

#endif /* HUD_TEMP_FLOW_LAYOUT_H_ */
