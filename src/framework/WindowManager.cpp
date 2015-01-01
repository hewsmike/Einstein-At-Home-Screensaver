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
#include "ogl_utility.h"
#include "SDL_events.h"

const std::string WindowManager::m_WindowTitle("Einstein At Home");

const int WindowManager::RED_BITS(8);
const int WindowManager::GREEN_BITS(8);
const int WindowManager::BLUE_BITS(8);
const int WindowManager::ALPHA_BITS(8);
const int WindowManager::DEPTH_BITS(16);
const int WindowManager::ENABLE_VERTICAL_SYNC(1);
const int WindowManager::ENABLE_DOUBLE_BUFFER(1);
const int WindowManager::NUM_MULTISAMPLE_BUFFERS(1);
const int WindowManager::NUM_MULTISAMPLES(2);

const int WindowManager::CONTEXT_PROFILE_TYPE(SDL_GL_CONTEXT_PROFILE_CORE);
const int WindowManager::OGL_MAJOR_VERSION(3);
const int WindowManager::OGL_MINOR_VERSION(2);

const int WindowManager::DISPLAY_ZERO(0);

const int WindowManager::EVENT_PENDING(1);

const int WindowManager::WINDOW_PLEASE(0);

const int WindowManager::LEFT_MOUSE_BUTTON(1);
const int WindowManager::MIDDLE_MOUSE_BUTTON(2);
const int WindowManager::RIGHT_MOUSE_BUTTON(3);

const float WindowManager::TIMER_DELAY_BOINC(1000);
const float WindowManager::MILLISECONDS_PER_SECOND(1000.0f);

Uint32 WindowManager::RenderEvent(0);
Uint32 WindowManager::BOINCUpdateEvent(0);

WindowManager::WindowManager(void) {
	m_Mode = NULL;
	m_Window = NULL;
	m_Context = NULL;
	m_WindowID = 0;
	m_RenderEventInterval = 0;
	m_DesktopWidth = 0;
	m_DesktopHeight = 0;
	m_DesktopBitsPerPixel = 0;
	m_CurrentWidth = 0;
	m_CurrentHeight = 0;
	m_WindowedWidth = 0;
	m_WindowedHeight = 0;
	m_ScreensaverMode = true;
    m_BoincAdapter = new BOINCClientAdapter("");
    }

WindowManager::~WindowManager() {
	if(m_Window != NULL){
		SDL_DestroyWindow(m_Window);
		}
	if(m_Context != NULL){
		SDL_GL_DeleteContext(m_Context);
		}
    if(m_Mode != NULL) {
        delete m_Mode;
        }
    if(m_BoincAdapter != NULL) {
        delete m_BoincAdapter;
        }
    }

