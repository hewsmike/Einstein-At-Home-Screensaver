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

#include "HUDFlowLayout.h"

#include "ErrorHandler.h"
#include "HUDTextLine.h"
#include "HUDImage.h"

#include <iostream>
#include <sstream>

HUDFlowLayout::HUDFlowLayout(Axis axis, HUDContainer::Mode mode) :
                HUDContainer(mode),
                ax(axis) {
    primary_axis_gap_count = 0;
    primary_axis_total_white_space = 0;
    primary_axis_start_offset = 0;
    primary_axis_item_gap = 0;
    primary_just = CENTRE;
    secondary_just = MIDDLE;
    }

HUDFlowLayout::~HUDFlowLayout() {
    }

void HUDFlowLayout::addItem(HUDItem* item) {
    // Only add if NULL wasn't passed.
    if(item != NULL) {
        // Just index these by their order of being added.
        HUDContainer::addItem(itemCount(), item);
        }
    else {
        ErrorHandler::record("HUDFlowLayout::addItem() - NULL passed for content",
                             ErrorHandler::FATAL);
        }
    }

void HUDFlowLayout::setPrimaryJustification(primaryJustification justification) {
    primary_just = justification;
    allocateItemBases();
    }

void HUDFlowLayout::setSecondaryJustification(secondaryJustification justification) {
    secondary_just = justification;
    allocateItemBases();
    }

HUDFlowLayout::primaryJustification HUDFlowLayout::getPrimaryJustification(void) const {
    return primary_just;
    }

HUDFlowLayout::secondaryJustification HUDFlowLayout::getSecondaryJustification(void) const {
    return secondary_just;
    }

std::pair<GLuint, GLuint> HUDFlowLayout::reassessMinimumDimensions(void) {
    // Assume an empty container.
    GLuint layoutMinWidth = 0;
    GLuint layoutMinHeight = 0;

    std::map<int, HUDItem*>& map = getMap();

    // Go through and access each one.
    for(std::map<int, HUDItem*>::iterator item = map.begin();
        item != map.end();
        ++item) {
        // Examine this item.
        HUDItem* current = (*item).second;

        switch(ax) {
            case HORIZONTAL:
                {
                // We add up all the minimum widths.
                layoutMinWidth += current->minWidth();
                // But take the overall minimum height to be the
                // greatest of the minima of the contained items.
                GLuint currentHeight = current->minHeight();
                if(currentHeight > layoutMinHeight) {
                    layoutMinHeight = currentHeight;
                    }
                }
                break;
            case VERTICAL:
                {
                // We add up all the minimum heights.
                layoutMinHeight += current->minHeight();
                // But take the overall minimum width to be the
                // greatest of the minima of the contained items.
                GLuint currentWidth = current->minWidth();
                if(currentWidth > layoutMinWidth) {
                    layoutMinWidth = currentWidth;
                    }
                }
                break;
            default:
                ErrorHandler::record("HUDFlowLayout::reassessMinimumDimensions() - bad switch case reached (default)",
                                     ErrorHandler::FATAL);
                break;
            }
        }

    return std::pair<GLuint, GLuint>(layoutMinWidth, layoutMinHeight);
    }

void HUDFlowLayout::allocateItemBases(void) {
    // We only need to bother with allocations
    // if there are items to render.
    if(itemCount() > 0) {
        // Determine the whitespace dispositions for the primary axis.
        setPrimaryAxisGaps();

        // Initial settings for placement of any items.
        GLuint primary_axis_coord = primary_axis_start_offset;
        GLuint secondary_axis_coord = 0;

        // Then contents are placed in the order of their insertion.
        std::map<int, HUDItem*>& container = getMap();
        for(std::map<int, HUDItem*>::iterator item = container.begin();
            item != container.end(); ++item) {
            // Determine offset in secondary axis for this item.
            GLuint secondary_axis_white_space = 0;
            switch(ax) {
                case HORIZONTAL :
                    secondary_axis_white_space = this->height() - (*item).second->height();
                    break;
                case VERTICAL :
                    secondary_axis_white_space = this->width() - (*item).second->width();
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch (1) case ( axis ) reached (default)",
                                     ErrorHandler::FATAL);
                    break;
                }

            // For a given amount of secondary axis whitespace then
            // how is that to be divided? That depends upon secondary
            // axis justification.
            switch(secondary_just) {
                case PROXIMAL:
                    secondary_axis_coord = 0;
                    break;
                case MIDDLE:
                    secondary_axis_coord = secondary_axis_white_space/2;
                    break;
                case DISTAL:
                    secondary_axis_coord = secondary_axis_white_space;
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case ( secondary justification ) reached (default)",
                                 ErrorHandler::FATAL);
                    break;
                }

            // Place the current content, as per primary axis choice,
            // followed by a shift to next insert position according to
            // the dimensions of the current item's content plus any gap.
            switch(ax) {
                case HORIZONTAL :
                    (*item).second->reBase(this->horzBase() + primary_axis_coord,
                                           this->vertBase() + secondary_axis_coord);
                    primary_axis_coord += (*item).second->width() + primary_axis_item_gap;
                    break;
                case VERTICAL :
                    (*item).second->reBase(this->horzBase() + secondary_axis_coord,
                                           this->vertBase() +
                                           this->height() -
                                           primary_axis_coord -
                                           (*item).second->height());
                    primary_axis_coord += (*item).second->height() + primary_axis_item_gap;
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch (2) case ( axis ) reached (default)",
                                         ErrorHandler::FATAL);
                    break;
                }
            }
        }
    }

