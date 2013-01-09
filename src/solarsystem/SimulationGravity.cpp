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

SimulationGravity::SimulationGravity(BOINCClientAdapter* boinc_adapter, EinsteinGravityAdapter* adapter) :
                                     Simulation(boinc_adapter),
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
    // Refresh our BOINC data.
    EG_adapter->refresh();

    // First empty of any existing content.
    container->erase();

    // Right ascension.
    double right_ascension = EG_adapter->wuSkyPosRightAscension();
    stringstream msg_RA;
    msg_RA << "Right ascension : ";
    msg_RA.setf(std::ios::showpos | ios::fixed);
    msg_RA << std::setw(6) << std::setprecision(2) << right_ascension;
    container->addItem(new HUDTextLine(msg_RA.str().size(), msg_RA.str(), 0, 2));

    // Declination.
    double declination = EG_adapter->wuSkyPosDeclination();
    stringstream msg_dec;
    msg_dec << "Declination : ";
    msg_dec.setf(std::ios::showpos | ios::fixed);
    msg_dec << std::setw(5) << std::setprecision(2) << declination;
    container->addItem(new HUDTextLine(msg_dec.str().size(), msg_dec.str(), 0, 2));

    // Fraction completed, as a percentage.
    double fraction_complete = EG_adapter->wuFractionDone() * 100;
    stringstream msg_fc;
    msg_fc << "Fraction complete : ";
    msg_fc.setf(std::ios::showpos | ios::fixed);
    msg_fc << std::setw(6) << std::setprecision(2)
           << fraction_complete << " %" ;
    container->addItem(new HUDTextLine(msg_fc.str().size(), msg_fc.str(), 0, 2));

    // CPU time.
    double CPU_time = EG_adapter->wuCPUTime();
    stringstream msg_cpu;
    msg_cpu << "CPU time : ";
    msg_cpu.setf(std::ios::showpos | ios::fixed);
    msg_cpu << std::setw(8) << std::setprecision(0)
            << CPU_time << " s" ;
    container->addItem(new HUDTextLine(msg_cpu.str().size(), msg_cpu.str(), 0, 2));

    container->activate();
    }

void SimulationGravity::renderObservatories(void) {
    }
