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
const unsigned int TargetReticle::INITIAL_PHASE(0);
const GLuint TargetReticle::RETICLE_WIDTH(64);
const GLuint TargetReticle::RETICLE_HEIGHT(64);

TargetReticle::TargetReticle(std::string resource_base_name,
                             GLuint screen_width,
                             GLuint screen_height,
                             GLuint centre_offset_x,
                             GLuint centre_offset_y,
                             unsigned int frame_count) :
                                HUDContent(0,0),
                                base_name(resource_base_name),
                                max_frames(frame_count) {
    resize(screen_width, screen_height);
    reset();
    }

TargetReticle::~TargetReticle() {
    release();
    }

void TargetReticle::reset(void) {
    phase = INITIAL_PHASE;
    }

void TargetReticle::resize(GLuint screen_width, GLuint screen_height) {
    scr_width = screen_width;
    scr_height = screen_height;
    reBase(scr_width/2 - RETICLE_WIDTH/2 + 1, scr_height/2 - RETICLE_HEIGHT/2 + 1);
    for(unsigned int fc = 0; fc < frames.size(); ++fc) {
        frames.at(fc)->reBase(this->horzBase(), this->vertBase());
        }
    }

void TargetReticle::prepare(SolarSystemGlobals::render_quality rq) {
    // Eliminate any prior stored frames.
    frames.clear();

    // Construct each animation frame as an image acquired by
    // resource name.
    for(unsigned int fc = 0; fc < max_frames; ++fc) {
        std::stringstream frame_name;
        frame_name << base_name;
        frame_name.precision(1);
        frame_name << fc;
        frame_name << FRAME_RESOURCE_END_NAME;
        HUDImage* frame = new HUDImage(frame_name.str(), 0, 0);
        frame->reBase(this->horzBase(), this->vertBase());
        frame->activate();
        frames.add(frame);
        }
    }

void TargetReticle::release(void) {
    frames.clear();
    }

void TargetReticle::render(void) {
    // Position
    phase = (phase + 1) % max_frames;
    HUDImage* frame = static_cast<HUDImage*>(frames.at(phase));
    frame->draw();
    }
