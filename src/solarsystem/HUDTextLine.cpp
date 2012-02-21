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

#include "HUDTextLine.h"

#include <cmath>

#include "HUDContainer.h"

HUDTextLine::HUDTextLine(GLuint length, OGLFT_ft* font,
                         GLuint horizontalMargin, GLuint verticalMargin) :
                           HUDContent(horizontalMargin, verticalMargin),
                           len(length),
                           lineFont(font) {
   // Initial setting of minimum dimensions are those of the initial null
   // content ie. zero width and height, in combination with the
   // given fixed margins.
   setMinimumDimensions(2*horizontalMargin, 2*verticalMargin);
   }

HUDTextLine::~HUDTextLine() {
   }

void HUDTextLine::setText(const std::string text) {
   // Overwrite any previous content, but truncate any input to
   // within correct size.
   txt.assign(text.substr(0, len));

   // Call base class to set minima for the given text content.
   this->setMinimumDimensions(width() + 2*horzMargin(),
                              height() + 2*vertMargin());

   // Any enclosing container must be made aware of size change.
   HUDContainer* outer = getEnclosingContainer();
   if(outer !=NULL) {
      outer->adjust();
      }
   }

std::string HUDTextLine::getText(void) const {
   return txt;
   }

GLuint HUDTextLine::maxLength(void) const {
   return len;
   }

GLuint HUDTextLine::width(void) const {
   // Lazy evaluate.
   // Ask OGLFT what the pixel bounds are for the current text.
   OGLFT::BBox currentBox = lineFont->measure(txt.c_str());
   // Round up the advance in horizontal direction to nearest integer.
   return ceil(currentBox.advance_.dx_);
   }

GLuint HUDTextLine::height(void) const {
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
   lineFont->draw(horzBase(), vertBase(), txt.c_str());
   }