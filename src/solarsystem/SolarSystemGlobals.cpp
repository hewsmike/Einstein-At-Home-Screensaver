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
const GLfloat SolarSystemGlobals::MINUTES_PER_HOUR(60.0f);
const GLfloat SolarSystemGlobals::SECONDS_PER_MINUTE(60.0f);
const GLfloat SolarSystemGlobals::MINUTES_PER_DEGREE(60.0f);
const GLfloat SolarSystemGlobals::DEGREES_PER_HOUR(15.0f);

const GLuint SolarSystemGlobals::MIN_SCREEN_WIDTH(800);
const GLuint SolarSystemGlobals::MIN_SCREEN_HEIGHT(600);

// Four channels with the alpha, three without.
const GLuint SolarSystemGlobals::NO_ALPHA_CHANNEL(3);
const GLuint SolarSystemGlobals::ALPHA_CHANNEL(NO_ALPHA_CHANNEL + 1);

std::set<SSGObserver*> SolarSystemGlobals::quality_observers;

SolarSystemGlobals::SolarSystemGlobals() {
    }

SolarSystemGlobals::~SolarSystemGlobals() {
    }

void SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::render_quality rq) {
    // If the render quality is actually different from before.
    if(qual != rq) {
        // Store the new quality level.
        qual = rq;
        // Now call back each observer to let
        // them know of the quality change.
        for(std::set<SSGObserver*>::const_iterator ob = SolarSystemGlobals::quality_observers.begin();
            ob != SolarSystemGlobals::quality_observers.end();
            ++ob) {
            (*ob)->SSGUpdate();
            }
        }
    }

SolarSystemGlobals::render_quality SolarSystemGlobals::getRenderLevel(void) {
    return qual;
    }

void SolarSystemGlobals::registerRenderQualityObserver(SSGObserver* observer) {
    // Bar null pointers.
    if(observer != NULL) {
        // Using std::set semantics will ensure no repeated entries.
        SolarSystemGlobals::quality_observers.insert(observer);
        }
    }

void SolarSystemGlobals::unRegisterRenderQualityObserver(SSGObserver* observer) {
    // Bar null pointers.
    if(observer != NULL) {
        // Using std::set semantics will ensure removal if present.
        SolarSystemGlobals::quality_observers.erase(observer);
        }
    }
