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

HUDFlowLayout::HUDFlowLayout(HUDContainer* enclosing) :
               HUDContainer(enclosing) {
   primary_just = CENTRE;
   secondary_just = CENTRE;
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

void HUDFlowLayout::setPrimaryJustification(Justification justification) {
   primary_just = justification;
   allocateItemBases();
   }

void HUDFlowLayout::setSecondaryJustification(Justification justification) {
   secondary_just = justification;
   allocateItemBases();
   }

HUDFlowLayout::Justification HUDFlowLayout::getPrimaryJustification(void) const {
   return primary_just;
   }

HUDFlowLayout::Justification HUDFlowLayout::getSecondaryJustification(void) const {
   return secondary_just;
   }

GLuint HUDFlowLayout::gapCount(GLuint numItems) const {
   // For the items, what is the number of gaps to distribute whitespace
   // amongst? This depends upon the choice of justification style, but
   // not upon the flow 'axis'.
   GLuint gap_count = 0;
   switch(getPrimaryJustification()) {
      case START:
      case END:
         // There's only a single gap on the other side to the justification.
         gap_count = 1;
         break;
      case CENTRE:
         // We put all items in the centre and then have whitespace either side.
         gap_count = 2;
         break;
      case START_AND_END:
         // Whitespace is distributed between all items, but none at either end.
         if(numItems == 1) {
            // With one item : like a START or END case.
            gap_count = 1;
            }
         else {
            // With more than one item : one less gap than there are items.
            gap_count = numItems - 1;
            }
         break;
      case SPAN:
         // Whitespace is distributed between all items, and some at either end.
         if(numItems == 1) {
            // With one item : like a START or END case.
            gap_count = 1;
            }
         else {
            // With more than one item : one more gap than there are items.
            gap_count = numItems + 1;
            }
         break;
      default:
         // Shouldn't ever get here!!
         ErrorHandler::record("HUDFlowLayout::gapCount() - bad switch case reached (default)",
                              ErrorHandler::FATAL);
         break;
      }

   return gap_count;
   }
