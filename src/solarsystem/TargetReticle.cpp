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

const unsigned int TargetReticle::FRAMES(4);

TargetReticle::TargetReticle(void) {
    reset();
    }

TargetReticle::~TargetReticle() {
    }

void TargetReticle::reset(void) {
    phase = 0;
    }

void prepare(SolarSystemGlobals::render_quality rq) {
    }

void release(void) {
    }

void render(void) {
    phase = (phase + 1) % FRAMES;
    }
