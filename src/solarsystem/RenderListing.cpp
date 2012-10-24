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

#include "RenderListing.h"

RenderListing::RenderListing() {
    }

RenderListing::~RenderListing() {
    clear();
    }

void RenderListing::add(Renderable* renderable_ptr) {
    // Prevent a NULL sneaking in.
    if(renderable_ptr != NULL) {
        r_ptrs.push_back(renderable_ptr);
        }
    }

void RenderListing::clear(void) {
    // Go through all elements currently in the container.
    for(std::vector<Renderable*>::iterator rp = r_ptrs.begin();
        rp != r_ptrs.end();
        ++rp) {
        // Free the heap memory that was allocated, this
        // will also call the destructor for the Renderable
        // object that, by design, releases OpenGL resources.
        delete (*rp);
        }
    // Finally empty the container using a call to std::vector.clear() that
    // calls the destructor for each item ( being pointers in this case ) and
    // then finally sets the vector's size to zero. NB A pointer's destructor
    // "does" nothing ...
    r_ptrs.clear();
    }
