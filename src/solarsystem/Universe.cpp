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

#include "Universe.h"

const GLuint Universe::CELESTIAL_SPHERE_RADIUS(10000);

Universe::Universe() : cs(CELESTIAL_SPHERE_RADIUS),
                       ps(CELESTIAL_SPHERE_RADIUS - 25),
                       sn(CELESTIAL_SPHERE_RADIUS - 50),
                       gg(CELESTIAL_SPHERE_RADIUS + 50, 36, 19),
                       home() {
   // Radius of rendering is ( slightly ) different for each scene element.
   // I'm not actually sure why, but I think that looks better ie. less
   // pixel level flickering than when all at the same radius. Presumably
   // as I have essentially reduced any z-order contention by use of offsets ??
	}

Universe::~Universe() {
   }

void Universe::draw() {
   // Invoke the draw method for each scene element.
   ps.draw();
   sn.draw();
   cs.draw();
   gg.draw();
   home.draw();
   }

void Universe::activate() {
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
         home.activate();
         break;
      case SolarSystemGlobals::RENDER_MEDIUM:
         ps.activate();
         sn.activate();
         cs.activate();
         gg.activate();
         home.activate();
         break;
      case SolarSystemGlobals::RENDER_HIGHEST:
         ps.activate();
         sn.activate();
         cs.activate();
         gg.activate();
         home.activate();
         break;
      default:
         // Ought not get here !!
         std::string msg = "Universe::activate() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
			break;
      }
	}

void Universe::toggle(Universe::content ct) {
   // Which scene element to toggle?
   switch(ct) {
      case Universe::AXES:
			break;
		case Universe::CONSTELLATIONS:
         cs.toggle_activation();
			break;
		case Universe::EARTH:
         home.toggle_activation();
			break;
		case Universe::GRID:
         gg.toggle_activation();
			break;
		case Universe::PULSARS:
			ps.toggle_activation();
			break;
		case Universe::SUPERNOVAE:
         sn.toggle_activation();
			break;
		default:
         // Ought not get here !!
         std::string msg = "Universe::toggle() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
			break;
      }
   }
