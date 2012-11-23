/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#ifndef LINE_H_
#define LINE_H_

#include "framework.h"

/**
 * \addtogroup solarsystem Solarsystem
 *  @{
 */

/**
 * \brief A helper class that encapsulates the features of an
 *        OpenGL renderable line.
 *
 * \author Mike Hewson\n
 */

class Line {
    public:
        /**
         * \brief Constructor
         */
        Line(void);

        /**
         * \brief Constructor.
         *
         * \param width : the displayed width in pixels
         * \param red : the red component of RGBA color
         * \param green : the green component of RGBA color
         * \param blue : the blue componewnt of RGBA color
         * \param alpha : the alpha component of RGBA color
         */
        Line(GLfloat width,
             GLfloat red,
             GLfloat green,
             GLfloat blue,
             GLfloat alpha);

        /**
         * \brief Destructor.
         */
        virtual ~Line();

        /**
         * \brief Obtain the line width.
         *
         * \return the line width
         */
        GLfloat width(void) const;

        /**
         * \brief Obtain the red component of the line's RGBA color
         *
         * \return the red value
         */
        GLfloat red(void) const;

        /**
         * \brief Obtain the green component of the line's RGBA color
         *
         * \return the green value
         */
        GLfloat green(void) const;

        /**
         * \brief Obtain the blue component of the line's RGBA color
         *
         * \return the blue value
         */
        GLfloat blue(void) const;

        /**
         * \brief Obtain the alpha component of the line's RGBA color
         *
         * \return the alpha value
         */
        GLfloat alpha(void) const;

    private:
        static const GLfloat LINE_WIDTH_DEFAULT;
        static const GLfloat LINE_RED_DEFAULT;
        static const GLfloat LINE_GREEN_DEFAULT;
        static const GLfloat LINE_BLUE_DEFAULT;
        static const GLfloat LINE_ALPHA_DEFAULT;

        GLfloat line_width;
        GLfloat line_red;
        GLfloat line_green;
        GLfloat line_blue;
        GLfloat line_alpha;
    };

/**
* @}
*/

#endif // LINE_H_

