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

Vertex::Vertex(void) {
    }

Vertex::Vertex(const Vector3D& position, const Vector3D& normal,
               const std::pair<GLfloat, GLfloat>& texture_coords)
                   : pos(position), norm(normal), text_cds(texture_coords) {
    }

Vertex::Vertex(const Vertex& other) {
    this->pos = other.position();
    this->norm = other.normal();
    this->text_cds = other.texture_co_ords();
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
    return text_cds;
    }
