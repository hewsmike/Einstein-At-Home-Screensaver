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

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <iostream>
#include <map>
#include <string>

#include <oglft/OGLFT.h>

#include "Constellations.h"
#include "Craft.h"
#include "ErrorHandler.h"
#include "Globe.h"
#include "GridGlobe.h"
#include "HUDBorderLayout.h"
#include "HUDContent.h"
#include "HUDFlowHorizontalLayout.h"
#include "HUDFlowVerticalLayout.h"
#include "HUDImage.h"
#include "HUDTextLine.h"
#include "HUDTextLineScroll.h"
#include "Pulsars.h"
#include "Renderable.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "SunOrbit.h"
#include "Supernovae.h"
#include "UTC.h"
#include "Vector3D.h"
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

class Simulation : public Renderable {
   public:
      /// Enumerants for the scene elements
      enum content {AXES, CONSTELLATIONS, EARTH, SUN, GRID, PULSARS, SUPERNOVAE, HUDOVER};

      /**
       * \brief Constructor
       */
      Simulation(void);

      /**
       * \brief Destructor
       */
      ~Simulation();

      Vector3D getViewPosition(void) const;

      Vector3D getViewDirection(void) const;

      Vector3D getViewUp(void) const;

      void step(void);

      void moveRequest(SolarSystemGlobals::movements mv);

      void resize(GLuint width, GLuint height);

      void setFont(content element, OGLFT_ft* font);

      // This is not the Renderable base class function,
      // it is additional to that interface and allows
      // per component cycling.
      void cycle(Simulation::content ct);

   protected:
      /// These three routines below satisfy the Renderable interface.

      /// Provide OpenGL code to prepare for rendering.
      virtual void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      virtual void release(void);

      /// Provide OpenGL code to render the object.
      virtual void render(void);

   private:
      static const std::string EARTH_NAME;
      static const std::string EARTH_IMAGE_FILE;
      static const GLuint EARTH_STACKS;
      static const GLuint EARTH_SLICES;
      static const GLfloat EARTH_TEXTURE_OFFSET;

      static const std::string SUN_NAME;
      static const std::string SUN_IMAGE_FILE;
      static const GLuint SUN_STACKS;
      static const GLuint SUN_SLICES;
      static const GLfloat SUN_TEXTURE_OFFSET;

      static const GLfloat AT_INFINITY;

      /// Ephemeris interval counter limit values.
      static const unsigned int COUNT_START;
      static const unsigned int COUNT_END;

      static const GLuint CONSTELLATIONS_RADIUS;
      static const GLuint PULSARS_RADIUS;
      static const GLuint SUPERNOVAE_RADIUS;
      static const GLuint GRID_RADIUS;
      static const GLuint GRID_STACKS;
      static const GLuint GRID_SLICES;

      /// HUD orthographic clip bounds.
      static const GLint HUD_LEFT_CLIP;
      static const GLint HUD_BOTTOM_CLIP;
      static const GLint HUD_NEAR_CLIP;
      static const GLint HUD_FAR_CLIP;

      /// The current screen/window dimensions
      GLuint screen_width;
      GLuint screen_height;

      /// Ephemeris refresh interval counter.
      unsigned int count_down;

      /// Last UTC query
      int min60;

      int hour24;

      GLfloat day366;

      /// Variable Earth factors.
      GLfloat earth_hour_angle;

      /// Variable Sun factors
      GLfloat sun_rot_angle;
      Vector3D sun_pos;

      /// The renderable scene elements
      Constellations cs;
      Pulsars ps;
      Supernovae sn;
      GridGlobe gg;
      Globe earth;
      Globe sun;

      Craft flyboy;

      UTC clock;

      std::map<content, OGLFT_ft*> fonts;

      HUDImage* geo_image;
      HUDImage* ligo_image;
      HUDImage* aps_image;
      HUDImage* wyp_image;
      HUDImage* lsc_image;
      HUDTextLineScroll* welcome_text;

      HUDBorderLayout overlay;

      HUDFlowHorizontalLayout north_panel;
      HUDFlowHorizontalLayout south_panel;
      HUDFlowVerticalLayout east_panel;
      HUDFlowVerticalLayout west_panel;
   };

/**
 * @}
 */

#endif /* SIMULATION_H_*/
