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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ErrorHandler.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "OpenGLExts.h"

const GLuint SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS(10000);

const GLuint SolarSystemGlobals::EARTH_RADIUS(100);

const GLuint SolarSystemGlobals::SUN_RADIUS(800);

// Default rendering quality is the lowest.
SolarSystemGlobals::render_quality SolarSystemGlobals::qual = SolarSystemGlobals::RENDER_LOWEST;

const GLfloat SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);
const GLfloat SolarSystemGlobals::HALF_CIRCLE_DEG(180.0f);
const GLfloat SolarSystemGlobals::QUARTER_CIRCLE_DEG(90.0f);

const GLuint SolarSystemGlobals::MIN_SCREEN_WIDTH(800);
const GLuint SolarSystemGlobals::MIN_SCREEN_HEIGHT(600);

// Four channels with the alpha, three without.
const GLuint SolarSystemGlobals::NO_ALPHA_CHANNEL(3);
const GLuint SolarSystemGlobals::ALPHA_CHANNEL(NO_ALPHA_CHANNEL + 1);

// NB Choose v2.1 as this PROBABLY fits all desired platforms,
// including Mac OSX 10.7 ....
const GLuint SolarSystemGlobals::PREFERRED_OGL_COMPATIBILITY_MODE_MAJOR(2);
const GLuint SolarSystemGlobals::PREFERRED_OGL_COMPATIBILITY_MODE_MINOR(1);

bool SolarSystemGlobals::ogl_extension_strings_obtained(false);

std::vector<std::string> SolarSystemGlobals::extension_strings;

SolarSystemGlobals::SolarSystemGlobals() {
   }

SolarSystemGlobals::~SolarSystemGlobals() {
   }

void SolarSystemGlobals::check_OpenGL_Error() {
   GLenum error_code = glGetError();

   // If glGetError is called when there is no current OpenGL context, then
   // a hard coded value decimal '1282' is returned.
   // In any case hit glGetError() until it runs out of errors.
   while(error_code != GL_NO_ERROR) {
      std::string message = "SolarSystemGlobals::check_OpenGL_Error() - Reported OpenGL error with code : ";
      message += error_code;
      message += " - ";
      message += convertGLstring(gluErrorString(glGetError()));
      // Record any message string obtained from OpenGL.
      ErrorHandler::record(message, ErrorHandler::WARN);
      // Fetch the next error code.
      error_code = glGetError();
      }
   }

std::string SolarSystemGlobals::convertGLstring(const GLubyte* glstring) {
   // gluErrorString(), and others, return 'const GLubyte *' - essentially
   // devolving to 'const unsigned char *'  - but string expects
   // 'const char *'. Trouble is that type conversion/cast is problematic, so
   // one has to traverse an OpenGL 'string' ( fortunately null terminated )
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

SDL_Surface* SolarSystemGlobals::loadImage(std::string filename, GLenum* format) {
   // See if SDL will load the image file into a surface struct ....
   SDL_Surface* ret_val = SDL_LoadBMP(filename.c_str());

   // Did that work?
   if(ret_val == NULL) {
      // No, the image did not load into a surface struct.
      ErrorHandler::record("SolarSystemGlobals::loadImage() - image file NOT loaded ", ErrorHandler::FATAL);
      }

   // Get the pixel storage format.
   GLuint num_colors = ret_val->format->BytesPerPixel;
   switch(num_colors) {
      case ALPHA_CHANNEL:
         if(ret_val->format->Rmask == 0x000000ff) {
            *format = GL_RGBA;
            }
         else {
            *format = GL_BGRA;
            }
         break;
      case NO_ALPHA_CHANNEL:
         if(ret_val->format->Rmask == 0x000000ff) {
            *format = GL_RGB;
            }
         else {
            *format = GL_BGR;
            }
         break;
         default:
         ErrorHandler::record("SolarSystemGlobals::loadImage() - unrecognised image file format", ErrorHandler::FATAL);
         break;
      }

   return ret_val;
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

   std::cout << "Major version = " << ver_strings.at(0) << std::endl;
   std::cout << "Minor version = " << ver_strings.at(1) << std::endl;

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

bool SolarSystemGlobals::setOGLContextVersion(GLuint major, GLuint minor) {
   // Assume failure.
   bool ret_val = false;

   // Convert version to a floating point. Beware minor version variable length.
   // How many significant digits for minor version ?
   GLuint num_minor_digits = floor(log10(minor)) + 1;
   // Raise that to a power of ten.
   GLfloat minor_digit_divisor = pow(10, num_minor_digits);

   // Now have correct divisor to represent version as a float.
   GLfloat version = major + minor/minor_digit_divisor;

   // Are we at least v1.5 ??
   if(version >= 1.5) {
      // Are we less than v3.0 ??
      if(version <= 3.0) {
         // Accept this. No further action required as post
         // v1.4 functions will be available by default.
         ret_val = true;
         }
      else {
         // No, we are more than v3.0
         if(version == 3.1) {
            // V3.1 is special in that we can use it, but only
            // if ARB_compatibility is defined.
            if(isOGLExtensionAvailable("GL_ARB_COMPATIBILITY")) {
               // Yes, but how do we actually take advantage of this ??
               // ret_val = true;
               }
            }
         else {
            // Ok, we are above 3.1 so want to invoke the
            // compatibility profile mechanism.
            // Again the question is how ......
            }
         }
      }

   return ret_val;
   }

void SolarSystemGlobals::getOGLExtensions(void) {
   // Assuming we haven't already done this.
   if(ogl_extension_strings_obtained == false) {
      extension_strings.clear();

      GLuint major = 0;
      GLuint minor = 0;

      // Obtain the current version from context.
      getOGLVersion(&major, &minor);

      // How we recover the extensions depends on the major version.
      if(major < 3) {
         // For up to v3.0 we get one long string,
         // that we have to tokenise ourselves.
         const GLubyte* extensions = glGetString(GL_EXTENSIONS);
         std::string exts = convertGLstring(extensions);
         tokeniseString(exts, ' ', extension_strings);
         }
      else {
         // For v3.0+ can use a better interface to step thru the extensions.
         // First find out how many extensions are there to be had.
         GLint num_extensions = 0;
         // GL_NUM_EXTENSIONS = 33309;
         glGetIntegerv(33309, &num_extensions);
         std::cout << "GL_NUM_EXTENSIONS = " << num_extensions << std::endl;
         // Now retrieve each separately.
         for(GLuint item = 0; item < num_extensions; ++item) {
            const GLubyte* ext_str = NULL;
            // Indirection with either build.
            ext_str = OpenGLExts::ExtGLGetStringi(GL_EXTENSIONS, item);
            // Store away for later.
            std::string extension = SolarSystemGlobals::convertGLstring(ext_str);
            std::cout << "Extension #" << item << " = \"" << extension << "\"" << std::endl;
            extension_strings.push_back(extension);
            }
         }
      }

   // Mark as done.
   ogl_extension_strings_obtained = true;
   }

bool SolarSystemGlobals::isOGLExtensionAvailable(std::string extension_name) {
   // Assume failure.
   bool ret_val = false;
   for(GLuint ext = 0; ext < extension_strings.size(); ++ext) {
      // Comparing the strings in their entireity.
      if(extension_strings[ext].compare(extension_name) == 0) {
         ret_val = true;
         break;
         }
      }

   return ret_val;
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
