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

#ifndef POSITION_PLATFORM_H_
#define POSITION_PLATFORM_H

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Position platform.
 *
 *      This class comprises position state data, accessors and mutators thereof.
 *
 * \author Mike Hewson\n
 */

class PositionPlatform {
    public:
        PositionPlatform(void);

        /**
         * \brief Constructor.
         *
         * \param position : a vector to the desired point
         */
        PositionPlatform(const Vector3D& position);

        /**
         * \brief Destructor.
         */
        virtual ~PositionPlatform();

        /**
         * \brief Get the position.
         */
        Vector3D position(void) const;

        /**
         * \brief Set the position.
          *
         * \param position : a vector to the desired point
         */
        void setPosition(const Vector3D& position);

    protected:
        /**
         * \brief Reset the platform in position.
         */
        virtual void reset(void);

    private:
        /// Initial position.
        static const Vector3D INITIAL_POSITION;

        /// Current position in space.
        Vector3D pos;
    };

/**
 * @}
 */

#endif // Position_PLATFORM_H_
