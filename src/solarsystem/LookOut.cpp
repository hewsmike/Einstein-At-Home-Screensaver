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
    }

LookOut::LookOut(const Vector3D& position,
                 const Vector3D& focus,
                 const Vector3D& orientation) :
                    CameraState(position, focus, orientation) {
    }

LookOut::~LookOut() {
    clearImages();
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

const std::vector<const HUDImage*> LookOut::getImages(void) const {
    return images;
    }

void LookOut::clearImages(void) {
    // Provided we have any images allocated.
    if(images.size() != 0) {
        for (int im = 0; im < images.size(); ++im) {
            // Inactivate this image, thus releasing OpenGL server resources.
            images[im]->inactivate();
            // Then free the heap memory.
            delete images[im];
            }
        }
    }
