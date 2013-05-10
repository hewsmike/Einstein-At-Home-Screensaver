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

#include "HUDTextLine.h"

#include <cstring>

#include "ErrorHandler.h"
#include "HUDContainer.h"
#include "SolarSystemGlobals.h"

HUDTextLine::HUDTextLine(GLuint length,
                         const std::string& text,
                         OGLFT_ft* font,
                         GLuint horizontalMargin, GLuint verticalMargin) :
                           HUDContent(horizontalMargin, verticalMargin),
                           len(length) {
    // Store the font choice.
    setFont(font);

    // Allocate enough characters on the heap.
    txt = new char[length + 1];

    // Transfer characters to this heap storage.
    setText(text);

    // Initial setting of minimum dimensions are those of the initial text
    // content in combination with the given fixed margins.
    setMinimumDimensions(width() + 2*horzMargin(),
                         height() + 2*vertMargin());

    // Any enclosing container must be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer != NULL) {
        outer->adjust();
        }
    }

HUDTextLine::~HUDTextLine() {
    // Free up any allocated heap for character data.
    delete[] txt;

    // Plus any OpenGL resources.
    release();
    }

GLuint HUDTextLine::maxLength(void) const {
    return len;
    }

GLuint HUDTextLine::width(void) {
    OGLFT_ft* lineFont = getFont();
    std::cout << "HUDTextLine::width() : txt = '" << txt << "'"  << std::endl;

    // Lazy evaluate.
    // Ask OGLFT what the pixel bounds are for the current text.
    OGLFT::BBox currentBox = lineFont->measure(txt);
    // Round up the advance in horizontal direction to nearest integer.
    return ceil(currentBox.advance_.dx_);
    }

GLuint HUDTextLine::height(void) {
    OGLFT_ft* lineFont = getFont();
    // Lazy evaluate.
    // Round up the height to nearest integer.
    return ceil(lineFont->height());
    }

void HUDTextLine::prepare(SolarSystemGlobals::render_quality rq) {
    // Call base class to set minima for the given text content.
    this->setMinimumDimensions(width() + 2*horzMargin(),
                               height() + 2*vertMargin());

    // Any enclosing container must be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer != NULL) {
        outer->adjust();
        }
    }

void HUDTextLine::release(void) {
    // Policy is to set size to zero when rendering is not implied.

    // Call base class to set minima to no content.
    setMinimumDimensions(0, 0);

    // Release of text implies any enclosing
    // container must be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer !=NULL) {
        outer->adjust();
        }
    }

void HUDTextLine::render(void) {
    OGLFT_ft* lineFont = getFont();
    lineFont->draw(horzBase() + horzMargin(),
                   vertBase() + vertMargin(), txt);
    }

std::string HUDTextLine::text(void) const {
    return txt;
    }

void HUDTextLine::setText(const std::string& text) {
    // Move the character data.
    strncpy(txt, text.c_str(), len);

    // To be sure ... :-)
    txt[len] = '\0';
    std::cout << "HUDTextLine::setText() : txt = '" << txt << "'"  << std::endl;
    }
