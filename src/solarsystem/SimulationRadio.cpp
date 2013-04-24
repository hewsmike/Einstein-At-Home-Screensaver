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
                 ER_adapter(adapter) {
    }

SimulationRadio::~SimulationRadio() {
    }

void SimulationRadio::includeLogo(HUDLogoCycle* container) {
    container->addItem(new HUDImage("palfaTGA", 10, 10));
    }

void SimulationRadio::includeSearchInformation(HUDFlowLayout* container) {
    // Refresh our BOINC data.
    ER_adapter->refresh();

    // First empty of any existing content.
    container->erase();

    stringstream work_unit_heading;
    work_unit_heading << "Current Work Unit Details";
    container->addItem(new HUDTextLine(work_unit_heading.str().size(), work_unit_heading.str(), 0, 2));

	// Right ascension.
    double right_ascension = ER_adapter->wuSkyPosRightAscension();
    stringstream msg_RA;
    msg_RA << "Right ascension : ";
    msg_RA.setf(std::ios::showpos | ios::fixed);
    msg_RA << std::setw(6) << std::setprecision(2) << right_ascension;
    container->addItem(new HUDTextLine(msg_RA.str().size(), msg_RA.str(), 0, 2));

    // Declination.
    double declination = ER_adapter->wuSkyPosDeclination();
    stringstream msg_dec;
    msg_dec << "Declination : ";
    msg_dec.setf(std::ios::showpos | ios::fixed);
    msg_dec << std::setw(5) << std::setprecision(2) << declination;
    container->addItem(new HUDTextLine(msg_dec.str().size(), msg_dec.str(), 0, 2));

    // Dispersion.
    double dispersion = ER_adapter->wuDispersionMeasure();
    stringstream msg_disp;
    msg_disp << "Dispersion : ";
    msg_disp.setf(std::ios::showpos | ios::fixed);
    msg_disp << std::setw(5) << std::setprecision(2) << dispersion;
    container->addItem(new HUDTextLine(msg_disp.str().size(), msg_disp.str(), 0, 2));

    // Orbital radius.
    double orb_radius = ER_adapter->wuTemplateOrbitalRadius();
    stringstream msg_orb_radius;
    msg_orb_radius << "Template Orbital Radius : ";
    msg_orb_radius.setf(std::ios::showpos | ios::fixed);
    msg_orb_radius << std::setw(5) << std::setprecision(2) << orb_radius;
    container->addItem(new HUDTextLine(msg_orb_radius.str().size(), msg_orb_radius.str(), 0, 2));

    // Orbital period.
    double orb_period = ER_adapter->wuTemplateOrbitalPeriod();
    stringstream msg_orb_period;
    msg_orb_period << "Template Orbital Period : ";
    msg_orb_period.setf(std::ios::showpos | ios::fixed);
    msg_orb_period << std::setw(5) << std::setprecision(2) << orb_period;
    container->addItem(new HUDTextLine(msg_orb_period.str().size(), msg_orb_period.str(), 0, 2));

    // Orbital phase.
    double orb_phase = ER_adapter->wuTemplateOrbitalPhase();
    stringstream msg_orb_phase;
    msg_orb_phase << "Template Orbital Phase : ";
    msg_orb_phase.setf(std::ios::showpos | ios::fixed);
    msg_orb_phase << std::setw(5) << std::setprecision(2) << orb_phase;
    container->addItem(new HUDTextLine(msg_orb_phase.str().size(), msg_orb_phase.str(), 0, 2));

    // Fraction completed, as a percentage.
    double fraction_complete = ER_adapter->wuFractionDone() * 100;
    stringstream msg_fc;
    msg_fc << "Fraction complete : ";
    msg_fc.setf(std::ios::showpos | ios::fixed);
    msg_fc << std::setw(6) << std::setprecision(2)
           << fraction_complete << " %" ;
    container->addItem(new HUDTextLine(msg_fc.str().size(), msg_fc.str(), 0, 2));

    // CPU time.
    double CPU_time = ER_adapter->wuCPUTime();
    stringstream msg_cpu;
    msg_cpu << "CPU time : ";
    msg_cpu.setf(std::ios::showpos | ios::fixed);
    msg_cpu << std::setw(8) << std::setprecision(0)
            << CPU_time << " s" ;
    container->addItem(new HUDTextLine(msg_cpu.str().size(), msg_cpu.str(), 0, 2));

    container->activate();
    }

void SimulationRadio::renderObservatories(void) {
    }
