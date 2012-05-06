/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   As amended 2012 by Mike Hewson                                        *
 *   hewsmike[AT]iinet.net.au                                              *
 *                                                                         *
 * This file is part of Einstein@Home.                                     *
 *                                                                         *
 * Einstein@Home is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published       *
 * by the Free Software Foundation, version 2 of the License.              *
 *                                                                         *
 * Einstein@Home is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#include "WindowManager.h"

#include <sstream>
#include <string>

#include "ErrorHandler.h"
#include "Events.h"
#include "SolarSystemGlobals.h"

int WindowManager::OPEN_GL_VERSION_MINIMUM_MAJOR(1);
int WindowManager::OPEN_GL_VERSION_MINIMUM_MINOR(5);
int WindowManager::DEPTH_BUFFER_GRAIN(24);
int WindowManager::DEPTH_BUFFER_GRAIN_FALLBACK(16);
int WindowManager::NO_STENCIL(0);

WindowManager::WindowManager(void) {
   m_ScreensaverMode = false;
   isFullScreenMode = false;
   best_depth_buffer_grain = DEPTH_BUFFER_GRAIN;
   m_BoincAdapter = new BOINCClientAdapter("");
   }

WindowManager::~WindowManager() {
   delete m_BoincAdapter;
   }

bool WindowManager::initialize(const int width, const int height, const int frameRate) {
   if(glfwInit() == GL_FALSE) {
      ErrorHandler::record("WindowManager::initialize() : Window system could not be initalized - GLFW init fail", ErrorHandler::WARN);
      return false;
      }
   else {
   	int major = 0;
      int minor = 0;
      int revision = 0;

      glfwGetVersion(&major, &minor, &revision);
      std::stringstream msg;
      msg << "WindowManager::initialize() : Using GLfW version "
      	 << major
			 << "."
			 << minor
			 << "."
			 << revision;
      ErrorHandler::record(msg.str(), ErrorHandler::INFORM);
      }

   atexit(glfwTerminate);

   // Retrieve current video hardware settings for the user's desktop.
   glfwGetDesktopMode(&current_desktop_mode);

   m_DesktopWidth = current_desktop_mode.Width;
   m_DesktopHeight = current_desktop_mode.Height;
   m_DesktopBitsPerPixel = current_desktop_mode.RedBits +
                           current_desktop_mode.GreenBits +
                           current_desktop_mode.BlueBits;

   // get initial non-fullscreen resolution and frame rate from project preferences
   m_BoincAdapter->initialize();
   int preferredWidth = m_BoincAdapter->graphicsWindowWidth();
   int preferredHeight = m_BoincAdapter->graphicsWindowHeight();
   int preferredFrameRate = m_BoincAdapter->graphicsFrameRate();

   // override optional given default values if preferred values are set
   m_WindowedWidth = (preferredWidth != 0) ? preferredWidth : width;
   m_WindowedHeight = (preferredHeight != 0) ? preferredHeight : height;
   m_RenderEventInterval = 1000.0f / ((preferredFrameRate != 0) ? preferredFrameRate : frameRate);

   // Find available video modes for the current hardware.
   getVideoModes();

   // Form structure for fullscreen mode.
   GLFWvidmode fullscreen;
   fullscreen.Width = m_DesktopWidth;
   fullscreen.Height = m_DesktopHeight;
   fullscreen.RedBits = current_desktop_mode.RedBits;
   fullscreen.GreenBits = current_desktop_mode.GreenBits;
   fullscreen.BlueBits = current_desktop_mode.BlueBits;

   // check fullscreen video mode.
   m_FullscreenModeAvailable = false;
   // TODO - write case(s) for close matches
   if(matchVideoMode(fullscreen) == WindowManager::MATCH_EXACT) {
      m_FullscreenModeAvailable = true;
      }
   else {
      ErrorHandler::record("WindowManager::initialize() : Fullscreen video mode not supported", ErrorHandler::WARN);
      }

   // Form structure for windowed mode.
   GLFWvidmode windowed;
   windowed.Width = m_WindowedWidth;
   windowed.Height = m_WindowedHeight;
   windowed.RedBits = current_desktop_mode.RedBits;
   windowed.GreenBits = current_desktop_mode.GreenBits;
   windowed.BlueBits = current_desktop_mode.BlueBits;

   // check initial windowed video mode
   m_WindowedModeAvailable = false;
   // TODO - write case(s) for close matches
   if(matchVideoMode(windowed) == WindowManager::MATCH_EXACT) {
      m_WindowedModeAvailable = true;
      }
   else {
      ErrorHandler::record("WindowManager::initialize() : Windowed video mode not supported", ErrorHandler::WARN);
      }

   // Both checks failed
   // if(!m_FullscreenModeAvailable || !m_WindowedModeAvailable) {
   if(false) {
      ErrorHandler::record("WindowManager::initialize() : No suitable video mode available", ErrorHandler::WARN);
      return false;
      }

   // Suggest our OpenGL version minimum.
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, OPEN_GL_VERSION_MINIMUM_MAJOR);
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, OPEN_GL_VERSION_MINIMUM_MINOR);

   // We always start in windowed mode as starting
	// in fullscreen fails with high CPU load.
   m_CurrentWidth = m_WindowedWidth;
   m_CurrentHeight = m_WindowedHeight;
   isFullScreenMode = false;

   int window_open = glfwOpenWindow(m_CurrentWidth, m_CurrentHeight,
                                    current_desktop_mode.RedBits,
                                    current_desktop_mode.GreenBits,
                                    current_desktop_mode.BlueBits,
                                    current_desktop_mode.RedBits,			// Alpha range same as individual colors
                                    best_depth_buffer_grain,
                                    NO_STENCIL,
                                    GLFW_WINDOW);

	// If that didn't work, then maybe it was the depth buffer,
	// and try again with a lesser spec.
   if(window_open == GL_FALSE) {
		best_depth_buffer_grain = DEPTH_BUFFER_GRAIN_FALLBACK;
		int window_open = glfwOpenWindow(m_CurrentWidth, m_CurrentHeight,
                                    	current_desktop_mode.RedBits,
	                                    current_desktop_mode.GreenBits,
   	                                 current_desktop_mode.BlueBits,
      	                              current_desktop_mode.RedBits,    // Alpha range same as individual colors
         	                           best_depth_buffer_grain,
            	                        NO_STENCIL,
               	                     GLFW_WINDOW);

		// Did that work?
      if(window_open == GL_FALSE) {
         ErrorHandler::record("WindowManager::initialize() : Could not acquire rendering surface", ErrorHandler::WARN);
   	   return false;
			}
      }
      
   std::stringstream msg1;
   msg1 << "WindowManager::initialize() : Rendering surface acquired "
   	  << m_CurrentWidth
		  << " x "
		  << m_CurrentHeight
		  << " @ "
		  << (current_desktop_mode.RedBits + current_desktop_mode.GreenBits + current_desktop_mode.BlueBits)
		  << " colors with a "
		  << best_depth_buffer_grain
		  << " bit depth buffer";
	ErrorHandler::record(msg1.str(), ErrorHandler::INFORM);

	glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK) {
      ErrorHandler::record("WindowManager::initialize() : Window system could not be initalized - GLEW init fail", ErrorHandler::WARN);
      return false;
      }
   else {
      std::string msg2 = "WindowManager::initialize() : Using GLEW version ";
      msg2 += SolarSystemGlobals::convertGLstring(glewGetString(GLEW_VERSION));
		ErrorHandler::record(msg2, ErrorHandler::INFORM);
      }

   // Manage Windows OpenGL backwards compatibility issue.
