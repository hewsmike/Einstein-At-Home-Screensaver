/**************************************************************************
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

#ifndef SOLARSYSTEM_GLOBALS_H_
#define SOLARSYSTEM_GLOBALS_H_

#include <iostream>
#include <string>

#include "ErrorHandler.h"
#include "SDL_opengl.h"

// SIN and COS take arguments in DEGREES
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/**
 * \addtogroup solarsystem SolarSystem
 * @{
 */

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
      // Levels of rendering quality.
      enum render_quality {RENDER_LOWEST, RENDER_MEDIUM, RENDER_HIGHEST};

      /// Degree measures of arc.
      static const GLfloat FULL_CIRCLE_DEG;
      static const GLfloat HALF_CIRCLE_DEG;
      static const GLfloat QUARTER_CIRCLE_DEG;

   private:
      // The current quality level.
      static SolarSystemGlobals::render_quality qual;

      // gluErrorString(), and others, return 'const GLubyte *' - essentially
      // devolving to 'const unsigned char *'  - but string expects
      // 'const char *'. Trouble is that type conversion/cast is problematic, so
      // one has to traverse an OpenGL 'string' ( fortunately null terminated )
      // in order to construct a workable C++ std::string version. This
      // assumes that the typedef 'khronos_uint8_t' won't change in future ....
      static std::string parseGLstring(const GLubyte* glstring);

   public:
      SolarSystemGlobals();
      ~SolarSystemGlobals();

      static void check_OpenGL_Error(void);

      static void set_render_level(SolarSystemGlobals::render_quality rq);

      static SolarSystemGlobals::render_quality get_render_level(void);
   };

#endif /*SOLARSYSTEM_GLOBALS_H_*/
