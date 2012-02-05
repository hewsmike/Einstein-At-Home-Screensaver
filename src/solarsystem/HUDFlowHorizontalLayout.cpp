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

#include "HUDFlowHorizontalLayout.h"

HUDFlowHorizontalLayout::HUDFlowHorizontalLayout(HUDContainer* enclosing) :
                         HUDFlowLayout(enclosing) {
   }

HUDFlowHorizontalLayout::~HUDFlowHorizontalLayout() {
   }

std::pair<GLuint, GLuint> HUDFlowHorizontalLayout::reassessMinimumDimensions(void) {
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

void HUDFlowHorizontalLayout::allocateItemBases(void) {
   // TODO write cases to justify along the secondary axis.

   // How many items are contained?
   GLuint num_items = itemCount();

   // We only need to bother with this if there are items to render.
   if(num_items > 0) {
      // Determine any available horizontal 'whitespace' b/w items. This is
      // the amount above the minimum required for display.
      GLuint w_space = 0;

      // By design whitespace ought be non-negative, but let's check.
      if(width() >= minWidth()) {
         w_space = width() - minWidth();
         }
      else {
         ErrorHandler::record("HUDFlowHorizontalLayout::allocateItemBases() - negative whitespace!!",
                              ErrorHandler::FATAL);
         }

      // What is the number of gaps to distribute whitespace amongst?
      GLuint gap_count = gapCount(num_items);

      // As a float it will likely round/truncate later.
      GLfloat white_space_per_gap = static_cast<GLfloat>(w_space)/gap_count;

      // Start horizontal offets at the left edge.
      GLuint newHorz = horzBase();
      // For a horizontal flow, all content is at the same vertical offset.
      GLuint newVert = vertBase();

      switch(getPrimaryJustification()) {
         case START:
            // Then contents are placed rightwards in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Place the current content.
               getItem(count)->reBase(newHorz, newVert);
               // Shift right to next insert position by the width of this content.
               newHorz += getItem(count)->minWidth();
               }
            break;
         case END:
            // Horizontal placement begins at the right edge of container
            // minus the total width of all contained content.
            newHorz += width() - minWidth();

            // Then contents are placed rightwards in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Place the current content.
               getItem(count)->reBase(newHorz, newVert);
               // Shift right to the next insert position by the width of this content.
               newHorz += getItem(count)->minWidth();
               }
            break;
         case CENTRE:
            // We put all items in the centre and then have whitespace either side.
            // So horizontal placement begins at the left edge of container plus
            // a gap's worth of whitespace.
            newHorz += white_space_per_gap;

            // Then contents are placed in the order of their insertion.
            for(GLuint count = 0; count < num_items; ++count) {
               // Place the current content.
               getItem(count)->reBase(newHorz, newVert);
               // Shift right to the next insert position by the width of this content.
               newHorz += getItem(count)->minWidth();
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
            ErrorHandler::record("HUDFlowHorizontalLayout::allocateItemBases() - bad switch case reached (default)",
                                 ErrorHandler::FATAL);
            break;
         }
      }
   }
