/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#include "SolarSystem.h"

#include "ErrorHandler.h"

const int SolarSystem::FAR_LOOK_RATIO(1000);
const GLdouble SolarSystem::FOV_ANGLE(45.0f);
const GLdouble SolarSystem::NEAR_CLIP(0.5f);
const GLdouble SolarSystem::FAR_CLIP(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS * 2.5f);
const int SolarSystem::FAR_LOOK_DISTANCE(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS*SolarSystem::FAR_LOOK_RATIO);

SolarSystem::SolarSystem(string sharedMemoryAreaIdentifier) :
    AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
    /**
     * Parameters and State info
     */
    spaceFontResource = NULL;
    }

SolarSystem::~SolarSystem() {
    }

/**
 * Window resize/remap. This is not directly called from WindowManager
 * but from initialize().
 */
void SolarSystem::resize(const int width, const int height) {
    // store current settings
    m_CurrentWidth = width;
    m_CurrentHeight = height;
    aspect = (float)width / (float)height;

    // adjust aspect ratio and projection
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV_ANGLE, aspect, NEAR_CLIP, FAR_CLIP);
    glMatrixMode(GL_MODELVIEW);

    // Tell the underlying simulation that it's window
    // has been resized, and what it's dimensions are.
    sim.resize(static_cast<GLuint>(width), static_cast<GLuint>(height));
    }

/**
 *  What to do when graphics are "initialized". Due to a Windows quirk
 *  however this will be called with each resize event ( including the
 *  initial window creation and fullscreen toggle ) from WindowManager.
 *  Currently WindowManager always calls this routine with recycle = true,
 *  and main() calls it with recycle = false ( ie. once only ). Also
 *  note that the default ( ie. if not explicitly specified upon call )
 *  value for recycle = false
 */
void SolarSystem::initialize(const int width, const int height, const Resource* font, const bool recycle) {
    // Check whether we initialize the first time or have to recycle (required for windoze)
    if(recycle == false) {
        // This is the first call of this routine from main().
        if(font != NULL) {
            // So remember the font resource pointer, if non NULL.
            spaceFontResource = font;
            }
        // initialize the BOINC client adapter
        m_BoincAdapter.initialize();
        }
    else {
        // This is the recurrent call of this routine from WindowManager.
        // Seems that windoze also "resets" our OpenGL fonts, so
        // let's clean up before reinitializing them.
        if(skygridFont != NULL) {
            delete skygridFont;
            }
        if(earthgridFont != NULL) {
            delete earthgridFont;
            }
        if(constellationFont != NULL) {
            delete constellationFont;
            }
        if(HUDFont != NULL) {
            delete HUDFont;
            }
        }

    // We might be called to recycle even before initialization. Why's that ??
    if(spaceFontResource == NULL) {
        // So we are here because spaceFontResource was not assigned
        // to an actual resource instance. Display a warning ???
        cerr << "Warning: font resource still unknown! You might want to recycle at a later stage..." << endl;
        }
    else {
        // create font instance using font resource (base address + size)
        skygridFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                                   spaceFontResource->data()->size(),
                                   13, 78);

        // Note short-circuit evaluation relevant in this if clause ie. right side
        // expression is evaluated only if left side expression is false. Matters
        // for pointer dereference so don't swap order of expressions here.
        if(skygridFont == NULL || (skygridFont->isValid() == false)) {
            /// TODO - better error path
            std::string msg = "SolarSystem::initialize() - Could not construct sky grid font face from in memory resource!";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            }
        skygridFont->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
        skygridFont->setForegroundColor(1.0f, 1.0f, 1.0f, 0.6f);

        // create font instance using font resource (base address + size)
        earthgridFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                                     spaceFontResource->data()->size(),
                                     13, 78);

        if(earthgridFont == NULL || (earthgridFont->isValid() == false)) {
            // TODO - better error path
            std::string msg = "SolarSystem::initialize() - Could not construct earth grid font face from in memory resource!";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            }
        earthgridFont->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
        earthgridFont->setForegroundColor(1.0f, 1.0f, 1.0f, 0.6f);

        // create font instance using font resource (base address + size)
        constellationFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                                         spaceFontResource->data()->size(),
                                         13, 78);
        // Short-circuit .....
        if(constellationFont == NULL || (constellationFont->isValid() == false)) {
            // TODO - better error path ?
            std::string msg = "SolarSystem::initialize() - Could not construct constellation font face from in memory resource!";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            }
        constellationFont->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
        constellationFont->setForegroundColor(1.0f, 0.84f, 0.0f, 0.6f);

        // create font instance using font resource (base address + size)
        HUDFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                               spaceFontResource->data()->size(),
                               18, 90);

        // Short-circuit .....
        if(HUDFont == NULL || (HUDFont->isValid() == false)) {
            // TODO - better error path ?
            std::string msg = "SolarSystem::initialize() - Could not construct HUD font face from in memory resource!";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            }
        HUDFont->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
        HUDFont->setForegroundColor(1.0f, 1.0f, 1.0f, 0.9f);
        }

    // Some Simulation components need to have a font before activation.
    sim.setFont(Simulation::CONSTELLATIONS, constellationFont);
    sim.setFont(Simulation::SKY_GRID, skygridFont);
    sim.setFont(Simulation::EARTH_GRID, earthgridFont);
    sim.setFont(Simulation::HUDOVER, HUDFont);

    // more font setup and optimizations
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
#if defined( GL_RASTER_POSITION_UNCLIPPED_IBM )
    glEnable( GL_RASTER_POSITION_UNCLIPPED_IBM );
