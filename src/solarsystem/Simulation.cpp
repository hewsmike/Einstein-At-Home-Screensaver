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

#include "Simulation.h"

const std::string Simulation::EARTH_NAME("Earth");
const std::string Simulation::EARTH_IMAGE_FILE("earthmap.bmp");

const GLuint Simulation::EARTH_STACKS(37);
const GLuint Simulation::EARTH_SLICES(72);
const GLfloat Simulation::GREENWICH_TEXTURE_OFFSET(-0.5f);

const unsigned int Simulation::COUNT_START(110000);
const unsigned int Simulation::COUNT_END(0);

Simulation::Simulation() : cs(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS),
                       ps(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 25),
                       sn(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 50),
                       gg(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS + 50, 36, 19),
                       earth(EARTH_NAME,
                             EARTH_IMAGE_FILE,
                             SolarSystemGlobals::EARTH_RADIUS,
                             EARTH_STACKS,
                             EARTH_SLICES,
                             GREENWICH_TEXTURE_OFFSET) {
   // Radius of rendering is ( slightly ) different for each scene element that lies
   // upon the celestial sphere.
   // I'm not actually sure why, but I think that looks better ie. less
   // pixel level flickering than when all at the same radius. Presumably
   // as I have essentially reduced any z-order contention by use of offsets ??
   }

Simulation::~Simulation() {
   }

void Simulation::draw() {
   // Invoke the draw method for each scene element.
   ps.draw();
   sn.draw();
   cs.draw();
   gg.draw();
   earth.draw();
   // glPushMatrix();
      // glTranslatef(SUN_RADIUS*3, 0, 0);
      // sun.draw();
   // glPopMatrix();
   }

void Simulation::activate() {
   // Examine current render-level choice.
   switch(SolarSystemGlobals::get_render_level()) {
      // One could fiddle with these per-case calls to include/exclude
      // features according to desired rendering 'quality'. This is also
      // the point of control for default behaviour in screensaver mode.
      case SolarSystemGlobals::RENDER_LOWEST:
         ps.activate();
         sn.activate();
         cs.activate();
         gg.activate();
         earth.activate();
         break;
      case SolarSystemGlobals::RENDER_MEDIUM:
         ps.activate();
         sn.activate();
         cs.activate();
         gg.activate();
         earth.activate();
         break;
      case SolarSystemGlobals::RENDER_HIGHEST:
         ps.activate();
         sn.activate();
         cs.activate();
         gg.activate();
         earth.activate();
         break;
      default:
         // Ought not get here !!
         std::string msg = "Simulation::activate() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void Simulation::toggle(Simulation::content ct) {
   // Which scene element to toggle?
   switch(ct) {
      case Simulation::AXES:
         break;
      case Simulation::CONSTELLATIONS:
         cs.toggle_activation();
         break;
      case Simulation::EARTH:
         earth.toggle_activation();
         break;
      case Simulation::GRID:
         gg.toggle_activation();
         break;
      case Simulation::PULSARS:
         ps.toggle_activation();
         break;
      case Simulation::SUPERNOVAE:
         sn.toggle_activation();
         break;
      default:
         // Ought not get here !!
         std::string msg = "Simulation::toggle() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

Vector3D Simulation::getViewPosition(void) const {
   return flyboy.get_platform().position();
   }

Vector3D Simulation::getViewDirection(void) const {
   return flyboy.get_platform().look();
   }

Vector3D Simulation::getViewUp(void) const {
   return flyboy.get_platform().up();
   }

void Simulation::step(void) {
   flyboy.step();
   }

void Simulation::moveRequest(SolarSystemGlobals::movements mv) {
   flyboy.manouevre(mv);
   }
