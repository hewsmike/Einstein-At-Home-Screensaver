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

#include "TriggerTimer.h"

const bool TriggerTimer::TRIGGERED(true);
const bool TriggerTimer::WAITING(!TRIGGERED);

const Uint32 TriggerTimer::INITIAL_TRIGGER(0);
const Uint32 TriggerTimer::TRIGGER_DEFAULT(500);
const Uint32 TriggerTimer::WRAPAROUND_TEST_VALUE(4294967295/2);

TriggerTimer::TriggerTimer(void) {
   trigger_interval = TRIGGER_DEFAULT;
   trigger_time = INITIAL_TRIGGER;
   }

TriggerTimer::~TriggerTimer() {
   }

void TriggerTimer::setInterval(Uint32 interval) {
   trigger_interval = interval;
   }

bool TriggerTimer::isTriggered(void) {
   // Assume it isn't trigger time yet.
   bool ret_val = false;

   // Get number of milliseconds since last SDL library initialisation.
   Uint32 now = SDL_GetTicks();

   // Have we passed the trigger time?
   if(now > trigger_time) {
      // Yes, then fire.
      ret_val = true;
      // Then reset the trigger to a future time as per
      // the designated interval ( in milliseconds ).
      trigger_time = now + trigger_interval;
      }
   else {
      // So 'now' is strictly less than 'trigger_time',
      // however is this way too large an interval?
      if((trigger_time - now) > WRAPAROUND_TEST_VALUE) {
         // Yes, so we have wrapped around the GetTicks timer.
         // This would occur with ~ 49 days of continuous running
         // since SDL library initialisation. Thus 'restart'
         // this algorithm.
         trigger_time = INITIAL_TRIGGER;
         }
      }

   return ret_val;
   }