#endif

    // drawing setup:
    glClearColor(0.0, 0.0, 0.0, 0.0); // background is black
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // FSAA will be enabled explicitly when needed!
    glDisable(GL_MULTISAMPLE_ARB);

    // we need alpha blending for proper font rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable depth buffering for 3D graphics
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // enable opt-in quality feature
    switch (m_BoincAdapter.graphicsQualitySetting()) {
        case BOINCClientAdapter::LowGraphicsQualitySetting :
            SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_LOWEST);
            break;
        case BOINCClientAdapter::MediumGraphicsQualitySetting :
            SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_MEDIUM);
            break;
        case BOINCClientAdapter::HighGraphicsQualitySetting :
            SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_HIGHEST);
            break;
        default :
            // Ought not get here !!
            std::string msg = "SolarSystem::initialize() - bad switch case reached (default)";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            break;
        }

    //// Set up lighting.
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    // GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

    // LIGHT0 is from the Sun towards the Earth.
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    // GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    // LIGHT1 is from our viewpoint towards the Sun.
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

    glDisable(GL_CLIP_PLANE0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    sim.activate();

    // Setup dimensions. NB this is currently the only call to
    // invoke this !!
    resize(width, height);

    ErrorHandler::check_OpenGL_Error();
    }

/**
 * Rendering routine: this is what does the drawing:
 */
void SolarSystem::render(const double tOD) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear the canvas plus set z ordering to the far-clip plane.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set modelview matrix to unity.
    glLoadIdentity();

    // Evolve the simulation.
    sim.step();

    // Where are we etc .... in our virtual world?
    const CameraState& cam = sim.viewPoint();

    // Set up the camera position and orientation.
    gluLookAt(cam.position().x(),
              cam.position().y(),                               // eyes position
              cam.position().z(),
              SolarSystem::FAR_LOOK_DISTANCE*cam.focus().x(),
              SolarSystem::FAR_LOOK_DISTANCE*cam.focus().y(),   // looking towards here
              SolarSystem::FAR_LOOK_DISTANCE*cam.focus().z(),
              cam.orientation().x(),
              cam.orientation().y(),                            // which way is up?
              cam.orientation().z());

    // Render from the current viewpoint.
    sim.draw();

    // Check for and report any errors generated.
    ErrorHandler::check_OpenGL_Error();

    // Switch buffers.
    glfwSwapBuffers();
    }

void SolarSystem::mouseButtonEvent(const int positionX, const int positionY,
                                   const AbstractGraphicsEngine::MouseButton buttonPressed) {
    }

