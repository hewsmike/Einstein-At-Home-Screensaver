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

#include "Renderable.h"

// Mark initial state as being inactive, but able to be shown if later
// activated, with lowest render quality and no associated font.
Renderable::Renderable() : activity(Renderable::INACTIVE),
                           show_flag(true),
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

    // Register for callback should the global rendering level alter.
    attach();

    // Mark as active.
    activity = Renderable::ACTIVE;
    }

void Renderable::inactivate(void) {
    std::cout << "Renderable::inactivate() 1" << std::endl;
    // Release resources as we have decided not to show it.
    release();

    std::cout << "Renderable::inactivate() 2" << std::endl;
    // Unregister any callback triggered by global rendering level alteration.
    detach();

    std::cout << "Renderable::inactivate() 3" << std::endl;
    // Mark as inactive.
    activity = Renderable::INACTIVE;

    std::cout << "Renderable::inactivate() 4" << std::endl;
    }

Renderable::activity_state Renderable::isActivated(void) const {
    return activity;
    }

void Renderable::cycleActivation(void) {
    // In this base class the activity state is simply toggled.
    // Re-define this in a derived class if you want other behaviours.
    isActivated() ? inactivate() : activate();
    }

void Renderable::show(void) {
    show_flag = true;
    }

void Renderable::hide(void) {
    show_flag = false;
    }

void Renderable::toggleShow(void) {
    show_flag = !show_flag;
    }

bool Renderable::isShown(void) const {
    return show_flag;
    }

void Renderable::draw(void) {
    // Only display if it is marked as to be shown, with resources acquired.
    if((show_flag == true) && (activity == Renderable::ACTIVE)) {
        render();
        }
    }

void Renderable::SSGUpdate(void) {
    // Store the new rendering level.
    quality = SolarSystemGlobals::getRenderLevel();

    // Provided the item is marked as active.
    if(isActivated() == true) {
        // Then inactivate the item, thus releasing resources.
        inactivate();

        // Followed by re-activation which will trigger preparation
        // at the new level.
        activate();
        }
    }

void Renderable::setFont(OGLFT_ft* a_font) {
    myFont = a_font;
    }

OGLFT_ft* Renderable::getFont(void) const {
    return myFont;
    }
