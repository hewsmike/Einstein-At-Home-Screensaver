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

#ifndef SUN_ORBIT_H_
#define SUN_ORBIT_H_

#include <iostream>

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Models the Sun's orbit around the Earth.
 *
 * \author Mike Hewson\n
 */

class SunOrbit {
   public:
      static const GLfloat ECLIPTIC_ANGLE_DEG;

      static const GLfloat DAYS_PER_YEAR;

      /// The radius of the Sun's orbit.
      static const GLfloat SUN_ORBIT_RADIUS;

      /// What number of days since Jan 1st is the vernal equinox ?
      static const int VERNAL_EQUINOX_DAY;

      /**
       * \brief Destructor
       */
      ~SunOrbit();

      /**
       * \brief Where is the Sun on a given day?
       *
       * \param days366 : the integral number of days since January the 1st,
       *                  ie. range is 0 - 365
       *
       * \return position vector from the origin to the centre of the Sun
       */
      static Vector3D getPosition(GLfloat days366);

   private:
      // Private constructor as static
       /**
       * \brief Constructor
       */
      SunOrbit(void);
   };

/**
 * @}
 */

#endif /* SUN_ORBIT_H_*/

