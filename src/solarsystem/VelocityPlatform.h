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

#ifndef VELOCITY_PLATFORM_H_
#define VELOCITY_PLATFORM_H_

#include "PositionPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Velocity platform
 *
 *      This class comprises velocity state data,
 * accessors and mutators thereof.
 *
 * \author Mike Hewson\n
 */

class VelocityPlatform : public PositionPlatform {
    public:
        VelocityPlatform(void);

        /**
         * \brief Constructor
         *
         * \param velocity : a vector indicating the desired velocity.
         */
        VelocityPlatform(const Vector3D& velocity);

        /**
         * \brief Destructor
         */
        virtual ~VelocityPlatform();

        /**
         * \brief Get the velocity
         */
        Vector3D velocity(void) const;

        /**
         * \brief Set the velocity
         *
         * \param velocity : a vector indicating the desired velocity.
         */
        void setVelocity(const Vector3D& velocity);

    protected:
        /**
         * \brief Reset the platform in velocity and position.
         */
        virtual void reset(void);

        /**
         * \brief Evolve platform one unit step in time
         */
        virtual void step(void);

    private:
        /// Fiducial velocity.
        static const Vector3D INITIAL_VELOCITY;

        /// Current velocity in space.
        Vector3D vel;
    };

/**
 * @}
 */

#endif // VELOCITY_PLATFORM_H_
