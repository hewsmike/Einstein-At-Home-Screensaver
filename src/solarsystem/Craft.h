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
 * \brief Simulates the kinetic/dynamic behaviours of a spaceship.
 *
 * \author Mike Hewson\n
 */

class Craft {
    public:
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
         * \brief Constructor
         */
        Craft();

        /**
         * \brief Destructor
         */
        ~Craft();

        /**
         * \brief Manouevre the craft
         *
         * \param movement : one of the allowable enumerants
         */
        void manouevre(Craft::movements movement);

        /**
         * \brief Obtain the platform data
         *
         * \return a constant reference to the platform
         */
        const AcceleratedPlatform& platform() const;

        /**
         * \brief Evolve the craft's dynamics
         *
         *      Update craft state variables by one notional time
         * unit.
         *
         * \param dayOfYear : number of days since Jan 1st,
         *        used to determine, if needed, position of
         *        other simulated objects that mau be dynamically
         *        relevant.
         */
        void step(GLfloat dayOfYear);

        /**
         * \brief Obtain the view state of the craft in camera format
         *
         * \return the camera state
         */
        CameraState viewState(void) const;

        /**
         * \brief Set the view state of the craft in camera format
         *
         * \return camera : the desired camera state
         */
        void setViewState(const CameraState& camera);

        static const Vector3D START_POSITION;

    private:
        static const vec_t START_RADIUS;

        /// Global bounds for craft position
        static const vec_t MAX_RANGE;
        static const vec_t MIN_EARTH_RANGE;
        static const vec_t MIN_SUN_RANGE;

        /// Top speed of craft in whatever nett direction
        static const vec_t MAX_SPEED;

        /// Rates for about centre of mass craft rotations
        static const vec_t PITCH_RATE_DELTA;
        static const vec_t ROLL_RATE_DELTA;
        static const vec_t YAW_RATE_DELTA;
        static const vec_t RATE_FUDGE;

        /// Speed setting post 'bounce' off objects
        static const vec_t REBOUND_SPEED;

        /// Speed change steps for the aft-to-forward craft axis
        static const vec_t INLINE_THRUST_DELTA;

        /// Speed change step for the left-to-right craft axis
        static const vec_t LATERAL_THRUST_DELTA;

        /// Speed change step for the floor-to-ceiling craft axis
        static const vec_t VERTICAL_THRUST_DELTA;

        AcceleratedPlatform state;

        /**
         * \brief Return the craft to a 'home' state
         */
        void goHome(void);


        void noseDown();
        void noseUp();
        void rollLeft();
        void rollRight();
        void stop(void);
        void reverseThrust(void);
        void forwardThrust(void);
        void yawLeft();
        void yawRight();
        void rightThrust(void);
        void leftThrust(void);
        void nullRotation(void);
        void upThrust(void);
        void downThrust(void);

        void vectorThrust(Vector3D thrust);
    };

/**
 * @}
 */

#endif // CRAFT_H_
