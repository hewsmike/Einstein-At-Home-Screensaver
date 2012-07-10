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

#include "Craft.h"

#include <iostream>

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"
#include "SunOrbit.h"

const vec_t Craft::MAX_RANGE(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS * 1.05f);
const vec_t Craft::MIN_EARTH_RANGE(SolarSystemGlobals::EARTH_RADIUS*1.05f);
const vec_t Craft::MIN_SUN_RANGE(SolarSystemGlobals::SUN_RADIUS*1.2f);

const vec_t Craft::START_RADIUS(SolarSystemGlobals::EARTH_RADIUS*2.0f);
const Vector3D Craft::START_POSITION(-3*Craft::START_RADIUS, 0, Craft::START_RADIUS);

const vec_t Craft::REBOUND_SPEED(3.0f);
const vec_t Craft::MAX_SPEED(50.0f);

const vec_t Craft::SPEED_DEC(2.5f);
const vec_t Craft::SPEED_INC(2.5f);

const vec_t Craft::LATERAL_THRUST_RATE(0.5f);
const vec_t Craft::VERTICAL_THRUST_RATE(0.5f);
const vec_t Craft::RATE_FUDGE(0.2f);
const vec_t Craft::PITCH_RATE_INC(RATE_FUDGE*(PI/360.0f));
const vec_t Craft::ROLL_RATE_INC(RATE_FUDGE*(PI/360.0f));
const vec_t Craft::YAW_RATE_INC(RATE_FUDGE*(PI/360.0f));

Craft::Craft() {
    goHome();
    }

Craft::~Craft() {
    }

const AcceleratedPlatform& Craft::platform() const {
    return state;
    }

void Craft::manouevre(Craft::movements mov) {
    switch(mov) {
        case Craft::GO_HOME :
            goHome();
            break;
        case Craft::STOP_TRANSLATION :
            stop();
            break;
        case Craft::STOP_ROTATION :
            nullRotation();
            break;
        case Craft::FORWARD :
            forwardThrust();
            break;
        case Craft::REVERSE :
            reverseThrust();
            break;
        case Craft::UPWARDS :
            upThrust();
            break;
        case Craft::DOWNWARDS :
            downThrust();
            break;
        case Craft::LEFTWARDS :
            leftThrust();
            break;
        case Craft::RIGHTWARDS :
            rightThrust();
            break;
        case Craft::PITCH_UP :
            noseUp();
            break;
        case Craft::PITCH_DOWN :
            noseDown();
            break;
        case Craft::ROLL_LEFT :
            rollLeft();
            break;
        case Craft::ROLL_RIGHT :
            rollRight();
            break;
        case Craft::YAW_LEFT :
            yawLeft();
            break;
        case Craft::YAW_RIGHT :
            yawRight();
            break;
        default :
            ErrorHandler::record("Craft::manouevre() - bad switch case reached (default)", ErrorHandler::FATAL);
            break;
        }
    }

void Craft::step(GLfloat dayOfYear) {
    state.step();
    // Bounds and collision checking ... this is an Earth centric
    // simulation so it's always at the origin.

    // Too far away from home ?
    if(state.position().len() > Craft::MAX_RANGE) {
        // TODO turn him around to point homewards?
        // Give a little nudge to send it back in the direction of home.
        state.set_velocity(-Craft::REBOUND_SPEED*state.position().unit());
        std::cout << "Too far away from Earth - nudged back" << std::endl;
        }

    // Too close to home ?
    if(state.position().len() < Craft::MIN_EARTH_RANGE) {
        // TODO turn him around to point outwards?
        // Give a little nudge to send it back away from home.
        state.set_velocity(+Craft::REBOUND_SPEED*state.position().unit());
        std::cout << "Too close to Earth - nudged away" << std::endl;
        }

    // Where is the Sun ? That depends upon it's orbital position,
    // and hence what time of the year it is.
    Vector3D sun_pos = SunOrbit::getPosition(dayOfYear);
    // This is the vector FROM the Sun TO the craft.
    Vector3D sun_relative = state.position() - sun_pos;
    // The length of that vector is the distance separating Sun and craft.
    GLfloat sun_dist = sun_relative.len();
    // Are we too close to the Sun ?
    if(sun_dist < Craft::MIN_SUN_RANGE) {
        // Give a little nudge to send it back AWAY from the Sun.
        state.set_velocity(+Craft::REBOUND_SPEED*sun_relative.unit());
        std::cout << "Too close to Sun - nudged away" << std::endl;
        }
    }

