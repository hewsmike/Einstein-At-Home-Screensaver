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

#include "PtrHolder.h"

PtrHolder::PtrHolder(void) {
    }

PtrHolder::~PtrHolder() {
    clear();
    }

void PtrHolder::add(const Renderable* pointer) {
    ptr_list.push_back(pointer);
    }

void PtrHolder::clear(void) {
    // Provided we have any pointers allocated.
    for(unsigned int index = 0; index < ptr_list.size(); ++index) {
        Renderable* ptr = ptr_list[index];
        if(ptr != NULL) {
            // Free up any OpenGL resources.
            ptr->inactivate();
            // Free the heap memory.
            delete ptr;
            }
        }
    // Empty the list.
    ptr_list.clear();
    }

const std::vector<Renderable*>& PtrHolder::pointers(void) {
    return ptr_list;
    }
