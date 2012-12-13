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

const GLuint GreatCircle::ARRAY_START(0);
const GLsizei GreatCircle::ARRAY_STRIDE(0);
const GLuint GreatCircle::BYTE_OFFSET(0);
const GLuint GreatCircle::COORDS_PER_VERTEX(3);

GreatCircle::GreatCircle(const Vector3D& normal, const Vector3D& zero_long,
                         Line line, GLfloat radius) :
                            norm(normal.unit()),
                            z_long(zero_long.unit()),
                            gc_line(line),
                            rad(radius),
                            segs(segments) {
    }

GreatCircle::~GreatCircle() {
    release();
    }

void GreatCircle::prepare(SolarSystemGlobals::render_quality rq) {
    loadVertexBuffer();
    }

void GreatCircle::release(void) {
    // Release the buffer object's resources.
    buff_obj_points.release();
    }

void GreatCircle::render(void) {
    // Make our vertex buffer identifier OpenGL's current one.
    glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

    // We will use a vertex array within that buffer.
    glEnableClientState(GL_VERTEX_ARRAY);

    // The vertex array pointer points to the start of the buffer.
    glVertexPointer(COORDS_PER_VERTEX, GL_FLOAT, ARRAY_STRIDE, BUFFER_OFFSET(BYTE_OFFSET));

    glLineWidth(gc_line.width());
    glColor3f(gc_line.red(), gc_line.green(), gc_line.blue());

    // Finally we get to render the lines.
    glDrawArrays(GL_LINES, ARRAY_START, segs);

    // Stop using vertex arrays.
    glDisableClientState(GL_VERTEX_ARRAY);

    // Unbind the buffers.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
    }

void GreatCircle::loadVertexBuffer(void) {
    // Get an OpenGL buffer object.
    buff_obj_points.acquire();

    // What size allocation are we after?
    GLsizeiptr buffer_size = sizeof(Vert) * segs;

    // Allocate temporary heap buffer of correct size.
    Vert* buffer_base_ptr = new Vert[segs];
    Vert* buffer_ptr = buffer_base_ptr;

    Vector3D orthog = norm * z_long;

    // Store the vertex position data in the buffer.
    for(GLuint point = 0; point < segs; ++point) {
        // Positive going anti-clockwise when looking down on the
        // plane along the tip to the base of the given normal.
        GLfloat theta = (point/segs)*(SolarSystemGlobals::FULL_CIRCLE_DEG(360.0f);

        Vector3D pt = rad * (COS(theta) * z_long + SIN(theta) * orthog);

        buffer_ptr->x_pos = pt.x();
        buffer_ptr->y_pos = pt.y();
        buffer_ptr->z_pos = pt.z();
        ++buffer_ptr;
        }

    buff_obj_points.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);

    delete[] buffer_base_ptr;
    }
