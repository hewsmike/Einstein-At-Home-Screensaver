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

#include "framework.h"

#include <string>
#include <vector>

#include <oglft/OGLFT.h>

// SIN and COS take arguments in DEGREES
// TODO - Make these static constants.
#define PI 3.14159265
#define PI2 (2*PI)
// TODO - Make these static ( inline ? ) functions.
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
 *        functions that I didn't put elsewhere. Didn't want to call
 *        it 'general miscellaneous', but of course may refactor in future. :-)
 */

class SolarSystemGlobals {
   public:
      // Hmmmm .... why did I put these here ?? :-)
      /// TODO - correct this ..... ???
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

      /**
       * \brief Set the current render level setting.
       *
       * \param : the desired render level setting
       */
      static void set_render_level(SolarSystemGlobals::render_quality rq);

      /**
       * \brief Obtain the current render level setting.
       *
       * \return : the current render level setting
       */
      static render_quality get_render_level(void);

      /**
       * \brief Transform an OpenGL string type to an STL string type.
       *
       * \param glstring : the OpenGL string
       *
       * \return : an std::string version
       */
      static std::string convertGLstring(const GLubyte* glstring);

      /**
       * \brief Obtain the major and minor version number of the
       *        current OpenGL context.
       *
       * \param major : pointer to place the major version number
       * \param minor : pointer to place the minor version number
       */
      static void getOGLVersion(GLuint* major, GLuint* minor);

      /**
       * \brief Tokenise a given string using a given delimiter character
       *
       * \param str : the string to tokenise
       * \param delimiter : the character to delimit by
       * \param store : a reference to the container to store the tokens in
       *
       */
      static void tokeniseString(const std::string str, const char delimiter, std::vector<std::string>& store);

   private:
      /// If needed, what is the preferred OpenGL compatibility mode?
      static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MAJOR;
      static const GLuint PREFERRED_OGL_COMPATIBILITY_MODE_MINOR;

      /// How many color channels if an alpha channel included.
      static const GLuint ALPHA_CHANNEL;

      /// How many color channels if an alpha channel NOT included.
      static const GLuint NO_ALPHA_CHANNEL;

      /// The current quality level.
      static SolarSystemGlobals::render_quality qual;
   };

#endif /*SOLARSYSTEM_GLOBALS_H_*/
