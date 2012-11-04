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
    std::cout << "RenderListing::~RenderListing() 1" << std::endl;
    clear();
    std::cout << "RenderListing::~RenderListing() 2" << std::endl;
    }

void RenderListing::add(Renderable* renderable_ptr) {
    // Prevent a NULL sneaking in.
    if(renderable_ptr != NULL) {
        r_ptrs.push_back(renderable_ptr);
        }
    }

void RenderListing::clear(void) {
    // Go through all elements currently in the container.
    while(r_ptrs.size() != 0) {
        std::cout << "RenderListing::clear() 1" << std::endl;
        Renderable* r_ptr = r_ptrs.at(r_ptrs.size() - 1);
        std::cout << "RenderListing::clear() 2" << std::endl;
        r_ptr->inactivate();
        std::cout << "RenderListing::clear() 3" << std::endl;
        delete(r_ptr);
        std::cout << "RenderListing::clear() 4" << std::endl;
        r_ptrs.pop_back();
        std::cout << "RenderListing::clear() 5" << std::endl;
        }
// for(std::vector<Renderable*>::iterator rp = r_ptrs.begin();
// rp != r_ptrs.end();
// ++rp) {
// std::cout << "RenderListing::clear() 1" << std::endl;
// // Releases OpenGL resources.
// (*rp)->inactivate();
// std::cout << "RenderListing::clear() 2" << std::endl;
// // Free the heap memory that was allocated.
// delete (*rp);
// std::cout << "RenderListing::clear() 3" << std::endl;
//
// }
    // Finally empty the container using a call to std::vector.clear() that
    // calls the destructor for each item ( being pointers in this case ) and
    // then finally sets the vector's size to zero. NB A pointer's destructor
    // "does" nothing ...
    r_ptrs.clear();
    }

const Renderable* RenderListing::at(GLuint index) const {
    // Assume out of range index.
    Renderable* ret_val = NULL;

    // If container non-empty and index in range then ...
    if((size() != 0) && (index < size())) {
        // As written expect an exception to be thrown from
        // std::vector if out-of-range anyway.
        ret_val = r_ptrs.at(index);
        }

    return ret_val;
    }

unsigned int RenderListing::size(void) const {
    return r_ptrs.size();
    }
