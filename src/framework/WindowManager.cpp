/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   As amended 2013 by Mike Hewson                                        *
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

const std::string WindowManager::m_WindowTitle("Einstein At Home");

const int WindowManager::RED_BITS(8);
const int WindowManager::GREEN_BITS(8);
const int WindowManager::BLUE_BITS(8);
const int WindowManager::ALPHA_BITS(8);
const int WindowManager::DEPTH_BITS(16);
const int WindowManager::HAS_DEPTH_BUFFER(1);

const int WindowManager::OGL_MAJOR_VERSION(3);
const int WindowManager::OGL_MINOR_VERSION(2);

const int WindowManager::DISPLAY_ZERO(0);

WindowManager::WindowManager(void) {
    m_BoincAdapter = new BOINCClientAdapter("");
    }

WindowManager::~WindowManager() {
    delete m_BoincAdapter;
    }

bool WindowManager::initialize(const int width, const int height, const int frameRate) {
    /// TODO - check error return on this.
    SDL_GetDesktopDisplayMode(WindowManager::DISPLAY_ZERO, m_Mode);

    m_DesktopWidth = m_Mode->w;
    stringstream msg_init_current_desktop_mode_width;
    msg_init_current_desktop_mode_width << "WindowManager::initialize() : current desktop width = "
                                        << m_DesktopWidth;
    ErrorHandler::record(msg_init_current_desktop_mode_width.str(), ErrorHandler::INFORM);


    m_DesktopHeight = m_Mode->h;
    stringstream msg_init_current_desktop_mode_height;
    msg_init_current_desktop_mode_height << "WindowManager::initialize() : current desktop height = "
                                         << m_DesktopHeight;
    ErrorHandler::record(msg_init_current_desktop_mode_height.str(), ErrorHandler::INFORM);


    m_DesktopBitsPerPixel = SDL_BITSPERPIXEL(m_Mode->format);
    stringstream msg_init_current_desktop_bitsperpixel;
    msg_init_current_desktop_bitsperpixel << "WindowManager::initialize() : current desktop bits per pixel = "
                                          << m_DesktopBitsPerPixel;
    ErrorHandler::record(msg_init_current_desktop_bitsperpixel.str(), ErrorHandler::INFORM);

    // Get initial non-fullscreen resolution and frame rate from project preferences
    m_BoincAdapter->initialize();
    int preferredWidth = m_BoincAdapter->graphicsWindowWidth();
    stringstream msg_init_prefwidth;
    msg_init_prefwidth << "WindowManager::initialize() : BOINC preferred width = "
                       << preferredWidth;
    ErrorHandler::record(msg_init_prefwidth.str(), ErrorHandler::INFORM);

    int preferredHeight = m_BoincAdapter->graphicsWindowHeight();
    stringstream msg_init_prefheight;
    msg_init_prefheight << "WindowManager::initialize() : BOINC preferred height = "
                        << preferredHeight;
    ErrorHandler::record(msg_init_prefheight.str(), ErrorHandler::INFORM);

    int preferredFrameRate = m_BoincAdapter->graphicsFrameRate();
    stringstream msg_init_pref_frame_rate;
    msg_init_pref_frame_rate << "WindowManager::initialize() : BOINC preferred frame rate = "
                             << preferredFrameRate;
    ErrorHandler::record(msg_init_pref_frame_rate.str(), ErrorHandler::INFORM);

    // Override optional given default values if preferred values are set
    m_WindowedWidth = (preferredWidth != 0) ? preferredWidth : width;
    m_WindowedHeight = (preferredHeight != 0) ? preferredHeight : height;
    m_RenderEventInterval = 1000.0f / ((preferredFrameRate != 0) ? preferredFrameRate : frameRate);

    // Set desired OpenGL context attributes for our window.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, WindowManager::RED_BITS);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, WindowManager::GREEN_BITS);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, WindowManager::BLUE_BITS);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, WindowManager::ALPHA_BITS);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, WindowManager::DEPTH_BITS);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, WindowManager::HAS_DEPTH_BUFFER);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, WindowManager::OGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, WindowManager::OGL_MINOR_VERSION);

    // Start in windowed mode.
    m_Window = SDL_CreateWindow(m_WindowTitle.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_WindowedWidth,
                                m_WindowedHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_GRABBED);

    // Check that the window was successfully made.
    if (m_Window == NULL) {
        // In the event that the window could not be made...
        ErrorHandler::record("WindowManager::initialise() : Couldn't obtain window !!", ErrorHandler::FATAL);
        }

    // Create a desired OpenGL for use with that window.
    /// TODO - Check error on return here, how ?
    m_Context = SDL_GL_CreateContext(m_Window);

    return true;
    }

