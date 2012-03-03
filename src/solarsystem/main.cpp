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

#include <cstdlib>
#include <iostream>
#include <string>

#include <diagnostics.h>
#include <boinc_api.h>
#include <svn_version.h>

#include "../erp_git_version.h"

#include "AbstractGraphicsEngine.h"
#include "GraphicsEngineFactory.h"
#include "OpenGLExts.h"
#include "SolarSystem.h"
#include "SolarSystemGlobals.h"
#include "ResourceFactory.h"
#include "WindowManager.h"

#ifdef __APPLE__
#include "EaHMacIcon.h"
#ifdef __cplusplus
extern "C" {
#endif
    void SetMacSSLevel();
#ifdef __cplusplus
}
#endif
#endif

int main(int argc, char **argv) {
   // print version info
   if(argc == 2) {
      string param(argv[1]);
      if(param == "--version" || param == "-v") {
         cout << "Version information:" << endl;
         cout << "Graphics Application Revision: " << ERP_GIT_VERSION << endl;
         cout << "BOINC Revision: " << SVN_VERSION << endl;
         exit(0);
         }
      }

   // enable BOINC diagnostics
   // TODO: we might want to optimize this for glibc- and mingw-based stacktraces!
   boinc_init_graphics_diagnostics(BOINC_DIAG_DEFAULTS);

#ifdef __APPLE__
  setMacIcon(argv[0], MacAppIconData, sizeof(MacAppIconData));
#endif

   // choose application to be build/used
   GraphicsEngineFactory::Applications scienceApplication;
#ifdef SCIENCE_APP
   scienceApplication = GraphicsEngineFactory::SCIENCE_APP;
#else
   scienceApplication = GraphicsEngineFactory::EinsteinS5R3;
#endif

   // prepare main objects
   WindowManager window;
   ResourceFactory factory;
   // Edit this call for different build sources ie. swap 'Solarsystem' for whatever
   AbstractGraphicsEngine* graphics = GraphicsEngineFactory::createInstance(
                                         GraphicsEngineFactory::SolarSystem,
                                         scienceApplication);
   if(!graphics) {
      cerr << "Requested graphics engine could not be found/instantiated!" << endl;
      exit(1);
      }

   // initialize window manager
   if(!window.initialize()) {
      cerr << "Window manager could not be initialized!" << endl;
      delete graphics;
      exit(1);
      }

   // Hmmm .... I suppose we ought have an OpenGL context by now.
   OpenGLExts::acquire();

   // Find out the OpenGL version.
   GLuint major = 0;
   GLuint minor = 0;
   SolarSystemGlobals::getOGLVersion(&major, &minor);
   std::cout << "OpenGL version = "
             << major << '.' << minor << std::endl;

   // Find out the OpenGL vendor.
   const GLubyte* vendor = glGetString(GL_VENDOR);
   if(vendor != NULL) {
      std::cout << "OpenGL vendor string = '"
                << SolarSystemGlobals::convertGLstring(vendor)
                << "'" << std::endl;
      }
   else {
      std::cout << "I got a null for GL_VENDOR" << std::endl;
      }

   // Find out the OpenGL renderer.
   const GLubyte* renderer = glGetString(GL_RENDERER);
   if(renderer != NULL) {
      std::cout << "OpenGL renderer string = '"
                << SolarSystemGlobals::convertGLstring(renderer)
                << "'" << std::endl;
      }
   else {
      std::cout << "I got a null for GL_RENDERER" << std::endl;
      }

   // Find out the OpenGL extensions.
   SolarSystemGlobals::getOGLExtensions();

   if(SolarSystemGlobals::setOGLContextVersion(major, minor) == true){
      std::cout << "The OpenGL context is adequate" << std::endl;
      }
   else {
      std::cout << "The OpenGL context is inappropriate" << std::endl;
      }

   // create font and icon resource instances
   const Resource* fontResource = factory.createInstance("FontSansSerif");
   const Resource* iconResource = factory.createInstance("AppIconBMP");

   if(fontResource == NULL) {
      cerr << "Font resource could not be loaded!" << endl;
      delete graphics;
      exit(1);
      }

   if(fontResource->data()->size() <= 0) {
      cerr << "Font resource could not be loaded!" << endl;
      delete graphics;
      delete fontResource;
      exit(1);
      }

   if(iconResource != NULL && iconResource->data()->size() > 0) {
      window.setWindowIcon(&iconResource->data()->at(0), iconResource->data()->size());
      delete iconResource;
      }
   else {
      cerr << "Icon resource could not be loaded! Continuing anyway..." << endl;
      }

   window.setWindowCaption("Einstein@Home");

   // register starsphere as event observer
   window.registerEventObserver(graphics);

   // prepare rendering
   graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);
   graphics->refreshBOINCInformation();

   // check optional command line parameters
   if(argc == 2) {
      string param(argv[1]);
      if(param == "--fullscreen") {
         // set non-interactive mode (must do this first on Apple)
         window.setScreensaverMode(true);
         }
      if(param == "--fullscreen" || param == "--demo") {
         // switch to fullscreen (on windoze: after init!)
         window.toggleFullscreen();
#ifdef __APPLE__
         SetMacSSLevel();
#endif
         }
      }

   // enter main event loop
   window.eventLoop();

   // clean up end exit
   window.unregisterEventObserver(graphics);
   delete graphics;
   delete fontResource;

   exit(0);
   }
