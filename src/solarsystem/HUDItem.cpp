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

#include "HUDItem.h"

HUDItem::HUDItem(void) {
    // Set base coordinates to zero ie. lower left corner of the
    // area assigned during acquisition of the graphics context.
    // A safe assignment, independent of screen/window size.
    hz_base = 0;
    vt_base = 0;
    // Set sizes to zero.
    wd = 0;
    ht = 0;
    wd_min = 0;
    ht_min = 0;
    // Assume no enclosing container.
    surround = NULL;
    }

HUDItem::~HUDItem() {
    }

GLuint HUDItem::width(void) const {
    return wd;
    }

GLuint HUDItem::height(void) const {
    return ht;
    }

GLuint HUDItem::minWidth(void) const {
    return wd_min;
    }

GLuint HUDItem::minHeight(void) const {
    return ht_min;
    }

GLuint HUDItem::horzBase(void) const {
    return hz_base;
    }

GLuint HUDItem::vertBase(void) const {
    return vt_base;
    }

void HUDItem::reBase(GLuint newHorz, GLuint newVert) {
    hz_base = newHorz;
    vt_base = newVert;
    }

void HUDItem::setMinimumDimensions(GLuint minWidth, GLuint minHeight) {
    // Set minimum dimensions, adjust dimensions upwards if need be.
    wd_min = minWidth;
    if(wd_min > wd) {
        wd = wd_min;
        }
    ht_min = minHeight;
    if(ht_min > ht) {
        ht = ht_min;
        }
    }

void HUDItem::setDimensions(GLuint newWidth, GLuint newHeight) {
    // Set the new dimensions, but only above minimum.
    if(newWidth >= wd_min) {
        wd = newWidth;
        }
    if(newHeight >= ht_min) {
        ht = newHeight;
        }
    }

void HUDItem::setEnclosingContainer(HUDContainer* enclosing) {
    surround = enclosing;
    }

HUDContainer* HUDItem::getEnclosingContainer(void) const {
    return surround;
    }
