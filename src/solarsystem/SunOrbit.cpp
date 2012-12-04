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

#include "SunOrbit.h"

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"

const GLfloat SunOrbit::ECLIPTIC_ANGLE_DEG(23.5f);
const GLfloat SunOrbit::DAYS_PER_YEAR(365.00f);
// This gives close to 26 days per rotation ( about right ),
// and a nice integral number of rotations per year - so no
// jumps on Jan 1st
const GLfloat SunOrbit::DAYS_PER_ROTATION(DAYS_PER_YEAR/14.0f);

// Arbitrary but looks nice
const GLfloat SunOrbit::SUN_ORBIT_RADIUS(5*SolarSystemGlobals::SUN_RADIUS);

// Ignoring leap years this is March 21st
const int SunOrbit::VERNAL_EQUINOX_DAY(79);

SunOrbit::SunOrbit(void) {
    }

SunOrbit::~SunOrbit() {
    }

Vector3D SunOrbit::getPosition(GLfloat days) {
    // Well to start with : on Mar 21st the Sun is at the vernal equinox.
    days = days - VERNAL_EQUINOX_DAY;

    // Bring into the 'principal value' domain.
    // days = moduloDays(days, DAYS_PER_YEAR);

    // OK, now get an angle in the ecliptic plane between a vector to the
    // vernal equinox and a vector to the position of the Sun on the given
    // day. Positive going anti-clockwise when looking down on that plane
    // from the northern side.
    GLfloat theta = (days/DAYS_PER_YEAR)*SolarSystemGlobals::FULL_CIRCLE_DEG;

    // Just modelling a circle but tilted with respect to the celestial equator.
    return SUN_ORBIT_RADIUS * Vector3D(COS(theta),
                                       SIN(theta) * COS(ECLIPTIC_ANGLE_DEG),
                                       SIN(theta) * SIN(ECLIPTIC_ANGLE_DEG));
    }

GLfloat SunOrbit::getRotation(GLfloat days) {
    // Rotation for the Sun, for our purposes, is quite arbitrary
    // ( not a rigid body and without constant features ) ...
    days = moduloDays(days, DAYS_PER_ROTATION);

    return (days/DAYS_PER_ROTATION)*SolarSystemGlobals::FULL_CIRCLE_DEG;
    }

GLfloat SunOrbit::moduloDays(GLfloat day, GLfloat days_per_cycle) {
    GLfloat ret_val = 0;

    // Note : the lower range boundary is included.
    if(day <= 0) {
        ret_val = day + days_per_cycle;
        }

    // Note : the upper range boundary is excluded.
    if(day > days_per_cycle) {
        ret_val = day - days_per_cycle;
        }

    return ret_val;
    }
