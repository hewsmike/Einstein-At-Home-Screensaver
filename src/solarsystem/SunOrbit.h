/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#ifndef SUN_ORBIT_H_
#define SUN_ORBIT_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Models the Sun's orbit around the Earth.
 *
 *      Simple model of a circular orbit, crossing from the Southern
 * celestial hemisphere to the Northern at the zero of right ascension
 * on the vernal equinox day. Any input parameter in days is adjusted
 * modulo 365 to produce the position of the Sun's centre and/or the
 * rotation angle of the Sun around it's own axis.
 *
 * \author Mike Hewson\n
 */

class SunOrbit {
    public:
        /// The angle between the Earth's equatorial plane
        /// and the plane of the Earth's orbit around the Sun.
        static const GLfloat ECLIPTIC_ANGLE_DEG;

        /// The number of (sidereal) days in a year.
        static const GLfloat DAYS_PER_YEAR;

        /// The number of days for the Sun to rotate around
        /// it's own North/South axis.
        static const GLfloat DAYS_PER_ROTATION;

        /// The radius of the Sun's orbit.
        static const GLfloat SUN_ORBIT_RADIUS;

        /// The number of days from Jan 1st to the vernal equinox.
        static const int VERNAL_EQUINOX_DAY;

        /**
         *    \brief Destructor
         */
        virtual ~SunOrbit();

        /**
         * \brief Where is the Sun on a given day?
         *
         * \param days : the number of days before/since January the 1st,
         *               whereby modulo arithmetic is used to bring into
         *               the range [0.0, 365.0)
         *
         * \return position vector from the origin to the centre of the Sun
         */
        static Vector3D getPosition(GLfloat days);

        /**
         * \brief What is the rotation angle of the Sun on a given day?
         *
         * \param days : the number of days before/since January the 1st,
         *               whereby modulo arithmetic is used to bring into
         *               the range [0.0, 365.0)
         *
         * \return angle of the rotation, modulo 360 degrees
         */
        static GLfloat getRotation(GLfloat days);

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

