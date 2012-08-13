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

#include "PulsarsEAH.h"

const GLfloat PulsarsEAH::LINK_RGB_RED(0.12f);
const GLfloat PulsarsEAH::LINK_RGB_GREEN(0.82f);
const GLfloat PulsarsEAH::LINK_RGB_BLUE(0.12f);
const GLfloat PulsarsEAH::MAG_SIZE(4.0f);
const float PulsarsEAH::VIEW_OFFSET(3000);

PulsarsEAH::PulsarsEAH(vec_t rad, GLfloat magsize,
                       GLclampf red,
                       GLclampf green,
                       GLclampf blue) :
                       Stars::Stars(rad, magsize, red, green, blue) {
    radius = SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - VIEWOFFSET;
    }

PulsarsEAH::~PulsarsEAH() {
    }

unsigned int PulsarsEAH::numberOfWayPoints(void) const {
    return cons_list.size();
    }

LookOut PulsarsEAH::getView(unsigned int sequence) const {
    // Default view - in absence of any pulsars - the
    // simulation's initial view.
    LookOut ret_val;

    const std::vector<PulsarEAH>& list = listing();

    // Assuming there is at least one pulsar.
    if(list.size() > 0) {
        if(sequence > (list.size() - 1)) {
            sequence = list.size() - 1;
            }

        PulsarEAH current_pulsar = list.at(sequence);

        VectorSP pulsar_position = VectorSP(current_pulsar.right_ascension(),
                                            current_pulsar.declination(),
                                            radius);

        Vector3D viewpoint = pulsar_position.unit() * (radius - VIEW_OFFSET);

        ret_val.setPosition(viewpoint);
        ret_val.setFocus(pulsar_position);
        ret_val.setOrientation(VIEW_UP);

        const std::vector<std::string>& current_description = current_pulsar.getDescription();

        for(unsigned int index = 0; index < current_description.size(); ++index) {
            ret_val.addToDescription(current_description[index]);
            }
        }

    return ret_val;
    }
