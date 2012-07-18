/***************************************************************************
 * Copyright (C) 2012 by Mike Hewson                                       *
 * hewsmike[AT]iinet.net.au                                                *
 *                                                                         *
 * This file is part of Einstein@Home.                                     *
 *                                                                         *
 * Einstein@Home is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published       *
 * by the Free Software Foundation, version 2 of the License.              *
 *                                                                         *
 * Einstein@Home is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#include "TriggerTimer.h"

#include "ErrorHandler.h"

// This caps callbacks at 100Hz.
const GLuint TriggerTimer::TRIGGER_MIN(10);

TriggerTimer::TriggerTimer(double interval, void (*cbFunc)(void)) {
    // Having a NULL callback gives a useless timer, so
    // this ought be immediately disclosed as a fatal.
    if(cbFunc == NULL) {
        ErrorHandler::record("TriggerTimer::TriggerTimer() : NULL passed for timer callback", ErrorHandler::FATAL);
        }

    // Remember the callback routine to be invoked.
    callback = cbFunc;

    // Enforce minimum trigger interval.
    if(interval < TRIGGER_MIN) {
        interval = TRIGGER_MIN;
        }

    // Convert interval in milliseconds to minutes.
    trigger_interval = interval/1000;

    // Set first trigger point.
    trigger_time = glfwGetTime() + trigger_interval;
    }

TriggerTimer::~TriggerTimer() {
    }

void TriggerTimer::update(void) {
    // Get number of seconds since the GLFW initialisation.
    double now = glfwGetTime();

    // Fire the trigger if we have passed the trigger point.
    if(now > trigger_time) {
        // Assuming we have a non-NULL callback target, invoke it.
        if(callback != NULL) {
            callback();
            }

        // Reset the trigger point.
        trigger_time = now + trigger_interval;
        }
    }
