/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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
#include "SolarSystemGlobals.h"

const int SolarSystem::FAR_LOOK_RATIO(1000);
const GLdouble SolarSystem::FOV_ANGLE_MIN(20.0f);
const GLdouble SolarSystem::FOV_ANGLE_MAX(70.0f);
const GLdouble SolarSystem::FOV_ANGLE_INITIAL((FOV_ANGLE_MAX + FOV_ANGLE_MIN)/2);
const GLdouble SolarSystem::FOV_ANGLE_GRADATIONS(12);
const GLdouble SolarSystem::FOV_ANGLE_DIFFERENTIAL((FOV_ANGLE_MAX - FOV_ANGLE_MIN)/FOV_ANGLE_GRADATIONS);

const GLdouble SolarSystem::NEAR_CLIP(0.5f);
const GLdouble SolarSystem::FAR_CLIP(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS * 2.5f);
const int SolarSystem::FAR_LOOK_DISTANCE(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS*SolarSystem::FAR_LOOK_RATIO);

SolarSystem::SolarSystem(string sharedMemoryAreaIdentifier) :
    AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
    /**
     * Parameters and State info
     */
    sim_instance = NULL;
    spaceFontResource = NULL;
    renderUpdateFlag = false;
    mouse_wheel_differential = 0;
    fov_angle = FOV_ANGLE_INITIAL;
    }

SolarSystem::~SolarSystem() {
    }

/**
 * Window resize/remap. This is not directly called from WindowManager
 * but from initialize().
 */
