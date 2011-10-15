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

#include "SolarSystem.h"

const int SolarSystem::FAR_LOOK_RATIO(1000);
const GLdouble SolarSystem::FOV_ANGLE(45.0f);
const GLdouble SolarSystem::NEAR_CLIP(0.5f);
const GLdouble SolarSystem::FAR_CLIP(Universe::CELESTIAL_SPHERE_RADIUS * 10);
const int SolarSystem::FAR_LOOK_DISTANCE(Universe::CELESTIAL_SPHERE_RADIUS*SolarSystem::FAR_LOOK_RATIO);

SolarSystem::SolarSystem(string sharedMemoryAreaIdentifier) :
	AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
   /**
	 * Parameters and State info
	 */
	}

SolarSystem::~SolarSystem() {
}

/**
 * Window resize/remap
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
}

/**
 *  What to do when graphics are "initialized".
 */
void SolarSystem::initialize(const int width, const int height, const Resource *font, const bool recycle) {
	// setup initial dimensions
	resize(width, height);

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

   // create pre-drawn display lists
   uni.activate();

	glDisable(GL_CLIP_PLANE0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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

   // Evolve the craft's mechanics.
   flyboy.step();

   // Where are we etc .... in our virtual world?
   const InertialPlatform& here = flyboy.get_platform();

   // Set up the camera position and orientation.
	gluLookAt(here.position().x(),
             here.position().y(),                  // eyes position
             here.position().z(),
				 SolarSystem::FAR_LOOK_DISTANCE*here.look().x(),
             SolarSystem::FAR_LOOK_DISTANCE*here.look().y(),    // looking towards here
             SolarSystem::FAR_LOOK_DISTANCE*here.look().z(),
				 here.up().x(),
             here.up().y(),                        // which way is up?
             here.up().z());

   uni.draw();

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
         flyboy.forward_thrust();
         message += "keyboard A : forward thrust";
         break;
      case KeyB:
			// Thrust down.
         flyboy.down_thrust();
         message += "keyboard B : downwards thrust";
         break;
      case KeyC:
			// Toggle the state of constellation display.
			// uni.toggle_feature(CelestialSphere::);
         message += "keyboard C : unassigned";
         break;
      case KeyD:
			// Pull nose up.
			flyboy.nose_up();
         message += "keyboard D : nose up";
         break;
      case KeyE:
			// Push nose down.
			flyboy.nose_down();
         message += "keyboard E : nose down";
         break;
      case KeyF:
			// Roll to the right.
         flyboy.roll_right();
         message += "keyboard F : roll right";
         break;
      case KeyG:
			// Go home.
         flyboy.go_home();
         message += "keyboard G : go home";
         break;
      case KeyH:
         message += "keyboard H : unassigned";
         break;
      case KeyM:
         // Toggle the state of axes display.
         // uni.toggle_axes();
         message += "keyboard M : toggle axes";
         break;
      case KeyN:
         message += "keyboard N : unassigned";
         break;
		case KeyP:
			// Toggle the state of pulsar display.
			message += "keyboard P : unassigned";
         break;
      case KeyQ:
			// Stop.
			flyboy.stop();
         message += "keyboard Q : stop";
         break;
      case KeyR:
			// Thrust to the right.
			flyboy.right_thrust();
         message += "keyboard R : right thrust";
         break;
		case KeyS:
			// Roll to the left.
         flyboy.roll_left();
         message += "keyboard S : roll left";
         break;
      case KeyT:
			// Thrust up.
         flyboy.up_thrust();
         message += "keyboard T : upwards thrust";
         break;
      case KeyV:
			// Yaw right.
         flyboy.yaw_right();
         message += "keyboard V : yaw right";
         break;
      case KeyW:
			// Thrust to the left.
			flyboy.left_thrust();
         message += "keyboard W : left thrust";
         break;
      case KeyX:
			// Yaw left.
         flyboy.yaw_left();
         message += "keyboard X : yaw left";
         break;
      case KeyZ:
			// Reverse thrust.
         flyboy.reverse_thrust();
         message += "keyboard Z : reverse thrust";
         break;
      case KeyF1:
         message += "keyboard F1 : unassigned";
         break;
      case KeyF2:
         // Set the overall rendering level to lowest.
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_LOWEST);
         message += "keyboard F2 : set render level to LOWEST";
         break;
      case KeyF3:
         // Set the overall rendering level to medium.
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_MEDIUM);
         message += "keyboard F3 : set render level to MEDIUM";
         break;
      case KeyF4:
         // Set the overall rendering level to highest.
         SolarSystemGlobals::set_render_level(SolarSystemGlobals::RENDER_HIGHEST);
         message += "keyboard F4 : set render level to HIGHEST";
         break;
      case KeyF5:
       	// Toggle the state of constellation display.
			uni.toggle(Universe::CONSTELLATIONS);
         message += "keyboard F5 : toggle constellations";
         break;
      case KeyF6:
         // Toggle the state of pulsar display.
			uni.toggle(Universe::PULSARS);
         message += "keyboard F6 : toggle pulsars";
         break;
      case KeyF7:
         // Toggle the state of pulsar display.
			uni.toggle(Universe::SUPERNOVAE);
         message += "keyboard F7 : toggle supernovae";
         break;
      case KeyF8:
         // Toggle the state of celestial sphere grid display.
			uni.toggle(Universe::GRID);
         message += "keyboard F8 : toggle grid";
         break;
		default:
      	break;
	   }
   ErrorHandler::record(message, ErrorHandler::INFORM);
	}

void SolarSystem::refreshLocalBOINCInformation() {
   }
