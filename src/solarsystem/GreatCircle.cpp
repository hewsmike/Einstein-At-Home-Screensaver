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

#include "GreatCircle.h"

#include "SolarSystemGlobals.h"

GreatCircle::GreatCircle(const Vector3D& normal, const Vector3D& zero_long,
                         Line line, GLfloat radius) :
                            norm(normal.unit()),
                            z_long(zero_long.unit()),
                            gc_line(line),
                            rad(radius),
                            segs(segments) {
    }

GreatCircle::~GreatCircle() {
    }

void GreatCircle::prepare(SolarSystemGlobals::render_quality rq) {

    }

void GreatCircle::release(void) {
    }

void GreatCircle::render(void) {
    }

void GreatCircle::loadVertexBuffer(void) {
    // Get an OpenGL buffer object.
    buff_obj_points.acquire();

    // What size allocation are we after?
    GLsizeiptr buffer_size = sizeof(Vert) * segs;

    // Allocate temporary heap buffer of correct size.
    Vert* buffer_base_ptr = new Vert[segs];
    Vert* buffer_ptr = buffer_base_ptr;



    // Store the vertex position data in the buffer.
    for(GLuint point = 0; point < segs; ++point) {
        // Well to start with : on Mar 21st the Sun is at the vernal equinox.
        days = days - VERNAL_EQUINOX_DAY;

    // OK, now get an angle in the ecliptic plane between a vector to the
    // vernal equinox and a vector to the position of the Sun on the given
    // day. Positive going anti-clockwise when looking down on that plane
    // from the northern side.
    GLfloat theta = (point/SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f)DAYS_PER_YEAR)*SolarSystemGlobals::FULL_CIRCLE_DEG;

    // Just modelling a circle but tilted with respect to the celestial equator.
        rad * Vector3D(COS(theta),
                       SIN(theta) * COS(ECLIPTIC_ANGLE_DEG),
                       SIN(theta) * SIN(ECLIPTIC_ANGLE_DEG));
        Vector3D pt();
        Vert vt = ;
        buffer_ptr->x_pos = vt.x;
        buffer_ptr->y_pos = vt.y;
        buffer_ptr->z_pos = vt.z;
        ++buffer_ptr;
        }

    buff_obj_points.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);

    delete[] buffer_base_ptr;
    }
