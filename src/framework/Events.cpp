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

#include "Events.h"

#include <sstream>

#include "ErrorHandler.h"

// Use minimum timing intervals in milliseconds for BOINC and render callbacks.
GLuint Events::BOINC_CALLBACK_INTERVAL(1000);
GLuint Events::RENDER_CALLBACK_INTERVAL_MIN(TriggerTimer::TRIGGER_MIN);

TriggerTimer* Events::boinc_timer(NULL);
TriggerTimer* Events::render_timer(NULL);
std::list<Event> Events::event_queue;

std::auto_ptr<Events> Events::p_instance;

Events::Events(void) {
	}

Events::Events(const Events&) {
	}

Events& Events::operator=(const Events&) {
	}
	
Events* Events::Instance(GLuint render_interval) {
	// If we have no instance, then make one and initialise it.
	if(Events::p_instance.get() == NULL) {
		Events::p_instance = std::auto_ptr<Events>(new Events());
		init(render_interval);
		}

	return Events::p_instance.get();
	}

void Events::init(GLuint render_interval) {
	// Empty the event queue.
	event_queue.clear();
	ErrorHandler::record("Events::Instance() : cleared event queue", ErrorHandler::INFORM);

	// Set GLFW global event behaviours.

	// Ensure keys are non-sticky.
	glfwDisable(GLFW_STICKY_KEYS);
	ErrorHandler::record("Events::Instance() : disabled sticky keys", ErrorHandler::INFORM);

	// Ensure keys do not repeat if held down.
	glfwDisable(GLFW_KEY_REPEAT);
	ErrorHandler::record("Events::Instance() : disabled key repeat", ErrorHandler::INFORM);

	// Ensure system key-strokes remain active.
	glfwEnable(GLFW_SYSTEM_KEYS);
	ErrorHandler::record("Events::Instance() : enabled system keys", ErrorHandler::INFORM);

	// Ensure mouse buttons are non-sticky.
	// Actually, I didn't know you could have sticky ones ... :-)
	glfwDisable(GLFW_STICKY_MOUSE_BUTTONS);
	ErrorHandler::record("Events::Instance() : disabled sticky mouse buttons", ErrorHandler::INFORM);

	// Non GLFW callbacks. See note below.

//	// Set the BOINC callback timing.
//	boinc_timer = new TriggerTimer(BOINC_CALLBACK_INTERVAL, boincUpdate);
//	ErrorHandler::record("Events::Instance() : create BOINC update timer and set callback", ErrorHandler::INFORM);

	// Set the render callback timing, but adhere to minimum.
	if(render_interval < RENDER_CALLBACK_INTERVAL_MIN){
		render_interval = RENDER_CALLBACK_INTERVAL_MIN;
		}
	render_timer = new TriggerTimer(render_interval, render);
	ErrorHandler::record("Events::Instance() : create rendering timer and set callback", ErrorHandler::INFORM);

	// Those GLFW callbacks below herewith defined are to 'ordinary' functions ie. not
	// part of any C++ class definition - not even static. The reason is that GLFW is
	// a C library, hence has no 'this' pointer and other OOP stuff, etc. The BOINC
	// and render callbacks mechanisms are of my own making, not GLFW library
	// linked and so such concerns are not relevant. An issue of language
	// calling conventions ultimately.

	// Set the keypress callback.
	glfwSetKeyCallback(keyPress);
	ErrorHandler::record("Events::Instance() : set keypress callback", ErrorHandler::INFORM);

	// Set the character input callback.
	glfwSetCharCallback(charInput);
	ErrorHandler::record("Events::Instance() : set character input callback", ErrorHandler::INFORM);

	// Set the mouse button callback.
	glfwSetMouseButtonCallback(mouseButton);
	ErrorHandler::record("Events::Instance() : set mouse button callback", ErrorHandler::INFORM);

	// Set the mouse movement callback.
	glfwSetMousePosCallback(mouseMotion);
	ErrorHandler::record("Events::Instance() : set mouse motion callback", ErrorHandler::INFORM);

	// Set the mouse wheel callback.
	glfwSetMouseWheelCallback(mouseWheel);
	ErrorHandler::record("Events::Instance() : set mouse wheel callback", ErrorHandler::INFORM);

	// Set the window resize callback.
	glfwSetWindowSizeCallback(resize);
	ErrorHandler::record("Events::Instance() : set window resize callback", ErrorHandler::INFORM);

	// Set the window quit callback.
	glfwSetWindowCloseCallback(quit);
	ErrorHandler::record("Events::Instance() : set window close callback", ErrorHandler::INFORM);

	// Finally GLFW will poll devices for events with every buffer swap.
	glfwEnable(GLFW_AUTO_POLL_EVENTS);
	ErrorHandler::record("Events::Instance() : enabled auto-polling", ErrorHandler::INFORM);
	}

