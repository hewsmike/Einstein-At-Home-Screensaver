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

#include "Traverse.h"

#include <iostream>
#include <sstream>
#include <string>

#include "Curve.h"
#include "ErrorHandler.h"

unsigned int Traverse::MIN_LOOKOUTS(2);

Traverse::Traverse(void) {
    current_path_index = 0;
    }

Traverse::~Traverse() {
    }

void Traverse::clear() {
    cam_states.clear();
    current_path_index = 0;
    }

void Traverse::addLookout(const LookOut& cam) {
    cam_states.push_back(cam);
    current_path_index = 0;
    }

unsigned int Traverse::numLookouts(void) const {
    return cam_states.size();
    }

Path Traverse::getFirstPath(void) {
    Path ret_val;

    if(numLookouts() >= MIN_LOOKOUTS) {
        current_path_index = 0;
        ret_val = makePath();
        }
    else {
        // Or if there aren't enough Lookouts ....
        std::stringstream msg;
        msg << "Traverse::getFirstPath() : Fewer than ";
        msg << MIN_LOOKOUTS;
        msg << " LookOuts present in Traverse !!";
        ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
        }

    return ret_val;
    }

Path Traverse::getNextPath(void) {
    Path ret_val;

    if(numLookouts() >= MIN_LOOKOUTS) {
        // We traverse the lookouts in a modulo fashion.
        current_path_index = (current_path_index + 1) % cam_states.size();
        ret_val = makePath();
        }
    else {
        // Or if there aren't enough Lookouts ....
        std::stringstream msg;
        msg << "Traverse::getNextPath() : Fewer than ";
        msg << MIN_LOOKOUTS;
        msg << " LookOuts present in Traverse !!";
        ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
        }

    return ret_val;
    }

Path Traverse::makePath() {
    // If there aren't enough Lookouts ....
    if(numLookouts() < MIN_LOOKOUTS) {
        std::stringstream msg;
        msg << "Traverse::makePath() : Fewer than ";
        msg << MIN_LOOKOUTS;
        msg << " LookOuts present in Traverse !!";
        ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
        }

    // Set the indices of the Lookouts that bracket this Path.
    unsigned int start_lookout = current_path_index;
    unsigned int finish_lookout = (start_lookout + 1) % cam_states.size();

    // Construct the three curves between these Lookouts.
    Curve pos(cam_states.at(start_lookout).position(), cam_states.at(finish_lookout).position());
    Curve focus(cam_states.at(start_lookout).focus(), cam_states.at(finish_lookout).focus());
    Curve orient(cam_states.at(start_lookout).orientation(), cam_states.at(finish_lookout).orientation());

    Path ret_val(pos, focus, orient);

    // Retrieve the LookOut descriptions too.
    const std::vector<std::string>& start_msg = cam_states.at(start_lookout).getDescription();
    ret_val.setStartMessage(start_msg);
    const std::vector<std::string>& finish_msg = cam_states.at(finish_lookout).getDescription();
    ret_val.setFinishMessage(finish_msg);

    return ret_val;
    }
