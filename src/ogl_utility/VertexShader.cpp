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

#include "VertexShader.h"

#include "ErrorHandler.h"

VertexShader::VertexShader(const std::string source, std::vector<std::pair<GLuint, std::string> >& attribs ) :
              Shader(GL_VERTEX_SHADER, source),
              m_attribs(attribs) {
    }

VertexShader::~VertexShader() {
    }

GLuint VertexShader::attribCount(void) const {
    return m_attribs.size();
    }

std::pair<GLuint, std::string> VertexShader::getAttrib(GLuint index) const {
    // Check were are not asking an attribute we don't have.
    if(index >= this->attribCount()) {
        ErrorHandler::record("VertexShader::getAttrib() : bad attribute request !", ErrorHandler::FATAL);
        }

    return m_attribs.at(index);
    }
