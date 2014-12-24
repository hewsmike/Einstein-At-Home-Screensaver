/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   As amended 2013 by Mike Hewson                                        *
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

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include "framework.h"

#include <string>
#include <list>
#include <util.h>

#include "AbstractGraphicsEngine.h"
#include "BOINCClientAdapter.h"

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief This class is responsible for the application's window and event
 *        management.
 *
 *      %WindowManager provides an initialized OpenGL context needed by any
 * given \ref AbstractGraphicsEngine. In addition to that it serves as the main
 * event controller. That means it handles all window and user input \ref Events and
 * propagates them to all registered observers of type \ref
 * AbstractGraphicsEngine. This also includes all timer \ref Events required for
 * rendering and information retrieval control.
 *
 * \see AbstractGraphicsEngine
 * \see BOINCClientAdapter
 * \see ErrorHandler
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 *
 * \author Mike Hewson
 * hewsmike[AT]iinet.net.au
 */

class WindowManager {
    public:

        /**
         * \brief Constructor.
         */
        WindowManager(void);

        /// Destructor
        virtual ~WindowManager();

        /**
         * \brief Initializes the %WindowManager.
         *
         *      Call this method first ( after instantiation ) to prepare the
         * main application window as well as the OpenGL context. Please
         * note that the optional parameters \c width, \c height and \c
         * frameRate will be overridden by the values set by the user in the
         * project preferences! Note that SDL MUST be successfully
         * initialised prior to use of this method.
         *
         * \param width The optional initial width of the main window.
         * \param height The optional initial height of the main window.
         * \param frameRate The optional initial frame rate to be used.
         *
         * \return TRUE if successful, otherwise FALSE
         */
        bool initialize(const int width = 800,
                        const int height = 600,
                        const int frameRate = 20);

        /**
         * \brief Registers a new event observer.
         *
         *      All registered observers are notified in case one of the events
         * specified in \ref EventCodes occurrs.
         *
         * \param engine The pointer to the \ref AbstractGraphicsEngine instance
         *        to register.
         *
         * \see AbstractGraphicsEngine::mouseButtonEvent()
         * \see AbstractGraphicsEngine::mouseMoveEvent()
         * \see AbstractGraphicsEngine::keyboardPressEvent()
         */
        void registerEventObserver(AbstractGraphicsEngine* engine);

        /**
         * \brief Unregisters an event observer.
         *
         * \param engine The pointer to the \ref AbstractGraphicsEngine instance
         *        to unregister.
         */
        void unregisterEventObserver(AbstractGraphicsEngine* engine);

        /**
         * \brief The main event loop.
         *
         *      Call this method to enter the main window's event loop. All
         * subsequent application control is defined here. The method returns
         * when the window is closed or destroyed.
         */
        void eventLoop(void);

        /**
         * \brief Retrieve the current main window's width.
         *
         * \return The current window width.
         */
        int windowWidth(void) const;

        /**
         * \brief Retrieve the current main window's height.
         *
         * \return The current window height.
         */
        int windowHeight(void) const;

        /**
         * \brief Set the main window's caption.
         *
         * \param caption The new caption of the main window.
         */
        void setWindowCaption(const std::string& caption) const;

        /**
         * \brief Set the main window's icon.
         *
         * This method uses the provided filename to load a bitmap (BMP) image
         * from disk which in turn is displayed as the main window's icon.
         *
         * Note: The icon should have a size of 32x32 pixels!
         *
         * \param filename The new icon's filename.
         */
        /// TODO - should we delete this method to enforce that the
        /// final executable must contain all it's ( constant ) resources ??
        void setWindowIcon(const std::string& filename) const;

        /**
         * \brief Set the main window's icon.
         *
         * This method uses the provided raw data pointer to load a bitmap
         * (BMP) image from memory which in turn is displayed as the main
         * window's icon.
         *
         * Note: The icon should have a size of 32x32 pixels!
         *
         * \param data Pointer to the bitmap data buffer
         * \param size Size of the bitmap data buffer
         */
        void setWindowIcon(const unsigned char* data, const int size) const;

        /**
         * \brief Toggles the fullscreen state of the main window
         *
         * Note: the initial state is windowed (not fullscreen).
         *
         */
        void toggleFullscreen(void);

        /**
         * \brief Set the screensaver mode indicator.
         *
         * When enabled, all user input will cause the application to quit
         * (common screensaver behavior)
         *
         * \param enabled The new value for the screensaver mode indicator
         */
        void setScreensaverMode(const bool enabled);

    private:
        /// Most of these statics are to remove magic numbers.
        /// The window's title/caption.
        static const std::string m_WindowTitle;

        /// OpenGL framebuffer characteristics.
        static const int RED_BITS;
        static const int GREEN_BITS;
        static const int BLUE_BITS;
        static const int ALPHA_BITS;
        static const int DEPTH_BITS;
        static const int HAS_DEPTH_BUFFER;
        static const int ENABLE_VERTICAL_SYNC;
        static const int ENABLE_DOUBLE_BUFFER;
        static const int NUM_MULTISAMPLE_BUFFERS;
        static const int NUM_MULTISAMPLES;

