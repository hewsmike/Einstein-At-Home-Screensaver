/***************************************************************************
* Copyright (C) 2012 by Mike Hewson                                        *
* hewsmike@iinet.net.au                                                    *
*                                                                          *
* This file is part of Einstein@Home.                                      *
*                                                                          *
* Einstein@Home is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published        *
* by the Free Software Foundation, version 2 of the License.               *
*                                                                          *
* Einstein@Home is distributed in the hope that it will be useful,         *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.    *
*                                                                          *
***************************************************************************/

#include "HUDTextBox.h"

HUDTextBox::HUDTextBox(GLuint width, GLuint height,
                       OGLFT_ft* font, GLuint padding,
                       GLuint horizontalMargin, GLuint verticalMargin) {
    char_width = width;
    char_height = height;
    box_font = font;
    pad = padding;
    horzMargin = horizontalMargin;
    vertMargin = verticalMargin;
    // Initial setting of minimum dimensions are those of the initial
    // null content ie. zero width and height, in combination with the
    // given fixed margins.
    setMinimumDimensions(2*horizontalMargin, 2*verticalMargin);
    }

HUDTextBox::~HUDTextBox() {
    }

GLuint HUDTextBox::widthInChars(void) const {
    return char_width;
    }

GLuint HUDTextBox::heightInLines(void) const {
    return lines;
    }

GLuint HUDTextBox::widthInPixels(void) const {
    // Lazy evaluated.
    }

GLuint HUDTextBox::heightInPixels(void) const {
    // Lazy evaluated.
    }

void HUDTextBox::setText(const std::string& text) {

    }

std::string HUDTextBox::text(void) const {
    // Lazy evaluated.
    }

void HUDTextBox::setLineText(const std::string& text, GLuint line) {

    }

std::string HUDTextBox::lineText(GLuint line) const {

    }