#ifdef WIN_OGL_WORKAROUND
   if(setOGLContext() == true) {
   	// Good to go.
   	ErrorHandler::record("WindowManager::initialize() : acquired correct OpenGL backward compatible version", ErrorHandler::INFORM);
   	// Find out the OpenGL version.
   	GLuint major = 0;
   	GLuint minor = 0;
   	SolarSystemGlobals::getOGLVersion(&major, &minor);
   	std::stringstream msg1;
   	msg1 << "WindowManager::initialize() : OpenGL version = "
   		  << major << '.' << minor;
   	ErrorHandler::record(msg1.str(), ErrorHandler::INFORM);
		}
   else {
   	ErrorHandler::record("WindowManager::initialize() : could not acquire correct OpenGL backward compatible version", ErrorHandler::INFORM);
   	return false;
		}
#endif

   // This being the first call to this routine will have the effect
   // of establishing the event queue/callbacks, plus set the
  	// render timing.
   Events::Instance(m_RenderEventInterval);

   return true;
   }

void WindowManager::eventLoop(void) {
   // Provided we have at least one observer.
   if(!eventObservers.empty()) {
      // Infinite looping until an exit is triggered.
      while(true) {
         // Holder of event type.
         Event current_event;

         // Keep our special timers ticking over.
         Events::Instance(0)->tick();

         // Remember whether we have performed a render callback or
         // a BOINC callback for this queue inspection.
         bool boinc_update_invoked = false;
         bool render_invoked = false;

         // Keep extracting any events from the queue, until it is empty.
         // Events are gathered 'behind the scenes' from input devices .....
         // and placed in a 'queue of next available events'
         while(Events::Instance(0)->next(&current_event)) {
            // Only one render invocation per queue inspection,
				// lest any delay during rendering causes render event
				// requests to pile up.
            if((current_event.type == Events::RenderEventType) &&
               (render_invoked == false)) {
               // notify our observers (currently exactly one, hence front())
               eventObservers.front()->render(dtime());
               render_invoked = true;
               }

            // Only one BOINC update invocation per queue inspection,
				// lest any delay during rendering causes BOINC update event
				// requests to pile up.
            else if((current_event.type == Events::BOINCUpdateEventType) &&
                    (boinc_update_invoked == false)) {
               // notify observers (currently exactly one, hence front()) to fetch a BOINC update
               eventObservers.front()->refreshBOINCInformation();
               boinc_update_invoked = true;
               }

            // Check for any user input if in screensaver mode.
            else if(m_ScreensaverMode &&
                    ((current_event.type == Events::MouseMotionEventType) ||
                     (current_event.type == Events::MouseButtonEventType) ||
                     (current_event.type == Events::CharInputEventType) ||
                     (current_event.type == Events::KeyPressEventType))) {
               // Close window, terminate GLFW and leave this window manager.
               glfwTerminate();
               return;
               }

            else if((current_event.type == Events::MouseMotionEventType) &&
                    (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
                  // notify our observer of mouse movement with left button pressed down.
                  eventObservers.front()->mouseMoveEvent(current_event.m_motion.xrel,
                                                         current_event.m_motion.yrel,
                                                         AbstractGraphicsEngine::MouseButtonLeft);
                  }

            else if((current_event.type == Events::MouseMotionEventType) &&
                    (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) {
                  // notify our observers of mouse movement with right button pressed down.
                  eventObservers.front()->mouseMoveEvent(current_event.m_motion.xrel,
                                                         current_event.m_motion.yrel,
                                                         AbstractGraphicsEngine::MouseButtonRight);
                  }

            else if(current_event.type == Events::ResizeEventType) {
               m_CurrentWidth = m_WindowedWidth = current_event.resize.width;
               m_CurrentHeight = m_WindowedHeight = current_event.resize.height;

					int window_open = glfwOpenWindow(m_CurrentWidth, m_CurrentHeight,
                                    				current_desktop_mode.RedBits,
				                                    current_desktop_mode.GreenBits,
            				                        current_desktop_mode.BlueBits,
                        				            current_desktop_mode.RedBits,          // Alpha range same as individual colors
                                    				best_depth_buffer_grain,
				                                    NO_STENCIL,
            				                        GLFW_WINDOW);

   				if(window_open == GL_FALSE) {
      				std::cerr << "WindowManager::eventLoop() : Could not resize window !" << std::endl;
                  }
               else {
               	// Need to re-initialise GLEW after we acquire a context to apply it to.
   					if(glewInit() != GLEW_OK) {
      					ErrorHandler::record("WindowManager::initialize() : Window system could not be initalized - GLEW init fail", ErrorHandler::WARN);
      					return;
      					}
   					else {
      					std::string msg = "WindowManager::initialize() : Using GLEW version ";
      					msg += SolarSystemGlobals::convertGLstring(glewGetString(GLEW_VERSION));
							ErrorHandler::record(msg, ErrorHandler::INFORM);
      					}
						}

               // notify our observers (currently exactly one, hence front())
               // (windoze needs to be reinitialized instead of just resized, oh well)
               /// \todo Can we determine the host OS? On X11 a resize() is sufficient!
               eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
               }

               else if((current_event.type == Events::QuitEventType) ||
                      ((current_event.type == Events::KeyPressEventType) &&
                       (current_event.k_press.pressed == true) &&
                       (current_event.k_press.key_code == GLFW_KEY_ESC))) {
               // Close window, terminate GLFW and leave this window manager.
               glfwTerminate();
               return;
               }

            // Process printable character input.
            else if((current_event.type == Events::CharInputEventType) &&
                    (current_event.c_input.pressed == true)) {
					// Note : we account for upper and lower case characters from the same key.
               switch(current_event.c_input.char_code) {
                  // notify our observers (currently exactly one, hence front())
                  case ',':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyComma);
                     break;
                  case '.':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyPeriod);
                     break;
                  case '/':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyForwardSlash);
                     break;
                  case ';':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySemiColon);
                     break;
                  case 'a':
                  case 'A':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyA);
                     break;
                  case 'b':
                  case 'B':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyB);
                     break;
                  case 'c':
                  case 'C':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyC);
                     break;
                  case 'd':
                  case 'D':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyD);
                     break;
                  case 'e':
                  case 'E':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyE);
                     break;
                  case 'f':
                  case 'F':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF);
                     break;
                  case 'g':
                  case 'G':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyG);
                     break;
                  case 'h':
                  case 'H':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyH);
                     break;
                  case 'i':
                  case 'I':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyI);
                     break;
                  case 'l':
                  case 'L':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyL);
                     break;
                  case 'm':
                  case 'M':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyM);
                     break;
                  case 'n':
                  case 'N':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyN);
                     break;
                  case 'o':
                  case 'O':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyO);
                     break;
                  case 'p':
                  case 'P':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyP);
                     break;
                  case 'q':
                  case 'Q':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyQ);
                     break;
                  case 'r':
                  case 'R':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyR);
                     break;
                  case 's':
                  case 'S':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyS);
                     break;
                  case 't':
                  case 'T':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyT);
                     break;
                  case 'v':
                  case 'V':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyV);
                     break;
                  case 'w':
                  case 'W':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyW);
                     break;
                  case 'x':
                  case 'X':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyX);
                     break;
                  case 'z':
                  case 'Z':
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyZ);
                     break;
                  default:
                     break;
                  }
               }

            // Process non-printable keypresses.
            else if((current_event.type == Events::KeyPressEventType) &&
                    (current_event.k_press.pressed == true)) {
               switch(current_event.k_press.key_code) {
                  // notify our observers (currently exactly one, hence front())
                  case GLFW_KEY_F1:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF1);
                     break;
                  case GLFW_KEY_F2:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF2);
                     break;
                  case GLFW_KEY_F3:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF3);
                     break;
                  case GLFW_KEY_F4:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF4);
                     break;
                  case GLFW_KEY_F5:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF5);
                     break;
                  case GLFW_KEY_F6:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF6);
                     break;
                  case GLFW_KEY_F7:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF7);
                     break;
                  case GLFW_KEY_F8:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF8);
                     break;
                  case GLFW_KEY_F9:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF9);
                     break;
                  case GLFW_KEY_F10:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF10);
                     break;
                  case GLFW_KEY_F11:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF11);
                     break;
                  case GLFW_KEY_F12:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF12);
                     break;
                  case GLFW_KEY_SPACE:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySpace);
                     break;
                  case GLFW_KEY_ENTER:
                     toggleFullscreen();
                     break;
                  case GLFW_KEY_KP_2:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP2);
                     break;
                  case GLFW_KEY_KP_4:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP4);
                     break;
                  case GLFW_KEY_KP_5:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP5);
                     break;
                  case GLFW_KEY_KP_6:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP6);
                     break;
                  case GLFW_KEY_KP_8:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP8);
                     break;
                  case GLFW_KEY_KP_0:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP0);
                     break;
                  case GLFW_KEY_KP_DECIMAL:
                     eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKPPeriod);
                     break;
                  default:
                     break;
                  }
               }
            }
         }
      }
   else {
      // This is a logic error, as we shoulda/woulda/coulda have an observer by now !!
      ErrorHandler::record("WindowManager::eventLoop() : no event observer present", ErrorHandler::FATAL);
      }
   }

