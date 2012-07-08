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

#ifndef SPIN_PLATFORM_H_
#define SPIN_PLATFORM_H_

#include "OrthoNormalPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem rotatable right handed Cartesian orthonormal vector set
 *
 *      This class comprises angular rate data, accessors and mutators
 * thereof for an OrthoNormalPlatform.
 *
 * \see OrthoNormalPlatform
 *
 * \author Mike Hewson\n
 */

class SpinPlatform : public OrthoNormalPlatform {
    public:
        /**
         * \brief Constructor
         */
        SpinPlatform(void);

        /**
         * \brief Destructor
         */
        virtual ~SpinPlatform();

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

    protected:
        /**
         * \brief Reset the platform in spin
         */
        virtual void reset(void);

        /**
         * \brief Evolve the platform in spin
         */
        virtual void step(void);

        /**
         * \brief Set the pitch rate
         *
         * \param the pitch angular rate ( notional units )
         */
        void setPitchRate(vec_t rate);

        /**
         * \brief Set the roll rate
         *
         * \param the roll angular rate ( notional units )
         */
        void setRollRate(vec_t rate);

        /**
         * \brief Set the yaw rate
         *
         * \param the yaw angular rate ( notional units )
         */
        void setYawRate(vec_t rate);

    private:
        static const vec_t NULL_RATE;

        // Current rotation rates.
        vec_t pitch_rate;
        vec_t roll_rate;
        vec_t yaw_rate;
	};

/**
 * @}
 */

#endif // SPIN_PLATFORM_H_
