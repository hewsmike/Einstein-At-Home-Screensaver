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

#include "SolarSystemGlobals.h"

const GLuint SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS(10000);

const GLuint SolarSystemGlobals::EARTH_RADIUS(100);

// Default rendering quality is the lowest.
SolarSystemGlobals::render_quality SolarSystemGlobals::qual = SolarSystemGlobals::RENDER_LOWEST;

const GLfloat SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);
const GLfloat SolarSystemGlobals::HALF_CIRCLE_DEG(180.0f);
const GLfloat SolarSystemGlobals::QUARTER_CIRCLE_DEG(90.0f);

SolarSystemGlobals::SolarSystemGlobals() {
   }

SolarSystemGlobals::~SolarSystemGlobals() {
   }

void SolarSystemGlobals::check_OpenGL_Error() {
   GLenum error_code = glGetError();

   // Some linux implementations ( despite OpenGL specs ) have 'no error'
   // hard coded - decimal '1282' here - and not as an enumerant. I guess if any
   // others turn up they'll need factoring into the boolean test. Go figure ...
   // In any case hit glGetError() until it runs out of errors.
   while((error_code != GL_NO_ERROR) && (error_code != 1282)) {
      std::string message = "SolarSystemGlobals::check_OpenGL_Error() - Reported OpenGL error with code : ";
      message += error_code;
      message += " - ";
      message += parseGLstring(gluErrorString(glGetError()));
      // Record any message string obtained from OpenGL.
      ErrorHandler::record(message, ErrorHandler::WARN);
      // Fetch the next error code.
      error_code = glGetError();
      }
   }

std::string SolarSystemGlobals::parseGLstring(const GLubyte* glstring) {
   // Start with an empty string as the return value.
   std::string ret_val;

   // Traverse the OpenGL 'string' from it's begining.
   int i = 0;
   char current = 0;
   // Assuming we haven't reached the null character terminator.
   while((current = *(glstring + i)) != 0) {
      // Add the current character onto the string.
      ret_val += current;
      // Increment to the next character position.
      i++;
      }

   // Return any string accumulated ( which may be empty ).
   return ret_val;
   }

void SolarSystemGlobals::set_render_level(SolarSystemGlobals::render_quality rq) {
   qual = rq;
   }

SolarSystemGlobals::render_quality SolarSystemGlobals::get_render_level(void) {
   return qual;
   }
