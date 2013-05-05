/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

const GLfloat SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS(10000);

const GLfloat SolarSystemGlobals::EARTH_RADIUS(100);

const GLfloat SolarSystemGlobals::SUN_RADIUS(800);

const GLfloat SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);
const GLfloat SolarSystemGlobals::HALF_CIRCLE_DEG(FULL_CIRCLE_DEG/2);
const GLfloat SolarSystemGlobals::QUARTER_CIRCLE_DEG(FULL_CIRCLE_DEG/4);
const GLfloat SolarSystemGlobals::MINUTES_PER_HOUR(60.0f);
const GLfloat SolarSystemGlobals::HOURS_PER_DAY(24.0f);
const GLfloat SolarSystemGlobals::SECONDS_PER_MINUTE(60.0f);
const GLfloat SolarSystemGlobals::MINUTES_PER_DEGREE(60.0f);
const GLfloat SolarSystemGlobals::DEGREES_PER_HOUR(FULL_CIRCLE_DEG/HOURS_PER_DAY);

const GLuint SolarSystemGlobals::MIN_SCREEN_WIDTH(800);
const GLuint SolarSystemGlobals::MIN_SCREEN_HEIGHT(600);

// Four channels with the alpha, three without.
const GLuint SolarSystemGlobals::NO_ALPHA_CHANNEL(3);
const GLuint SolarSystemGlobals::ALPHA_CHANNEL(NO_ALPHA_CHANNEL + 1);

GLuint SolarSystemGlobals::width(0);
GLuint SolarSystemGlobals::height(0);

// Default rendering quality is the lowest.
SolarSystemGlobals::render_quality SolarSystemGlobals::qual(RENDER_LOWEST);

WindowManager::displaymode SolarSystemGlobals::operating_mode(WindowManager::WINDOW);

std::set<RenderQualityObserver*> SolarSystemGlobals::quality_observers;
std::map<SolarSystemGlobals::content, OGLFT_ft*> SolarSystemGlobals::fonts;

SolarSystemGlobals::SolarSystemGlobals() {
    }

SolarSystemGlobals::~SolarSystemGlobals() {
    }

void SolarSystemGlobals::setFont(SolarSystemGlobals::content element, OGLFT_ft* font) {
    fonts.erase(element);
    // Then put in a new key/value combo using pair semantics.
    fonts.insert(std::make_pair(element, font));
    }

OGLFT_ft* SolarSystemGlobals::getFont(SolarSystemGlobals::content element) {
    return fonts[element];
    }

void SolarSystemGlobals::setDisplayMode(WindowManager::displaymode mode) {
    operating_mode = mode;
    }

WindowManager::displaymode SolarSystemGlobals::getDisplayMode(void) {
    return operating_mode;
    }

void SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::render_quality rq) {
    // If the render quality is actually different from before.
    if(qual != rq) {
        // Store the new quality level.
        qual = rq;
        // Now call back each observer to let them know of the quality change.
        // Each ( if interested ) will have to discover the quality level
        // via getRenderLevel().
        for(std::set<RenderQualityObserver*>::const_iterator ob = quality_observers.begin();
            ob != SolarSystemGlobals::quality_observers.end();
            ++ob) {
            (*ob)->renderQualityUpdate();
            }
        }
    }

SolarSystemGlobals::render_quality SolarSystemGlobals::getRenderLevel(void) {
    return qual;
    }

void SolarSystemGlobals::registerRenderQualityObserver(RenderQualityObserver* observer) {
    // Bar null pointers.
    if(observer != NULL) {
        // Using std::set semantics will ensure no repeated entries.
        quality_observers.insert(observer);
        }
    }

void SolarSystemGlobals::unRegisterRenderQualityObserver(RenderQualityObserver* observer) {
    // Bar null pointers.
    if(observer != NULL) {
        // Using std::set semantics will ensure removal only if present.
        quality_observers.erase(observer);
        }
    }
