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

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <string>

#include "Constellations.h"
#include "Earth.h"
#include "ErrorHandler.h"
#include "GridGlobe.h"
#include "Pulsars.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Supernovae.h"
#include "VectorSP.h"

class Universe {
   public:
      // TODO - arbitrary distance units here, what of 'realistic' scaling ??
      // How far away is the distant sky ?
      static const GLuint CELESTIAL_SPHERE_RADIUS;

      enum content {AXES, CONSTELLATIONS, EARTH, GRID, PULSARS, SUPERNOVAE};

	private:
      Constellations cs;
		Pulsars ps;
      Supernovae sn;
      GridGlobe gg;
      Earth home;

   public:
		Universe();
      ~Universe();

      void activate();
      void toggle(Universe::content ct);
      void draw();
   };

#endif /*UNIVERSE_H_*/
