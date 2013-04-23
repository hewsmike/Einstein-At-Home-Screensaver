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

#include "HUDContainer.h"

#include <iostream>
#include <sstream>

#include "ErrorHandler.h"
#include "HUDImage.h"
#include "HUDTextLine.h"

HUDContainer::HUDContainer(HUDContainer::Axis axis) :
                HUDItem(),
                ax(axis),
                primary_just(HUDContainer::CENTRE),
                secondary_just(HUDContainer::MIDDLE) {
    }

HUDContainer::~HUDContainer() {
    erase();
    }

bool HUDContainer::requestResize(GLuint newWidth, GLuint newHeight) {
    // Assume failure.
    bool ret_val = false;

    // Are the requested dimensions above or equal to the minimum allowed ?
    if((newWidth >= minWidth()) && (newHeight >= minHeight())) {
        // Yes, set those valid dimensions ....
        setDimensions(newWidth, newHeight);
        // and then rebase the panel's contents.
        allocateItemBases();
        // Indicate success.
        ret_val = true;
        }

    return ret_val;
    }

void HUDContainer::reBase(GLuint newHorz, GLuint newVert) {
    // Rebase this container and then ...
    HUDItem::reBase(newHorz, newVert);

    // ... rebase the contained items.
    allocateItemBases();
    }

GLuint HUDContainer::itemCount(void) const {
    return container.size();
    }

bool HUDContainer::isEmpty(void) const {
    return container.empty();
    }

void HUDContainer::erase(void) {
    container.clear();
    // Notify any possible containing object of this size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer != NULL) {
        outer->adjust();
        }
    }

void HUDContainer::adjust() {
    // Reassess this container's correct/current minimal dimensions.
    std::pair<GLuint, GLuint> min_size = reassessMinimumDimensions();

    // Call base class to adjust to such minima.
    this->setMinimumDimensions(min_size.first, min_size.second);

    // However the 'outer' dimensions may need to expand because
    // either/both of the minimum height/width was increased. In
    // which case set those outer dimensions to the minima.
    if(min_size.first > width()) {
        setDimensions(min_size.first, height());
        }
    if(min_size.second > height()) {
        setDimensions(width(), min_size.second);
        }     // Rebase the contained items.

    allocateItemBases();
    // Notify any possible containing object of this size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer != NULL) {
        outer->adjust();
        }
    }

void HUDContainer::addItem(int handle, HUDItem* item) {
    // Make sure a NULL pointer was not passed.
    if(item == NULL) {
        // A significant error & ought not go un-managed.
        ErrorHandler::record("HUDContainer::additem() - NULL passed for object!",
                             ErrorHandler::WARN);
        }
    else {
        // If the given handle is not present in the map, erase has no effect.
        container.erase(handle);

        // Then put in a new key/value combo using pair semantics.
        container.insert(std::pair<int, HUDItem*>(handle, item));

        // Allow this container to be the enclosing one for this included item.
        item->setEnclosingContainer(this);

        // Policy is to activate all items upon inclusion.
        item->activate();

        // Adjust container to account for this addition.
        adjust();
        }
    }

void HUDContainer::setPrimaryJustification(primaryJustification justification) {
    primary_just = justification;
    allocateItemBases();
    }

void HUDContainer::setSecondaryJustification(secondaryJustification justification) {
    secondary_just = justification;
    allocateItemBases();
    }

HUDContainer::primaryJustification HUDContainer::getPrimaryJustification(void) const {
    return primary_just;
    }

HUDContainer::secondaryJustification HUDContainer::getSecondaryJustification(void) const {
    return secondary_just;
    }

std::map<int, HUDItem*>& HUDContainer::getMap(void) {
    return container;
    }

HUDItem* HUDContainer::getItem(int handle) {
    // Default return value indicates nothing found.
    HUDItem* ret_val = NULL;

    // Let's see if we can find an item with the given handle.
    std::map<int, HUDItem*>::const_iterator pos = container.find(handle);

    // Was it recovered?
    if(pos != container.end()) {
        // Yes, it was there. So the other member of the pair at
        // that position is ( the pointer to ) our desired item.
        ret_val = pos->second;
        }

    return ret_val;
    }

HUDContainer::Axis HUDContainer::getAxis(void) const {
    return ax;
    }

void HUDContainer::prepare(SolarSystemGlobals::render_quality rq) {
    }

void HUDContainer::release(void) {
    }

void HUDContainer::render(void) {
    // Go through the container, rendering each item within.
    for(std::map<int, HUDItem*>::iterator obj = container.begin();
        obj != container.end();
        ++obj) {
        // Similiar to prepare() and release() above, drawing has
        // the same relation to rendering.

        // Check on nullity ie. assume the iterator is valid, but
        // does it dereference to a NULL pointer ?
        if(obj->second != NULL) {
            obj->second->draw();
            }
        else {
            // Shouldn't ever get here, but ...
            ErrorHandler::record("HUDContainer::render() - NULL found for object!",
                                 ErrorHandler::FATAL);
            }
        }
    }
