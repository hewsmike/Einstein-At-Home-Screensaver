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

HUDTempFlowLayout::HUDTempFlowLayout(HUDFlowLayout::Axis axis) :
                    HUDFlowLayout(axis) {
    }

HUDTempFlowLayout::~HUDTempFlowLayout() {
    }

void HUDTempFlowLayout::erase(void) {
    HUDContainer::erase();
    }

void HUDTempFlowLayout::addItem(int handle, HUDContent* item) {
    HUDContainer::addItem(handle, item);
    }

void HUDTempFlowLayout::removeItem(int handle) {
    HUDContainer::removeItem(handle);
    }
