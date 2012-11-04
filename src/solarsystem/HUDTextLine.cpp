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

#include "HUDTextLine.h"

#include "HUDContainer.h"

HUDTextLine::HUDTextLine(GLuint length, OGLFT_ft* font,
                         const std::string& text,
                         GLuint horizontalMargin, GLuint verticalMargin) :
                           HUDContent(horizontalMargin, verticalMargin),
                           len(length),
                           lineFont(font),
                           txt(text) {
    // Initial setting of minimum dimensions are those of the initial text
    // content in combination with the given fixed margins.
    this->setMinimumDimensions(width() + 2*horzMargin(),
                               height() + 2*vertMargin());

    // Any enclosing container must be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer !=NULL) {
        outer->adjust();
        }
    }

HUDTextLine::~HUDTextLine() {
    release();
    }

GLuint HUDTextLine::maxLength(void) const {
    return len;
    }

GLuint HUDTextLine::width(void) const {
    // Lazy evaluate.
    // Ask OGLFT what the pixel bounds are for the current text.
    OGLFT::BBox currentBox = lineFont->measure(txt.substr(0, len).c_str());
    // Round up the advance in horizontal direction to nearest integer.
    return ceil(currentBox.advance_.dx_);
    }

GLuint HUDTextLine::height(void) const {
    // Lazy evaluate.
    // Round up the height to nearest integer.
    return ceil(lineFont->height()*2);
    }

void HUDTextLine::prepare(SolarSystemGlobals::render_quality rq) {
    // Call base class to set minima for the given text content.
    this->setMinimumDimensions(width() + 2*horzMargin(),
                               height() + 2*vertMargin());

    // Any enclosing container must be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer !=NULL) {
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
    lineFont->draw(horzBase(), vertBase() + this->height()/4, txt.substr(0, len).c_str());
    }

std::string HUDTextLine::text(void) const {
    return txt;
    }

void HUDTextLine::setText(const std::string& text) {
    txt = text;
    }
