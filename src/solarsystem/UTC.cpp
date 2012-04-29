/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

#include "UTC.h"

UTC::UTC(void) {
   }

UTC::~UTC() {
   }

void UTC::getTime(int* minutes60, int* hours24, int* days366) {
   // Get the number of seconds since 00:00 on Jan 1st 1970
   time_t seconds = time(NULL);

   // Pointer to a 'tm' struct
   struct tm* now;

   // Fill out the 'tm' struct given the 'time_t' value
   now = gmtime(&seconds);

   // Dereference the pointer as passed to set the minutes value, while bewaring
   // of nullity.
   if(minutes60 != NULL) {
      *minutes60 = now->tm_min;
      }
   else {
      ErrorHandler::record("UTC::getTime() : NULL pointer passed for minutes60 parameter", ErrorHandler::FATAL);
      }

   // Dereference the pointer as passed to set the hour value, while bewaring
   // of nullity.
   if(hours24 != NULL) {
      *hours24 = now->tm_hour;
      }
   else {
      ErrorHandler::record("UTC::getTime() : NULL pointer passed for hours24 parameter", ErrorHandler::FATAL);
      }

   // Dereference the pointer as passed to set the day value, while bewaring
   // of nullity.
   if(days366 != NULL) {
      *days366 = now->tm_yday;
      }
   else {
      ErrorHandler::record("UTC::getTime() : NULL pointer passed for days366 parameter", ErrorHandler::FATAL);
      }
   }
