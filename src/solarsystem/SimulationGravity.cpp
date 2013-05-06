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
    }

void SimulationGravity::includeLogo(HUDLogoCycle* container) {
    container->addItem(new HUDImage("geoTGA", 10, 10));
    container->addItem(new HUDImage("ligoTGA", 10, 10));
    container->addItem(new HUDImage("virgoTGA", 10, 10));
    }

void SimulationGravity::includeSearchInformation(HUDTempFlowLayout* container) {
    // Refresh our BOINC data.
    EG_adapter->refresh();

    // First empty of any existing content.
    container->erase();

    stringstream work_unit_heading;
    work_unit_heading << "Current Work Unit Details";
    setText(container, work_unit_heading.str());

    // Right ascension.
    double right_ascension = EG_adapter->wuSkyPosRightAscension();
    stringstream msg_RA;
    msg_RA << "Right ascension : ";
    msg_RA.setf(std::ios::showpos | ios::fixed);
    msg_RA << std::setw(6) << std::setprecision(2) << right_ascension;
    setText(container, msg_RA.str());

    // Declination.
    double declination = EG_adapter->wuSkyPosDeclination();
    stringstream msg_dec;
    msg_dec << "Declination : ";
    msg_dec.setf(std::ios::showpos | ios::fixed);
    msg_dec << std::setw(5) << std::setprecision(2) << declination;
    setText(container, msg_dec.str());

    // Fraction completed, as a percentage.
    double fraction_complete = EG_adapter->wuFractionDone() * 100;
    stringstream msg_fc;
    msg_fc << "Fraction complete : ";
    msg_fc.setf(std::ios::showpos | ios::fixed);
    msg_fc << std::setw(6) << std::setprecision(2)
           << fraction_complete << " %" ;
    setText(container, msg_fc.str());

    // CPU time.
    double CPU_time = EG_adapter->wuCPUTime();
    stringstream msg_cpu;
    msg_cpu << "CPU time : ";
    msg_cpu.setf(std::ios::showpos | ios::fixed);
    msg_cpu << std::setw(8) << std::setprecision(0)
            << CPU_time << " s" ;
    setText(container, msg_cpu.str());

    container->activate();
    }

void SimulationGravity::renderObservatories(void) {
    }
