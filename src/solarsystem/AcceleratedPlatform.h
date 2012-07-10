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

#ifndef ACCELERATED_PLATFORM_H_
#define ACCELERATED_PLATFORM_H_

#include "CameraState.h"
#include "VelocityPlatform.h"
#include "RotatablePlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Fully accelerated Cartesian orthonormal vector set
 *
 *      This class comprises acceleration state data, accessors
 * and mutators thereof. HOWEVER this doesn't model angular
 * acceleration ( around centre of mass ), only linear ( of
 * centre of mass ) and CONSTANT angular rotation rates.
 *
 * \author Mike Hewson\n
 */

class AcceleratedPlatform : public VelocityPlatform, public RotatablePlatform {
    public:
        /**
         * \brief Constructor
         */
        AcceleratedPlatform(void);

        /**
         * \brief Destructor
         */
        virtual ~AcceleratedPlatform();

        /**
         * \brief Get the acceleration
         *
         * \return a vector representing the acceleration
         */
        Vector3D linearAcceleration(void) const;

        /**
         * \brief Set the acceleration
         *
         * \param acceleration : a vector indicating the desired acceleration
         */
        void setLinearAcceleration(const Vector3D& acceleration);

        /**
         * \brief Get the current rate of change of pitch
         *
         * \return the pitch rate
         */
        vec_t pitchRate(void) const;

        /**
         * \brief Get the current rate of change of roll
         *
         * \return the roll rate
         */
        vec_t rollRate(void) const;

        /**
         * \brief Get the current rate of change of yaw
         *
         * \return the yaw rate
         */
        vec_t yawRate(void) const;

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

        /**
         * \brief Get the current view state information
         *
         * \return the view state in camera format
         */
        CameraState viewState(void) const;

        /**
         * \brief Set the current view state
         *
         * \param the desired view state in camera format
         */
        void setViewState(const CameraState& cam);

        /**
         * \brief Reset the platform in acceleration, velocity,
         *        position and rotation
         */
        virtual void reset(void);

        /**
         * \brief Evolve platform one unit step in time
         */
        virtual void step(void);

    private:
        /// Initial centre of mass acceleration.
        static const Vector3D INITIAL_LINEAR_ACCELERATION;

        /// Initial around centre of mass rotation.
        static const vec_t NULL_RATE;

        // Current rotation rates.
        vec_t pitch_rate;
        vec_t roll_rate;
        vec_t yaw_rate;

        // Current centre of mass acceleration.
        Vector3D acc;
    };

/**
 * @}
 */

#endif // ACCELERATED_PLATFORM_H_
