/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

#include "SolarSystemGlobals.h"

#include "framework.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

const GLuint SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS(10000);

const GLuint SolarSystemGlobals::EARTH_RADIUS(100);

const GLuint SolarSystemGlobals::SUN_RADIUS(800);

// Default rendering quality is the lowest.
SolarSystemGlobals::render_quality SolarSystemGlobals::qual(SolarSystemGlobals::RENDER_LOWEST);

const GLfloat SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);
const GLfloat SolarSystemGlobals::HALF_CIRCLE_DEG(180.0f);
const GLfloat SolarSystemGlobals::QUARTER_CIRCLE_DEG(90.0f);

const GLuint SolarSystemGlobals::MIN_SCREEN_WIDTH(800);
const GLuint SolarSystemGlobals::MIN_SCREEN_HEIGHT(600);

// Four channels with the alpha, three without.
const GLuint SolarSystemGlobals::NO_ALPHA_CHANNEL(3);
const GLuint SolarSystemGlobals::ALPHA_CHANNEL(NO_ALPHA_CHANNEL + 1);

SolarSystemGlobals::SolarSystemGlobals() {
   }

SolarSystemGlobals::~SolarSystemGlobals() {
   }

void SolarSystemGlobals::set_render_level(SolarSystemGlobals::render_quality rq) {
   qual = rq;
   }

SolarSystemGlobals::render_quality SolarSystemGlobals::get_render_level(void) {
   return qual;
   }
