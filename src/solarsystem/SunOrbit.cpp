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

#include "SunOrbit.h"

const GLfloat SunOrbit::ECLIPTIC_ANGLE_DEG(23.5f);
const GLfloat SunOrbit::DAYS_PER_YEAR(365.0f);
const GLfloat SunOrbit::DAYS_PER_ROTATION(27.0f);
const GLfloat SunOrbit::SUN_ORBIT_RADIUS(5*SolarSystemGlobals::SUN_RADIUS);
const int SunOrbit::VERNAL_EQUINOX_DAY(79);

SunOrbit::SunOrbit(void) {
   }

SunOrbit::~SunOrbit() {
   }

Vector3D SunOrbit::getPosition(GLfloat days366) {
   // TODO - be bothered with leap years !!! :-)

   if((days366 < 0) || (days366 > DAYS_PER_YEAR)) {
      std::string msg = "SunOrbit:getPosition() - bad days366 parameter passed";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Well to start with on, say, Mar 21st the Sun is at the vernal equinox.
   days366 = (days366 - VERNAL_EQUINOX_DAY);
   if(days366 < 0) {
      days366 += DAYS_PER_YEAR;
      }

   GLfloat theta = (days366/DAYS_PER_YEAR)*SolarSystemGlobals::FULL_CIRCLE_DEG;

   // Just a circle.
   return SUN_ORBIT_RADIUS * Vector3D(COS(theta),
                                      SIN(theta) * COS(ECLIPTIC_ANGLE_DEG),
                                      SIN(theta) * SIN(ECLIPTIC_ANGLE_DEG));
   }

GLfloat SunOrbit::getRotation(GLfloat days366) {
   // TODO - be bothered with leap years !!! :-)

   if((days366 < 0) || (days366 >= DAYS_PER_YEAR)) {
      std::string msg = "SunOrbit:getPosition() - bad days366 parameter passed";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // How many days are we into this rotation - modulo rotation period ?
   days366 = days366 - DAYS_PER_ROTATION * static_cast<int>(days366/DAYS_PER_ROTATION);

   return (days366/DAYS_PER_ROTATION)*SolarSystemGlobals::FULL_CIRCLE_DEG;;
   }
