/***************************************************************************
* Copyright (C) 2012 by Mike Hewson                                        *
* hewsmike@iinet.net.au                                                    *
*                                                                          *
* This file is part of Einstein@Home.                                      *
*                                                                          *
* Einstein@Home is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published        *
* by the Free Software Foundation, version 2 of the License.               *
*                                                                          *
* Einstein@Home is distributed in the hope that it will be useful,         *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.    *
*                                                                          *
***************************************************************************/

#include "HUDFlowLayout.h"

#include "ErrorHandler.h"
#include "HUDTextLine.h"
#include "HUDImage.h"

#include <iostream>

HUDFlowLayout::HUDFlowLayout(HUDContainer* enclosing, Axis axis) :
                HUDContainer(enclosing),
                ax(axis) {
    gap_count = 0;
    total_white_space = 0;
    start_offset = 0;
    item_gap = 0;
    primary_just = CENTRE;
    secondary_just = MIDDLE;
    load_dir = FIRST;
    }

HUDFlowLayout::~HUDFlowLayout() {
    }

void HUDFlowLayout::addContent(HUDContent* content) {
    // Only add if NULL wasn't passed.
    if(content != NULL) {
        // Just index these by their order of being added.
        addItem(itemCount(), content);
        }
    else {
        ErrorHandler::record("HUDFlowLayout::addContent() - NULL passed for content",
                             ErrorHandler::FATAL);
        }
    }

void HUDFlowLayout::reBase(GLuint newHorz, GLuint newVert) {
    HUDContainer::reBase(newHorz, newVert);
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

    // How many items are contained?
    GLuint num_items = itemCount();

    // Go through and access each one.
    for(GLuint count = 0; count < num_items; ++count) {
        // Examine this item.
        HUDItem* current = getItem(count);

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
    /// TODO write cases to justify along the secondary axis.

    // We only need to bother with this if there are items to render.
    if(itemCount() > 0) {
        setGaps();

        // The settings apply for the case FIRST.
        GLuint newHorz = horzBase();
        GLuint newVert = vertBase();
        GLint flip = 1;

        // But if it is LAST ...
        if(load_dir == LAST) {
            /// TODO - this case is currently faulty.
//            switch(ax) {
//                case HORIZONTAL:
//                    newHorz += width();
//                    break;
//                case VERTICAL:
//                    // Default loading sense ( FIRST ) is from the top
//                    // going downwards, so for LAST is from the bottom
//                    // going upwards BUT we measure the vertical scale
//                    // as zero at the bottom and increasing upwards.
//                    newVert -= height();
//                    break;
//                default:
//                    // Shouldn't ever get here!!
//                    ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case (load/last) reached (default)",
//                                         ErrorHandler::FATAL);
//                    break;
//                }
//            flip = -1;
            }

        // Offset added as per flow axis.
        switch(ax) {
            case HORIZONTAL:
                newHorz += flip*start_offset;
                break;
            case VERTICAL:
                newVert += flip*start_offset;
                break;
            default:
                // Shouldn't ever get here!!
                ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case (offset) reached (default)",
                                     ErrorHandler::FATAL);
                break;
            }

        // Then contents are placed in the order of their insertion.
        std::map<int, HUDItem*>& container = getMap();
        for(std::map<int, HUDItem*>::iterator item = container.begin();
            item != container.end(); ++item) {
            // Place the current content.

            // Shift to next insert position by the dimensions of this content plus any gap,
            // accounting for the flow axis and direction of load.
            switch(ax) {
                case HORIZONTAL:
                    (*item).second->reBase(newHorz,
                                           newVert + setSideGap(secondary_just, this->height() - (*item).second->minHeight()));
                    newHorz += flip*((*item).second->minWidth() + item_gap);
                    break;
                case VERTICAL:
                    {
                    GLuint temp_minWidth = (*item).second->minWidth();
                    GLuint temp_horz = newHorz + setSideGap(secondary_just, this->width() - temp_minWidth);

                    HUDItem* ptr = (*item).second;
                    if(ptr == NULL) {
                        std::cout << "ptr is a NULL flubber !!" << std::endl;
                        }
                    else {
                        std::cout << "ptr is a not a NULL flubber !!" << std::endl;
                        }

                    HUDTextLine* htl_ptr = dynamic_cast<HUDTextLine*>(ptr);
                    if(htl_ptr != NULL) {
                        std::cout << "ptr has dynamic type HUDTextLine" << std::endl;
                        }
                    else {
                        std::cout << "htl_ptr is a NULL flubber !!" << std::endl;
                        }

                    HUDImage* hi_ptr = dynamic_cast<HUDImage*>(ptr);
                    if(hi_ptr != NULL) {
                        std::cout << "ptr has dynamic type HUDImage" << std::endl;
                        }
                    else {
                        std::cout << "hi_ptr is a NULL flubber !!" << std::endl;
                        }

                    ptr->reBase(temp_horz, newVert);
                    newVert += flip*((*item).second->minHeight() + item_gap);
                    }
                    break;
                default:
                    // Shouldn't ever get here!!
                    ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case (axis) reached (default)",
                                         ErrorHandler::FATAL);
                break;
                }
            }
        }
    }

