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

#include "Vector3D.h"

#include <cmath>
#include <iostream>

// The null vector connects the origin to itself :-)
const vec_t Vector3D::NULL_LENGTH(0.0f);
const vec_t Vector3D::NULL_X(0.0f);
const vec_t Vector3D::NULL_Y(0.0f);
const vec_t Vector3D::NULL_Z(0.0f);

const Vector3D Vector3D::NULLV(NULL_X, NULL_Y, NULL_Z);

Vector3D::Vector3D(void) : x_comp(Vector3D::NULL_X),
                           y_comp(Vector3D::NULL_Y),
                           z_comp(Vector3D::NULL_Z) {
    }

Vector3D::Vector3D(vec_t xc, vec_t yc, vec_t zc) : x_comp(xc),
                                                   y_comp(yc),
                                                   z_comp(zc) {
    }

Vector3D::Vector3D(const Vector3D& other) : x_comp(other.x()),
                                            y_comp(other.y()),
                                            z_comp(other.z()) {
    }

Vector3D::~Vector3D() {
    }

vec_t Vector3D::len(void) {
    // Square root of the dot product of the vector with itself.
    return sqrt(x_comp * x_comp + y_comp * y_comp + z_comp * z_comp);
    }

Vector3D Vector3D::unit(void) {
    // Return the null vector ( courtesy of the no-argument constructor )
    // by default.
    Vector3D ret_val;

    // How long am I?
    vec_t mag = this->len();

    // Careful that I am not already the zero vector.
    // TODO some tolerance level rather exact nullity ??
    if (mag != Vector3D::NULL_LENGTH) {
        // OK, safe to divide each component by the length of the vector.
        // TODO - Consider if we might get machine infinity here? Maybe a
        // projective/homogenous geometry system? Tempting but too much work ....
        ret_val.set_x(x_comp / mag);
        ret_val.set_y(y_comp / mag);
        ret_val.set_z(z_comp / mag);
        }
    return ret_val;
    }

void Vector3D::print(void) const {
    std::cout << "x = " << x_comp << "\ty = " << y_comp << "\tz = " << z_comp << std::endl;
    }

Vector3D Vector3D::nullv(void) {
    // Return a vector with zero for all components.
    return Vector3D(NULL_X, NULL_Y, NULL_Z);
    }

bool Vector3D::isNullVector(void) {
    // TODO - consider a tolerance level for nullity.
    return this->len() ? false : true;
    }

vec_t Vector3D::x(void) const {
    return x_comp;
    }

vec_t Vector3D::y(void) const {
    return y_comp;
    }

vec_t Vector3D::z(void) const {
    return z_comp;
    }

void Vector3D::set_x(vec_t xc) {
    x_comp = xc;
    }

void Vector3D::set_y(vec_t yc) {
    y_comp = yc;
    }

void Vector3D::set_z(vec_t zc) {
    z_comp = zc;
    }

Vector3D operator+(const Vector3D& first, const Vector3D& second) {
    return Vector3D(first.x() + second.x(),
                    first.y() + second.y(),
                    first.z() + second.z());
    }

Vector3D operator-(const Vector3D& first, const Vector3D& second) {
    return Vector3D(first.x() - second.x(),
                    first.y() - second.y(),
                    first.z() - second.z());
    }

Vector3D operator-(const Vector3D& vec) {
    return Vector3D(-vec.x(),
                    -vec.y(),
                    -vec.z());
    }

Vector3D operator*(const Vector3D& first, const Vector3D& second) {
    return Vector3D(first.y() * second.z() - first.z() * second.y(),
                    first.z() * second.x() - first.x() * second.z(),
                    first.x() * second.y() - first.y() * second.x());
    }

Vector3D operator*(vec_t factor, const Vector3D& vec) {
    return Vector3D(factor * vec.x(),
                    factor * vec.y(),
                    factor * vec.z());
    }

Vector3D operator*(const Vector3D& vec, vec_t factor) {
    return Vector3D(factor * vec.x(),
                    factor * vec.y(),
                    factor * vec.z());
    }

vec_t operator^(const Vector3D& first, const Vector3D& second) {
    return first.x() * second.x() +
           first.y() * second.y() +
           first.z() * second.z();
    }
