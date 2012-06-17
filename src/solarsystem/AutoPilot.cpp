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

#include "ErrorHandler.h"

const float AutoPilot::LENGTH_PER_FRAME(10.0f);
const float AutoPilot::LEAST_PATH_LENGTH(0.00001f);
const unsigned int AutoPilot::PAUSE_FRAME_COUNT(250);

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

CameraState AutoPilot::getViewState(void) {
	static GLuint count_down = PAUSE_FRAME_COUNT;
	static bool pause_flag = false;

	if(pause_flag != true) {
		// Increment along the current path.
		lambda += current_delta_lambda;

		// Is it the end of the current path ??
		if(lambda >= Path::LAMBDA_UPPER_BOUND) {
			// Then pause movement and reset pause counter.
			pause_flag = true;
			count_down = PAUSE_FRAME_COUNT;
			}
		}
	else {
		// One less frame to wait here.
		--count_down;

		// End of pause interval ?
		if(count_down == 0) {
			current_path = current_traverse.getNextPath();
			set_delta_lambda();
			pause_flag = false;
			}
		}

	description_change_flag = false;

	if(lambda > PATH_LATE_BOUNDARY) {
		if(path_stage_flag == MIDDLE) {
			path_stage_flag = LATE;
			description_change_flag = true;
			current_description = current_path.getFinishMessage();
			}
		}
	else {
		if (lambda < PATH_EARLY_BOUNDARY) {
			if(path_stage_flag == LATE) {
				path_stage_flag = EARLY;
				description_change_flag = true;
				current_description = current_path.getStartMessage();
				}
			}
		else {
			if(path_stage_flag == EARLY) {
				path_stage_flag = MIDDLE;
				description_change_flag = true;
				current_description.clear();
				}
			}
		}

	view = current_path.value(lambda);

	return view;
   }

const std::vector<std::string>& AutoPilot::getDescription(void) const {
	return current_description;
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
	LookOut first(cam.position(), cam.focus(), cam.orientation());
	// Kick off traverse with the given camera state.
	current_traverse.addWayPoint(first);

	// The remainder from our given Traversable object.
	for(unsigned int way_point = 0; way_point < trav.numberOfWayPoints(); ++way_point) {
		current_traverse.addWayPoint(trav.getView(way_point));
		}
	}