bool Events::next(Event* ev) {
   // Assume failure.
   bool ret_val = false;

   // Assuming a non-NULL pointer has been passed.
   if(ev != NULL) {
      // If there is some event available.
      if(!event_queue.empty()) {
         // Copy the event to the calling routine.
         *ev = event_queue.front();

         // Remove this event from the queue.
         event_queue.pop_front();

         // Signal success.
         ret_val = true;
         }
      }

   return ret_val;
   }

void Events::tick(void) {
	boinc_timer->update();
   render_timer->update();
   glfwPollEvents();
	}

void Events::boincUpdate(void) {
	Event ev;
   ev.b_update.type = BOINCUpdateEventType;

   event_queue.push_back(ev);
   }
   
void Events::render(void) {
   Event ev;
   ev.render.type = RenderEventType;

   event_queue.push_back(ev);
   }

void GLFWCALL Events::charInput(int character, int action) {
	std::stringstream msg;
	msg << "Events::charInput() : character_code = "
		 << character
		 << " \taction = "
		 << action;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.c_input.type = Events::CharInputEventType;
   ev.c_input.char_code = character;

   // Default is key release.
   ev.c_input.pressed = false;
   if(action == GLFW_PRESS){
      // Otherwise key was pressed.
      ev.c_input.pressed = true;
      }

   event_queue.push_back(ev);
   }

void GLFWCALL Events::keyPress(int key, int action) {
	std::stringstream msg;
	msg << "Events::keyPress() : key_code = "
		 << key
		 << " \taction = "
		 << action;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.k_press.type = Events::KeyPressEventType;
   ev.k_press.key_code = key;

   // Default is key release.
   ev.k_press.pressed = false;
   if(action == GLFW_PRESS){
      // Otherwise key was pressed.
      ev.k_press.pressed = true;
      }

   event_queue.push_back(ev);
   }

void GLFWCALL Events::mouseButton(int button, int action) {
	std::stringstream msg;
	msg << "Events::mouseButton() : button_code = "
		 << button
		 << " \taction = "
		 << action;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.m_button.type = Events::MouseButtonEventType;

   event_queue.push_back(ev);
   }

void GLFWCALL Events::mouseMotion(int x, int y) {
	std::stringstream msg;
	msg << "Events::mouseMotion() : x_rel = "
		 << x
		 << " \ty_rel = "
		 << y;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.m_motion.type = Events::MouseMotionEventType;
   ev.m_motion.xrel = x;
   ev.m_motion.yrel = y;

   event_queue.push_back(ev);
   }

void GLFWCALL Events::mouseWheel(int pos) {
	std::stringstream msg;
	msg << "Events::mouseWheel() : position = "
		 << pos;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.m_wheel.type = Events::MouseWheelEventType;

   event_queue.push_back(ev);
   }

void GLFWCALL Events::resize(int width, int height) {
	std::stringstream msg;
	msg << "Events::resize() : width = "
		 << width
		 << " \theight = "
		 << height;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.resize.type = Events::ResizeEventType;
   ev.resize.width = width;
   ev.resize.height = height;

   event_queue.push_back(ev);
   }

int GLFWCALL Events::quit(void) {
	std::stringstream msg;
	msg << "Events::quit() : ";
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   Event ev;
   ev.quit.type = Events::QuitEventType;

   event_queue.push_back(ev);

   // Allow the window closure to continue ( GL_FALSE would prevent that )
   return GL_TRUE;
   }