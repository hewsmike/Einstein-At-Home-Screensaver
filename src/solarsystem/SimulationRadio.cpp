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

void SimulationRadio::includeSearchInformation(HUDtempFlowLayout* container) {
    // Refresh our BOINC data.
    ER_adapter->refresh();

    // First empty of any existing content.
    container->erase();

    stringstream work_unit_heading;
    work_unit_heading << "Current Work Unit Details";
    setText(container, work_unit_heading.str());

	// Right ascension.
    double right_ascension = ER_adapter->wuSkyPosRightAscension();
    stringstream msg_RA;
    msg_RA << "Right ascension : ";
    msg_RA.setf(std::ios::showpos | ios::fixed);
    msg_RA << std::setw(6) << std::setprecision(2) << right_ascension;
    setText(container, msg_RA.str());

    // Declination.
    double declination = ER_adapter->wuSkyPosDeclination();
    stringstream msg_dec;
    msg_dec << "Declination : ";
    msg_dec.setf(std::ios::showpos | ios::fixed);
    msg_dec << std::setw(5) << std::setprecision(2) << declination;
    setText(container, msg_dec.str());

    // Dispersion.
    double dispersion = ER_adapter->wuDispersionMeasure();
    stringstream msg_disp;
    msg_disp << "Dispersion : ";
    msg_disp.setf(std::ios::showpos | ios::fixed);
    msg_disp << std::setw(5) << std::setprecision(2) << dispersion;
    setText(container, msg_disp.str());

    // Orbital radius.
    double orb_radius = ER_adapter->wuTemplateOrbitalRadius();
    stringstream msg_orb_radius;
    msg_orb_radius << "Template Orbital Radius : ";
    msg_orb_radius.setf(std::ios::showpos | ios::fixed);
    msg_orb_radius << std::setw(5) << std::setprecision(2) << orb_radius;
    setText(container, msg_orb_radius.str());

    // Orbital period.
    double orb_period = ER_adapter->wuTemplateOrbitalPeriod();
    stringstream msg_orb_period;
    msg_orb_period << "Template Orbital Period : ";
    msg_orb_period.setf(std::ios::showpos | ios::fixed);
    msg_orb_period << std::setw(5) << std::setprecision(2) << orb_period;
    setText(container, msg_orb_period.str());

    // Orbital phase.
    double orb_phase = ER_adapter->wuTemplateOrbitalPhase();
    stringstream msg_orb_phase;
    msg_orb_phase << "Template Orbital Phase : ";
    msg_orb_phase.setf(std::ios::showpos | ios::fixed);
    msg_orb_phase << std::setw(5) << std::setprecision(2) << orb_phase;
    setText(container, msg_orb_phase.str());

    // Fraction completed, as a percentage.
    double fraction_complete = ER_adapter->wuFractionDone() * 100;
    stringstream msg_fc;
    msg_fc << "Fraction complete : ";
    msg_fc.setf(std::ios::showpos | ios::fixed);
    msg_fc << std::setw(6) << std::setprecision(2)
           << fraction_complete << " %" ;
    setText(container, msg_fc.str());

    // CPU time.
    double CPU_time = ER_adapter->wuCPUTime();
    stringstream msg_cpu;
    msg_cpu << "CPU time : ";
    msg_cpu.setf(std::ios::showpos | ios::fixed);
    msg_cpu << std::setw(8) << std::setprecision(0)
            << CPU_time << " s" ;
    setText(container, msg_cpu.str());

    container->activate();
    }

void SimulationRadio::renderObservatories(void) {
    }
