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

#include "HUDBorderLayout.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

HUDBorderLayout::HUDBorderLayout(HUDContainer* enclosing, HUDContainer::Mode mode) :
                    HUDContainer(enclosing, mode) {
    }

HUDBorderLayout::~HUDBorderLayout() {
    }

bool HUDBorderLayout::requestResize(GLuint newWidth, GLuint newHeight) {
    /// TODO Test if on windows, maybe compile level define ... ?
    if(true) {
        this->reAcquire();
        }
    HUDContainer::requestResize(newWidth, newHeight);

    return true;
    }

void HUDBorderLayout::setPanel(enum BorderPanel panel, HUDContainer* container) {
    // Don't store a NULL pointer!!
    if(container != NULL) {
        // Now which panel was nominated?
        switch(panel) {
            case NORTH :
            case SOUTH :
            case EAST :
            case WEST :
                addItem(panel, container);
                break;
            default :
                // Shouldn't ever get here!!
                ErrorHandler::record("HUDBorderLayout::setPanel() - bad switch case reached (default)",
                                     ErrorHandler::FATAL);
                break;
            }
        }
    else {
        ErrorHandler::record("HUDBorderLayout::setPanel() - NULL passed for item)",
                             ErrorHandler::FATAL);
        }
    }

std::pair<GLuint, GLuint> HUDBorderLayout::reassessMinimumDimensions(void) {
    // Minimum overall width is the maximum of :
    //    - the minimum of the north area
    //    - the minimum of the south area
    //    - the sum of :
    //          - the minimum of the east, plus
    //          - the minimum of the west
    // But we have to acknowledge the possibility that none/some/all
    // panels are actually occupied in this container.

    // Determine the north panel's minimal dimensions.
    // Assume none or empty container.
    GLuint north_width = 0;
    GLuint north_height = 0;
    getPanelMinimumDimensions(&north_width, &north_height, NORTH);

    // Determine the south panel's minimal dimensions.
    // Assume none or empty container.
    GLuint south_width = 0;
    GLuint south_height = 0;
    getPanelMinimumDimensions(&south_width, &south_height, SOUTH);

    // Determine the east panel's minimal dimensions.
    // Assume none or empty container.
    GLuint east_width = 0;
    GLuint east_height = 0;
    getPanelMinimumDimensions(&east_width, &east_height, EAST);

    // Determine the west panel's minimal dimensions.
    // Assume none or empty container.
    GLuint west_width = 0;
    GLuint west_height = 0;
    getPanelMinimumDimensions(&west_width, &west_height, WEST);

    GLuint layoutMinWidth = std::max(std::max(north_width, south_width),
                                     east_width + west_width);

    // Minimum overall height is the sum of :
    //    - the minimum of the north area
    //    - the minimum of the south area
    //    - whichever is the larger of :
    //          - the minimum of the east
    //          - the minimum of the west

    GLuint layoutMinHeight = north_height + south_height +
                             std::max(east_height, west_height);

    return std::pair<GLuint, GLuint>(layoutMinWidth, layoutMinHeight);
    }

void HUDBorderLayout::getPanelMinimumDimensions(GLuint* w_ptr, GLuint* h_ptr,
                                                enum BorderPanel panel) {
    // Provided non-NULL pointers have been passed.
    if((w_ptr != NULL) && (h_ptr != NULL)) {
        // Assume there is no, or an empty, container
        // associated with given panel.
        *w_ptr = 0;
        *h_ptr = 0;
        // Now see if a valid panel has been nominated.
        switch(panel) {
            case NORTH :
            case SOUTH :
            case EAST :
            case WEST :
                // With a valid panel to query ... see
                // if there is a container allocated.
                {
                HUDItem* hi_ptr = getItem(panel);
                if(hi_ptr != NULL) {
                    *w_ptr = hi_ptr->minWidth();
                    *h_ptr = hi_ptr->minHeight();
                    }
                }
                break;
            default :
                // Shouldn't ever get here!!
                ErrorHandler::record("HUDBorderLayout::getPanelMinimumDimensions() - bad switch case reached (default)",
                                      ErrorHandler::FATAL);
                break;
            }
        }
    else {
        ErrorHandler::record("HUDBorderLayout::getPanelMinimumDimensions() - NULL passed for pointer(s)",
                             ErrorHandler::FATAL);
        }
    }