void WindowManager::eventLoop(void) {
     Provided we have at least one observer.
    if(!eventObservers.empty()) {
        // Infinite looping until an exit is triggered.
        while(true) {
            bool resize_flag = false;

            // Holder of event type.
            Event current_event;

            // Keep our special timers ticking over.
            Events::Instance(0)->tick();

            // Keep extracting any events from the queue, until it is empty.
            // Events are gathered 'behind the scenes' from input devices
            // asynchronously ... and placed in a 'queue of next available
            // events'. Currently enacting only one listener, which is of
            // AbstractGraphicsEngine type.
            while(Events::Instance(0)->next(&current_event)) {
                if(current_event.type == Events::RenderEventType) {
                    // Frame render falling due.
                    eventObservers.front()->render(dtime());
                    }

                else if(current_event.type == Events::BOINCUpdateEventType) {
                    // BOINC update falling due.
                    eventObservers.front()->refreshBOINCInformation();
                    }

                // Check for any user input if in screensaver mode.
                else if((m_ScreensaverMode == true) &&
                        ((current_event.type == Events::MouseMotionEventType) ||
                         (current_event.type == Events::MouseButtonEventType) ||
                         (current_event.type == Events::CharInputEventType) ||
                         (current_event.type == Events::KeyPressEventType) ||
                         (current_event.type == Events::MouseWheelEventType))) {
                    // Close window, terminate GLFW and leave this window manager.
                    ErrorHandler::record("WindowManager::eventLoop() : Exiting on account of user input", ErrorHandler::INFORM);
                    glfwTerminate();
                    return;
                    }

                else if((current_event.type == Events::MouseMotionEventType) &&
                        (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
                    // Mouse movement with left button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.m_motion.xrel,
                                                           current_event.m_motion.yrel,
                                                           AbstractGraphicsEngine::MouseButtonLeft);
                    }

                else if((current_event.type == Events::MouseMotionEventType) &&
                        (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) {
                    // Mouse movement with right button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.m_motion.xrel,
                                                           current_event.m_motion.yrel,
                                                           AbstractGraphicsEngine::MouseButtonRight);
                    }

                else if(current_event.type == Events::MouseWheelEventType) {
                    // Mouse wheel has been moved.
                    eventObservers.front()->mouseWheelEvent(current_event.m_wheel.diff_pos);
                    }

                else if((current_event.type == Events::ResizeEventType) &&
                        (resize_flag == false)) {
                    resize_flag = true;
                    m_CurrentWidth = m_WindowedWidth = current_event.resize.width;
                    m_CurrentHeight = m_WindowedHeight = current_event.resize.height;

                    setWindowedMode();

                    // Use actual acquired ( as distinct from requested ) size.
                    glfwGetWindowSize(&m_CurrentWidth, &m_CurrentHeight);

                    eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
                    }

                // 'Normal' exit pathway if not screensaver.
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
                        // Note : we account for shifted characters from the same key.
                    switch(current_event.c_input.char_code) {
                        case ',':
                        case '<':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyComma);
                            break;
                        case '.':
                        case '>':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyPeriod);
                            break;
                        case '/':
                        case '?':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyForwardSlash);
                            break;
                        case ';':
                        case ':':
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
                        case 'j':
                        case 'J':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyJ);
                            break;
                        case 'k':
                        case 'K':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyK);
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
                        case 'u':
                        case 'U':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyU);
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
                        case 'y':
                        case 'Y':
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyY);
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
                            break;
                        case GLFW_KEY_KP_1:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP1);
                            break;
                        case GLFW_KEY_KP_2:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP2);
                            break;
                        case GLFW_KEY_KP_3:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP3);
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
    // SDL_SetWindowTitle(m_Window, caption.c_str());
    }

void WindowManager::setWindowIcon(const string filename) const {
    //if (filename.length() > 0) {
    //      SDL_WM_SetIcon(SDL_LoadBMP(filename.c_str()), NULL);
    //      }
    // SDL_SetWindowIcon(m_Window, SDL_Surface* icon);
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
    // SDL_SetWindowIcon(m_Window, SDL_Surface* icon);
    }

bool WindowManager::initializeGLEW(void) {
    // Assume failure.
    bool ret_val = false;

    // Give GLEW the best chance of finding
    // functionality with experimental drivers.
    glewExperimental = GL_TRUE;

    // Now initialise GLEW.
    if(glewInit() != GLEW_OK) {
        ErrorHandler::record("WindowManager::initializeGLEW() : GLEW initialisation fail", ErrorHandler::WARN);
        }
    else {
        std::string msg2 = "WindowManager::initializeGLEW() : Using GLEW version ";
        msg2 += ErrorHandler::convertGLstring(glewGetString(GLEW_VERSION));
        ErrorHandler::record(msg2, ErrorHandler::INFORM);
        ret_val = true;
        }

    return ret_val;
    }

