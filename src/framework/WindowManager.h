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

using namespace std;

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
         * project preferences!
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
        void registerEventObserver(AbstractGraphicsEngine *engine);

        /**
         * \brief Unregisters an event observer.
         *
         * \param engine The pointer to the \ref AbstractGraphicsEngine instance
         *        to unregister.
         */
        void unregisterEventObserver(AbstractGraphicsEngine *engine);

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
        void setWindowCaption(const string caption) const;

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
        void setWindowIcon(const string filename) const;

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
        void setWindowIcon(const unsigned char *data, const int size) const;

    private:
        /**
         * \brief Initialise the GLEW system, essentially establishing
         *        dynamic linking to the video driver for OpenGL functionality.
         *
         * \return boolean indicating success ( TRUE ) or failure ( FALSE )
         */
        bool initializeGLEW(void);

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

        /// The event observer registry.
        list<AbstractGraphicsEngine *> eventObservers;
    };

/**
 * @}
 */

#endif /*WINDOWMANAGER_H_*/
