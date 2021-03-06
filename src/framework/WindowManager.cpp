/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include "WindowManager.h"

WindowManager::WindowManager()
{
	m_ScreensaverMode = false;
	m_BoincAdapter = new BOINCClientAdapter("");
}

WindowManager::~WindowManager()
{
	delete m_BoincAdapter;
}

bool WindowManager::initialize(const int width, const int height, const int frameRate)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		cerr << "Window system could not be initalized: " << SDL_GetError() << endl;
		return false;
	}

	atexit(SDL_Quit);

	// retrieve current video settings
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();

	if (videoInfo->current_w != 0) {
		m_DesktopWidth = videoInfo->current_w;
	}

	if (videoInfo->current_h != 0) {
		m_DesktopHeight = videoInfo->current_h;
	}

	if (videoInfo->vfmt->BitsPerPixel != 0) {
		m_DesktopBitsPerPixel = videoInfo->vfmt->BitsPerPixel;
	}

	// get initial non-fullscreen resolution and frame rate from project preferences
	m_BoincAdapter->initialize();
	int preferredWidth = m_BoincAdapter->graphicsWindowWidth();
	int preferredHeight = m_BoincAdapter->graphicsWindowHeight();
	int preferredFrameRate = m_BoincAdapter->graphicsFrameRate();

	// override optional default values if preferred values are set
	m_WindowedWidth = preferredWidth != 0 ? preferredWidth : width;
	m_WindowedHeight = preferredHeight != 0 ? preferredHeight : height;
	m_RenderEventInterval = 1000.0f / (preferredFrameRate != 0 ? preferredFrameRate : frameRate);

	/*
	 * SDL_ASYNCBLIT - Surface benutzt asynchrone Blits, wenn möglich
	 * SDL_ANYFORMAT - Erlaubt jedes Pixel-Format (nur beim display-surface)
	 * SDL_FULLSCREEN - Surface im Full-Screen-Mode initialisieren (nur display-surface)
	 * SDL_OPENGL - Surface nutzt OpenGL (nur display-surface)
	 * SDL_RESIZABLE - Surfacefenster ist veränderbar (nur display-Surface)
	 * SDL_HWACCEL- Surface blit nutzt Hardwarebeschleunigung
	 * SDL_SRCCOLORKEY - Surface nutzt colorkey blitting
	 * SDL_RLEACCEL - Colorkey blitting ist durch RLE beschleunigt
	 * SDL_SRCALPHA - Surface blit nutzt alpha blending
	 * SDL_PREALLOC - Surface nutzt vorher allokierten Speicher
	 */

	// set common video flags
	// (for OpenGL nothing more than SDL_OPENGL and SDL_FULLSCREEN should be used)
	m_VideoModeFlags = SDL_OPENGL;

	// check fullscreen video mode
	m_FullscreenModeAvailable = true;
	Uint32 bitPerPixel = SDL_VideoModeOK(
							m_DesktopWidth,
							m_DesktopHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags | SDL_FULLSCREEN);

	if(!bitPerPixel) {
		cerr << "Fullscreen video mode not supported: " << SDL_GetError() << endl;
		m_FullscreenModeAvailable = false;
	}

	// check initial windowed video mode
	m_WindowedModeAvailable = true;
	bitPerPixel = SDL_VideoModeOK(
							m_WindowedWidth,
							m_WindowedHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags | SDL_RESIZABLE);

	if(!bitPerPixel) {
		cerr << "Windowed video mode not supported: " << SDL_GetError() << endl;
		m_WindowedModeAvailable = false;
	}

	// both checks failed
	if(!(m_FullscreenModeAvailable || m_WindowedModeAvailable)) {
		cerr << "No suitable video mode available!"<< endl;
		return false;
	}

	// minimum requirements
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	// desired requirement for high quality mode
	//FIXME: commented out right now as it interferes with the FSAA attributes below!)