void WindowManager::registerEventObserver(AbstractGraphicsEngine *engine) {
   // We're only accepting/using ONE observer
   // The 'assign' member function on an std::list object at position
   // 1 will delete all content up to position 1 and then overwrites.
   // Thus list now has one item only.
   eventObservers.assign(1, engine);
   }

void WindowManager::unregisterEventObserver(AbstractGraphicsEngine *engine) {
   // The 'remove' member function on an std::list object with a given
	// value of item will remove said item. List will now be empty.
   eventObservers.remove(engine);
   }

int WindowManager::windowWidth(void) const {
   return m_CurrentWidth;
   }

int WindowManager::windowHeight(void) const {
   return m_CurrentHeight;
   }

void WindowManager::setWindowCaption(const string caption) const {
   glfwSetWindowTitle(caption.c_str());
   }

void WindowManager::setWindowIcon(const string filename) const {
   //if (filename.length() > 0) {
   //      SDL_WM_SetIcon(SDL_LoadBMP(filename.c_str()), NULL);
   //      }
   }

void WindowManager::setWindowIcon(const unsigned char *data, const int size) const {
   //// prepare data buffer structure
//   SDL_RWops *buffer = SDL_RWFromMem((void*) data, size);
//
//   if(buffer != NULL) {
//      // load BMP from prepared data buffer
//      SDL_Surface *surface = SDL_LoadBMP_RW(buffer, 1);
//
//      if(surface != NULL) {
//         // set window icon
//         SDL_WM_SetIcon(surface, NULL);
//         SDL_FreeSurface(surface);
//         }
//      else {
//         cerr << "Could not create window icon surface: " << SDL_GetError() << endl;
//         }
//      }
//   else {
//      cerr << "Could not prepare window icon data: " << SDL_GetError() << endl;
//      }
   }

