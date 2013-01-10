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

#ifndef HUD_TEXT_LINE_SCROLL_H_
#define HUD_TEXT_LINE_SCROLL_H_

#include "HUDTextLine.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A single horizontal line of text on the HUD, which may scroll
 *        to the left or right or not at all.
 *
 * \author Mike Hewson\n
 */

class HUDTextLineScroll : public HUDTextLine {
    public:
        /// Enumerants for the scroll behaviour.
        enum mode {NONE, LEFT, RIGHT};

        /**
         * \brief Constructor
         *
         * \param length : the maximum length of displayed characters
         * \param element : the scene element type as per Simulation class
         * \param text : a string containing the desired text
         * \param horizontalMargin : the horizontal pixel margin to be applied
         *                           either side of the enclosed content
         * \param verticalMargin : the vertical pixel margin to be applied
         *                         above and below the enclosed content
         * \param direction : the direction in which to scroll the text
         * \param scroll_interval : the number of frames between transitions
         */
        HUDTextLineScroll(GLuint length,
                          const std::string& text,
                          GLuint horizontalMargin, GLuint verticalMargin,
                          mode direction, GLuint scroll_interval);

        /**
         * \brief Destructor
         */
        virtual ~HUDTextLineScroll();

        /**
         * \brief Get the direction of scroll
         *
         * \return the direction in which to scroll the text
         */
        HUDTextLineScroll::mode getDirection(void);

        /**
         * \brief Set the direction of scroll
         *
         * \param direction : the direction in which to scroll the text
         */
        void setDirection(mode direction);

        /**
         * \brief Reverse the direction of scroll
         */
        void toggleDirection(void);

    protected:
        /// This routine satisfies the Renderable interface.

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        mode dir;

        GLuint interval;

        GLuint frame_count;
    };

/**
 * @}
 */

#endif /* HUD_TEXT_LINE_SCROLL_H_ */
