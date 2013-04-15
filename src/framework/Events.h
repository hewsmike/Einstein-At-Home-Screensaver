/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#ifndef EVENTS_H_
#define EVENTS_H_

#include "framework.h"

#include <list>
#include <memory>

#include "TriggerTimer.h"

// The various events to manage, with their individual parameters.

struct BOINCUpdateEvent {
    GLuint type;
    };

struct CharInputEvent {
    GLuint type;
    int char_code;
    bool pressed;
    };

struct KeyPressEvent {
    GLuint type;
    int key_code;
    bool pressed;
    };

struct MouseButtonEvent {
    GLuint type;
    int button;
    };

struct MouseMotionEvent {
    GLuint type;
    int xrel;
    int yrel;
    };

struct MouseWheelEvent {
    GLuint type;
    int diff_pos;
    };

struct RenderEvent {
    GLuint type;
    };

struct ResizeEvent {
    GLuint type;
    GLuint width;
    GLuint height;
    };

struct QuitEvent {
    GLuint type;
    };

// Unite the events.
union Event {
    GLuint type;
    BOINCUpdateEvent b_update;
    CharInputEvent c_input;
    KeyPressEvent k_press;
    MouseMotionEvent m_motion;
    MouseButtonEvent m_button;
    MouseWheelEvent m_wheel;
    RenderEvent render;
    ResizeEvent resize;
    QuitEvent quit;
    };

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief This class is responsible for event capture and management
 *        of an event queue, currently utilising GLFW.
 *
 *  Based upon the design of Mr Paul Whitehead :
 *
 *     A Leak-Free Singleton Class
 *     http://www.codeguru.com/cpp/cpp/cpp_mfc/singletons/article.php/c755/A-LeakFree-Singleton-Class.htm
 *
 * The idea being to improve upon the Singleton paradigm of
 *
 *     Design Patterns: Elements of Reusable Object-Oriented Software
 *     by Erich Gamma; Richard Helm; Ralph Johnson; John Vlissides
 *
 * ... see p127+ thereof. Memory leakage is avoided, as destruction
 * of a ( static ) pointer doesn't per se de-allocate heap upon
 * program exit.
 */

class Events {
    public:
        static const int INITIAL_WHEEL_POSITION;

        enum eventType {BOINCUpdateEventType,
                        CharInputEventType,
                        KeyPressEventType,
                        MouseMotionEventType,
                        MouseButtonEventType,
                        MouseWheelEventType,
                        RenderEventType,
                        ResizeEventType,
                        QuitEventType};

        enum buttonType {LEFT_BUTTON,
                         MIDDLE_BUTTON,
                         RIGHT_BUTTON};

        /**
         * \brief Retrieve ( maybe create ) the singleton, possibly
         *        initialising event management.
         *
         * \param render_interval : number of milliseconds between
         *                          render calls, only relevant for
         *                          the first call to this routine.
         */
        static Events* Instance(GLuint render_interval);

        /**
         * \brief Get the next event, if any, from the queue ( non-blocking ).
         *
         * \param ev  : pointer to an Event which may be filled with
         *              the detail of said event.
         * \return boolean indicating :
         *                  true, if an event was available, in which
         *                        case it will be removed and the queue
         *                        will have one fewer.
         *                  false, if no event was waiting to be read.
         */
        static bool next(Event* ev);

        /**
         * \brief Keep our special timers ticking over.
         */
        static void tick(void);

        /**
         * \brief Discard all events currently in the queue.
         */
        static void flush(void);

    private:
        /// Prevent direct construction, copying and assignment.
        Events(void);
        Events(const Events& other);
        Events& operator=(const Events& other);

        /// Pointer to any single instance.
        static std::auto_ptr<Events> p_instance;

        /// Intervals in milliseconds for our special timers.
        static const GLuint BOINC_CALLBACK_INTERVAL;
        static const GLuint RENDER_CALLBACK_INTERVAL_MIN;

        /**
         * \brief Initialise this event manager.
         *
         * \param render_interval : desired number of milliseconds
         *                          between render calls.
         */
        static void init(GLuint render_interval);

        /**
         * \brief Callback for a BOINC update event.
         */
        static void boincUpdate(void);

        /**
         * \brief Callback for a render event.
         */
        static void render(void);

        /**
         * \brief Callback for a character input event.
         *
         * \param character : from Unicode ( ISO 10646 ).
         * \param action : one of GLFW_PRESS or GLFW_RELEASE.
         */
        static void GLFWCALL charInput(int character, int action);

        /**
         * \brief Callback for a key event.
         *
         * \param character : uppercase printable ISO 8859-1 character
         *                    a special key identifier per GLFW.
         * \param action : one of GLFW_PRESS or GLFW_RELEASE.
         */
        static void GLFWCALL keyPress(int key, int action);

        /**
         * \brief Callback for a mouse button event.
         *
         * \param button - one of the GLFW mouse button identifiers.
         * \param action - one of GLFW_PRESS or GLFW_RELEASE.
         */
        static void GLFWCALL mouseButton(int button, int action);

        /**
         * \brief Callback for a mouse motion event.
         *
         * \param x - mouse position in the 'horizontal' axis.
         * \param y - mouse position in the 'vertical' axis.
         */
        static void GLFWCALL mouseMotion(int x, int y);

        /**
         * \brief Callback for a mousewheel event.
         *
         * \param pos - mouse wheel position.
         */
        static void GLFWCALL mouseWheel(int pos);

        /**
         * \brief Callback for a window resize event.
         *
         * \param width - the new width in the 'horizontal' axis.
         * \param height - the new width in the 'vertical' axis.
         */
        static void GLFWCALL resize(int width, int height);

        /**
         * \brief Callback for a window close event.
         *
         * \return - whether or not closure ought proceed, GL_TRUE or GL_FALSE
         */
        static int GLFWCALL quit(void);

        /// A timer for BOINC updates.
        static TriggerTimer* boinc_timer;

        /// A timer for rendering a frame.
        static TriggerTimer* render_timer;

        /// The event queue with double ended semantics.
        static std::list<Event> event_queue;
    };

/**
 * @}
 */

#endif /*EVENTS_H_*/