void HUDFlowLayout::setGaps(void) {
    // Determine any available horizontal 'whitespace' b/w items. This is
    // the amount above the minimum required for display. By design this
    // whitespace ought be non-negative, but let's check to be sure.
    switch(this->ax) {
        case HORIZONTAL:
            if(width() >= minWidth()) {
                total_white_space = width() - minWidth();
                }
            else  {
                ErrorHandler::record("HUDFlowLayout::setGaps() - negative horizontal whitespace!!",
                                     ErrorHandler::FATAL);
                }
            break;
        case VERTICAL:
            if(height() >= minHeight()) {
                total_white_space = height() - minHeight();
                }
            else {
                ErrorHandler::record("HUDFlowLayout::setGaps() - negative vertical whitespace!!",
                                     ErrorHandler::FATAL);
                }
            break;
        default :
            ErrorHandler::record("HUDFlowLayout::setGaps() - bad switch case reached (default)",
                                  ErrorHandler::FATAL);
            break;
        }

    // For the items, what is the distribution of whitespace?
    // This depends upon the chosen primary justification style.
    switch(getPrimaryJustification()) {
        case START:
            // There's only a single gap on the other side to the justification.
            gap_count = 1;
            item_gap = 0;
            start_offset = 0;
            break;
        case END:
            // There's only a single gap on the other side to the justification.
            gap_count = 1;
            item_gap = 0;
            start_offset = total_white_space;
            break;
        case CENTRE:
            // We put all items in the centre and then have whitespace either side.
            gap_count = 2;
            item_gap = 0;
            start_offset = total_white_space/2;
            break;
        case START_AND_END:
            // Whitespace is distributed between all items, but none at either end.
            if(itemCount() == 1) {
                // With one item : like a START case.
                gap_count = 1;
                }
            else {
                // With more than one item : one less gap than there are items.
                gap_count = itemCount() - 1;
                }
            item_gap = total_white_space/gap_count;
            start_offset = 0;
            break;
        case SPAN:
            // Whitespace is distributed between all items, and some at either end.
            if(itemCount() == 1) {
                // With one item : like a CENTRE case.
                gap_count = 1;
                }
            else {
                // With more than one item : one more gap than there are items.
                gap_count = itemCount() + 1;
                }
            item_gap = total_white_space/gap_count;
            start_offset = item_gap/2;
            break;
        default:
            // Shouldn't ever get here!!
            ErrorHandler::record("HUDFlowLayout::setGaps() - bad switch case reached (default)",
                                 ErrorHandler::FATAL);
            break;
        }
    }

void HUDFlowLayout::setLoad(load end) {
    load_dir = end;
    }

GLuint HUDFlowLayout::setSideGap(secondaryJustification just, GLuint gap_total) {
    GLuint ret_val = 0;

    switch(just) {
        case PROXIMAL:
            ret_val = 0;
            break;
        case MIDDLE:
            ret_val = gap_total/2;
            break;
        case DISTAL:
            ret_val = gap_total;
            break;
        default:
            // Shouldn't ever get here!!
            ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case (horz/sec-adjust) reached (default)",
                                 ErrorHandler::FATAL);
            break;
        }

    return ret_val;
    }