CameraState Craft::getViewState(void) const {
    return state.getViewState();
    }

void Craft::setViewState(const CameraState& cam) {
    state.setViewState(cam);
    }

void Craft::go_home(void) {
    // Initially in good position, stationary and not rotating.
    state.reset();
    state.set_position(START_POSITION);
    }

void Craft::forward_thrust(void) {
    // Add to the current velocity vector a fraction of the 'look' vector.
    // That is : thrust is applied along the aft-TO-fore axis.
    vector_thrust(+Craft::SPEED_INC*state.look());
    }

void Craft::reverse_thrust(void) {
    // Subtract from the current velocity vector a fraction of the 'look' vector.
    // That is : thrust is applied along the fore-TO-aft axis.
    vector_thrust(-Craft::SPEED_DEC*state.look());
    }

void Craft::nose_down() {
    // TODO - cap these rotation rates
    state.set_pitch_rate(state.get_pitch_rate() - Craft::PITCH_RATE_INC);
    }

void Craft::nose_up() {
    state.set_pitch_rate(state.get_pitch_rate() + Craft::PITCH_RATE_INC);
    }

void Craft::roll_right() {
    state.set_roll_rate(state.get_roll_rate() - Craft::ROLL_RATE_INC);
    }

void Craft::roll_left() {
    set_roll_rate(state.get_roll_rate() + Craft::ROLL_RATE_INC);
    }

void Craft::yaw_right() {
    state.set_yaw_rate(state.get_yaw_rate() - Craft::YAW_RATE_INC);
    }

void Craft::yaw_left() {
    state.set_yaw_rate(state.get_yaw_rate() + Craft::YAW_RATE_INC);
    }

void Craft::null_rotation(void) {
    state.set_pitch_rate(0.0f);
    state.set_roll_rate(0.0f);
    state.set_yaw_rate(0.0f);
    }

void Craft::stop(void) {
    state.set_velocity(Vector3D(0.0f, 0.0f, 0.0f));
    }

void Craft::right_thrust() {
    // Add to the current velocity vector a fraction of the 'cross' vector.
    // That is : thrust is applied along the left-TO-right wing axis.
    vector_thrust(+Craft::LATERAL_THRUST_RATE*state.cross());
    }

void Craft::left_thrust() {
    // Add to the current velocity vector a fraction of the 'cross' vector.
    // That is : thrust is applied along the right-TO-left wing axis.
    vector_thrust(-Craft::LATERAL_THRUST_RATE*state.cross());
    }

void Craft::up_thrust() {
    // Add to the current velocity vector a fraction of the 'up' vector.
    // That is : thrust is applied along the floor-TO-ceiling axis.
    vector_thrust(+Craft::VERTICAL_THRUST_RATE*state.up());
    }

void Craft::down_thrust() {
    // Subtract from the current velocity vector a fraction of the 'up' vector.
    // That is : thrust is applied along the ceiling-TO-floor axis.
    vector_thrust(-Craft::VERTICAL_THRUST_RATE*state.up());
    }

void Craft::vector_thrust(Vector3D thrust) {
    // Apply thrust along given vector.
    Vector3D new_vel = state.velocity() + thrust;

    // How fast are we going? Too fast?
    if(new_vel.len() > Craft::MAX_SPEED) {
        // Yes, so cap speed along the line of the new velocity.
        new_vel = Craft::MAX_SPEED*new_vel.unit();
        std::cout << "Speed capped at " << new_vel.len() << std::endl;
        }

    state.set_velocity(new_vel);
    }
