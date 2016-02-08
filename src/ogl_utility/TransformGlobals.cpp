/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#include "TransformGlobals.h"

#include "ErrorHandler.h"

/// Initial values of static variables.
/// 3D transform address unknown.
GLvoid* TransformGlobals::m_transform_matrix(NULL);
/// Transform address not yet set.
bool TransformGlobals::m_transform_matrix_set(false);
/// Screen has zero dimensions.
GLuint TransformGlobals::m_height(0);
GLuint TransformGlobals::m_width(0);

TransformGlobals::TransformGlobals(void) {
    }

TransformGlobals::~TransformGlobals(){
    }

void TransformGlobals::setTransformMatrix(GLvoid* load_point) {
    // Can only do this once.
    if(m_transform_matrix_set == false) {
        m_transform_matrix = load_point;
        m_transform_matrix_set = true;
        }
    }

GLvoid* TransformGlobals::getTransformMatrix(void) {
    if(m_transform_matrix_set == false) {
        ErrorHandler::record("TransformGlobals::getTransformMatrix() : access attempt before set !",
                         ErrorHandler::FATAL);
    	}
    return m_transform_matrix;
    }

void TransformGlobals::setClientScreenDimensions(GLuint height, GLuint width) {
    m_height = height;
    m_width = width;
    }

GLuint TransformGlobals::getClientScreenHeight(void) {
    return m_height;
    }

GLuint TransformGlobals::getClientScreenWidth(void) {
    return m_width;
    }

glm::vec2 TransformGlobals::getClientScreenUniform(void) {
    return glm::vec2(2.0/m_width, 2.0/m_height);
    }
