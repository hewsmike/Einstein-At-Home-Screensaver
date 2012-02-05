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

#ifndef SOLARSYSTEM_GLOBALS_H_
#define SOLARSYSTEM_GLOBALS_H_

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include <oglft/OGLFT.h>

#include "ErrorHandler.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Vector3D.h"

// SIN and COS take arguments in DEGREES
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

/// Convenience type for OGLFT font specification
typedef OGLFT::TranslucentTexture OGLFT_ft;

/**
 * \brief %SolarSystem Helper class with global definitions and useful static
 *        functions that one doesn't want to put elsewhere. Didn't want to call
 *        it 'general miscellaneous', but of course may refactor in future. :-)
 *
 * This class contains the following :
 *    - enumerants of rendering quality levels
 *    - circular trigonemetric constants
 *    - the current rendering quality level, with mutator and accessor
 *    - routine to check OpenGL error reports
 */

class SolarSystemGlobals {
   public:
      enum movements {GO_HOME,
                      STOP_TRANSLATION,
                      STOP_ROTATION,
                      FORWARD,
                      REVERSE,
                      UPWARDS,
                      DOWNWARDS,
                      LEFTWARDS,
                      RIGHTWARDS,
                      PITCH_UP,
                      PITCH_DOWN,
                      ROLL_LEFT,
                      ROLL_RIGHT,
                      YAW_LEFT,
                      YAW_RIGHT };

      // Levels of rendering quality.
      enum render_quality {RENDER_LOWEST, RENDER_MEDIUM, RENDER_HIGHEST};

      // TODO - arbitrary distance units here, what of 'realistic' scaling ??
      // How far away is the distant sky ?
      static const GLuint CELESTIAL_SPHERE_RADIUS;

      static const GLuint EARTH_RADIUS;

      static const GLuint SUN_RADIUS;

      /// Useful degree measures of arc.
      static const GLfloat FULL_CIRCLE_DEG;
      static const GLfloat HALF_CIRCLE_DEG;
      static const GLfloat QUARTER_CIRCLE_DEG;

      /// Minimum accepted screen/window dimensions
      static const GLuint MIN_SCREEN_WIDTH;
      static const GLuint MIN_SCREEN_HEIGHT;

      SolarSystemGlobals();
      ~SolarSystemGlobals();

      static void check_OpenGL_Error(void);

      static void set_render_level(SolarSystemGlobals::render_quality rq);

      static render_quality get_render_level(void);

      static SDL_Surface* loadImage(std::string filename, GLenum* format);

   private:
      /// How many color channels if an alpha channel included.
      static const GLuint ALPHA_CHANNEL;

      /// How many color channels if an alpha channel NOT included.
      static const GLuint NO_ALPHA_CHANNEL;

      // The current quality level.
      static SolarSystemGlobals::render_quality qual;

      /**
       * \brief Transfrom an OpenGL string type to an STL string type.
       *
       * \param glstring : the OpenGL string
       *
       * \return : an std::string version
       */
      static std::string parseGLstring(const GLubyte* glstring);
   };

#endif /*SOLARSYSTEM_GLOBALS_H_*/
