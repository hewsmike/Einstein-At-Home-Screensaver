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
#include "Events.h"
#include "SolarSystemGlobals.h"

unsigned int WindowManager::OPEN_GL_VERSION_MINIMUM_MAJOR(1);
unsigned int WindowManager::OPEN_GL_VERSION_MINIMUM_MINOR(5);
int WindowManager::DEPTH_BUFFER_GRAIN(24);
int WindowManager::DEPTH_BUFFER_GRAIN_FALLBACK(16);
int WindowManager::NO_STENCIL(0);
int WindowManager::VERTICAL_RETRACE_COUNT(1);

WindowManager::WindowManager(displaymode mode) :
                                operating_mode(mode) {
    m_ScreensaverMode = false;
    if(mode == WindowManager::SCREENSAVER) {
        m_ScreensaverMode = true;
        }
    SolarSystemGlobals::setDisplayMode(mode);
    best_depth_buffer_grain = DEPTH_BUFFER_GRAIN;
    m_BoincAdapter = new BOINCClientAdapter("");
    }

WindowManager::~WindowManager() {
    delete m_BoincAdapter;
    }

bool WindowManager::initialize(const int width, const int height, const int frameRate) {
    // Can the GLFW system be initialised ?
    if(glfwInit() == GL_FALSE) {
        // No it can't, and that's fatal.
        ErrorHandler::record("WindowManager::initialize() : Window system could not be initalized - GLFW init fail", ErrorHandler::FATAL);
        return false;
        }
    else {
        // Yes it can be, and now is, initialised.
        // Emit GLFW version info.
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

    // GLFW is up and running by now, and glfwInit() has put a callback in
    // place for GLFW cleanup on normal program exit ie. atexit(glfwTerminate)

    // Determines that a buffer swap ought await the
    // return of the raster line to the top of the screen.
    glfwSwapInterval(VERTICAL_RETRACE_COUNT);

    // Retrieve current video hardware settings for the user's desktop.
    glfwGetDesktopMode(&current_desktop_mode);

    m_DesktopWidth = current_desktop_mode.Width;
    stringstream msg_init_current_desktop_mode_width;
    msg_init_current_desktop_mode_width << "WindowManager::initialize() : current desktop width = "
                                        << m_DesktopWidth;
    ErrorHandler::record(msg_init_current_desktop_mode_width.str(), ErrorHandler::INFORM);


    m_DesktopHeight = current_desktop_mode.Height;
    stringstream msg_init_current_desktop_mode_height;
    msg_init_current_desktop_mode_height << "WindowManager::initialize() : current desktop height = "
                                         << m_DesktopHeight;
    ErrorHandler::record(msg_init_current_desktop_mode_height.str(), ErrorHandler::INFORM);


    m_DesktopBitsPerPixel = current_desktop_mode.RedBits +
                            current_desktop_mode.GreenBits +
                            current_desktop_mode.BlueBits;
    stringstream msg_init_current_desktop_bitsperpixel;
    msg_init_current_desktop_bitsperpixel << "WindowManager::initialize() : current desktop bits per pixel = "
                                          << m_DesktopBitsPerPixel
                                          << " ( red = "
                                          << current_desktop_mode.RedBits
                                          << ", green = "
                                          << current_desktop_mode.GreenBits
                                          << ", blue = "
                                          << current_desktop_mode.BlueBits
                                          << " )";
    ErrorHandler::record(msg_init_current_desktop_bitsperpixel.str(), ErrorHandler::INFORM);

    // get initial non-fullscreen resolution and frame rate from project preferences
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

    // override optional given default values if preferred values are set
    m_WindowedWidth = (preferredWidth != 0) ? preferredWidth : width;
    m_WindowedHeight = (preferredHeight != 0) ? preferredHeight : height;
    m_RenderEventInterval = 1000.0f / ((preferredFrameRate != 0) ? preferredFrameRate : frameRate);

    // Suggest our OpenGL version minimum.
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, OPEN_GL_VERSION_MINIMUM_MAJOR);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, OPEN_GL_VERSION_MINIMUM_MINOR);

    // Start in selected screen and operational mode.
    switch(operating_mode) {
        case WINDOW :
            setWindowedMode();
            break;
        case SCREENSAVER :
            setFullScreenMode();
            break;
        case DEMO :
            setFullScreenMode();
            break;
        default:
            ErrorHandler::record("WindowManager::initialize() : bad switch case ( default )", ErrorHandler::FATAL);
            break;
        }

    // Manage Windows OpenGL backwards compatibility issue.