void WindowManager::toggleFullscreen(void) {
	bool canTransition = false;
	GLenum desiredMode;
	
	if(isFullScreenMode == true) {
		// Want a transition to windowed mode.
		if(m_WindowedModeAvailable == true) {
         canTransition = true;
         
         // Set dimensions for whatever was the previous windowed mode.
	      m_CurrentWidth = m_WindowedWidth;
   	   m_CurrentHeight = m_WindowedHeight;

        	// show cursor in fullscreen mode
      	glfwEnable(GLFW_MOUSE_CURSOR);
      	desiredMode = GLFW_WINDOW;
         }
		else {
			// Alas a windowed mode is not possible.
         ErrorHandler::record("WindowManager::toggleFullscreen() : can't transition to windowed mode", ErrorHandler::WARN);
			}
		}
	else {
		// Want a transition to fullscreen mode.
		if(m_FullscreenModeAvailable == true) {
		   canTransition = true;
		   
		   // Set dimensions of fullscreen as those of user's desktop.
      	m_CurrentWidth = m_DesktopWidth;
      	m_CurrentHeight = m_DesktopHeight;

      	// hide cursor
      	glfwDisable(GLFW_MOUSE_CURSOR);
      	desiredMode = GLFW_FULLSCREEN;
		   }
		else {
			// Alas a fullscreen mode is not possible.
			ErrorHandler::record("WindowManager::toggleFullscreen() : can't transition to fullscreen mode", ErrorHandler::WARN);
			}
		}

	if(canTransition == true) {
      // reset video mode, noting that we retain the original discovered user's color depths.
      /// TODO - manage variants on depth buffer size.
  		int window_open = glfwOpenWindow(m_CurrentWidth, m_CurrentHeight,
                                    	current_desktop_mode.RedBits,
                                    	current_desktop_mode.GreenBits,
                                    	current_desktop_mode.BlueBits,
                                    	current_desktop_mode.RedBits,          // Alpha range same as individual colors
                                    	best_depth_buffer_grain,
                                    	NO_STENCIL,
                                    	desiredMode);

		// Did that request work?
   	if(window_open == GL_FALSE) {
			// It would be pretty screwy if we got here, so fail out.
         ErrorHandler::record("WindowManager::toggleFullscreen() : Could not acquire rendering surface", ErrorHandler::FATAL);
      	}
      else {
      	// Need to re-initialise GLEW after we acquire a context to apply it to.
      	// TODO - Keep glewExperimental=TRUE in mind ....
   		if(glewInit() != GLEW_OK) {
      		ErrorHandler::record("WindowManager::initialize() : Window system could not be initalized - GLEW init fail", ErrorHandler::WARN);
      		return;
      		}
   		else {
      		std::string msg = "WindowManager::initialize() : Using GLEW version ";
      		msg += SolarSystemGlobals::convertGLstring(glewGetString(GLEW_VERSION));
				ErrorHandler::record(msg, ErrorHandler::INFORM);
      		}
			}

		// All is good, so toggle state flag.
		isFullScreenMode = isFullScreenMode;
			
   	// notify our observers (currently exactly one, hence front())
   	// (windoze needs to be reinitialized instead of just resized, oh well)
   	/// \todo Can we determine the host OS? On X11 a resize() is sufficient!
   	// Check that the list of observers is non-empty, else we get undefined behaviour.
   	if(!eventObservers.empty()) {
      	eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
      	}
   	else {
      	// This is a logic error, we shoulda/woulda/coulda have an observer by now !!
      	ErrorHandler::record("WindowManager::toggleFullscreen() : no event observer present", ErrorHandler::FATAL);
      	}
		}
	}

