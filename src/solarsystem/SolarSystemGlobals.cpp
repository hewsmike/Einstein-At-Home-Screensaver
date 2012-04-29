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

#include "SolarSystemGlobals.h"

#include "framework.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

const GLuint SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS(10000);

const GLuint SolarSystemGlobals::EARTH_RADIUS(100);

const GLuint SolarSystemGlobals::SUN_RADIUS(800);

// Default rendering quality is the lowest.
SolarSystemGlobals::render_quality SolarSystemGlobals::qual(SolarSystemGlobals::RENDER_LOWEST);

const GLfloat SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);
const GLfloat SolarSystemGlobals::HALF_CIRCLE_DEG(180.0f);
const GLfloat SolarSystemGlobals::QUARTER_CIRCLE_DEG(90.0f);

const GLuint SolarSystemGlobals::MIN_SCREEN_WIDTH(800);
const GLuint SolarSystemGlobals::MIN_SCREEN_HEIGHT(600);

// Four channels with the alpha, three without.
const GLuint SolarSystemGlobals::NO_ALPHA_CHANNEL(3);
const GLuint SolarSystemGlobals::ALPHA_CHANNEL(NO_ALPHA_CHANNEL + 1);

SolarSystemGlobals::SolarSystemGlobals() {
   }

SolarSystemGlobals::~SolarSystemGlobals() {
   }

std::string SolarSystemGlobals::convertGLstring(const GLubyte* glstring) {
   // gluErrorString(), and others, return 'const GLubyte *' - essentially
   // devolving to 'const unsigned char *'  - but string expects
   // 'const char *'. Trouble is that direct type conversion/cast is problematic,
   // so one has to traverse an OpenGL 'string' ( fortunately null terminated )
   // in order to construct a workable C++ STL string version. This
   // assumes that the typedef 'khronos_uint8_t' won't change in future ...

   // Start with an empty string as the return value.
   std::string ret_val;

   // Check that we weren't given a NULL.
   if(glstring != NULL) {
      // Traverse the OpenGL 'string' from it's begining.
      int i = 0;
      char current = 0;
      // Assuming we haven't reached the null character terminator.
      while((current = *(glstring + i)) != 0) {
         // Add the current character onto the string.
         ret_val += current;
         // Increment to the next character position.
         ++i;
         }
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

void SolarSystemGlobals::getOGLVersion(GLuint* major, GLuint* minor) {
   // Try to obtain the version string from the context.
   const GLubyte* version = glGetString(GL_VERSION);

   // Did that succeed ?
   if(version == NULL) {
      // No, can't proceed.
      std::string msg = "SolarSystemGlobals::getOGLVersion() : couldn't obtain version string";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Convert the version string to STL format.
   std::string ver = SolarSystemGlobals::convertGLstring(version);

   // Now tokenise the string thus obtained.
   std::vector<std::string> ver_strings;
   tokeniseString(ver, '.', ver_strings);

   if(ver_strings.size() < 2) {
      std::string msg = "SolarSystemGlobals::getOGLVersion() : couldn't tokenise version string";
      }

   int major_candidate = 0;
   std::stringstream convert1(ver_strings[0]);
   convert1 >> major_candidate;

   if(convert1.goodbit != 0) {
      std::string msg = "SolarSystemGlobals::getOGLVersion() : couldn't interpret major version string";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }
   *major = major_candidate;

   int minor_candidate = 0;
   std::stringstream convert2(ver_strings[1]);
   convert2 >> minor_candidate;

   if(convert2.goodbit != 0) {
      std::string msg = "SolarSystemGlobals::getOGLVersion() : couldn't interpret minor version string";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }
   *minor = minor_candidate;
   }

void SolarSystemGlobals::tokeniseString(const std::string str,
                                        const char delimiter,
                                        std::vector<std::string>& store) {
   size_t string_pos = 0;
   size_t delimiter_pos = 0;

   // Provided we don't get to the end of ( what remains
   // of ) the input string and not find a delimiter.
   while((delimiter_pos = str.find(delimiter, string_pos)) != std::string::npos) {
      // Are the delimiters adjacent ? Policy is that
      // we will not create empty tokens, hence multiple
      // adjacent delimiters are treated as one.
      if(delimiter_pos > string_pos) {
         // Non-adjacent.
         size_t token_len = delimiter_pos - string_pos;
         std::string token = str.substr(string_pos, token_len);
         store.push_back(token);
         }
      // Move along to the position just after last delimiter found.
      string_pos = delimiter_pos + 1;
      }
   }