bool WindowManager::initialize(const int width, const int height, const int frameRate) {

	m_Mode = new SDL_DisplayMode();

	if(m_Mode == NULL){
		// Make this a fatal, can't really proceed otherwise.
		ErrorHandler::record("WindowManager::initialize() : could not obtain SDL_DisplayMode !", ErrorHandler::FATAL);
		}

    // Assume failure of this method.
    bool ret_val = false;

    // Get SDL to assign event codes for render and BOINC events. This occurs
    // dynamically to avoid runtime clashes ( an SDL Wiki recommendation ).
    RenderEvent = SDL_DEBUG(SDL_RegisterEvents(1));

    // Check if that succeeded.
    if(RenderEvent == ((Uint32)-1)) {
        // Make this a fatal, can't really proceed otherwise.
        ErrorHandler::record("WindowManager::initialize() : could not obtain RenderEvent code !", ErrorHandler::FATAL);
        }

    BOINCUpdateEvent = SDL_DEBUG(SDL_RegisterEvents(1));
    // Check if that succeeded.
    if(BOINCUpdateEvent == ((Uint32)-1)) {
        // Make this a fatal, can't really proceed otherwise.
        ErrorHandler::record("WindowManager::initialize() : could not obtain BOINCUpdateEvent code !", ErrorHandler::FATAL);
        }

    int get_desktop_mode_flag = SDL_DEBUG(SDL_GetDesktopDisplayMode(WindowManager::DISPLAY_ZERO, m_Mode));

    if(get_desktop_mode_flag == 0) {
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
        // int preferredWidth = m_BoincAdapter->graphicsWindowWidth();
        int preferredWidth = 1024;
        stringstream msg_init_prefwidth;
        msg_init_prefwidth << "WindowManager::initialize() : BOINC preferred width = "
                           << preferredWidth;
        ErrorHandler::record(msg_init_prefwidth.str(), ErrorHandler::INFORM);

        // Obtain BOINC preferred window height.
        // int preferredHeight = m_BoincAdapter->graphicsWindowHeight();
        int preferredHeight = 768;
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
        m_RenderEventInterval = WindowManager::MILLISECONDS_PER_SECOND / ((preferredFrameRate != 0) ? preferredFrameRate : frameRate);

        m_CurrentWidth = m_WindowedWidth;
        m_CurrentHeight = m_WindowedHeight;

        /// TODO - check after successful context creation to see if we got what we asked for.
        // Set desired OpenGL context attributes for our window.

        // Start in windowed mode.
        m_Window = SDL_DEBUG(SDL_CreateWindow(m_WindowTitle.c_str(),
                                    		  SDL_WINDOWPOS_CENTERED,
											  SDL_WINDOWPOS_CENTERED,
											  m_WindowedWidth,
											  m_WindowedHeight,
											  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

        // Check that the window was successfully made.
        if(m_Window == NULL) {
            // In the event that the window could not be made...
            ErrorHandler::record("WindowManager::initialise() : Couldn't obtain window !!", ErrorHandler::FATAL);
            }

        // Get a window identifier, it is needed in the event loop.
        m_WindowID = SDL_DEBUG(SDL_GetWindowID(m_Window));

        // Initial display is as a window, not fullscreen.
        m_CurrentScreenMode = WindowManager::WINDOWED;

        setContextAttributes();

        /// TODO - Check error on return here, how ?
        m_Context = SDL_DEBUG(SDL_GL_CreateContext(m_Window));

        if(m_Context == NULL) {
            std::stringstream SDL_error_string;
            SDL_error_string << "WindowManager::initialise() : Couldn't obtain context !!\n"
                             << SDL_GetError() << std::endl;
            ErrorHandler::record(SDL_error_string.str(), ErrorHandler::FATAL);
            }



        std::stringstream init_version_msg;
        init_version_msg << "WindowManager::initialize() : Video driver reports GL_VERSION = " << glGetString(GL_VERSION);
        ErrorHandler::record(init_version_msg.str(), ErrorHandler::INFORM);

        // OK we have a window and a valid context, so initialise GLEW.
        // This matters especially if the installable device driver's function pointers need runtime
        // linkage ie. Win32 target.
        initializeGLEW();

        // Display the comparison between what was requested versus what was delivered.
        checkContextAttributes();

        ret_val = true;
        }
    else {
        // Could not get the desktop parameters.
        stringstream msg_get_desktop_error;
        msg_get_desktop_error << "WindowManager::initialize() : can't obtain current desktop mode : "
                              << ErrorHandler::check_SDL2_Error(__FILE__, __LINE__) << endl;
        ErrorHandler::record(msg_get_desktop_error.str(), ErrorHandler::WARN);
        }

    return ret_val;
    }

void WindowManager::eventLoop(void) {
    // Provided we have at least one observer.
    if(!eventObservers.empty()) {
        // Set two main timers (interval in ms).
    	SDL_DEBUG(SDL_AddTimer(m_RenderEventInterval, &timerCallbackRenderEvent, NULL));
    	SDL_DEBUG(SDL_AddTimer(WindowManager::TIMER_DELAY_BOINC, &timerCallbackBOINCUpdateEvent, NULL));

        // Holder of current event type.
        SDL_Event current_event;
        // Infinite looping until an exit is triggered.
        while(true) {
            // Keep extracting any events from the queue, until it is empty.
            // Events are gathered 'behind the scenes' from input devices
            // asynchronously ... by SDL ... and placed in a queue.
            // Currently enacting only one listener, which is of
            // AbstractGraphicsEngine type.
            while(SDL_PollEvent(&current_event) == WindowManager::EVENT_PENDING) {
                // NB Having pulled the event off queue, what is
                // subsequently not handled here is thus ignored.
                if((current_event.type == SDL_USEREVENT) &&
                   (current_event.user.code == WindowManager::RenderEvent)) {
                    // Frame render falling due.
                    eventObservers.front()->render(dtime());
                    //Swap the buffers.
                    this->swap();
                    }

                else if((current_event.type == SDL_USEREVENT) &&
                        (current_event.user.code == WindowManager::BOINCUpdateEvent)) {
                    // BOINC update falling due.
                    eventObservers.front()->refreshBOINCInformation();
                    }

                // Check for ANY user input if in screensaver mode.

                else if((m_ScreensaverMode == true) &&
                        ((current_event.type == SDL_MOUSEMOTION) ||
                         (current_event.type == SDL_MOUSEBUTTONDOWN) ||
                         (current_event.type == SDL_MOUSEWHEEL) ||
                         (current_event.type == SDL_KEYDOWN))) {
                    // Close window, terminate SDL and leave this window manager.
                    ErrorHandler::record("WindowManager::eventLoop() : Exiting on account of user input", ErrorHandler::INFORM);
                    SDL_DEBUG(SDL_DestroyWindow(m_Window));
                    SDL_Quit();
                    return;
                    }

                else if((current_event.type == SDL_MOUSEMOTION) &&
                        (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(WindowManager::LEFT_MOUSE_BUTTON))) {
                    // Mouse movement with left button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.motion.xrel,
                                                           current_event.motion.yrel,
                                                           AbstractGraphicsEngine::MouseButtonLeft);
                    }

                else if((current_event.type == SDL_MOUSEMOTION) &&
                        (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(WindowManager::RIGHT_MOUSE_BUTTON))) {
                    // Mouse movement with right button pressed down.
                    eventObservers.front()->mouseMoveEvent(current_event.motion.xrel,
                                                           current_event.motion.yrel,
                                                           AbstractGraphicsEngine::MouseButtonRight);
                    }

                else if(current_event.type == SDL_MOUSEWHEEL) {
                    // Mouse wheel has been moved.
                    eventObservers.front()->mouseWheelEvent(current_event.wheel.y);
                    }

                // Finicky : check the window event refers to the window we are using ... :-)
                else if((current_event.type == SDL_WINDOWEVENT) &&
                        (current_event.window.windowID ==  m_WindowID) &&
                        (current_event.window.event == SDL_WINDOWEVENT_RESIZED)) {
                    m_CurrentWidth = m_WindowedWidth = current_event.window.data1;
                    m_CurrentHeight = m_WindowedHeight = current_event.window.data2;

                    // Use actual acquired ( as distinct from requested ) size.
                    SDL_GetWindowSize(m_Window,
                                      &m_CurrentWidth,
                                      &m_CurrentHeight);

                    eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, NULL);
                    }

                // 'Normal' exit pathway if not screensaver.
                // NB The ESC key mapping here is virtual and so eliminates
                // any modified versions triggering eg. Ctrl + ESC. But on
                // the other hand ( though not likely to matter ) any
                // re-mapping of the keyboard will be transparent ie. some
                // facility substituting/aliasing another physical key to perform
                // as the ESC key.
                else if((current_event.type == SDL_QUIT) ||
                        (current_event.type == SDL_WINDOWEVENT_CLOSE) ||
                        ((current_event.type == SDL_KEYDOWN) &&
                         (current_event.key.keysym.sym == SDLK_ESCAPE))) {
                    // Close window, terminate SDL and leave this window manager.
                    ErrorHandler::record("WindowManager::eventLoop() : normal exit on user request", ErrorHandler::INFORM);
                    SDL_DestroyWindow(m_Window);
                    SDL_Quit();
                    return;
                    }

                // Process printable character input using virtual keys, see
                // above comments for ESC key.
                else if(current_event.type == SDL_KEYDOWN) {
                    // Note : we account for shifted characters from
                    // the same physical key.
                    switch(current_event.key.keysym.sym) {
                        case SDLK_COMMA:
                        case SDLK_LESS:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyComma);
                            break;
                        case SDLK_PERIOD:
                        case SDLK_GREATER:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyPeriod);
                            break;
                        case SDLK_SLASH:
                        case SDLK_QUESTION:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyForwardSlash);
                            break;
                        case SDLK_SEMICOLON:
                        case SDLK_COLON:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySemiColon);
                            break;
                        // Upper and lower case alphabetic characters DO map to the
                        // same virtual key, despite the lowercase tail of the enumerant.
                        case SDLK_a:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyA);
                            break;
                        case SDLK_b:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyB);
                            break;
                        case SDLK_c:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyC);
                            break;
                        case SDLK_d:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyD);
                            break;
                        case SDLK_e:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyE);
                            break;
                        case SDLK_f:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF);
                            break;
                        case SDLK_g:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyG);
                            break;
                        case SDLK_h:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyH);
                            break;
                        case SDLK_i:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyI);
                            break;
                        case SDLK_j:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyJ);
                            break;
                        case SDLK_k:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyK);
                            break;
                        case SDLK_l:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyL);
                            break;
                        case SDLK_m:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyM);
                            break;
                        case SDLK_n:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyN);
                            break;
                        case SDLK_o:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyO);
                            break;
                        case SDLK_p:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyP);
                            break;
                        case SDLK_q:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyQ);
                            break;
                        case SDLK_r:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyR);
                            break;
                        case SDLK_s:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyS);
                            break;
                        case SDLK_t:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyT);
                            break;
                        case SDLK_u:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyU);
                            break;
                        case SDLK_v:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyV);
                            break;
                        case SDLK_w:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyW);
                            break;
                        case SDLK_x:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyX);
                            break;
                        case SDLK_y:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyY);
                            break;
                        case SDLK_z:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyZ);
                            break;
                        default:
                            break;
                        }
                    }

                // Process non-printable keypresses using virtual keys, see
                // above comments for ESC key.
                else if(current_event.type == SDL_KEYDOWN) {
                    // Only unmodified version of these keys are handled
                    // eg. NOT catching Shift + F1 .... etc
                    switch(current_event.key.keysym.sym) {
                        case SDLK_F1:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF1);
                            break;
                        case SDLK_F2:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF2);
                            break;
                        case SDLK_F3:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF3);
                            break;
                        case SDLK_F4:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF4);
                            break;
                        case SDLK_F5:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF5);
                            break;
                        case SDLK_F6:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF6);
                            break;
                        case SDLK_F7:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF7);
                            break;
                        case SDLK_F8:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF8);
                            break;
                        case SDLK_F9:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF9);
                            break;
                        case SDLK_F10:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF10);
                            break;
                        case SDLK_F11:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF11);
                            break;
                        case SDLK_F12:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyF12);
                            break;
                        case SDLK_SPACE:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeySpace);
                            break;
                        case SDLK_RETURN:
                            break;
                        case SDLK_KP_1:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP1);
                            break;
                        case SDLK_KP_2:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP2);
                            break;
                        case SDLK_KP_3:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP3);
                            break;
                        case SDLK_KP_4:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP4);
                            break;
                        case SDLK_KP_5:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP5);
                            break;
                        case SDLK_KP_6:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP6);
                            break;
                        case SDLK_KP_8:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP8);
                            break;
                        case SDLK_KP_0:
                            eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyKP0);
                            break;
                        case SDLK_KP_PERIOD:
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
                                   << ErrorHandler::check_SDL2_Error(__FILE__, __LINE__) << endl;
            ErrorHandler::record(icon_surface_error_msg.str(), ErrorHandler::WARN);
            }

        }
    else {
        stringstream icon_load_error_msg;
        icon_load_error_msg << "Could not prepare window icon data : "
                            << ErrorHandler::check_SDL2_Error(__FILE__, __LINE__) << endl;
        ErrorHandler::record(icon_load_error_msg.str(), ErrorHandler::WARN);
        }
    }

