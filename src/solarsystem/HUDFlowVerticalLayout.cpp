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

#include "HUDFlowVerticalLayout.h"

HUDFlowVerticalLayout::HUDFlowVerticalLayout(HUDContainer* enclosing) :
                       HUDFlowLayout(enclosing) {
   }

HUDFlowVerticalLayout::~HUDFlowVerticalLayout() {
   }

std::pair<GLuint, GLuint> HUDFlowVerticalLayout::reassessMinimumDimensions(void) {
   // Assume an empty container.
   GLuint layoutMinWidth = 0;
   GLuint layoutMinHeight = 0;

   // How many items are contained?
   GLuint num_items = itemCount();

   // Go through and access each one.
   for(GLuint count = 0; count < num_items; ++count) {
      // Examine this item.
      HUDItem* current = getItem(count);

      // We add up all the minimum heights.
      layoutMinHeight += current->minHeight();

      // But take the overall minimum width to be the
      // greatest of the minima of the contained items.
      GLuint currentWidth = current->minWidth();
      if(currentWidth > layoutMinWidth) {
         layoutMinWidth = currentWidth;
         }
      }

   return std::pair<GLuint, GLuint>(layoutMinWidth, layoutMinHeight);
   }

void HUDFlowVerticalLayout::allocateItemBases(void) {
   // TODO write cases to justify along the secondary axis.

   // How many items are contained?
   GLuint num_items = itemCount();

   // We only need to bother with this if there are items to render.
   if(num_items > 0) {
      // Determine any available vertical 'whitespace' b/w items. This
      // is the amount above the minimum required for display.
      GLuint w_space = 0;

      // By design whitespace ought be non-negative, but let's check.
      if(height() >= minHeight()) {
         w_space = height() - minHeight();
         }
      else {
         ErrorHandler::record("HUDFlowVerticalLayout::allocateItemBases() - negative whitespace!!",
                              ErrorHandler::FATAL);
         }

      // What is the number of gaps to distribute whitespace amongst?
      GLuint gap_count = gapCount(num_items);

      // As a float it will likely round/truncate later.
      GLfloat white_space_per_gap = static_cast<GLfloat>(w_space)/gap_count;

      // For a vertical flow, all content is at the same horizontal offset.
      GLuint newHorz = horzBase();
      GLuint newVert = 0;

      switch(getPrimaryJustification()) {
         case START:
            // Vertical placement begins at the top edge of container
            newVert = vertBase() + height();

            // Then contents are placed downwards in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Shift down to current insert position by the height of this content.
               newVert -= getItem(count)->minHeight();
               // Place the current content.
               getItem(count)->reBase(newHorz, newVert);
               }
            break;
         case END:
            // Vertical placement begins at the bottom edge of container
            // plus the total height of all contained content.
            newVert = vertBase() + minHeight();

            // Then contents are placed downwards in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Shift down to the current insert position by the height of this content.
               newVert -= getItem(count)->minHeight();
               // Place the current content.
               this->getItem(count)->reBase(newHorz, newVert);
               }
            break;
         case CENTRE:
            // We put all items in the centre and then have whitespace either side.
            // So vertical placement begins at the top edge of container minus
            // a gap's worth of whitespace.
            newVert = vertBase() + height() - white_space_per_gap;

            // Then contents are placed in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Shift down to current insert position by the height of this content.
               newVert -= getItem(count)->minHeight();
               // Place the current content.
               getItem(count)->reBase(newHorz, newVert);
               }
            break;
         case START_AND_END:
            // Whitespace is distributed between all items, but none at either end.
            // Horizontal placement begins at the left edge of container
            // TODO : write this case !!

            break;
         case SPAN:
            // Whitespace is distributed between all items, and some at either end.
            // TODO : write this case !!
            break;
         default:
            // Shouldn't ever get here!!
            ErrorHandler::record("HUDFlowVerticalLayout::allocateItemBases() - bad switch case reached (default)",
                                 ErrorHandler::FATAL);
            break;
         }
      }
   }
