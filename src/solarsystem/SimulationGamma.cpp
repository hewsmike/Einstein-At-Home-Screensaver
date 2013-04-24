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

#include "SimulationGamma.h"

SimulationGamma::SimulationGamma(BOINCClientAdapter* boinc_adapter, EinsteinRadioAdapter* adapter) :
                 Simulation(boinc_adapter),
                 EG_adapter(adapter) {
    }

SimulationGamma::~SimulationGamma() {
    }

void SimulationGamma::includeLogo(HUDLogoCycle* container) {
    container->addItem(new HUDImage("fermiTGA", 10, 10));
    }

void SimulationGamma::includeSearchInformation(HUDFlowLayout* container) {
    /// TODO What should we show, if anything, in the absence of a gamma adapter ?
    }

void SimulationGamma::renderObservatories(void) {
    }
