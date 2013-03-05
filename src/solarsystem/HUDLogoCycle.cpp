/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#include "HUDLogoCycle.h"

#include <map>
#include <utility>

#include "ErrorHandler.h"

GLuint HUDLogoCycle::DEFAULT_FRAME_GAP(100);

HUDLogoCycle::HUDLogoCycle(GLuint frames) :
                frame_gap(frames) {
    vert_just = HUDLogoCycle::TOP;
    horz_just = HUDLogoCycle::RIGHT;
    current_image_index = 0;
    frame_count = 0;
    }

HUDLogoCycle::~HUDLogoCycle() {
    }

void HUDLogoCycle::addItem(HUDImage* logo) {
    // Only add if NULL wasn't passed.
    if(logo != NULL) {
        // Just index these by their order of being added.
        HUDContainer::addItem(HUDContainer::itemCount(), logo);
        }
    else {
        ErrorHandler::record("HUDLogoCycle::addLogo() - NULL passed for content",
                             ErrorHandler::FATAL);
        }
    }

GLuint HUDLogoCycle::getFrameGap(void) const {
    return frame_gap;
    }

void HUDLogoCycle::setFrameGap(GLuint gap) {
    frame_gap = gap;
    }

HUDLogoCycle::horizontalJustification HUDLogoCycle::getHorizontalJustification(void) const {
    return horz_just;
    }

HUDLogoCycle::verticalJustification HUDLogoCycle::getVerticalJustification(void) const {
    return vert_just;
    }

void HUDLogoCycle::setHorizontalJustification(HUDLogoCycle::horizontalJustification justification) {
    horz_just = justification;
    allocateItemBases();
    }

void HUDLogoCycle::setVerticalJustification(HUDLogoCycle::verticalJustification justification) {
    vert_just = justification;
    allocateItemBases();
    }

std::pair<GLuint, GLuint> HUDLogoCycle::reassessMinimumDimensions(void) {
    // Assume an empty container.
    GLuint layoutMinWidth = 0;
    GLuint layoutMinHeight = 0;

    std::map<int, HUDItem*>& container = HUDContainer::getMap();

    // Go through and access each one.
    for(std::map<int, HUDItem*>::iterator item = container.begin();
        item != container.end();
        ++item) {
        // Examine this item.
        HUDItem* logo = (*item).second;

        // Take the overall minimum height to be the
        // greatest of the minima of the contained items.
        GLuint currentHeight = logo->minHeight();
        if(currentHeight > layoutMinHeight) {
            layoutMinHeight = currentHeight;
            }

        // Take the overall minimum width to be the
        // greatest of the minima of the contained items.
        GLuint currentWidth = logo->minWidth();
        if(currentWidth > layoutMinWidth) {
            layoutMinWidth = currentWidth;
            }
        }

    return std::pair<GLuint, GLuint>(layoutMinWidth, layoutMinHeight);
    }

void HUDLogoCycle::allocateItemBases(void) {
    // We only need to bother with allocations
    // if there are items to render.
    if(HUDContainer::itemCount() > 0) {

        std::map<int, HUDItem*>& container = HUDContainer::getMap();
        for(std::map<int, HUDItem*>::iterator item = container.begin();
            item != container.end();
            ++item) {
            HUDItem* logo = (*item).second;

            // Offsets for both axes to place this item.
            GLuint horz_gap = 0;
            GLuint vert_gap = 0;

            // How much whitespace do we have in each direction to play with?
            GLuint horz_whitespace = this->minWidth() - logo->minWidth();
            GLuint vert_whitespace = this->minHeight() - logo->minHeight();

            // For a given amount of horizontal axis whitespace then
            // how is that to be divided? That depends upon horizontal
            // axis justification.
            switch(horz_just) {
                case LEFT:
                    break;
                case CENTRE:
                    horz_gap = horz_whitespace/2;
                    break;
                case RIGHT:
                    horz_gap = horz_whitespace;
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDLogoCycle::allocateItemBases() - bad switch case ( horizontal justification ) reached (default)",
                                         ErrorHandler::FATAL);
                    break;
                }

            // For a given amount of vertical axis whitespace then
            // how is that to be divided? That depends upon vertical
            // axis justification.
            switch(vert_just) {
                case TOP:
                    vert_gap = vert_whitespace;
                    break;
                case MIDDLE:
                    vert_gap = vert_whitespace/2;
                    break;
                case BOTTOM:
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDLogoCycle::allocateItemBases() - bad switch case ( vertical justification ) reached (default)",
                                         ErrorHandler::FATAL);
                    break;
                }


            logo->reBase(this->horzBase() + horz_gap,
                         this->vertBase() + vert_gap);
            }
        }
    }

void HUDLogoCycle::render(void) {
    // We only need to bother with allocations
    // if there are items to render.
    if(HUDContainer::itemCount() > 0) {
        // Find our current logo in the container.
        HUDItem* item = getItem(current_image_index);
        // Only access thru non-null pointer!
        if(item != NULL) {
            item->draw();
            }
        else {
            // Shouldn't ever get here, but ...
            ErrorHandler::record("HUDLogoCycle::render() - NULL found for object!",
                                 ErrorHandler::FATAL);
            }
        ++frame_count;
        if(frame_count > frame_gap) {
            frame_count = 0;
            ++current_image_index;
            if(current_image_index == this->itemCount()) {
                current_image_index = 0;
                }
            }
        }
    }