bool WindowManager::initializeGLEW(void) {
    // Assume failure.
    bool ret_val = false;

    // Give GLEW the best chance of finding functionality with experimental drivers,
    // or more to the point as of 31/12/2014 (ie. v1.10.0) you won't find any with core
    // contexts unless you do this.
    glewExperimental = GL_TRUE;

    // Now initialise GLEW.
    GLenum init_flag = glewInit();

    // Check for any induced OpenGL error. As of 31/12/2014 ie. v1.10.0 this will happen
    // with core context acquisition. AFAIW this has no material effect but clear the
    // error state in any case.
    ErrorHandler::check_OpenGL_Error(__FILE__, __LINE__);

    if(init_flag != GLEW_OK) {
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
    // Instantiate and populate a user event structure.
    SDL_Event event;

    // SDL_zero is a 'secret' SDL macro that sets the contents
    // of an object to binary zero values. See 'SDL_stdinc_h'.
    SDL_zero(event);
    event.type = SDL_USEREVENT;;
    event.user.code = WindowManager::RenderEvent;
    event.user.data1 = 0;
    event.user.data2 = 0;

    /// TODO - Handle event push failure ( likely full queue ) ??
    SDL_PushEvent(&event);

    // Don't change interval to next callback.
    return interval;
    }

Uint32 WindowManager::timerCallbackBOINCUpdateEvent(Uint32 interval, void* param) {
    // Instantiate and populate a user event structure.
    SDL_Event event;

    // SDL_zero is a 'secret' SDL macro that sets the contents
    // of an object to binary zero values. See 'SDL_stdinc_h'.
    SDL_zero(event);
    event.type = SDL_USEREVENT;;
    event.user.code = WindowManager::BOINCUpdateEvent;
    event.user.data1 = 0;
    event.user.data2 = 0;

    /// TODO - Handle event push failure ( likely full queue ) ??
    SDL_PushEvent(&event);

    // Don't change interval to next callback.
    return interval;
    }

void WindowManager::toggleFullscreen(void) {
    if(m_CurrentScreenMode == WindowManager::WINDOWED) {
        if(SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN) != 0){
            ErrorHandler::record("WindowManager::toggleFullscreen() : Couldn't toggle to fullscreen !", ErrorHandler::WARN);
            }
        else {
            m_CurrentScreenMode = WindowManager::FULLSCREEN;
            }
        }
    else {
        if(SDL_SetWindowFullscreen(m_Window, WindowManager::WINDOW_PLEASE) != 0){
            ErrorHandler::record("WindowManager::toggleFullscreen() : Couldn't toggle to window !", ErrorHandler::WARN);
            }
        else {
            m_CurrentScreenMode = WindowManager::WINDOWED;
            }
        }
    }