//	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if(m_BoincAdapter->graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
		// enable opt-in quality feature FSAA (4x)
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	}

	// unused requirements
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// we always start in windowed mode (starting in fullscreen fails with high CPU load!)
	m_CurrentWidth = m_WindowedWidth;
	m_CurrentHeight = m_WindowedHeight;
	m_VideoModeFlags |= SDL_RESIZABLE;

	// finally, get surface
	m_DisplaySurface = SDL_SetVideoMode(
							m_CurrentWidth,
							m_CurrentHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags);

	if (m_DisplaySurface == NULL) {
		cerr << "Could not acquire rendering surface (" << SDL_GetError() << "): will try fallback..." << endl;
	}

	// check if we got acceleration
	int accelerated = 0;
	if(SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &accelerated) == -1) {
		cerr << "Could not ensure accelerated rendering surface. Assuming no acceleration..." << endl;
	}

	if (m_DisplaySurface == NULL || !accelerated) {
		cerr << "Disabling high quality features..." << endl;

		// disable features that demand acceleration
//		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);

		// TODO: we should override m_BoincAdapter->graphicsQualitySetting() to medium or low!
		// note, requires to extend starsphere's constructor (uses its own BOINCClientAdapter!)

		// reset display surface
		m_DisplaySurface = SDL_SetVideoMode(
								m_CurrentWidth,
								m_CurrentHeight,
								m_DesktopBitsPerPixel,
								m_VideoModeFlags);

		if (m_DisplaySurface == NULL) {
			cerr << "Could not acquire rendering surface (" << SDL_GetError() << "): giving up!" << endl;
			return false;
		}
	}

	return true;
}

void WindowManager::eventLoop()
{
	// be sure there's at least one observer!
	assert(eventObservers.size() > 0);

	// set two main timers (interval in ms)
	SDL_AddTimer(m_RenderEventInterval, &timerCallbackRenderEvent, NULL);
	SDL_AddTimer(1000, &timerCallbackBOINCUpdateEvent, NULL);

	// events we don't ignore, hence use
	//SDL_EventState(SDL_QUIT, SDL_IGNORE);
	//SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	//SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	//SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	//SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
	//SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	// events we ignore
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

	SDL_Event event;

	while (SDL_WaitEvent(&event) ) {
		if (event.type == SDL_USEREVENT &&
			event.user.code == RenderEvent) {

#ifdef DEBUG_VALGRIND
			// stop after i iterations when running valgrinded
			static int i = 0;
			if(i < 500) {
				i++;
#endif
				// notify our observers (currently exactly one, hence front())
				eventObservers.front()->render(dtime());
#ifdef DEBUG_VALGRIND
			}
			else {
				if (m_DisplaySurface) SDL_FreeSurface(m_DisplaySurface);
				break;
			}
#endif
		}
		else if (event.type == SDL_USEREVENT &&
				 event.user.code == BOINCUpdateEvent) {

			// notify observers (currently exactly one, hence front()) to fetch a BOINC update
			eventObservers.front()->refreshBOINCInformation();
		}
		else if (m_ScreensaverMode &&
				(event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN ||
				 event.type == SDL_KEYDOWN)) {

			// we're in screensaver mode so exit on user input
			SDL_Quit();
		}
		else if (event.motion.state & (SDL_BUTTON(1) | SDL_BUTTON(3)) &&
				 event.type == SDL_MOUSEMOTION) {

			if (event.motion.state & SDL_BUTTON(1)) {
				// notify our observers (currently exactly one, hence front())
				eventObservers.front()->mouseMoveEvent(
										event.motion.xrel,
										event.motion.yrel,
										AbstractGraphicsEngine::MouseButtonLeft);
			}
			else if (event.motion.state & SDL_BUTTON(3)) {
				// notify our observers (currently exactly one, hence front())
				eventObservers.front()->mouseMoveEvent(
										event.motion.xrel,
										event.motion.yrel,
										AbstractGraphicsEngine::MouseButtonRight);
			}
		}
		else if (event.type == SDL_VIDEORESIZE) {
			m_CurrentWidth = m_WindowedWidth = event.resize.w;
			m_CurrentHeight = m_WindowedHeight = event.resize.h;

			// update video mode
			m_DisplaySurface = SDL_SetVideoMode(
									m_CurrentWidth,
									m_CurrentHeight,
									m_DesktopBitsPerPixel,
									m_VideoModeFlags);

			// notify our observers (currently exactly one, hence front())
			// (windoze needs to be reinitialized instead of just resized, oh well)
			/// \todo Can we determine the host OS? On X11 a resize() is sufficient!
			eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
		}
		else if (event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {

			// just exit (SDL_FreeSurface is called automatically)
			SDL_Quit();

			break;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				// notify our observers (currently exactly one, hence front())
				case SDLK_s:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyS);
					break;
				case SDLK_c:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyC);
					break;
				case SDLK_o:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyO);
					break;
				case SDLK_x:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyX);
					break;
				case SDLK_p:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyP);
					break;
				case SDLK_r:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyR);
					break;
				case SDLK_g:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyG);
					break;
				case SDLK_a:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyA);
					break;
				case SDLK_i:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyI);
					break;
				case SDLK_l:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyL);
					break;
				case SDLK_m:
					eventObservers.front()->keyboardPressEvent(AbstractGraphicsEngine::KeyM);
					break;
				case SDLK_RETURN:
					toggleFullscreen();
				default:
					break;
			}
		}
	}
}

