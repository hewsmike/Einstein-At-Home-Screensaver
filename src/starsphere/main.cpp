/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include <diagnostics.h>
#include <boinc_api.h>
#include <svn_version.h>

#include "../erp_git_version.h"
#include "framework.h"

#include "AbstractGraphicsEngine.h"
#include "ErrorHandler.h"
#include "GraphicsEngineFactory.h"
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
            stringstream boinc_version;
            boinc_version << "Version information:" << endl;
            boinc_version << "Graphics Application Revision: " << ERP_GIT_VERSION << endl;
            boinc_version << "BOINC Revision: " << SVN_VERSION << endl;
            ErrorHandler::record(boinc_version.str(), ErrorHandler::NORMAL_EXIT);
            }
        }

    // Put these first up here, in order to handle any failure(s)
    // immediately ( hopefully cleaner for debug ).
    // To circumvent potential later failure of SDL_Init(), as we are not
    // not using SDL_main() as a program entry point. The SDL2 Wiki entries
    // ( as of 05 Jan 2014 ) are :
    //
    // Under 'Initialization and Shutdown' : "It should be noted that on some
    // operating systems, SDL_Init() will fail if SDL_main() has not been defined
    // as the entry point for the program. Calling SDL_SetMainReady() prior to
    // SDL_Init() will circumvent this failure condition, however, users should be
    // careful when calling SDL_SetMainReady() as improper initalization may cause
    // crashes and hard to diagnose problems."
    //
    // Under 'SDL_SetMainReady' : "This function is defined in SDL_main.h, along
    // with the preprocessor rule to redefine main() as SDL_main(). Thus to ensure
    // that your main() function will not be changed it is necessary to define
    // SDL_MAIN_HANDLED before including SDL.h."
    //
    // Err ..... some each-way bet here ??? :-O
    //
    // NB : FWIW This order of usage ( as recommended in the SDL2 Wiki )
    // contradicts the statement that SDL_Init() must be called before
    // using any other SDL function ! :-0
    SDL_SetMainReady();
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        // Failed, which is FATAL.
        stringstream sdl_init_error;
        sdl_init_error << "\nUnable to initialize SDL:  "
                       << ErrorHandler::check_SDL2_Error()
                       << std::endl;
        ErrorHandler::record(sdl_init_error.str(), ErrorHandler::FATAL);
        }

    // At this point SDL initialisation will have succeeded, so make sure
    // that ( for whatever 'normal' exit modes later occur ) SDL will be
    // cleaned up.
    atexit(SDL_Quit);

    // Attempt to initialise SDL TTF.
    if(TTF_Init() != 0) {
        // Failed, which is FATAL.
        stringstream sdl_ttf_init_error;
        sdl_ttf_init_error << "\nUnable to initialize SDL TTF:  "
                           << ErrorHandler::check_SDL2_TTF_Error()
                           << std::endl;
        ErrorHandler::record(sdl_ttf_init_error.str(), ErrorHandler::FATAL);
        }

    // At this point SDL TTF initialisation will have succeeded, so make sure
    // that ( for whatever 'normal' exit modes later occur ) SDL TTF will be
    // cleaned up.
    atexit(TTF_Quit);

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
    // And if SCIENCE_APP was not given, this is the default.
    scienceApplication = GraphicsEngineFactory::EinsteinGravity;
#endif

    // Make an AbstractGraphicsEngine on the heap, FATAL on fail.
    // Edit this call for different build sources ie. swap 'Solarsystem' for whatever
    AbstractGraphicsEngine* graphics = GraphicsEngineFactory::createInstance(GraphicsEngineFactory::Starsphere,
                                                                             scienceApplication);
    if(graphics == NULL) {
        ErrorHandler::record("SolarSystem::main() : Requested graphics engine could not be found/instantiated!", ErrorHandler::FATAL);
        }

    // Instantiate and then attempt to initialize our window manager.
    WindowManager window;
    if(window.initialize() != true) {
        // Failure, so destroy the AbstractGraphicsEngine and ...
        delete graphics;
        // ... emit a message with exit.
        ErrorHandler::record("SolarSystem::main() : Window manager could not be initialized!", ErrorHandler::FATAL);
        }

    // Using a ResourceFactory instance, create font and icon resource instances.
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
        // Use that icon for the window ....
        window.setWindowIcon(&iconResource->data()->at(0), iconResource->data()->size());
        delete iconResource;
        }
    else {
        ErrorHandler::record("SolarSystem::main() : Icon resource could not be loaded! Continuing anyway...", ErrorHandler::WARN);
        }

    // Set the caption or window title.
    window.setWindowCaption("Einstein@Home");

    // Register AbstractGraphicsEngine as event observer.
    window.registerEventObserver(graphics);

    // Prepare for rendering by initialising chosen engine.
    graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);

    // Get up to date BOINC information.
    graphics->refreshBOINCInformation();

    // For testing keep out of screensaver mode.
    window.setScreensaverMode(false);

    // Check other optional command line parameters
    if(argc == 2) {
        string param(argv[1]);
        if(param == "--fullscreen") {
            // Set non-interactive mode ie. 'screensaver'
            // (must do this first on Apple).
            window.setScreensaverMode(true);
            }
        if(param == "--demo") {
            ErrorHandler::record("SolarSystem::main() : Fullscreen & demo mode ...", ErrorHandler::INFORM);
            window.setScreensaverMode(false);
            // Initialised display is as a window,
            // so we transition to a fullscreen.
            window.toggleFullscreen();
#ifdef __APPLE__
            SetMacSSLevel();
#endif
            }
        }

    // Enter main event loop.
    window.eventLoop();

    // Clean up and exit
    window.unregisterEventObserver(graphics);
    delete graphics;
    delete fontResource;

    exit(0);
    }
