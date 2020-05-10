/***************************************************************************
 *   Copyright (C) 2020 by Mike Hewson                                     *
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

#include <sstream>

#include "ErrorHandler.h"

/// Initial values of static variables.
glm::mat4* TransformGlobals::m_transform_matrix_perspective(NULL);
glm::mat4* TransformGlobals::m_transform_matrix_orthographic(NULL);

/// Screen has zero dimensions.
GLuint TransformGlobals::m_height(0);
GLuint TransformGlobals::m_width(0);

TransformGlobals::TransformGlobals(void) {
    }

TransformGlobals::~TransformGlobals(){
    }

void TransformGlobals::setPerspectiveTransformMatrix(glm::mat4* load_point) {
    m_transform_matrix_perspective = load_point;
    }

glm::mat4* TransformGlobals::getPerspectiveTransformMatrix(void) {
    if(m_transform_matrix_perspective == NULL) {
        ErrorHandler::record("TransformGlobals::getPerspectiveTransformMatrix() : access attempt before set !",
                         ErrorHandler::FATAL);
        }
    return m_transform_matrix_perspective;
    }

void TransformGlobals::setOrthographicTransformMatrix(glm::mat4* load_point) {
    m_transform_matrix_orthographic = load_point;
    }

glm::mat4* TransformGlobals::getOrthographicTransformMatrix(void) {
    if(m_transform_matrix_orthographic == NULL) {
        ErrorHandler::record("TransformGlobals::getOrthographicTransformMatrix() : access attempt before set !",
                         ErrorHandler::FATAL);
        }
    return m_transform_matrix_orthographic;
    }

void TransformGlobals::setClientScreenDimensions(GLuint height, GLuint width) {
    if((height > 0) && (width >0)) {
        m_height = height;
        m_width = width;
        }
    else {
        std::stringstream msg;
        msg << "TransformGlobals::setClientScreenDimensions() : non-strictly positive dimensions \t"
            << "height = " << height
            << "\twidth = " << width;
        ErrorHandler::record(msg.str(), ErrorHandler::FATAL);
        }
    }

GLuint TransformGlobals::getClientScreenHeight(void) {
    return m_height;
    }

GLuint TransformGlobals::getClientScreenWidth(void) {
    return m_width;
    }
