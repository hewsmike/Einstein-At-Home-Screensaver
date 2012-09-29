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

#include "LookOut.h"

LookOut::LookOut() {
    image = NULL;
    }

LookOut::LookOut(const Vector3D& position,
                 const Vector3D& focus,
                 const Vector3D& orientation) :
                    CameraState(position, focus, orientation) {
    image = NULL;
    }

LookOut::~LookOut() {
    clearImage();
    }

const std::vector<std::string>& LookOut::getDescription(void) const {
    return desc;
    }

void LookOut::addToDescription(const std::string& description) {
    desc.push_back(description);
    }

void LookOut::addImageResource(std::string resourceName) {
    clearImage();

    image = new HUDImage(resourceName, 0, 0);
    }

const HUDImage* LookOut::getImage(void) const {
    return image;
    }

void LookOut::clearImage(void) {
    // Provided we have an image allocated.
    if(image != NULL) {
        // Inactivate it, thus releasing OpenGL server resources.
        image->inactivate();
        // Then free the heap memory.
        delete image;
        }
    }