void SolarSystem::resize(const int width, const int height) {
    // Store current settings
    m_CurrentWidth = width;
    m_CurrentHeight = height;
    aspect = (float)width / (float)height;

    // Adjust aspect ratio and projection.
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // The given field-of-view angle is along the vertical axis, so also
    // applying the given aspect ratio then implicitly determines
    // field-of-view across the horizontal axis.
    gluPerspective(fov_angle, aspect, NEAR_CLIP, FAR_CLIP);
    glMatrixMode(GL_MODELVIEW);

    // Tell the underlying simulation that it's window
    // has been resized and/or field of view angle altered.
    sim_instance->resize(static_cast<GLuint>(width), static_cast<GLuint>(height));
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
    /// Check whether we initialize the first time or have to recycle (required for windoze)
    if(recycle == false) {
        // This is the first call of this routine from main().
        if(font != NULL) {
            // So remember the font resource pointer, if non NULL.
            spaceFontResource = font;
            // Then inform the simulation of that faont choice.
            sim_instance->setFonts(spaceFontResource);
            }
        else {
            // So we are here because there was no
            // font Resource assigned. Make it a FATAL !
            ErrorHandler::record("SolarSystem::initialize() : Warning: font resource still unknown! You might want to recycle at a later stage...",
                                 ErrorHandler::FATAL);
            }
        // initialize the BOINC client adapter
        m_BoincAdapter.initialize();
        }
    else {
        // This is the recurrent call of this routine from WindowManager.
        // Seems that Windoze also "resets" our OpenGL fonts, so
        // let's clean up before reinitializing them. The reason for the
        // loss of font pointer validity is that when a window is merely
        // resized on Windows machines ( for any reason ) then the entire
        // OpenGL context is lost ie. the settings of the OpenGL state
        // machine plus a 'canvas' to render to. This includes all OpenGL
        // display lists which are relied upon by the OGLFT implementation.
        // Not an issue for Linux or Mac.
#ifdef WIN_OGLFT_WORKAROUND


        sim_instance->setFonts(spaceFontResource);
#endif
        }

    // Specify tight unpacking of pixel data ( from data buffers )
    // ie. no padding generated. Default is 4 byte boundaries.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#if defined( GL_RASTER_POSITION_UNCLIPPED_IBM )
    // To allow bitmap/character data to partially display at screen edges.
    // Otherwise even a mere one pixel breach over the screen boundary
    // could cause failure to display an entire image.
    glEnable(GL_RASTER_POSITION_UNCLIPPED_IBM);
#endif

    // Black background.
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Allow polygon face culling.
    glEnable(GL_CULL_FACE);

    // Define the 'front' face of a polygon to be that which has been
    // constructed with a counterclockwise winding sense implied by
    // it's vertex listings.
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

    glDisable(GL_CLIP_PLANE0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    sim_instance->activate();

    // Setup dimensions.
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
    sim_instance->step();

    // Where are we etc .... in our virtual world?
    const CameraState& cam = sim_instance->viewPoint();

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
    sim_instance->draw();

    // Check for and report any errors generated.
    ErrorHandler::check_OpenGL_Error();

    // Switch buffers.
    glfwSwapBuffers();

    // Synchronise changes in render quality, affecting state machine
    // major settings, only upon completion of a frame.
    if(renderUpdateFlag == true) {
        changeRenderQuality();
        renderUpdateFlag = false;
        }
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

void SolarSystem::mouseWheelEvent(const int pos) {
    // Store the differential mouse wheel scroll value.
    mouse_wheel_differential = pos;

    // If we have rolled the mouse wheel forwards then zoom IN
    // ie. smaller angular size to field of view.
    if(pos > 0) {
        fov_angle -= FOV_ANGLE_DIFFERENTIAL;
        // Respect lower bounds.
        if(fov_angle < FOV_ANGLE_MIN){
            fov_angle = FOV_ANGLE_MIN;
            }
        }

    // If we have rolled the mouse wheel backwards then zoom OUT
    // ie. greater angular size to field of view.
    if(pos < 0) {
        fov_angle += FOV_ANGLE_DIFFERENTIAL;
        // Respect upper bounds.
        if(fov_angle > FOV_ANGLE_MAX) {
            fov_angle = FOV_ANGLE_MAX;
            }
        }

    // So even though we aren't altering the dimensions
    // of the rendering surface, we are changing the
    // field of view.
    resize(m_CurrentWidth, m_CurrentHeight);
    }

void SolarSystem::keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed) {
    switch(keyPressed) {
        case KeyC:
            sim_instance->moveRequest(Craft::REVERSE);
            break;
        case KeyD:
            sim_instance->moveRequest(Craft::STOP_TRANSLATION);
            break;
        case KeyE:
            sim_instance->moveRequest(Craft::FORWARD);
            break;
        case KeyF:
            sim_instance->moveRequest(Craft::RIGHTWARDS);
            break;
        case KeyG:
            sim_instance->moveRequest(Craft::GO_HOME);
            break;
        case KeyK:
            sim_instance->moveRequest(Craft::ROLL_LEFT);
            break;
        case KeyL:
            sim_instance->moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyO:
            sim_instance->moveRequest(Craft::PITCH_DOWN);
            break;
        case KeyR:
            sim_instance->moveRequest(Craft::UPWARDS);
            break;
        case KeyS:
            sim_instance->moveRequest(Craft::LEFTWARDS);
            break;
        case KeyV:
            sim_instance->moveRequest(Craft::DOWNWARDS);
            break;
        case KeyF1:
            sim_instance->cycle(SolarSystemGlobals::HELP_HUD);
            break;
        case KeyF2:
            // Cycle the rendering level.
            {
            SolarSystemGlobals::render_quality qual = SolarSystemGlobals::getRenderLevel();
            if(qual == SolarSystemGlobals::RENDER_HIGHEST) {
                SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_LOWEST);
                }
            if(qual == SolarSystemGlobals::RENDER_MEDIUM) {
                SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_HIGHEST);
                }
            if(qual == SolarSystemGlobals::RENDER_LOWEST) {
                SolarSystemGlobals::setRenderLevel(SolarSystemGlobals::RENDER_MEDIUM);
                }
            }
            break;
        case KeyF4:
            sim_instance->cycle(SolarSystemGlobals::HUDOVER);
            break;
        case KeyF5:
            sim_instance->cycle(SolarSystemGlobals::CONSTELLATIONS);
            break;
        case KeyF6:
            sim_instance->cycle(SolarSystemGlobals::PULSARS);
            break;
        case KeyF7:
            sim_instance->cycle(SolarSystemGlobals::SUPERNOVAE);
            break;
        case KeyF8:
            sim_instance->cycle(SolarSystemGlobals::SKY_GRID);
            break;
        case KeyF9:
            sim_instance->cycle(SolarSystemGlobals::EARTH_GRID);
            break;
        case KeyF12:
            // Toggle the state of the autopilot
            sim_instance->cycle(SolarSystemGlobals::AUTOPILOT);
            break;
        case KeySpace:
            // Whoa! Stop everything happening ...
            sim_instance->moveRequest(Craft::STOP_TRANSLATION);
            sim_instance->moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyKP1:
            sim_instance->moveRequest(Craft::YAW_LEFT);
            break;
        case KeyKP2:
            sim_instance->moveRequest(Craft::PITCH_UP);
            break;
        case KeyKP3:
            sim_instance->moveRequest(Craft::YAW_RIGHT);
            break;
        case KeyKP4:
            sim_instance->moveRequest(Craft::ROLL_LEFT);
            break;
        case KeyKP5:
            sim_instance->moveRequest(Craft::STOP_ROTATION);
            break;
        case KeyKP6:
            sim_instance->moveRequest(Craft::ROLL_RIGHT);
            break;
        case KeyKP8:
            sim_instance->moveRequest(Craft::PITCH_DOWN);
            break;
        case KeyComma:
            sim_instance->moveRequest(Craft::YAW_LEFT);
            break;
        case KeyPeriod:
            sim_instance->moveRequest(Craft::PITCH_UP);
            break;
        case KeyForwardSlash:
            sim_instance->moveRequest(Craft::YAW_RIGHT);
            break;
        case KeySemiColon:
            sim_instance->moveRequest(Craft::ROLL_RIGHT);
            break;
        default:
            break;
        }
    }

void SolarSystem::refreshLocalBOINCInformation() {
    }

void SolarSystem::renderQualityUpdate() {
    // While this update signal is asynchronously received,
    // store this for later use when a frame render is complete.
    renderUpdateFlag = true;
    }

void SolarSystem::changeRenderQuality(void) {
    // Put overall rendering quality decisions here.
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
