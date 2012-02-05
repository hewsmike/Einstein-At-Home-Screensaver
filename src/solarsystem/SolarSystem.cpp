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

#include "SolarSystem.h"

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
         // Remember the font resource pointer if non NULL
         spaceFontResource = font;
         }
      }
   else {
      // This is the recurrent call of this routine from WindowManager.
      // Seems that windoze also "resets" our OpenGL fonts, so
      // let's clean up before reinitializing them.
      if(gridFont != NULL) {
         delete gridFont;
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
      gridFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                              spaceFontResource->data()->size(),
                              13, 78 );

      // Note short-circuit evaluation relevant in this if clause ie. right side
      // expression is evaluated only if left side expression is false. Matters
      // for pointer dereference so don't swap order of expressions here.
      if(gridFont == NULL || (gridFont->isValid() == false)) {
         // TODO - better error path
         std::string msg = "SolarSystem::initialize() - Could not construct grid font face from in memory resource!";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         }
      gridFont->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
      gridFont->setForegroundColor(1.0f, 1.0f, 1.0f, 0.6f);

      // create font instance using font resource (base address + size)
      constellationFont = new OGLFT_ft(&spaceFontResource->data()->at(0),
                                       spaceFontResource->data()->size(),
                                       13, 78 );

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
                             13, 78 );

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
   sim.setFont(Simulation::GRID, gridFont);
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
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_LOWEST);
         break;
      case BOINCClientAdapter::MediumGraphicsQualitySetting :
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_MEDIUM);
         // fog aids depth perception
         glEnable(GL_FOG);
         glFogi(GL_FOG_MODE, GL_EXP2);
         glFogf(GL_FOG_DENSITY, 0.085);
         glHint(GL_FOG_HINT, GL_DONT_CARE);
         break;
      case BOINCClientAdapter::HighGraphicsQualitySetting :
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_HIGHEST);
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

   SolarSystemGlobals::check_OpenGL_Error();
   }

/**
 * Rendering routine:  this is what does the drawing:
 */
void SolarSystem::render(const double tOD) {
   // Clear the canvas plus set z ordering to the far-clip plane.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Set modelview matrix to unity.
   glLoadIdentity();

   // Evolve the simulation.
   sim.step();

   // Where are we etc .... in our virtual world?
   Vector3D view_position = sim.getViewPosition();
   Vector3D view_direction = sim.getViewDirection();
   Vector3D view_up = sim.getViewUp();

   // Set up the camera position and orientation.
   gluLookAt(view_position.x(),
             view_position.y(),                  // eyes position
             view_position.z(),
             SolarSystem::FAR_LOOK_DISTANCE*view_direction.x(),
             SolarSystem::FAR_LOOK_DISTANCE*view_direction.y(),    // looking towards here
             SolarSystem::FAR_LOOK_DISTANCE*view_direction.z(),
             view_up.x(),
             view_up.y(),                        // which way is up?
             view_up.z());

   // Render from the current viewpoint.
   sim.draw();

   // Check for and report any errors generated.
   SolarSystemGlobals::check_OpenGL_Error();

   // Switch frames.
   SDL_GL_SwapBuffers();
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
   std::string message = "SolarSystem::keyboardPressEvent() - ";
   switch(keyPressed) {
      case KeyA:
         // Thrust forwards.
         sim.moveRequest(SolarSystemGlobals::FORWARD);
         message += "KeyA : forward thrust";
         break;
      case KeyB:
         // Thrust down.
         sim.moveRequest(SolarSystemGlobals::DOWNWARDS);
         message += "KeyB : downwards thrust";
         break;
      case KeyC:
         message += "KeyC : unassigned";
         break;
      case KeyD:
         // Pull nose up.
         sim.moveRequest(SolarSystemGlobals::PITCH_UP);
         message += "KeyD : nose up";
         break;
      case KeyE:
         // Push nose down.
         sim.moveRequest(SolarSystemGlobals::PITCH_DOWN);
         message += "KeyE : nose down";
         break;
      case KeyF:
         // Roll to the right.
         sim.moveRequest(SolarSystemGlobals::ROLL_RIGHT);
         message += "KeyF : roll right";
         break;
      case KeyG:
         // Go home.
         sim.moveRequest(SolarSystemGlobals::GO_HOME);
         message += "KeyG : go home";
         break;
      case KeyH:
         // Toggle the HUD display.
         sim.cycle(Simulation::HUDOVER);
         message += "KeyH : toggle HUD";
         break;
      case KeyM:
         message += "KeyM : unassigned";
         break;
      case KeyN:
         message += "KeyN : unassigned";
         break;
      case KeyP:
         message += "KeyP : unassigned";
         break;
      case KeyQ:
         // Stop.
         sim.moveRequest(SolarSystemGlobals::STOP_TRANSLATION);
         message += "KeyQ : null translation";
         break;
      case KeyR:
         // Thrust to the right.
         sim.moveRequest(SolarSystemGlobals::RIGHTWARDS);
         message += "KeyR : right thrust";
         break;
      case KeyS:
         // Roll to the left.
         sim.moveRequest(SolarSystemGlobals::ROLL_LEFT);
         message += "KeyS : roll left";
         break;
      case KeyT:
         // Thrust up.
         sim.moveRequest(SolarSystemGlobals::UPWARDS);
         message += "KeyT : upwards thrust";
         break;
      case KeyV:
         // Yaw right.
         sim.moveRequest(SolarSystemGlobals::YAW_RIGHT);
         message += "KeyV : yaw right";
         break;
      case KeyW:
         // Thrust to the left.
         sim.moveRequest(SolarSystemGlobals::LEFTWARDS);
         message += "KeyW : left thrust";
         break;
      case KeyX:
         // Yaw left.
         sim.moveRequest(SolarSystemGlobals::YAW_LEFT);
         message += "KeyX : yaw left";
         break;
      case KeyZ:
         // Reverse thrust.
         sim.moveRequest(SolarSystemGlobals::REVERSE);
         message += "KeyZ : reverse thrust";
         break;
      case KeyF1:
         // Keep this for future 'help' functionality
         message += "KeyF1 : unassigned";
         break;
      case KeyF2:
         // Set the overall rendering level to lowest.
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_LOWEST);
         message += "KeyF2 : set render level to LOWEST";
         break;
      case KeyF3:
         // Set the overall rendering level to medium.
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_MEDIUM);
         message += "KeyF3 : set render level to MEDIUM";
         break;
      case KeyF4:
         message += "KeyF1 : unassigned";
         break;
      case KeyF5:
         // Cycle the state of constellation display.
         sim.cycle(Simulation::CONSTELLATIONS);
         message += "KeyF5 : cycle constellations";
         break;
      case KeyF6:
         // Cycle the state of pulsar display.
         sim.cycle(Simulation::PULSARS);
         message += "KeyF6 : cycle pulsars";
         break;
      case KeyF7:
         // cycle the state of supernovae display.
         sim.cycle(Simulation::SUPERNOVAE);
         message += "KeyF7 : cycle supernovae";
         break;
      case KeyF8:
         // Toggle the state of celestial sphere grid display.
         sim.cycle(Simulation::GRID);
         message += "KeyF8 : cycle grid";
         break;
      case KeySpace:
         // stop any craft rotation
         sim.moveRequest(SolarSystemGlobals::STOP_ROTATION);
         message += "KeySpace : null rotation";
         break;
      default:
         break;
      }
   ErrorHandler::record(message, ErrorHandler::INFORM);
   }

void SolarSystem::refreshLocalBOINCInformation() {
   }
