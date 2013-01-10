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

#include "Line.h"

/// Default line is white, opaque and of one pixel width.
const GLfloat Line::LINE_WIDTH_DEFAULT(1.0f);
const GLfloat Line::LINE_RED_DEFAULT(1.0f);
const GLfloat Line::LINE_GREEN_DEFAULT(1.0f);
const GLfloat Line::LINE_BLUE_DEFAULT(1.0f);
const GLfloat Line::LINE_ALPHA_DEFAULT(1.0f);

Line::Line(void) :
            line_width(LINE_WIDTH_DEFAULT),
            line_red(LINE_RED_DEFAULT),
            line_green(LINE_GREEN_DEFAULT),
            line_blue(LINE_BLUE_DEFAULT),
            line_alpha(LINE_ALPHA_DEFAULT) {
    }

Line::Line(GLfloat width,
           GLfloat red,
           GLfloat green,
           GLfloat blue,
           GLfloat alpha) :
            line_width(width),
            line_red(red),
            line_green(green),
            line_blue(blue),
            line_alpha(alpha) {
    }

Line::~Line() {
    }

GLfloat Line::width(void) const {
    return line_width;
    }

GLfloat Line::red(void) const {
    return line_red;
    }

GLfloat Line::green(void) const {
    return line_green;
    }

GLfloat Line::blue(void) const {
    return line_blue;
    }

GLfloat Line::alpha(void) const {
    return line_alpha;
    }
