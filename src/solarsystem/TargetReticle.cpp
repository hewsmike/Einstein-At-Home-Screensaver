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

#include "TargetReticle.h"

#include <sstream>

#include "HUDImage.h"

const string TargetReticle::FRAME_RESOURCE_END_NAME("TGA");

TargetReticle::TargetReticle(std::string resource_base_name, unsigned int frames) {
    base_name = resource_base_name;
    frame_count = frames;
    reset();
    }

TargetReticle::~TargetReticle() {
    clearFrames();
    }

void TargetReticle::reset(void) {
    phase = 0;
    }

void TargetReticle::SSGUpdate(void) {
    }

void TargetReticle::prepare(SolarSystemGlobals::render_quality rq) {
    // Eliminate any prior stored frames.
    clearFrames();
    for(unsigned int fc = 0; fc < frame_count; ++fc) {
        std::stringstream frame_name;
        frame_name << base_name;
        frame_name.precision(1);
        frame_name << fc;
        frame_name << FRAME_RESOURCE_END_NAME;
        frames.push_back(new HUDImage(frame_name, 0, 0));
        }
    }

void TargetReticle::release(void) {
    clearFrames();
    }

void TargetReticle::render(void) {
    phase = (phase + 1) % frame_count;

    }

void TargetReticle::clearFrames(void) {
    for(std::vector<HUDImage*>::iterator fr = frames.begin();
        fr != frames.end();
        ++fr) {
        if(fr != NULL) {
            fr->inactivate();
            }
        }
    frames.clear();
    }
