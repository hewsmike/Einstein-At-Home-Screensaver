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
#include "SDL_events.h"

const std::string WindowManager::m_WindowTitle("Einstein At Home");

const int WindowManager::RED_BITS(8);
const int WindowManager::GREEN_BITS(8);
const int WindowManager::BLUE_BITS(8);
const int WindowManager::ALPHA_BITS(8);
const int WindowManager::DEPTH_BITS(16);
const int WindowManager::HAS_DEPTH_BUFFER(1);
const int WindowManager::ENABLE_VERTICAL_SYNC(1);
const int WindowManager::ENABLE_DOUBLE_BUFFER(1);
const int WindowManager::NUM_MULTISAMPLE_BUFFERS(1);
const int WindowManager::NUM_MULTISAMPLES(2);

const int WindowManager::OGL_MAJOR_VERSION(3);
const int WindowManager::OGL_MINOR_VERSION(2);

const int WindowManager::DISPLAY_ZERO(0);

const int WindowManager::EVENT_PENDING(1);

const int WindowManager::LEFT_MOUSE_BUTTON(1);
const int WindowManager::MIDDLE_MOUSE_BUTTON(2);
const int WindowManager::RIGHT_MOUSE_BUTTON(3);

const float WindowManager::TIMER_DELAY_BOINC(1000);

WindowManager::WindowManager(void) {
    m_BoincAdapter = new BOINCClientAdapter("");
    }

WindowManager::~WindowManager() {
    delete m_BoincAdapter;
    }

