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

#include "Vertex.h"

Vertex::Vertex() {
    }

Vertex::Vertex(const Vector3D& ps, const Vector3D& nm,
               const std::pair<GLfloat, GLfloat>& tc)
                   : pos(ps), norm(nm), t_cds(tc) {
    }

Vertex::Vertex(const Vertex& other) {
    this->pos = Vector3D(other.position());
    this->norm = Vector3D(other.normal());
    this->t_cds = other.texture_co_ords();
    }

Vertex::~Vertex() {
    }

const Vector3D& Vertex::position(void) const {
    return pos;
    }

const Vector3D& Vertex::normal(void) const {
    return norm;
    }

const std::pair<GLfloat, GLfloat>& Vertex::texture_co_ords(void) const {
    return t_cds;
    }