void HUDBorderLayout::allocateItemBases(void) {
    // Policy is to maximise central area. BTW coordinates are zero based.

    // Acquire pointers to the panel containers, knowing
    // that HUDContainers were inserted ( by design ).
    HUDContainer* north = static_cast<HUDContainer*>(getItem(NORTH));
    HUDContainer* south = static_cast<HUDContainer*>(getItem(SOUTH));
    HUDContainer* east = static_cast<HUDContainer*>(getItem(EAST));
    HUDContainer* west = static_cast<HUDContainer*>(getItem(WEST));

    // Firstly we need to resize each panel.

    // Assume there is no container.
    GLuint newNorthWidth = 0;
    GLuint newNorthHeight = 0;
    // But if there is .....
    if(north != NULL) {
        // North is allocated this layout's overall width,
        // but it's own minimum height.
        newNorthWidth = width();
        newNorthHeight = north->minHeight();
        // Resize it.
        bool north_happy = north->requestResize(newNorthWidth, newNorthHeight);
        // This should not have failed, but ...
        if(!north_happy) {
            std::stringstream msg;
            msg << "HUDBorderLayout::reassessPanels() - north resizing failed with "
                << "newNorthWidth = " << newNorthWidth << " and newNorthHeight = " << newNorthHeight;
            ErrorHandler::record(msg.str().c_str(), ErrorHandler::FATAL);
            }
        }

    // Assume there is no container.
    GLuint newSouthWidth = 0;
    GLuint newSouthHeight = 0;
    // But if there is .....
    if(south != NULL) {
        // South is allocated this layout's overall width,
        // but it's own minimum height.
        newSouthWidth = width();
        newSouthHeight = south->minHeight();
        // Resize it.
        bool south_happy = south->requestResize(newSouthWidth, newSouthHeight);
        // This should not have failed, but ...
        if(!south_happy) {
            std::stringstream msg;
            msg << "HUDBorderLayout::reassessPanels() - south resizing failed with "
             << "newSouthWidth = " << newSouthWidth << " and newSouthHeight = " << newSouthHeight;
            ErrorHandler::record(msg.str().c_str(), ErrorHandler::FATAL);
            }
        }

    // What will now be the height of the central area, given that
    // north and south have been sized ?
    GLuint centreHeight = this->height() - (newNorthHeight + newSouthHeight);

    // Assume there is no container.
    GLuint newEastWidth = 0;
    GLuint newEastHeight = 0;
    // But if there is .....
    if(east != NULL) {
        // East is allocated it's own minimum width,
        // but the height of the centre.
        newEastWidth = east->minWidth();
        newEastHeight = centreHeight;
        // Resize it.
        bool east_happy = east->requestResize(newEastWidth, newEastHeight);
        // This should not have failed, but ...
        if(!east_happy) {
            std::stringstream msg;
            msg << "HUDBorderLayout::reassessPanels() - east resizing failed with "
                << "newEastWidth = " << newEastWidth << " and newEastHeight = " << newEastHeight;
            ErrorHandler::record(msg.str().c_str(), ErrorHandler::FATAL);
            }
        }

    // Assume there is no container.
    GLuint newWestWidth = 0;
    GLuint newWestHeight = 0;
    // But if there is .....
    if(west != NULL) {
        // West is allocated it's own minimum width,
        // but the height of the centre.
        newWestWidth = west->minWidth();
        newWestHeight = centreHeight;
        // Resize it.
        bool west_happy = west->requestResize(newWestWidth, newWestHeight);
        // This should not have failed, but ...
        if(!west_happy) {
            std::stringstream msg;
            msg << "HUDBorderLayout::reassessPanels() - west resizing failed with "
                << "newWestWidth = " << newWestWidth << " and newWestHeight = " << newWestHeight;
            ErrorHandler::record(msg.str().c_str(), ErrorHandler::FATAL);
            }
        }

    // Now we can rebase the panels.

    // The lower left corner of north abuts the left hand edge, and is
    // down from the top of the layout by it's own height.
    if(north != NULL) {
        north->reBase(0, height() - newNorthHeight);
        }

    // The lower left corner of south is always at the
    // lower left corner of the layout.
    if(south != NULL) {
        south->reBase(0, 0);
        }

    // The lower left corner of east is back from the right hand edge by it's
    // own width, and is up from the bottom edge by south's height.
    if(east != NULL) {
        east->reBase(width() - newEastWidth, newSouthHeight);
        }

    // The lower left corner of west abuts the left hand edge, and is
    // up from the bottom edge by south's height.
    if(west != NULL) {
        west->reBase(0, newSouthHeight);
        }
    }
