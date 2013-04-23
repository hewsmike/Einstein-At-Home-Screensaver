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

#include "SimulationRadio.h"

SimulationRadio::SimulationRadio(BOINCClientAdapter* boinc_adapter, EinsteinRadioAdapter* adapter) :
                 Simulation(boinc_adapter),
                 EG_adapter(adapter) {
    }

SimulationRadio::~SimulationRadio() {
    }

void SimulationRadio::includeLogo(HUDFlowLayout* container) {
    loadImageToPanel(ligo_image, container, "palfaTGA", 5, 5);
    }

void SimulationRadio::includeSearchInformation(HUDFlowLayout* container) {
    }

void SimulationRadio::renderObservatories(void) {
    }
