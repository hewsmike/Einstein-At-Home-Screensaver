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

#ifndef Simulation_H_
#define Simulation_H_

#include <string>

#include "Constellations.h"
#include "ErrorHandler.h"
#include "Globe.h"
#include "GridGlobe.h"
#include "Pulsars.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Supernovae.h"
#include "VectorSP.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Container for 3D scene renderable elements
 *
 * This class aggregates all the 3D scene elements ( non-HUD ) that one
 * wants in a given rendering frame.
 *
 * \see Renderable
 *
 * \author Mike Hewson\n
 */

class Simulation {
   public:
      // TODO - arbitrary distance units here, what of 'realistic' scaling ??
      // How far away is the distant sky ?
      static const GLuint CELESTIAL_SPHERE_RADIUS;

      static const GLuint EARTH_RADIUS;

      /// Enumerants for the scene elements
      enum content {AXES, CONSTELLATIONS, EARTH, GRID, PULSARS, SUPERNOVAE};

      /**
       * \brief Constructor
       */
      Simulation(void);

      /**
       * \brief Destructor
       */
      ~Simulation();

      /**
       * \brief Activate all enclosed scene elements as per current global
       *        rendering quality choice
       */
      void activate(void);

      /**
       * \brief Toggle the activation setting of an enclosed scene element
       *
       * \param ct the chosen content enumerator
       */
      void toggle(Simulation::content ct);

      /**
       * \brief Draw the activated enclosed elements for a single frame
       */
      void draw(void);

   private:
      static const std::string EARTH_NAME;
      static const std::string EARTH_IMAGE_FILE;
      static const GLuint EARTH_STACKS;
      static const GLuint EARTH_SLICES;
      static const GLfloat GREENWICH_TEXTURE_OFFSET;

      /// Ephemeris interval counter limit values.
      static const unsigned int COUNT_START;
      static const unsigned int COUNT_END;

      /// Ephemeris refresh interval counter.
      unsigned int count_down;

      /// Variable Earth factors.


      /// The renderable scene elements
      Constellations cs;
      Pulsars ps;
      Supernovae sn;
      GridGlobe gg;
      Globe earth;
   };

/**
 * @}
 */

#endif /*Simulation_H_*/
