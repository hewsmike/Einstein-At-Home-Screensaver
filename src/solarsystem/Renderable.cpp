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
   // Mark as active and prepare resources as we have decided to show it.
   activity = Renderable::ACTIVE;
   prepare(quality);
   }

void Renderable::inactivate(void) {
   // Mark as inactive and release resources as we have decided not to show it.
   activity = Renderable::INACTIVE;
   release();
   }

Renderable::activity_state Renderable::is_activated(void) const {
   return activity;
   }

void Renderable::toggle_activation(void) {
   is_activated() ? inactivate() : activate();
   }

void Renderable::draw(void) {
   // Only show if it is marked active.
   if(activity == Renderable::ACTIVE) {
      render();
      }
   }

SolarSystemGlobals::render_quality Renderable::render_level(void) {
   return quality;
   }

void Renderable::set_render_level(SolarSystemGlobals::render_quality rq) {
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
