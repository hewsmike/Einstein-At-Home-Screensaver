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

#ifndef TRIGGER_TIMER_H_
#define TRIGGER_TIMER_H_

#include "framework.h"

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief Enacts a countdown timer with recurrent triggering
 *	based upon a given interval, measured in milliseconds.
 *
 *		It requires polling to allow updates from the system clock,
 *	and determine if a trigger has fired. A given callback routine
 * is called when such occurs.
 *
 * \author Mike Hewson\n
 */

class TriggerTimer {
   public:
		/// Enact a minimum time in milliseconds between triggers.
      static const GLuint TRIGGER_MIN;

      /**
       * \brief Constructor
       *
       * \param interval : the minimum time in milliseconds between triggers.
       * \param cbFunc : the function to call upon triggering
       */
      TriggerTimer(double interval, void (*cbFunc)(void));

      /**
       * \brief Destructor
       */
      virtual ~TriggerTimer();

      /**
       * \brief Refresh the timer, and act if triggered.
       */
      void update(void);

   private:
      /// The time when the next trigger occurs.
      double trigger_time;

      /// The interval in seconds between triggers.
      double trigger_interval;

		/// Who you gonna call .... :-)
      void (*callback)(void);
   };

/**
 * @}
 */

#endif /* TRIGGER_TIMER_H_ */
