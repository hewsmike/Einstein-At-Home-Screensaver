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

#ifndef CRAFT_H_
#define CRAFT_H_

#include "AcceleratedPlatform.h"
#include "CameraState.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Simulates the kinetic/dynamic behaviours of a spacecraft.
 *
 *      The spacecraft translates as if it is in an inertial frame ie. persists
 * in a movement until otherwise instructed. Rotation commands act to alter the
 * rotation rate around a given axis. Simulation boundaries are tested for and
 * the craft movement is altered automatically to keep the craft within range
 * and outside of the Sun and the Earth.
 *
 *      Positive pitch is deemed nose up about right-to-left axis, positive roll
 * is deemed to the left about aft-to-fore axis and positive yaw is deemed to
 * the left about ceiling-to-floor axis.
 *
 * \see AcceleratedPlatform
 * \see CameraState
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class Craft {
    public:
        /// The craft's initial position.
        static const Vector3D START_POSITION;

        /// Enumerants describing control choices.
        enum movements {GO_HOME,
                        STOP_TRANSLATION,
                        STOP_ROTATION,
                        FORWARD,
                        REVERSE,
                        UPWARDS,
                        DOWNWARDS,
                        LEFTWARDS,
                        RIGHTWARDS,
                        PITCH_UP,
                        PITCH_DOWN,
                        ROLL_LEFT,
                        ROLL_RIGHT,
                        YAW_LEFT,
                        YAW_RIGHT };

        /**
         * \brief Constructor.
         */
        Craft();

        /**
         * \brief Destructor.
         */
        ~Craft();

        /**
         * \brief Manouevre the craft.
         *
         * \param movement : one of the allowable enumerants.
         */
        void manouevre(Craft::movements movement);

        /**
         * \brief Obtain the platform data.
         *
         * \return a constant reference to the platform.
         */
        const AcceleratedPlatform& platform() const;

        /**
         * \brief Evolve the craft's dynamics.
         *
         *      Update craft state variables using one notional time unit.
         *
         * \param dayOfYear : number of days since Jan 1st used to determine,
         *                    if needed, the position of other simulated objects
         *                    that may be dynamically relevant.
         */
        void step(GLfloat dayOfYear);

        /**
         * \brief Obtain the view state of the craft in camera format.
         *
         * \return the CameraState.
         */
        CameraState viewState(void) const;

        /**
         * \brief Set the view state of the craft in camera format.
         *
         * \param camera : the desired CameraState.
         */
        void setViewState(const CameraState& camera);

    private:
        /// Initial radial distance from origin.
        static const vec_t START_RADIUS;

        /// Global bounds for craft position
        static const vec_t MAX_RANGE;
        static const vec_t MIN_EARTH_RANGE;
        static const vec_t MIN_SUN_RANGE;

        /// Top speed of craft in whatever nett direction
        static const vec_t MAX_SPEED;

        /// Rates for about centre of mass craft rotations.
        static const vec_t PITCH_RATE_DELTA;
        static const vec_t ROLL_RATE_DELTA;
        static const vec_t YAW_RATE_DELTA;
        static const vec_t RATE_FUDGE;
        static const vec_t MAX_ROTATION_RATE;

        /// Speed setting post 'bounce' off objects.
        static const vec_t REBOUND_SPEED;

        /// Speed change steps for the aft-to-forward craft axis.
        static const vec_t INLINE_THRUST_DELTA;

        /// Speed change step for the left-to-right craft axis.
        static const vec_t LATERAL_THRUST_DELTA;

        /// Speed change step for the floor-to-ceiling craft axis.
        static const vec_t VERTICAL_THRUST_DELTA;

        /// Dynamic platform.
        AcceleratedPlatform state;

        /**
         * \brief Return the craft to a 'home' state.
         */
        void goHome(void);

        /**
         * \brief Cease translation.
         */
        void stop(void);

        /**
         * \brief Cease rotation.
         */
        void nullRotation(void);

        /**
         * \brief Apply thrust along aft-TO-fore axis.
         */
        void forwardThrust(void);

        /**
         * \brief Apply thrust along fore-TO-aft axis.
         */
        void reverseThrust(void);

        /**
         * \brief Apply thrust along floor-TO-ceiling axis.
         */
        void upThrust(void);

        /**
         * \brief Apply thrust along ceiling-TO-floor axis.
         */
        void downThrust(void);

        /**
         * \brief Apply thrust along right-TO-left axis.
         */
        void leftThrust(void);

        /**
         * \brief Apply thrust along left-TO-right axis.
         */
        void rightThrust(void);

         /**
         * \brief Increase the pitch rate.
         */
        void noseUp();

        /**
         * \brief Decrease the pitch rate.
         */
        void noseDown();

        /**
         * \brief Decrease the roll rate.
         */
        void rollLeft();

        /**
         * \brief Increase the roll rate.
         */
        void rollRight();

        /**
         * \brief Decrease the yaw rate.
         */
        void yawLeft();

        /**
         * \brief Increase the yaw rate.
         */
        void yawRight();

        /**
         * \brief Apply thrust in a given direction and strength.
         */
        void vectorThrust(Vector3D thrust);
    };

/**
 * @}
 */

#endif // CRAFT_H_
