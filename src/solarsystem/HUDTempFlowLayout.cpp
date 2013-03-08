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

#include "HUDTempFlowLayout.h"

#include <map>

HUDTempFlowLayout::HUDTempFlowLayout(HUDFlowLayout::Axis axis) :
                    HUDFlowLayout(axis) {
    }

HUDTempFlowLayout::~HUDTempFlowLayout() {
    erase();
    }

void HUDTempFlowLayout::erase(void) {
    std::map<int, HUDItem*>& container = this->getMap();
    while(container.empty() != true) {
        // Get an iterator to the first container element.
        std::map<int, HUDItem*>::const_iterator head = container.begin();
        // Actual type is HUDContent or more derived.
        // Free up heap resources used by this element. As destructor
        // will also be called, then OpenGl resources will be released too.
        delete head->second;
        // Remove element from the container.
        container.erase(head->first);
        }
    }

void HUDTempFlowLayout::addItem(HUDContent* item) {
    HUDFlowLayout::addItem(item);
    }