void WindowManager::registerEventObserver(AbstractGraphicsEngine *engine)
{
	// right now we're only accepting/using ONE observer
	eventObservers.assign(1, engine);
}

void WindowManager::unregisterEventObserver(AbstractGraphicsEngine *engine)
{
	eventObservers.remove(engine);
}

Uint32 WindowManager::timerCallbackRenderEvent(Uint32 interval, void *param)
{
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

Uint32 WindowManager::timerCallbackBOINCUpdateEvent(Uint32 interval,
        void *param)
{
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

int WindowManager::windowWidth() const
{
	return m_CurrentWidth;
}

int WindowManager::windowHeight() const
{
	return m_CurrentHeight;
}

void WindowManager::setWindowCaption(const string caption) const
{
	SDL_WM_SetCaption(caption.c_str(), NULL);
}

void WindowManager::setWindowIcon(const string filename) const
{
	if (filename.length() > 0) {
		SDL_WM_SetIcon(SDL_LoadBMP(filename.c_str()), NULL);
	}
}

void WindowManager::setWindowIcon(const unsigned char *data, const int size) const
{
	// prepare data buffer structure
	SDL_RWops *buffer = SDL_RWFromMem((void*) data, size);

	if(buffer != NULL) {
		// load BMP from prepared data buffer
		SDL_Surface *surface = SDL_LoadBMP_RW(buffer, 1);

		if(surface != NULL) {
			// set window icon
			SDL_WM_SetIcon(surface, NULL);
			SDL_FreeSurface(surface);
		}
		else {
			cerr << "Could not create window icon surface: " << SDL_GetError() << endl;
		}
	}
	else {
		cerr << "Could not prepare window icon data: " << SDL_GetError() << endl;
	}
}

void WindowManager::toggleFullscreen()
{
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
        } else
#endif
        {
            m_VideoModeFlags |= SDL_FULLSCREEN;
        }
		m_VideoModeFlags &= ~SDL_RESIZABLE;

		// hide cursor
		SDL_ShowCursor(SDL_DISABLE);
	}

	// reset video mode
	m_DisplaySurface = SDL_SetVideoMode(
							m_CurrentWidth,
							m_CurrentHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags);

	// notify our observers (currently exactly one, hence front())
	// (windoze needs to be reinitialized instead of just resized, oh well)
	/// \todo Can we determine the host OS? On X11 a resize() is sufficient!
	eventObservers.front()->initialize(m_CurrentWidth, m_CurrentHeight, 0, true);
}

void WindowManager::setScreensaverMode(const bool enabled)
{
	m_ScreensaverMode = enabled;
}
