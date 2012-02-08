/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#ifndef HUD_TRIGGER_TIMER_H_
#define HUD_TRIGGER_TIMER_H_

#include "SDL.h"
#include "SDL_opengl.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Enacts a timer with recurrent triggering based upon a
 *        given interval, measured in milliseconds.
 *
 *        It requires polling to determine whether or not a trigger
 *        interval has elapsed. This interval ought not be too short
 *        with respect to OS task switching, say, stay over 30
 *        milliseconds. Nor should it be too long with respect to
 *        the wraparound of the underlying hardware timer, say, stay
 *        under 10 days.
 *
 * \author Mike Hewson\n
 */

class TriggerTimer {
   public:
      static const bool TRIGGERED;
      static const bool WAITING;

      static const Uint32 INITIAL_TRIGGER;
      static const Uint32 TRIGGER_DEFAULT;
      static const Uint32 WRAPAROUND_TEST_VALUE;

      /**
       * \brief Constructor
       */
      TriggerTimer(void);

      /**
       * \brief Destructor
       */
      virtual ~TriggerTimer();

      /**
       * \brief Define the trigger inteval
       *
       * \param interval : the minimum time in milliseconds between triggers.
       */
      void setInterval(Uint32 interval);

      /**
       * \brief Poll the timer to see if it has triggered.
       *
       * \return bool : TRIGGERED if the trigger interval has been exceeded
       *                WAITING if the trigger interval has not been exceeded
       */
      bool isTriggered(void);

   private:
      // The time when the next trigger occurs.
      Uint32 trigger_time;

      // The interval in milliseconds between triggers.
      Uint32 trigger_interval;
   };

/**
 * @}
 */

#endif /* HUD_TRIGGER_TIMER_H_ */
