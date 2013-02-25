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

#include "HUDFlowLayout.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief HUDFlowLayout variant with release of memory via pointers assumed as
 *        heap based. It only accepts HUDContent items for inclusion though.
 *
 * \see HUDContent
 * \see HUDFlowLayout
 *
 * \author Mike Hewson\n
 */

class HUDTempFlowLayout : public HUDFlowLayout {
    public:
         /**
         * \brief Constructor
         */
        HUDTempFlowLayout(HUDFlowLayout::Axis axis);

        /**
         * \brief Destructor
         */
        virtual ~HUDTempFlowLayout();

        /**
         * \brief Add a HUDContent item to this container.
         *
         * IF the given pointer was not NULL, then
         * - the item was inserted, and
         * - this container's minimum dimensions were adjusted
         *
         * \param item : a pointer to heap based HUDContent instance.
         */
        void addItem(HUDContent* item);

        /**
         * \brief Empty the underlying container, while releasing heap memory.
         */
        void erase(void);
    protected:

    private:
    };

/**
* @}
*/

#endif /* HUD_TEMP_FLOW_LAYOUT_H_ */
