/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   As amended 2012 by Mike Hewson                                        *
 *   hewsmike[AT]iinet.net.au                                              *
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
#include <sstream>
#include <string>

#include <diagnostics.h>
#include <boinc_api.h>
#include <svn_version.h>

#include "../erp_git_version.h"

#include "AbstractGraphicsEngine.h"
#include "Events.h"
#include "GraphicsEngineFactory.h"
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
    // Print version info if the command line requests it.
    if(argc == 2) {
        string param(argv[1]);
        if(param == "--version" || param == "-v") {
            cout << "Version information:" << endl;
            cout << "Graphics Application Revision: " << ERP_GIT_VERSION << endl;
            cout << "BOINC Revision: " << SVN_VERSION << endl;
            exit(0);
            }
        }

    // Enable BOINC diagnostics
    /// TODO: we might want to optimize this for glibc- and mingw-based stacktraces!
    boinc_init_graphics_diagnostics(BOINC_DIAG_DEFAULTS);

#ifdef __APPLE__
    // Supply an icon for Mac's.
    setMacIcon(argv[0], MacAppIconData, sizeof(MacAppIconData));
#endif

    // Choose application type to be built/used.
    GraphicsEngineFactory::Applications scienceApplication;
#ifdef SCIENCE_APP
    // The SCIENCE_APP symbol is defined in the makefile
    // that produces the build for this product.
    scienceApplication = GraphicsEngineFactory::SCIENCE_APP;
#else
    // So if SCIENCE_APP was not given, this is the default.
    scienceApplication = GraphicsEngineFactory::EinsteinGravity;
#endif

    // Make an ABstractGraphicsEngine on the heap.
    // Edit this call for different build sources ie. swap 'Solarsystem' for whatever
    AbstractGraphicsEngine* graphics = GraphicsEngineFactory::createInstance(GraphicsEngineFactory::SolarSystem,
                                                                             scienceApplication);
    if(!graphics) {
        ErrorHandler::record("SolarSystem::main() : Requested graphics engine could not be found/instantiated!", ErrorHandler::FATAL);
        }

    // Instantiate and initialize our window manager.
    WindowManager window;
    // Register AbstractGraphicsEngine as event observer.
    window.registerEventObserver(graphics);
    if(window.initialize() != true) {
        // Failure, so destroy the AbstractGraphicsEngine and ...
        delete graphics;
        // ... emit a message with exit.
        ErrorHandler::record("SolarSystem::main() : Window manager could not be initialized!", ErrorHandler::FATAL);
        }


    // Discover and record the OpenGL version.
    GLuint major = 0;
    GLuint minor = 0;
    window.getOGLVersion(&major, &minor);
    std::stringstream msg1;
    msg1 << "SolarSystem::main() : OpenGL version = "
         << major << '.' << minor;
    ErrorHandler::record(msg1.str(), ErrorHandler::INFORM);

    // Discover and record the OpenGL vendor.
    const GLubyte* vendor = glGetString(GL_VENDOR);
    if(vendor != NULL) {
        std::stringstream msg2;
        msg2 << "SolarSystem::main() : OpenGL vendor string = "
             << "'"
             << ErrorHandler::convertGLstring(vendor)
             << "'";
        ErrorHandler::record(msg2.str(), ErrorHandler::INFORM);
        }
    else {
        ErrorHandler::record("SolarSystem::main() : I got a null for GL_VENDOR", ErrorHandler::WARN);
        }

    // Discover and record the OpenGL renderer.
    const GLubyte* renderer = glGetString(GL_RENDERER);
    if(renderer != NULL) {
        std::stringstream msg3;
        msg3 << "SolarSystem::main() : OpenGL renderer string = "
             << "'"
             << ErrorHandler::convertGLstring(renderer)
             << "'";
        ErrorHandler::record(msg3.str(), ErrorHandler::INFORM);
        }
    else {
        ErrorHandler::record("SolarSystem::main() : I got a null for GL_RENDERER", ErrorHandler::WARN);
        }

    // Using a ResourceFactory, create font and icon resource instances
    ResourceFactory factory;
    const Resource* fontResource = factory.createInstance("FontSansSerif");
    const Resource* iconResource = factory.createInstance("AppIconBMP");

    // Check if font resource is available.
    if(fontResource == NULL) {
        // No, so that's a fatal. Exit with appropriate deletions and message.
        delete graphics;
        ErrorHandler::record("SolarSystem::main() : Font resource is NULL!", ErrorHandler::FATAL);
        }

    // Given that you have a font resource, is the size meaningful?
    if(fontResource->data()->size() <= 0) {
        // No, so that's also a fatal. Exit with appropriate deletions and message.
        delete graphics;
        delete fontResource;
        ErrorHandler::record("SolarSystem::main() : Font resource could not be loaded!", ErrorHandler::FATAL);
        }

    // Check for an icon resource, but this is not fatal in the lack.
    if(iconResource != NULL && iconResource->data()->size() > 0) {
        window.setWindowIcon(&iconResource->data()->at(0), iconResource->data()->size());
        delete iconResource;
        }
    else {
        ErrorHandler::record("SolarSystem::main() : Icon resource could not be loaded! Continuing anyway...", ErrorHandler::WARN);
        }



    // Prepare for rendering by initialising chosen engine
    // and get up to date BOINC information.
    graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);
    graphics->refreshBOINCInformation();

    // Set the caption or window title.
    /// TODO - when GLFW 3.x arrives, put in a window icon as well.
    window.setWindowCaption("Einstein@Home");

    // Check other optional command line parameters
    if(argc == 2) {
        string param(argv[1]);
        if(param == "--fullscreen") {
            // Set non-interactive mode ie. 'screensaver'
            // (must do this first on Apple).
            window.setScreensaverMode(true);
            }
        if(param == "--fullscreen" || param == "--demo") {
            // Switch to fullscreen.
            // (on windoze: after init!)
            window.toggleFullscreen();
#ifdef __APPLE__
            SetMacSSLevel();
#endif
            }
        }

    // enter main event loop, but first flush any events.
    Events::Instance(0)->flush();
    window.eventLoop();

    // clean up end exit
    window.unregisterEventObserver(graphics);
    delete graphics;
    delete fontResource;

    exit(0);
    }