bool WindowManager::initialize(const int width, const int height, const int frameRate) {
    // Assume failure of this method.
    bool ret_val = false;

    if(SDL_GetDesktopDisplayMode(WindowManager::DISPLAY_ZERO, m_Mode) == 0) {
        // Obtain current desktop width.
        m_DesktopWidth = m_Mode->w;
        stringstream msg_init_current_desktop_mode_width;
        msg_init_current_desktop_mode_width << "WindowManager::initialize() : current desktop width = "
                                            << m_DesktopWidth;
        ErrorHandler::record(msg_init_current_desktop_mode_width.str(), ErrorHandler::INFORM);

        // Obtain current desktop height.
        m_DesktopHeight = m_Mode->h;
        stringstream msg_init_current_desktop_mode_height;
        msg_init_current_desktop_mode_height << "WindowManager::initialize() : current desktop height = "
                                             << m_DesktopHeight;
        ErrorHandler::record(msg_init_current_desktop_mode_height.str(), ErrorHandler::INFORM);

        // Obtain current desktop color depth.
        m_DesktopBitsPerPixel = SDL_BITSPERPIXEL(m_Mode->format);
        stringstream msg_init_current_desktop_bitsperpixel;
        msg_init_current_desktop_bitsperpixel << "WindowManager::initialize() : current desktop bits per pixel = "
                                              << m_DesktopBitsPerPixel;
        ErrorHandler::record(msg_init_current_desktop_bitsperpixel.str(), ErrorHandler::INFORM);

        // Get initial non-fullscreen resolution and frame rate from project preferences.
        // Reset the BOINC adapter.
        m_BoincAdapter->initialize();

        // Obtain BOINC preferred window width.
        int preferredWidth = m_BoincAdapter->graphicsWindowWidth();
        stringstream msg_init_prefwidth;
        msg_init_prefwidth << "WindowManager::initialize() : BOINC preferred width = "
                           << preferredWidth;
        ErrorHandler::record(msg_init_prefwidth.str(), ErrorHandler::INFORM);

        // Obtain BOINC preferred window height.
        int preferredHeight = m_BoincAdapter->graphicsWindowHeight();
        stringstream msg_init_prefheight;
        msg_init_prefheight << "WindowManager::initialize() : BOINC preferred height = "
                            << preferredHeight;
        ErrorHandler::record(msg_init_prefheight.str(), ErrorHandler::INFORM);

        // Obtain BOINC preferred frame rate ie. frames to render per second.
        int preferredFrameRate = m_BoincAdapter->graphicsFrameRate();
        stringstream msg_init_pref_frame_rate;
        msg_init_pref_frame_rate << "WindowManager::initialize() : BOINC preferred frame rate = "
                                 << preferredFrameRate;
        ErrorHandler::record(msg_init_pref_frame_rate.str(), ErrorHandler::INFORM);

        // Record requested values ( via this function's arguments including
        // defaults ) of width, height and frame rate.
        // NB : NO bounds checking on 'silly' values for height, width or frame rate !!
        /// TODO - bounds checking on width, height and framerate ??
        stringstream msg_init_req_width;
        msg_init_req_width << "WindowManager::initialize() : requested width = "
                           << width;
        ErrorHandler::record(msg_init_req_width.str(), ErrorHandler::INFORM);

        stringstream msg_init_req_height;
        msg_init_req_height << "WindowManager::initialize() : requested height = "
                            << height;
        ErrorHandler::record(msg_init_req_height.str(), ErrorHandler::INFORM);

        stringstream msg_init_req_framerate;
        msg_init_req_framerate << "WindowManager::initialize() : requested frame rate = "
                               << frameRate;
        ErrorHandler::record(msg_init_req_framerate.str(), ErrorHandler::INFORM);

        // If BOINC preferred values are not set then override with arguments given
        // to this machine.
        m_WindowedWidth = (preferredWidth != 0) ? preferredWidth : width;
        m_WindowedHeight = (preferredHeight != 0) ? preferredHeight : height;
        m_RenderEventInterval = 1000.0f / ((preferredFrameRate != 0) ? preferredFrameRate : frameRate);

        /// TODO - check after successful context creation to see if we got what we asked for.
        // Set desired OpenGL context attributes for our window.

        /// TODO - find a way to request vsync in platform independent way !!
        /// Neither Open GL nor SDL ( cleanly ) provide this. Maybe roll my own
        /// abstraction ?

        // Request a minimum number of multisample buffers.
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, WindowManager::NUM_MULTISAMPLE_BUFFERS);

        // Request a minimum of multisamples ( around a given pixel ).
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, WindowManager::NUM_MULTISAMPLES);

        // Request double buffering.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, WindowManager::ENABLE_DOUBLE_BUFFER);

        // Request a specific color depth.
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, WindowManager::RED_BITS);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, WindowManager::GREEN_BITS);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, WindowManager::BLUE_BITS);

        // Request a specific alpha channnel.
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, WindowManager::ALPHA_BITS);

        // Request a minimum number of bits in depth buffer.
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, WindowManager::DEPTH_BITS);

        // Request a depth buffer.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, WindowManager::HAS_DEPTH_BUFFER);

        /// TODO - Need to create compile switch here for use of ES with Android etc.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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

        // Create a desired OpenGL context for use with that window,
        // noting the above attribute selections.
        /// TODO - Check error on return here, how ?
        m_Context = SDL_GL_CreateContext(m_Window);

        // OK we have a window, so initialise GLEW. This matters especially if the
        // driver's function pointers need runtime linkage ie. Win32 target.
        initializeGLEW();

        ret_val = true;
        }
    else {
        // Could not get the desktop parameters.
        stringstream msg_get_desktop_error;
        msg_get_desktop_error << "WindowManager::initialize() : can't obtain current desktop mode : "
                              << << ErrorHandler::check_SDL2_Error() << endl;
        ErrorHandler::record(msg_get_desktop_error.str(), ErrorHandler::WARN);

        }

    return ret_val;
    }