void SolarSystem::mouseMoveEvent(const int deltaX, const int deltaY,
                                 const AbstractGraphicsEngine::MouseButton buttonPressed) {
    switch(buttonPressed) {
        case MouseButtonLeft:
            break;
        case MouseButtonRight:
            break;
        default:
            break;
        }
    }

void SolarSystem::keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed) {
    switch(keyPressed) {
        case KeyC:
            sim.moveRequest(Craft::REVERSE);
            break;
        case KeyD:
            sim.moveRequest(Craft::STOP_TRANSLATION);
            break;
        case KeyE:
            sim.moveRequest(Craft::FORWARD);
            break;
        case KeyF:
            sim.moveRequest(Craft::RIGHTWARDS);
            break;
        case KeyG:
            sim.moveRequest(Craft::GO_HOME);
            break;
        case KeyK:
            sim.moveRequest(Craft::ROLL_LEFT);
            break;
        case KeyL:
            sim.moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyO:
            sim.moveRequest(Craft::PITCH_DOWN);
            break;
        case KeyR:
            sim.moveRequest(Craft::UPWARDS);
            break;
        case KeyS:
            sim.moveRequest(Craft::LEFTWARDS);
            break;
        case KeyV:
            sim.moveRequest(Craft::DOWNWARDS);
            break;
        case KeyF1:
            // TODO - future 'help' functionality
            break;
        case KeyF2:
            // TODO - cycle the rendering level.
            // SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_LOWEST);
            break;
        case KeyF4:
            // TODO - cycle the HUD
            // sim.cycle(Simulation::HUDOVER);
            break;
        case KeyF5:
            sim.cycle(Simulation::CONSTELLATIONS);
            break;
        case KeyF6:
            sim.cycle(Simulation::PULSARS);
            break;
        case KeyF7:
            sim.cycle(Simulation::SUPERNOVAE);
            break;
        case KeyF8:
            sim.cycle(Simulation::SKY_GRID);
            break;
        case KeyF9:
            sim.cycle(Simulation::EARTH_GRID);
            break;
        case KeyF12:
            // Toggle the state of the autopilot
            sim.cycle(Simulation::AUTOPILOT);
            break;
        case KeySpace:
            // Whoa! Stop everything happening ...
            sim.moveRequest(Craft::STOP_TRANSLATION);
            sim.moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyKP1:
            sim.moveRequest(Craft::YAW_LEFT);
            break;
        case KeyKP2:
            sim.moveRequest(Craft::PITCH_UP);
            break;
        case KeyKP3:
            sim.moveRequest(Craft::YAW_RIGHT);
            break;
        case KeyKP4:
            sim.moveRequest(Craft::ROLL_LEFT);
            break;
        case KeyKP5:
            sim.moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyKP6:
            sim.moveRequest(Craft::ROLL_RIGHT);
            break;
        case KeyKP8:
            sim.moveRequest(Craft::PITCH_DOWN);
            break;
        case KeyComma:
            sim.moveRequest(Craft::YAW_LEFT);
            break;
        case KeyPeriod:
            sim.moveRequest(Craft::PITCH_UP);
            break;
        case KeyForwardSlash:
            sim.moveRequest(Craft::YAW_RIGHT);
            break;
        case KeySemiColon:
            sim.moveRequest(Craft::ROLL_RIGHT);
            break;
        default:
            break;
        }
    }

void SolarSystem::refreshLocalBOINCInformation() {
    }

void SSGUpdate(void) {
    switch(SolarSystemGlobals::getRenderLevel()) {
        case SolarSystemGlobals::RENDER_LOWEST:
            glDisable(GL_FOG);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
            break;
        case SolarSystemGlobals::RENDER_MEDIUM:
            // fog aids depth perception
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, GL_EXP2);
            glFogf(GL_FOG_DENSITY, 0.085);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
            break;
        case SolarSystemGlobals::RENDER_HIGHEST:
            // fog aids depth perception
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, GL_EXP2);
            glFogf(GL_FOG_DENSITY, 0.085);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            // some polishing
            glShadeModel(GL_SMOOTH);
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            break;
        default :
            // Ought not get here !!
            std::string msg = "SolarSystem::SSGUpdate() - bad switch case reached (default)";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            break;
        }
    }