void WindowManager::setScreensaverMode(const bool enabled) {
    m_ScreensaverMode = enabled;
    }

void WindowManager::swap(void) const {
    SDL_DEBUG(SDL_GL_SwapWindow(m_Window));
    }

void WindowManager::setContextAttributes(void) {
	/// TODO - find a way to request vsync in platform independent way !!
	/// Neither Open GL nor SDL ( cleanly ) provide this. Maybe roll my own
	/// abstraction ?

	// Request a minimum number of multisample buffers.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, WindowManager::NUM_MULTISAMPLE_BUFFERS));

	// Request a minimum of multisamples ( around a given pixel ).
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, WindowManager::NUM_MULTISAMPLES));

	// Request a specific color depth.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, WindowManager::RED_BITS));
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, WindowManager::GREEN_BITS));
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, WindowManager::BLUE_BITS));

	// Request a specific alpha channnel.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, WindowManager::ALPHA_BITS));

	// Request double buffering.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, WindowManager::ENABLE_DOUBLE_BUFFER));

	// Request a minimum number of bits in depth buffer.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, WindowManager::DEPTH_BITS));

	// Create a desired OpenGL context for use with that window,
	// noting the above attribute selections.
	/// TODO - Need to create compile switch here for use of ES with Android etc.
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, WindowManager::CONTEXT_PROFILE_TYPE));
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, WindowManager::OGL_MAJOR_VERSION));
	SDL_DEBUG(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, WindowManager::OGL_MINOR_VERSION));
	}

