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

#ifndef ACCELERATED_PLATFORM_H_
#define ACCELERATED_PLATFORM_H_

#include "CameraState.h"
#include "RotatablePlatform.h"
#include "Vector3D.h"
#include "VelocityPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Fully accelerated Cartesian orthonormal vector set.
 *
 *      This class comprises acceleration state data, accessors and
 * mutators thereof. However this doesn't model angular acceleration
 * around centre of mass, only linear acceleration of centre of mass
 * and constant angular rotation rates. This is because a constant
 * angular rotation is already an acceleration. :-)
 *
 * \see CameraState
 * \see RotatablePlatform
 * \see Vector3D
 * \see VelocityPlatform
 *
 * \author Mike Hewson\n
 */

class AcceleratedPlatform : public VelocityPlatform, public RotatablePlatform {
    public:
        /**
         * \brief Constructor.
         */
        AcceleratedPlatform(void);

        /**
         * \brief Destructor.
         */
        virtual ~AcceleratedPlatform();

        /**
         * \brief Obtain the linear acceleration.
         *
         * \return a Vector3D representing the acceleration.
         */
        Vector3D linearAcceleration(void) const;

        /**
         * \brief Set the linear acceleration.
         *
         * \param acceleration : a Vector3D indicating the desired acceleration.
         */
        void setLinearAcceleration(const Vector3D& acceleration);

        /**
         * \brief Get the current rate of change of pitch angle.
         *
         * \return The pitch rate ( radians per notional time unit ).
         */
        vec_t pitchRate(void) const;

        /**
         * \brief Get the current rate of change of roll angle.
         *
         * \return The roll rate ( radians per notional time unit ).
         */
        vec_t rollRate(void) const;

        /**
         * \brief Get the current rate of change of yaw angle.
         *
         * \return The yaw rate ( radians per notional time unit ).
         */
        vec_t yawRate(void) const;

        /**
         * \brief Set the pitch rate.
         *
         * \param rate : the pitch angular rate
         *               ( radians per notional time unit ).
         */
        void setPitchRate(vec_t rate);

        /**
         * \brief Set the roll rate.
         *
         * \param rate : the roll angular rate
         *               ( radians per notional time unit ).
         */
        void setRollRate(vec_t rate);

        /**
         * \brief Set the yaw rate.
         *
         * \param rate : the yaw angular rate
         *               ( radians per notional time unit ).
         */
        void setYawRate(vec_t rate);

        /**
         * \brief Get the current view state information.
         *
         * \return The view in CameraState format.
         */
        CameraState viewState(void) const;

        /**
         * \brief Set the current view state.
         *
         * \param cam : the desired view in CameraState format.
         */
        void setViewState(const CameraState& cam);

        /**
         * \brief Reset the platform in linear acceleration,
         *        linear velocity, position and rotation.
         */
        virtual void reset(void);

        /**
         * \brief Evolve platform one step ( notional units ) in time.
         */
        virtual void step(void);

    private:
        /// Initial centre of mass linear acceleration.
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