void WindowManager::setScreensaverMode(const bool enabled) {
   m_ScreensaverMode = enabled;
   }

void WindowManager::getVideoModes(void) {
   // Yes, 100 is rather silly, but will definitely retrieve
	// all modes, and is compacted to a smaller list anyway.
   static int VIDEO_MODES_BUFFER_SIZE = 100;
   GLFWvidmode vidmodes[VIDEO_MODES_BUFFER_SIZE];

   // Request as many modes as we dare.
   int num_modes = glfwGetVideoModes(vidmodes, VIDEO_MODES_BUFFER_SIZE);
   if(num_modes == VIDEO_MODES_BUFFER_SIZE) {
      ErrorHandler::record("WindowManager::getVideoModes() : maximum number of video modes retrieved", ErrorHandler::WARN);
      }

   // Report mode number.
   std::stringstream msg;
   msg << "WindowManager::getVideoModes() : Number of video modes found = " << num_modes;
   ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

   // Store the lesser listing, if any.
   for(int mode = 0; mode < num_modes; ++mode) {
		int color_depth = vidmodes[mode].RedBits + vidmodes[mode].GreenBits + vidmodes[mode].BlueBits;
      std::stringstream msg;
      msg << "WindowManager::getVideoModes() : "
			 << vidmodes[mode].Width
			 << " x "
			 << vidmodes[mode].Height
			 << " @ "
			 << color_depth;
      ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

      video_modes.push_back(vidmodes[mode]);
      }
   }

