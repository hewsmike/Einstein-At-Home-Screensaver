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

#include "SimulationGravity.h"

#include <iomanip>
#include <sstream>

SimulationGravity::SimulationGravity(EinsteinGravityAdapter* adapter) :
                                     EG_adapter(adapter) {
    }

SimulationGravity::~SimulationGravity() {
    if(ligo_image != NULL) {
        delete ligo_image;
        }
    }

void SimulationGravity::includeLogo(HUDFlowLayout* container) {
    loadImageToPanel(ligo_image, container, "ligoTGA", 5, 5);
    }

void SimulationGravity::includeSearchInformation(HUDFlowLayout* container) {
    // First empty of any existing content.
    container.erase();

    // Include declination value
    double declination = EG_adapter->wuSkyPosDeclination();
    stringstream msg_dec;
    msg_dec << "Declination : ";
    msg_dec.setf(std::ios::showpos | ios::fixed);
    msg_dec << std::setw(5) << std::setprecision(2) << declination;
    HUDTextLine* line = new HUDTextLine(15,"Declination : ", 0, 2);
    container->addItem(line);
    }

void SimulationGravity::renderObservatories(void) {
    }