        /// OpenGL type and version.
        static const int CONTEXT_PROFILE_TYPE;
        static const int OGL_MAJOR_VERSION;
        static const int OGL_MINOR_VERSION;

        /// Can have multiple displays, we only want one !!
        static const int DISPLAY_ZERO;

        /// Was an event retrieval successful.
        static const int EVENT_PENDING;

        /// Want a window, not fullscreen.
        static const int WINDOW_PLEASE;

        /// Mouse buttons.
        static const int LEFT_MOUSE_BUTTON;
        static const int MIDDLE_MOUSE_BUTTON;
        static const int RIGHT_MOUSE_BUTTON;

        /// For user timer delays.
        static const float TIMER_DELAY_BOINC;
        static const float MILLISECONDS_PER_SECOND;

        /**
         * \brief Swap the buffers
         */
        void swap(void) const;

        /**
         * \brief Timer callback to trigger render events
         *
         * This callback is used by an SDL timer registered in \ref eventLoop().
         * It creates a \ref RenderEvent which is handled by eventLoop().
         * In order to use a constant timer interval, \c interval is
         * returned as it was passed.
         *
         * \param interval The current timer interval
         * \param param The user supplied parameter of the timer event
         *
         * \return The timer interval to be used for the following events
         *
         * \see eventLoop()
         * \see EventCodes
         *
         * \todo Work around static callback, otherwise we might get event conflicts
         * when more than one instance. Maybe we should use a singleton here anyway...
         */
        static Uint32 timerCallbackRenderEvent(Uint32 interval, void* param);

        /**
         * \brief Timer callback to trigger BOINC update events
         *
         * This callback is used by an SDL timer registered in \ref eventLoop().
         * It creates a \ref BOINCUpdateEvent which is handled by eventLoop().
         * In order to use a constant timer interval, \c interval is
         * returned as it was passed.
         *
         * Note: it might seem a bit strange to trigger the BOINC updates here but it's
         * here where \b all event controlling and propagation takes place. BOINCClientAdapter
         * for example is meant to be used \b by an instance receiving this event, not
         * actually handling it itself. Thus AbstractGraphicsEngine handles this event
         * and \b uses its BOINC adapter accordingly.
         *
         * \param interval The current timer interval
         * \param param The user supplied parameter of the timer event
         *
         * \return The timer interval to be used for the following events
         *
         * \see eventLoop()
         * \see EventCodes
         *
         * \todo Work around static callback, otherwise we might get event conflicts
         * when more than one instance. Maybe we should use a singleton here anyway...
         */
        static Uint32 timerCallbackBOINCUpdateEvent(Uint32 interval, void* param);

        /**
         * \brief Initialise the GLEW system, essentially establishing
         *        dynamic linking to the video driver for OpenGL functionality.
         *
         * \return boolean indicating success ( TRUE ) or failure ( FALSE )
         */
        bool initializeGLEW(void);

        /**
         * \brief Set the desired attibutes for the OpenGL context.
         */
        void setContextAttributes(void);

        /**
         * \brief Check what the desired attibutes for the OpenGL context are.
         */
        void checkContextAttributes(void);

        /// The user's desktop mode.
        SDL_DisplayMode* m_Mode;

        /// Pointer to the window created by SDL.
        SDL_Window* m_Window;

        /// A window identifier for possible logging and interrogation.
        Uint32 m_WindowID;

        /// OpenGL context associated with the window.
        SDL_GLContext m_Context;

        /// Local BOINC adapter instance to read project preferences
        BOINCClientAdapter* m_BoincAdapter;

        /// The render event interval (in ms) for invoking the render event observer
        float m_RenderEventInterval;

        /// The current width of the host's desktop
        int m_DesktopWidth;

        /// The current height of the host's desktop
        int m_DesktopHeight;

        /// The current bits per pixel value of the host's desktop
        int m_DesktopBitsPerPixel;

        /// The current width of the application window (windowed and/or fullscreen)
        int m_CurrentWidth;

        /// The current height of the application window (windowed and/or fullscreen)
        int m_CurrentHeight;

        /// The width of the application window (windowed mode only)
        int m_WindowedWidth;

        /// The height of the application window (windowed mode only)
        int m_WindowedHeight;

        /// The screensaver mode indicator ie. was it requested ?
        bool m_ScreensaverMode;

        /// Are we currently in fullscreen or windowed ?
        enum screen_mode {
            WINDOWED,
            FULLSCREEN,
            };

        screen_mode m_CurrentScreenMode;

         /**
          * \brief The known event codes handled by %eventLoop()
          *
          * The values stored here MUST be obtained via SDL_RegisterEvents().
          *
          * \see eventLoop()
          * \see timerCallbackRenderEvent()
          * \see timerCallbackBOINCUpdateEvent()
          */
        static Uint32 RenderEvent;
        static Uint32 BOINCUpdateEvent;

        /// The event observer registry.
        list<AbstractGraphicsEngine*> eventObservers;
    };

/**
 * @}
 */

#endif /*WINDOWMANAGER_H_*/
