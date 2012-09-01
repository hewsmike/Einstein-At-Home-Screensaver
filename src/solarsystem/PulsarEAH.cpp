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

#include "PulsarEAH.h"

PulsarEAH::PulsarEAH(starpos_t right_asc,
                     starpos_t decl,
                     std::string name,
                     Pulsar::pulsar_source src,
                     float period,
                     float disp_mod,
                     float dist,
                     const std::string& disc) :
                        Pulsar(right_asc, decl, name, src),
                        pulse_period(period),
                        dispersion_modulus(disp_mod),
                        kp_distance(dist),
                        volunteers(disc) {
    }

PulsarEAH::~PulsarEAH() {
    }

const float PulsarEAH::period(void) const {
    return pulse_period;
    }

const float PulsarEAH::dispersionModulus(void) const {
    return dispersion_modulus;
    }

const float PulsarEAH::distance(void) const {
    return kp_distance;
    }

const std::string PulsarEAH::discoverers(void) const {
    return volunteers;
    }