#ifdef WIN_OGL_WORKAROUND
    if(setOGLContext() == true) {
        // Good to go.
        ErrorHandler::record("WindowManager::initialize() : acquired adequate OpenGL backward compatible version", ErrorHandler::INFORM);
        // Find out the OpenGL version.
        GLuint major = 0;
        GLuint minor = 0;
        getOGLVersion(&major, &minor);
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

    // Final test to see if the driver is "truthful".
    // If OPEN_GL_VERSION_MINIMUM_MAJOR or OPEN_GL_VERSION_MINIMUM_MINOR
    // altered need to change the test variable here.
    if(GLEW_VERSION_1_5) {
        // Fine as is.
        ErrorHandler::record("WindowManager::initialize() : satisfactory OpenGL v1.5+", ErrorHandler::INFORM);
        }
    else {
        // Whoops, some components at the minimum level not actually supported.
        std::stringstream msg;
        msg << "WindowManager::initialize() : Driver claims v"
            << OPEN_GL_VERSION_MINIMUM_MAJOR
            << "."
            << OPEN_GL_VERSION_MINIMUM_MINOR
            << " minimum interface, but some components missing ....";
        ErrorHandler::record(msg.str(), ErrorHandler::WARN);
        return false;
        }

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

                else if(current_event.type == Events::ResizeEventType) {
                    // In practice it is better to use the actual
                    // dimensions obtained by a resize, rather than
                    // the dimensions used when requesting the resize.
                    // Depending on the target system one may have the
                    // OS allowing or not for menu and taskbar dimensions ....
                    glfwGetWindowSize(&m_CurrentWidth,& m_CurrentHeight);

                    m_WindowedWidth = m_CurrentWidth;
                    m_WindowedHeight = m_CurrentHeight;
                    // Window resize has occurred. NB last argument is set to recycle
                    // the .... AbstractGraphicsEngine.
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

bool WindowManager::setWindowedMode(void) {
    bool ret_val = false;

    // Attempt to obtain a window.
    if(tryMode(m_WindowedWidth, m_WindowedHeight, GLFW_WINDOW) == GL_FALSE) {
        ErrorHandler::record("WindowManager::setWindowedMode() : Could not acquire rendering surface", ErrorHandler::WARN);
        }
    else {
        m_WindowedWidth = m_CurrentWidth;
        m_WindowedHeight = m_CurrentHeight;
        ret_val = true;
        }

    return ret_val;
    }

bool WindowManager::setFullScreenMode(void) {
    bool ret_val = false;

    // Attempt to obtain a fullscreen.
    if(tryMode(m_DesktopWidth, m_DesktopHeight, GLFW_FULLSCREEN) == GL_FALSE) {
        ErrorHandler::record("WindowManager::setFullScreenMode() : Could not acquire rendering surface", ErrorHandler::WARN);
        }
    else {
        ret_val = true;
        }

    return ret_val;
    }

bool WindowManager::tryMode(int width, int height, int mode) {
    // For this routine interpret 'window' as an OpenGL context
    // which may be EITHER a "OS window" OR a fullscreen.

    stringstream msg;
    msg << "WindowManager::tryMode() : attempt width = "
        << width << " and height = " << height
        << " using mode " << ((mode == GLFW_WINDOW)? "WINDOW" : "FULLSCREEN" )
        << std::endl;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

    // See if you can get a rendering surface from the OS.
    int window_open = glfwOpenWindow(width, height,
                                     current_desktop_mode.RedBits,
                                     current_desktop_mode.GreenBits,
                                     current_desktop_mode.BlueBits,
                                     current_desktop_mode.RedBits,			// Alpha range same as individual colors
                                     best_depth_buffer_grain,
                                     NO_STENCIL,
                                     mode);

    msg.clear();
    msg << "WindowManager::tryMode() : glfwOpenWindow() first attempt returned "
        << ((window_open == GL_TRUE) ? "true" : "false") << std::endl;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

    if(window_open == GL_FALSE) {
        // It may have failed to open because of the depth buffer choice,
        // so retry with a lower depth resolution ...
        best_depth_buffer_grain = DEPTH_BUFFER_GRAIN_FALLBACK;
        window_open = glfwOpenWindow(width, height,
                                     current_desktop_mode.RedBits,
                                     current_desktop_mode.GreenBits,
                                     current_desktop_mode.BlueBits,
                                     current_desktop_mode.RedBits,			// Alpha range same as individual colors
                                     best_depth_buffer_grain,
                                     NO_STENCIL,
                                     mode);

        msg.clear();
        msg << "WindowManager::tryMode() : glfwOpenWindow() second attempt returned "
            << ((window_open == GL_TRUE) ? "true" : "false") << std::endl;
        ErrorHandler::record(msg.str(), ErrorHandler::INFORM);
        }

    if(window_open == GL_TRUE) {
        // For MS Windows, the dynamic links will change
        // upon rendering surface acquisition !!
        initializeGLEW();

        // Inquire as to the actual client area obtained. Otherwise for
        // Linux systems one may expect desktop dimensions but only get
        // that minus taskbar and/or menubar dimensions. That in turn
        // depends upon the distro and window manager ( KDE, Gnome etc .... ).
        // In generality one ought expect a discrepancy between what you
        // ask for and what you get! :-)
        glfwGetWindowSize(&m_CurrentWidth,& m_CurrentHeight);

        msg.clear();
        msg << "WindowManager::tryMode() : Rendering surface acquired "
            << m_CurrentWidth
            << " x "
            << m_CurrentHeight
            << " @ "
            << (current_desktop_mode.RedBits + current_desktop_mode.GreenBits + current_desktop_mode.BlueBits)
            << " colors with a "
            << best_depth_buffer_grain
            << " bit depth buffer";
        ErrorHandler::record(msg.str(), ErrorHandler::INFORM);
        }

    return (window_open == GL_TRUE ? true : false);
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

#ifdef WIN_OGL_WORKAROUND
bool WindowManager::setOGLContext(void) {
    // Assume failure.
    bool ret_val = false;

    // Determine OpenGL version.
    GLuint major = 0;
    GLuint minor = 0;
    getOGLVersion(&major, &minor);
    std::stringstream msg;
    msg << "WindowManager::setOGLContext() : OpenGL v"
        << major
        << "."
        << minor;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

    // With v3.2+ then ...
    if((major > 3) ||
        ((major == 3) && (minor >= 2))) {
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

                // Did that work?
                if(new_context != NULL) {
                    // Make this new context the current one.
                    wglMakeCurrent(hdc, new_context);

                    // Delete the old context.
                    wglDeleteContext(hglrc);

                    // Give GLEW the best chance of finding
                    // functionality with experimental drivers.
                    glewExperimental = GL_TRUE;

                    // Re-initialise GLEW.
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
                    // Tried, but couldn't get a context.
                    ErrorHandler::record("WindowManager::setOGLContext() : Failed to created backward compatible context", ErrorHandler::WARN);
                    }
                }
            else {
                // No, backward context creation facility not available.
                ErrorHandler::record("WindowManager::setOGLContext() : WGL_ARB_create_context_profile NOT supported", ErrorHandler::WARN);
                }
            }
        }
    else {
        // OK, so we're less than v3.2, are we at least the minumum ?
        if((major > OPEN_GL_VERSION_MINIMUM_MAJOR) ||
            ((major == OPEN_GL_VERSION_MINIMUM_MAJOR) && (minor >= OPEN_GL_VERSION_MINIMUM_MINOR))) {
            ret_val = true;
            }
        else {
            // No, the declared OpenGL version is less than desired.
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

void WindowManager::getOGLVersion(GLuint* major, GLuint* minor) {
    // Try to obtain the version string from the context.
    const GLubyte* version = glGetString(GL_VERSION);

    // Did that succeed ?
    if (version == NULL) {
        // No, can't proceed.
        std::string msg = "WindowManager::getOGLVersion() : couldn't obtain version string";
        ErrorHandler::record(msg, ErrorHandler::FATAL);
        }

    // Convert the version string to STL format.
    std::string ver = ErrorHandler::convertGLstring(version);

    // Now tokenise the string thus obtained.
    std::vector<std::string> ver_strings;
    tokeniseString(ver, '.', ver_strings);

    if (ver_strings.size() < 2) {
        std::string msg = "WindowManager::getOGLVersion() : couldn't tokenise version string";
        }

    int major_candidate = 0;
    std::stringstream convert1(ver_strings[0]);
    convert1 >> major_candidate;

    if (convert1.goodbit != 0) {
        std::string msg =
                "WindowManager::getOGLVersion() : couldn't interpret major version string";
        ErrorHandler::record(msg, ErrorHandler::FATAL);
        }
    *major = major_candidate;

    int minor_candidate = 0;
    std::stringstream convert2(ver_strings[1]);
    convert2 >> minor_candidate;

    if (convert2.goodbit != 0) {
        std::string msg =
                "WindowManager::getOGLVersion() : couldn't interpret minor version string";
        ErrorHandler::record(msg, ErrorHandler::FATAL);
        }
    *minor = minor_candidate;
    }

void WindowManager::tokeniseString(const std::string str,
        const char delimiter, std::vector<std::string>& store) {
    size_t string_pos = 0;
    size_t delimiter_pos = 0;

    // Provided we don't get to the end of ( what remains
    // of ) the input string and not find a delimiter.
    while ((delimiter_pos = str.find(delimiter, string_pos))
            != std::string::npos) {
        // Are the delimiters adjacent ? Policy is that
        // we will not create empty tokens, hence multiple
        // adjacent delimiters are treated as one.
        if (delimiter_pos > string_pos) {
            // Non-adjacent.
            size_t token_len = delimiter_pos - string_pos;
            std::string token = str.substr(string_pos, token_len);
            store.push_back(token);
        }
        // Move along to the position just after last delimiter found.
        string_pos = delimiter_pos + 1;
    }
}
