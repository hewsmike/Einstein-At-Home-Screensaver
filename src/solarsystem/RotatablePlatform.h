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

#ifndef ROTATABLE_PLATFORM_H_
#define ROTATABLE_PLATFORM_H_

#include "OrthoNormalPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Rotatable right handed Cartesian orthonormal vector set.
 *
 *      This class performs specific angular changes upon the axes of
 * the OrthoNormalPlatform class.
 *
 * \see OrthoNormalPlatform
 *
 * \author Mike Hewson\n
 */

class RotatablePlatform : public OrthoNormalPlatform {
    public:
        /**
         * \brief Constructor
         */
        RotatablePlatform(void);

        /**
         * \brief Destructor
         */
        virtual ~RotatablePlatform();

        /**
         * \brief Rotate in pitch by a given angle.
         *
         *      A positive angle for this manouevre is toward the 'up'
         * vector. The 'cross' vector is the axis of rotation and is
         * hence unaffected.
         *
         * \param angle in radians
         */
        void pitch(vec_t angle);

        /**
         * \brief Rotate in roll by a given angle.
         *
         *      A positive angle for this manouevre is to the left side.
         * The 'look' vector is the axis of rotation and is hence unaffected.
         *
         * \param angle in radians
         */
        void roll(vec_t angle);

        /**
         * \brief Rotate in yaw by a given angle.
         *
         *      A positive angle is to the left side. The 'up' vector is
         * the axis of rotation and is hence unaffected.
         *
         * \param angle in radians
         */
        void yaw(vec_t angle);
    };

/**
 * @}
 */

#endif // ROTATABLE_PLATFORM_H_
