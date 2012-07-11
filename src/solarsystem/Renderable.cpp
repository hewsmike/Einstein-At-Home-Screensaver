/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
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

#include "Renderable.h"

// Mark initial state as being inactive, and lowest render quality.
Renderable::Renderable() : activity(Renderable::INACTIVE),
                           quality(SolarSystemGlobals::RENDER_LOWEST),
                           myFont(NULL) {
    // Don't call any preparation code from constructor as OpenGL context
    // may not exist or be valid at construction time.
    }

Renderable::~Renderable() {
    }

void Renderable::activate(void) {
    // Prepare resources as we have decided to show it.
    prepare(quality);
    // Mark as active.
    activity = Renderable::ACTIVE;
    }

void Renderable::inactivate(void) {
    // Release resources as we have decided not to show it.
    release();
    // Mark as inactive.
    activity = Renderable::INACTIVE;
    }

Renderable::activity_state Renderable::isActivated(void) const {
    return activity;
    }

void Renderable::cycleActivation(void) {
    // In this base class the activity state is simply toggled.
    // Re-define this in a derived class if you want other behaviours.
    is_activated() ? inactivate() : activate();
    }

void Renderable::draw(void) {
    // Only show if it is marked active.
    if(activity == Renderable::ACTIVE) {
        render();
        }
    }

SolarSystemGlobals::render_quality Renderable::renderLevel(void) {
    return quality;
    }

void Renderable::setRenderLevel(SolarSystemGlobals::render_quality rq) {
    // Remember new quality setting.
    quality = rq;

    // Inactivate the item, thus releasing resources.
    inactivate();

    // Then re-activate which will trigger preparation at the new level.
    activate();
    }

void Renderable::setFont(OGLFT_ft* a_font) {
    myFont = a_font;
    }

OGLFT_ft* Renderable::getFont(void) const {
    return myFont;
    }
