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

#include "HUDTextLineScroll.h"

#include "ErrorHandler.h"

HUDTextLineScroll::HUDTextLineScroll(GLuint length, OGLFT_ft* font,
                                     GLuint horizontalMargin, GLuint verticalMargin,
                                     mode direction = NONE) :
                                        HUDTextLine(length, font, horizontalMargin, verticalMargin),
                                        dir(direction) {
	//trig = new TriggerTimer(300.0f, &HUDTextLineScroll::trigger_callback);
   isTriggered = false;
   }

HUDTextLineScroll::~HUDTextLineScroll() {
   }

HUDTextLineScroll::mode HUDTextLineScroll::getDirection(void) {
   return dir;
   }

void HUDTextLineScroll::setDirection(mode direction = NONE) {
   dir = direction;
   }

void HUDTextLineScroll::toggleDirection(void) {
   switch(dir) {
      case LEFT:
         dir = RIGHT;
         break;
      case RIGHT:
         dir = LEFT;
         break;
      case NONE:
         break;
      default:
         // Ought not get here !!
         std::string msg = "HUDTextLineScroll::toggleDirection() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void HUDTextLineScroll::trigger_callback(void) {
   isTriggered = true;
   }

void HUDTextLineScroll::render(void) {
   static const GLuint MIN_STRING_SIZE = 2;

   // Has the timer triggered ?
   if(isTriggered == true) {
      // Yes, get the line's text content.
      std::string contents = getText();
      // It must have at least two characters
      if(contents.size() >= MIN_STRING_SIZE) {
         // Depending upon the scroll direction .....
         switch(dir) {
            case NONE:
               // Do nothing.
               break;
            case LEFT:
               // Rotate text to the left.
               {
                  // Get the first character.
                  std::string first = contents.substr(0, 1);
                  // Get the remainder.
                  std::string remainder = contents.substr(1);
                  // Reverse their order.
                  contents = remainder + first;
               }
               break;
            case RIGHT:
               // Rotate text to the right.
               {
                  // Get the last character.
                  std::string last = contents.substr(contents.length() - 1);
                  // Get the remainder.
                  std::string remainder = contents.substr(0, contents.length() - 1);
                  // Reverse their order.
                  contents = last + remainder;
               }
               break;
            default:
               // Ought not get here !!
               std::string msg = "HUDTextLineScroll::render() - bad switch case reached (default)";
               ErrorHandler::record(msg, ErrorHandler::FATAL);
               break;
            }
         // Set the (new) text contents.
         setText(contents);
         }
      isTriggered = false;
      }

   // Then render this (altered) line as per base class.
   HUDTextLine::render();
   }