int WindowManager::matchVideoMode(GLFWvidmode test_case) {
   // Assume failure.
   int retval = WindowManager::MATCH_NONE;

	// What is the total color depth in bits ?
   int desired_color_depth = test_case.RedBits + test_case.GreenBits + test_case.BlueBits;

   /// TODO - create code for nearest match case(s)

	// Search thru all the known video modes.
   for(std::vector<GLFWvidmode>::const_iterator vm = video_modes.begin();
       vm < video_modes.end();
       ++vm) {
		// The total color depth of this particular mode.
      int current_color_depth = vm->RedBits + vm->GreenBits + vm->BlueBits;
      // Do we match on color depth?
      if(current_color_depth == desired_color_depth) {
         // Yes, so look at width and height now.
         if((test_case.Width == vm->Width) &&
            (test_case.Height == vm->Height)){
            // All parameters match so exit with flag to indicate success.
            retval = WindowManager::MATCH_EXACT;
            break;
            }
         }
      }

   return retval;
   }

#ifdef WIN_OGL_WORKAROUND
bool WindowManager::setOGLContext(void) {
	// Assume failure.
	bool ret_val = false;

	// Determine OpenGL version.
	GLuint major = 0;
	GLuint minor = 0;
	SolarSystemGlobals::getOGLVersion(&major, &minor);
	std::stringstream msg;
	msg << "WindowManager::setOGLContext() : OpenGL v"
	  	 << OPEN_GL_VERSION_MINIMUM_MAJOR
	    << "."
		 << OPEN_GL_VERSION_MINIMUM_MINOR;
	ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

	// With v3.2+ then ...
	if((major > 3) ||
		((major = 3) && (minor >= 2))) {
		ErrorHandler::record("WindowManager::setOGLContext() : OpenGL v3.2+ version", ErrorHandler::INFORM);
		// ... try to establish a backwards compatible context.
		if(glewIsSupported("GL_ARB_compatibility")) {
			ErrorHandler::record("WindowManager::setOGLContext() : GL_ARB_compatibility supported", ErrorHandler::INFORM);
		   // Great, we have compatibility mode.
			// Get a handle to the device context.
			HDC hdc = wglGetCurrentDC();
			// Get a handle to the OpenGL context.
			HGLRC hglrc = wglGetCurrentContext();
			if(WGLEW_ARB_create_context_profile) {
				ErrorHandler::record("WindowManager::setOGLContext() : WGL_ARB_create_context_profile supported", ErrorHandler::INFORM);
				// Initialise attribute array for wgl.
				int attribList[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, OPEN_GL_VERSION_MINIMUM_MAJOR,
										  WGL_CONTEXT_MINOR_VERSION_ARB, OPEN_GL_VERSION_MINIMUM_MINOR,
										  0, 0};

				// Create new context from current.
				HGLRC new_context = wglCreateContextAttribsARB(hdc, hglrc, attribList);

				// Make this new context the current one.
				wglMakeCurrent(hdc, new_context);

				// Delete the old context.
				wglDeleteContext(hglrc);

				// Re-initialise GLEW.
				glewExperimental = GL_TRUE;
				if(glewInit() != GLEW_OK) {
					ErrorHandler::record("WindowManager::setOGLContext() : Window system could not be initalized - GLEW init fail", ErrorHandler::WARN);
				   }
				else {
					// Finally!!
					ErrorHandler::record("WindowManager::setOGLContext() : Window system could be initalized - GLEW init success", ErrorHandler::INFORM);
					ret_val = true;
					}
				}
			else {
				ErrorHandler::record("WindowManager::setOGLContext() : WGL_ARB_create_context_profile NOT supported", ErrorHandler::WARN);
				}
			}
		}
	else {
		// OK, so we're less than v3.2, are we at least the minumum ?
		if((major = OPEN_GL_VERSION_MINIMUM_MAJOR) && (minor >= OPEN_GL_VERSION_MINIMUM_MINOR)) {
			// Fine as is, no further effort required.
			ErrorHandler::record("WindowManager::setOGLContext() : satisfactory OpenGL version as is", ErrorHandler::INFORM);
			ret_val = true;
			}
		else {
			std::stringstream msg;
			msg << "WindowManager::setOGLContext() : OpenGL pre v"
			  	 << OPEN_GL_VERSION_MINIMUM_MAJOR
			    << "."
				 << OPEN_GL_VERSION_MINIMUM_MINOR
				 << " version";
			ErrorHandler::record(msg.str(), ErrorHandler::WARN);
			}
		}

	return ret_val;
	}
#endif
