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

#include "AutoPilot.h"

#include <sstream>
#include <cmath>
#include <iostream>

#include "ErrorHandler.h"

const float AutoPilot::LENGTH_PER_FRAME(15.0f);
const float AutoPilot::LEAST_PATH_LENGTH(0.00001f);
const unsigned int AutoPilot::PAUSE_FRAME_COUNT(150);

const float AutoPilot::PATH_EARLY_BOUNDARY(0.1f);
const float AutoPilot::PATH_LATE_BOUNDARY(0.9f);

AutoPilot::AutoPilot(void) {
    lambda = Path::LAMBDA_LOWER_BOUND;
    current_delta_lambda = 0.0f;
    active_flag = false;
    }

AutoPilot::~AutoPilot() {
    }

void AutoPilot::activate(const Traversable& trav, const CameraState& cam) {
    ErrorHandler::record("AutoPilot::activate() : ", ErrorHandler::INFORM);
    view = cam;

    getTraverse(trav, view);

    current_path = current_traverse.getFirstPath();

    set_delta_lambda();

    active_flag = true;

    path_stage_flag = EARLY;

    description_change_flag = false;
    }

void AutoPilot::inactivate(void) {
    ErrorHandler::record("AutoPilot::inactivate() : ", ErrorHandler::INFORM);
    // Save most recent view state.
    view = current_path.value(lambda);

    current_traverse.clear();

    active_flag = false;
    }

bool AutoPilot::isActive(void) const {
    return active_flag;
    }

CameraState AutoPilot::viewState(void) {
    // Counter for pausing at LookOuts.
    static GLuint count_down = PAUSE_FRAME_COUNT;
    // Flag to indicate if we are pausing or not.
    static bool pause_flag = false;

    // Are we waiting at a Lookout ?
    if(pause_flag != true) {
        // No, so increment along the current path.
        lambda += current_delta_lambda;

        // Is it the end of the current path ??
        if(lambda >= Path::LAMBDA_UPPER_BOUND) {
            // Then pause movement and (re-)set the pause counter.
            pause_flag = true;
            count_down = PAUSE_FRAME_COUNT;
            }
        }
    else {
        // Yes we are waiting, so one less frame to wait here.
        --count_down;

        // End of pause interval ?
        if(count_down == 0) {
            // Yes, so move on to evolve the next available Path.
            current_path = current_traverse.getNextPath();
            // Find the right lambda increment for this new Path.
            set_delta_lambda();
            // Unpause movement.
            pause_flag = false;
            }
        }

    // Set the description as unchanged.
    description_change_flag = false;

    // Are we in the LATE portion of a Path?
    if(lambda > PATH_LATE_BOUNDARY) {
        // Yes, so have we just transitioned from the MIDDLE portion?
        if(path_stage_flag == MIDDLE) {
            // Yes, thus set the stage as LATE.
            path_stage_flag = LATE;
            // Change the current description to that of the upcoming Lookout.
            for(std::vector<std::string>::const_iterator message = current_path.getFinishMessages.begin();
                message != current_path.getFinishMessages().end;
                ++message) {
                current_description.push_back(*message);
                }
            // Get the image set.
            current_images = current_path.getFinishImageResourceNames();
            // Flag that the description has therefore altered.
            description_change_flag = true;
            }
        }
    else {
        // No, so we aren't in the LATE part of the current Path.
        // Are we in the EARLY part?
        if (lambda < PATH_EARLY_BOUNDARY) {
            // Yes, so have we just transitioned from the LATE portion?
            if(path_stage_flag == LATE) {
                // Yes, thus set the stage as EARLY.
                path_stage_flag = EARLY;
                // NB : We don't need to change any description as, if properly
                // setup elsewhere, the Finish message for one Path is the
                // Start message for the next.
                }
            }
        else {
            // No, we aren't in the EARLY part either, must be the MIDDLE.
            // But have we just transitioned from the MIDDLE portion?
            if(path_stage_flag == EARLY) {
                // So set the stage as MIDDLE.
                path_stage_flag = MIDDLE;
                // Clear the descriptions.
                current_description.clear();
                current_images.clear();
                // Flag that the description has therefore altered.
                description_change_flag = true;
                }
            }
        }

    // Update the camera state according to the current lambda value.
    view = current_path.value(lambda);

    return view;
    }

const std::vector<std::string>& AutoPilot::getDescription(void) const {
    return current_description;
    }

const std::vector<std::string>& AutoPilot::getImageResourceNames(void) const {
    return current_images;
    }

bool AutoPilot::hasDescriptionChanged(void) const {
    return description_change_flag;
    }

void AutoPilot::set_delta_lambda(void) {
    // Note that a delta lambda of zero implies no movement !!
    lambda = Path::LAMBDA_LOWER_BOUND;

    // Select longest curve component of current path to avoid high rates of camera state change.
    float position_path_length = current_path.curveLength(Path::POSITION);
    float focus_path_length = current_path.curveLength(Path::FOCUS);
    float orientation_path_length = current_path.curveLength(Path::ORIENTATION);

    float longest_path_length = std::max(std::max(position_path_length, focus_path_length), orientation_path_length);

    current_delta_lambda = 0.0f;

    // Need care with rounding at values near zero.
    if(longest_path_length < LEAST_PATH_LENGTH) {
        // In effect, if a path has insufficient length then only one frame will be rendered for it.
        current_delta_lambda = Path::LAMBDA_UPPER_BOUND;
        }
    else {
        current_delta_lambda = LENGTH_PER_FRAME / longest_path_length;
        }

    // Prevent overshoot.
    if(current_delta_lambda > Path::LAMBDA_UPPER_BOUND) {
        current_delta_lambda = Path::LAMBDA_UPPER_BOUND;
        }
    }

void AutoPilot::getTraverse(const Traversable& trav, const CameraState& cam) {
    // Empty any existing Traverse data.
    current_traverse.clear();

    // Construct a Lookout from the existing position and orientation
    // within the Simulation.
    LookOut first(cam.position(), cam.focus(), cam.orientation());
    // Use that as the initial Lookout in the Traverse being constructed.
    current_traverse.addLookout(first);

    // The remaining Lookouts come from our given Traversable object.
    for(unsigned int way_point = 0; way_point < trav.numberOfWayPoints(); ++way_point) {
        current_traverse.addLookout(trav.getView(way_point));
        }
    }