void HUDFlowLayout::setPrimaryAxisGaps(void) {
    // Determine any available primary axis 'whitespace' b/w items. This is
    // the amount above the minimum required for display. By design this
    // whitespace ought be non-negative, but let's check to be sure.
    primary_axis_total_white_space = 0;
    primary_axis_gap_count = 0;
    primary_axis_item_gap = 0;
    primary_axis_start_offset = 0;
    switch(this->ax) {
        case HORIZONTAL:
            if(width() >= minWidth()) {
                primary_axis_total_white_space = width() - minWidth();
                }
            else  {
                ErrorHandler::record("HUDFlowLayout::setPrimaryAxisGaps() - negative horizontal whitespace!!",
                                     ErrorHandler::FATAL);
                }
            break;
        case VERTICAL:
            if(height() >= minHeight()) {
                primary_axis_total_white_space = height() - minHeight();
                }
            else {
                ErrorHandler::record("HUDFlowLayout::setPrimaryAxisGaps() - negative vertical whitespace!!",
                                     ErrorHandler::FATAL);
                }
            break;
        default :
            ErrorHandler::record("HUDFlowLayout::setPrimaryAxisGaps() - bad switch case ( axis ) reached (default)",
                                  ErrorHandler::FATAL);
            break;
        }

    // For the items, what is the distribution of whitespace?
    // This depends upon the chosen primary justification style.
    stringstream msg;
    switch(primary_just) {
        case START:
            // There's only a single gap on the other side to the justification.
            primary_axis_gap_count = 1;
            primary_axis_item_gap = 0;
            primary_axis_start_offset = 0;
            break;
        case END:
            // There's only a single gap on the other side to the justification.
            primary_axis_gap_count = 1;
            primary_axis_item_gap = 0;
            primary_axis_start_offset = primary_axis_total_white_space;
            break;
        case CENTRE:
            // We put all items in the centre and then have whitespace either side.
            primary_axis_gap_count = 2;
            primary_axis_item_gap = 0;
            primary_axis_start_offset = primary_axis_total_white_space/2;
            break;
        case START_AND_END:
            // Whitespace is distributed between all items, but none at either end.
            if(itemCount() == 1) {
                // With one item : like a START case.
                primary_axis_gap_count = 1;
                }
            else {
                // With more than one item : one less gap than there are items.
                primary_axis_gap_count = itemCount() - 1;
                }
            primary_axis_item_gap = primary_axis_total_white_space/primary_axis_gap_count;
            primary_axis_start_offset = 0;
            break;
        case SPAN:
            // Whitespace is distributed between all items, and some at either end.
            if(itemCount() == 1) {
                // With one item : like a CENTRE case.
                primary_axis_gap_count = 1;
                }
            else {
                // With more than one item : one more gap than there are items.
                primary_axis_gap_count = itemCount() + 1;
                }
            primary_axis_item_gap = primary_axis_total_white_space/primary_axis_gap_count;
            primary_axis_start_offset = primary_axis_item_gap/2;
            break;
        default:
            // Shouldn't ever get here!!
            ErrorHandler::record("HUDFlowLayout::setPrimaryAxisGaps() - bad switch case ( primary justification ) reached (default)",
                                 ErrorHandler::FATAL);
            break;
        }
    }