void WindowManager::eventLoop(void) {
    // Provided we have at least one observer.
    if(!eventObservers.empty()) {
        // Set two main timers (interval in ms).
        SDL_AddTimer(m_RenderEventInterval, &timerCallbackRenderEvent, NULL);
        SDL_AddTimer(WindowManager::TIMER_DELAY_BOINC, &timerCallbackBOINCUpdateEvent, NULL);

        // Holder of current event type.
        SDL_Event current_event;
        // Infinite looping until an exit is triggered.
        while(true) {
            bool resize_flag = false;

            // Keep extracting any events from the queue, until it is empty.
            // Events are gathered 'behind the scenes' from input devices
            // asynchronously ... and placed in a 'queue of next available
            // events'. Currently enacting only one listener, which is of
            // AbstractGraphicsEngine type.
            while(SDL_PollEvent(&current_event) == WindowManager::EVENT_PENDING) {
                if(current_event.type == SDL_USEREVENT) {
                    // Frame render falling due.
                    eventObservers.front()->render(dtime());
                    }

                else if(current_event.type == SDL_USEREVENT) {
                    // BOINC update falling due.
                    eventObservers.front()->refreshBOINCInformation();
                    }

                // Check for any user input if in screensaver mode.
                else if((m_ScreensaverMode == true) &&
                        ((current_event.type == SDL_MOUSEMOTION) ||
                         (current_event.type == SDL_MOUSEBUTTONDOWN) ||
                         (current_event.type == SDL_MOUSEWHEEL)
                         (current_event.type == SDL_KEYDOWN))) {
                    // Close window, terminate SDL and leave this window manager.
                    /// TODO - atexit(SDL_Quit) in main too ??
                    ErrorHandler::record("WindowManager::eventLoop() : Exiting on account of user input", ErrorHandler::INFORM);
                    SDL_DestroyWindow(m_Window);
                    SDL_Quit();
                    return;
                    }

                else if((current_event.type == SDL_MOUSEMOTION) &&
                        (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(WindowManager::LEFT_MOUSE_BUTTON))) {
                    // Mouse movement with left button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.xrel,
                                                           current_event.yrel,
                                                           AbstractGraphicsEngine::MouseButtonLeft);
                    }

                else if((current_event.type == SDL_MOUSEMOTION) &&
                        (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(WindowManager::RIGHT_MOUSE_BUTTON))) {
                    // Mouse movement with right button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.xrel,
                                                           current_event.yrel,
                                                           AbstractGraphicsEngine::MouseButtonRight);
                    }

                else if(current_event.type == SDL_MOUSEWHEEL) {
                    // Mouse wheel has been moved.
                    eventObservers.front()->mouseWheelEvent(current_event.y);
                    }

                else if((current_event.type == SDL_WINDOWEVENT) &&
                        (current_event.event == SDL_WINDOWEVENT_RESIZED) &&
                        (resize_flag == false)) {
                    resize_flag = true;
                    m_CurrentWidth = m_WindowedWidth = current_event.data1;
                    m_CurrentHeight = m_WindowedHeight = current_event.data2;

                    // Use actual acquired ( as distinct from requested ) size.
                    SDL_GetWindowSize(m_Window,
                                      &m_CurrentWidth,
                                      &m_CurrentHeight);

                    eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
                    }

                // 'Normal' exit pathway if not screensaver.
                else if((current_event.type == SDL_QUIT) ||
                        ((current_event.type == SDL_KEYDOWN) &&
                         (current_event.k_press.key_code == GLFW_KEY_ESC))) {
                    // Close window, terminate SDL and leave this window manager.
                    /// TODO - atexit(SDL_Quit) in main too ??
                    ErrorHandler::record("WindowManager::eventLoop() : normal exit on user request", ErrorHandler::INFORM);
                    SDL_DestroyWindow(m_Window);
                    SDL_Quit();
                    return;
                    }

                // Process printable character input.
                else if((current_event.type == Events::CharInputEventType) &&
                        (current_event.c_input.pressed == true)) {
//                        // Note : we account for shifted characters from the same key.
//                    switch(current_event.c_input.char_code) {
//                        case ',':
//                        case '<':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyComma);
//                            break;
//                        case '.':
//                        case '>':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyPeriod);
//                            break;
//                        case '/':
//                        case '?':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyForwardSlash);
//                            break;
//                        case ';':
//                        case ':':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySemiColon);
//                            break;
//                        case 'a':
//                        case 'A':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyA);
//                            break;
//                        case 'b':
//                        case 'B':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyB);
//                            break;
//                        case 'c':
//                        case 'C':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyC);
//                            break;
//                        case 'd':
//                        case 'D':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyD);
//                            break;
//                        case 'e':
//                        case 'E':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyE);
//                            break;
//                        case 'f':
//                        case 'F':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF);
//                            break;
//                        case 'g':
//                        case 'G':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyG);
//                            break;
//                        case 'h':
//                        case 'H':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyH);
//                            break;
//                        case 'i':
//                        case 'I':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyI);
//                            break;
//                        case 'j':
//                        case 'J':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyJ);
//                            break;
//                        case 'k':
//                        case 'K':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyK);
//                            break;
//                        case 'l':
//                        case 'L':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyL);
//                            break;
//                        case 'm':
//                        case 'M':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyM);
//                            break;
//                        case 'n':
//                        case 'N':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyN);
//                            break;
//                        case 'o':
//                        case 'O':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyO);
//                            break;
//                        case 'p':
//                        case 'P':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyP);
//                            break;
//                        case 'q':
//                        case 'Q':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyQ);
//                            break;
//                        case 'r':
//                        case 'R':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyR);
//                            break;
//                        case 's':
//                        case 'S':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyS);
//                            break;
//                        case 't':
//                        case 'T':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyT);
//                            break;
//                        case 'u':
//                        case 'U':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyU);
//                            break;
//                        case 'v':
//                        case 'V':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyV);
//                            break;
//                        case 'w':
//                        case 'W':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyW);
//                            break;
//                        case 'x':
//                        case 'X':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyX);
//                            break;
//                        case 'y':
//                        case 'Y':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyY);
//                            break;
//                        case 'z':
//                        case 'Z':
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyZ);
//                            break;
//                        default:
//                            break;
//                        }
                    }

                // Process non-printable keypresses.
                else if((current_event.type == SDL_KEYDOWN) &&
                        (current_event.k_press.pressed == true)) {
//                    switch(current_event.k_press.key_code) {
//                        case GLFW_KEY_F1:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF1);
//                            break;
//                        case GLFW_KEY_F2:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF2);
//                            break;
//                        case GLFW_KEY_F3:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF3);
//                            break;
//                        case GLFW_KEY_F4:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF4);
//                            break;
//                        case GLFW_KEY_F5:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF5);
//                            break;
//                        case GLFW_KEY_F6:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF6);
//                            break;
//                        case GLFW_KEY_F7:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF7);
//                            break;
//                        case GLFW_KEY_F8:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF8);
//                            break;
//                        case GLFW_KEY_F9:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF9);
//                            break;
//                        case GLFW_KEY_F10:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF10);
//                            break;
//                        case GLFW_KEY_F11:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF11);
//                            break;
//                        case GLFW_KEY_F12:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF12);
//                            break;
//                        case GLFW_KEY_SPACE:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySpace);
//                            break;
//                        case GLFW_KEY_ENTER:
//                            break;
//                        case GLFW_KEY_KP_1:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP1);
//                            break;
//                        case GLFW_KEY_KP_2:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP2);
//                            break;
//                        case GLFW_KEY_KP_3:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP3);
//                            break;
//                        case GLFW_KEY_KP_4:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP4);
//                            break;
//                        case GLFW_KEY_KP_5:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP5);
//                            break;
//                        case GLFW_KEY_KP_6:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP6);
//                            break;
//                        case GLFW_KEY_KP_8:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP8);
//                            break;
//                        case GLFW_KEY_KP_0:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP0);
//                            break;
//                        case GLFW_KEY_KP_DECIMAL:
//                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKPPeriod);
//                            break;
//                        default:
//                            break;
//                        }
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

void WindowManager::setWindowCaption(const std::string& caption) const {
    SDL_SetWindowTitle(m_Window, caption.c_str());
    }

void WindowManager::setWindowIcon(const std::string& filename) const {
    if (filename.length() > 0) {
        SDL_Surface* icon = SDL_LoadBMP(filename.c_str());
        SDL_SetWindowIcon(m_Window, icon);
        }
    }

void WindowManager::setWindowIcon(const unsigned char* data, const int size) const {
    // Prepare data buffer structure
    SDL_RWops* buffer = SDL_RWFromMem((void*) data, size);

    if(buffer != NULL) {
        // Load BMP from prepared data buffer, closing stream too.
        SDL_Surface* surface = SDL_LoadBMP_RW(buffer, 1);

        // Did that load succeed ?
        if(surface != NULL) {
            // set window icon, then release the SDL_Surface.
            SDL_SetWindowIcon(m_Window, surface);
            SDL_FreeSurface(surface);
            }
        else {
            stringstream icon_surface_error_msg;
            icon_surface_error_msg << "Could not create window icon surface : "
                                   << ErrorHandler::check_SDL2_Error() << endl;
            ErrorHandler::record(icon_surface_error_msg.str(), ErrorHandler::WARN);
            }

        }
    else {
        stringstream icon_load_error_msg;
        icon_load_error_msg << "Could not prepare window icon data : "
                            << ErrorHandler::check_SDL2_Error() << endl;
        ErrorHandler::record(icon_load_error_msg.str(), ErrorHandler::WARN);
        }
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

Uint32 WindowManager::timerCallbackRenderEvent(Uint32 interval, void* param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = RenderEvent;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return interval;
    }

Uint32 WindowManager::timerCallbackBOINCUpdateEvent(Uint32 interval, void* param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = BOINCUpdateEvent;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return interval;
    }

void WindowManager::toggleFullscreen() {
    // toggle fullscreen bit and reset video mode
    if(m_WindowedModeAvailable && (m_VideoModeFlags & SDL_FULLSCREEN)) {
        // set new dimensions
        m_CurrentWidth = m_WindowedWidth;
        m_CurrentHeight = m_WindowedHeight;

        // (un)set video mode flags
        m_VideoModeFlags &= ~SDL_FULLSCREEN;
        m_VideoModeFlags |= SDL_RESIZABLE;

        // show cursor in fullscreen mode
        SDL_ShowCursor(SDL_ENABLE);
        }
    else if(m_FullscreenModeAvailable && !(m_VideoModeFlags & SDL_FULLSCREEN)) {
        // set new dimensions
        m_CurrentWidth = m_DesktopWidth;
        m_CurrentHeight = m_DesktopHeight;

        // (un)set video mode flags
#ifdef __APPLE__
        if (m_ScreensaverMode) {
            m_CurrentWidth = m_DesktopWidth;
            m_CurrentHeight = m_DesktopHeight;
            m_VideoModeFlags |= SDL_NOFRAME;
            }
        else
#endif
            {
            m_VideoModeFlags |= SDL_FULLSCREEN;
            }
        m_VideoModeFlags &= ~SDL_RESIZABLE;

        // hide cursor
        SDL_ShowCursor(SDL_DISABLE);
        }

    // reset video mode
    m_DisplaySurface = SDL_SetVideoMode(m_CurrentWidth,
                                        m_CurrentHeight,
                                        m_DesktopBitsPerPixel,
                                        m_VideoModeFlags);

    // NB : No longer need to recycle a listener's window on a per OS basis.
    }

void WindowManager::setScreensaverMode(const bool enabled) {
    m_ScreensaverMode = enabled;
    }