void WindowManager::checkContextAttributes(void) {
	// Check the minimum number of multisample buffers.
	int SDL_MultiSampleBufferFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &SDL_MultiSampleBufferFlag));
	stringstream msg_MultiSampleBuffer;
	msg_MultiSampleBuffer << "SDL_GL_MULTISAMPLEBUFFERS granted = "
						  << SDL_MultiSampleBufferFlag
						  << " ( "
						  << WindowManager::NUM_MULTISAMPLE_BUFFERS
						  << " requested )";
	ErrorHandler::record(msg_MultiSampleBuffer.str(), ErrorHandler::INFORM);

	// Check the minimum of multisamples ( around a given pixel ).
	int SDL_MultiSampleSamplesFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &SDL_MultiSampleSamplesFlag));
	stringstream msg_MultiSampleSample;
	msg_MultiSampleSample << "SDL_GL_MULTISAMPLESAMPLES granted = "
						  << SDL_MultiSampleSamplesFlag
						  << " ( "
						  << WindowManager::NUM_MULTISAMPLES
						  << " requested )";
	ErrorHandler::record(msg_MultiSampleSample.str(), ErrorHandler::INFORM);

	// Check the specific color depths.
	// Check the red color depth.
	int SDL_RedSizeFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &SDL_RedSizeFlag));
	stringstream msg_RedSize;
	msg_RedSize << "SDL_GL_RED_SIZE granted = "
				<< SDL_RedSizeFlag
				<< " ( "
				<< WindowManager::RED_BITS
				<< " requested )";
	ErrorHandler::record(msg_RedSize.str(), ErrorHandler::INFORM);

	// Check the green color depth.
	int SDL_GreenSizeFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &SDL_GreenSizeFlag));
	stringstream msg_GreenSize;
	msg_GreenSize << "SDL_GL_GREEN_SIZE granted = "
				  << SDL_GreenSizeFlag
				  << " ( "
				  << WindowManager::GREEN_BITS
				  << " requested )";
	ErrorHandler::record(msg_GreenSize.str(), ErrorHandler::INFORM);

	// Check the blue color depth.
	int SDL_BlueSizeFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &SDL_BlueSizeFlag));
	stringstream msg_BlueSize;
	msg_BlueSize << "SDL_GL_BLUE_SIZE granted = "
				 << SDL_BlueSizeFlag
				 << " ( "
				 << WindowManager::BLUE_BITS
				 << " requested )";
	ErrorHandler::record(msg_BlueSize.str(), ErrorHandler::INFORM);

	// Check the alpha channnel.
	int SDL_AlphaSizeFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &SDL_AlphaSizeFlag));
	stringstream msg_AlphaSize;
	msg_AlphaSize << "SDL_GL_ALPHA_SIZE granted = "
				  << SDL_AlphaSizeFlag
				  << " ( "
				  << WindowManager::ALPHA_BITS
				  << " requested )";
	ErrorHandler::record(msg_AlphaSize.str(), ErrorHandler::INFORM);

	// Check double buffering.
	int SDL_DoubleBufferFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &SDL_DoubleBufferFlag));
	stringstream msg_DoubleBuffer;
	msg_DoubleBuffer << "SDL_GL_DOUBLEBUFFER granted = "
					 << SDL_DoubleBufferFlag
					 << " ( "
					 << WindowManager::ENABLE_DOUBLE_BUFFER
					 << " requested )";
	ErrorHandler::record(msg_DoubleBuffer.str(), ErrorHandler::INFORM);

	// Check the minimum number of bits in depth buffer.
	int SDL_DepthSizeFlag = -707;
	SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &SDL_DepthSizeFlag));
	stringstream msg_DepthSize;
	msg_DepthSize << "SDL_GL_DEPTH_SIZE granted = "
			      << SDL_DepthSizeFlag
				  << " ( "
				  << WindowManager::DEPTH_BITS
				  << " requested )";
	ErrorHandler::record(msg_DepthSize.str(), ErrorHandler::INFORM);

	// Check the OpenGL context profile achieved.
	int SDL_ContextProfile = -707;
    SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &SDL_ContextProfile));
    stringstream msg_ContextProfile;
    msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_MASK granted = ";
    switch(SDL_ContextProfile) {
    	case SDL_GL_CONTEXT_PROFILE_CORE:
			msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_CORE";
			break;
    	case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
    		msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY";
    		break;
    	case SDL_GL_CONTEXT_PROFILE_ES:
    	    msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_ES";
    	    break;
	    default:
	    	ErrorHandler::record("checkContextAttributes() : Bad switch case ( SDL_ContextProfile, default ) !", ErrorHandler::FATAL);
		    break;
    	}
    msg_ContextProfile << " ( ";
    switch(WindowManager::CONTEXT_PROFILE_TYPE) {
    	case SDL_GL_CONTEXT_PROFILE_CORE:
    		msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_CORE";
    		break;
        case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
        	msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY";
        	break;
        case SDL_GL_CONTEXT_PROFILE_ES:
            msg_ContextProfile << "SDL_GL_CONTEXT_PROFILE_ES";
            break;
    	default:
    	   	ErrorHandler::record("checkContextAttributes() : Bad switch case ( WindowManager::CONTEXT_PROFILE_TYPE, default ) !", ErrorHandler::FATAL);
    	    break;
        }
    msg_ContextProfile << " requested )";
    ErrorHandler::record(msg_ContextProfile.str(), ErrorHandler::INFORM);

    // Check the OpenGL context major version achieved.
    int SDL_ContextMajorVersion = -707;
    SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &SDL_ContextMajorVersion));
    stringstream msg_ContextMajorVersion;
    msg_ContextMajorVersion << "SDL_GL_CONTEXT_MAJOR_VERSION = "
    						<< SDL_ContextMajorVersion
							<< " ( "
							<< WindowManager::OGL_MAJOR_VERSION
							<< " requested )";
    ErrorHandler::record(msg_ContextMajorVersion.str(), ErrorHandler::INFORM);

    // Check the OpenGL context minor version achieved.
    int SDL_ContextMinorVersion = -707;
    SDL_DEBUG(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &SDL_ContextMinorVersion));
    stringstream msg_ContextMinorVersion;
    msg_ContextMinorVersion << "SDL_GL_CONTEXT_MINOR_VERSION = "
    						<< SDL_ContextMinorVersion
							<< " ( "
							<< WindowManager::OGL_MINOR_VERSION
							<< " requested )";
    ErrorHandler::record(msg_ContextMinorVersion.str(), ErrorHandler::INFORM);
	}
