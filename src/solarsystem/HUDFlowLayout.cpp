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

HUDFlowLayout::HUDFlowLayout(HUDContainer* enclosing, Axis axis) :
               HUDContainer(enclosing),
               ax(axis) {
   gap_count = 0;
   total_white_space = 0;
   start_offset = 0;
   item_gap = 0;
   primary_just = CENTRE;
   secondary_just = MIDDLE;
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

      // We add up all the minimum widths.
      layoutMinWidth += current->minWidth();

      // But take the overall minimum height to be the
      // greatest of the minima of the contained items.
      GLuint currentHeight = current->minHeight();
      if(currentHeight > layoutMinHeight) {
         layoutMinHeight = currentHeight;
         }
      }

   return std::pair<GLuint, GLuint>(layoutMinWidth, layoutMinHeight);
   }

void HUDFlowLayout::allocateItemBases(void) {
   // TODO write cases to justify along the secondary axis.

   // We only need to bother with this if there are items to render.
   if(itemCount() > 0) {
      setGaps();
      // Starting position.
      GLuint newHorz = horzBase();
      GLuint newVert = vertBase();

      // Offset added as per flow axis.
      switch(ax) {
      	case HORIZONTAL:
      		newHorz += start_offset;
      		break;
      	case VERTICAL:
      		newVert += start_offset;
      		break;
      	default:
      		// Shouldn't ever get here!!
      		ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case reached (default)",
      									ErrorHandler::FATAL);
      		break;
      	}

      // Then contents are placed in the order of their insertion.
      for(GLuint count = 0; count < itemCount(); ++count) {
      	// Place the current content.
      	getItem(count)->reBase(newHorz, newVert);

      	// Shift to next insert position by the dimensions of this content plus any gap,
      	// accounting for the flow axis.
      	switch(ax) {
      		case HORIZONTAL:
      			newHorz += getItem(count)->minWidth() + item_gap;
      			break;
      	   case VERTICAL:
      	   	newVert += getItem(count)->minHeight() + item_gap;;
      	      break;
      	   default:
      	   	// Shouldn't ever get here!!
      	      ErrorHandler::record("HUDFlowLayout::allocateItemBases() - bad switch case reached (default)",
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
   if(width() >= minWidth()) {
      total_white_space = width() - minWidth();
      }
   else {
      ErrorHandler::record("HUDFlowLayout::setGaps() - negative whitespace!!",
                           ErrorHandler::FATAL);
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
            // With one item : like a START or END case.
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
            // With one item : like a START or END case.
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
