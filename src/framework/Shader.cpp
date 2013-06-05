/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include "Shader.h"

#include <sstream>

#include "ErrorHandler.h"
#include "Resource.h"
#include "ResourceFactory.h"

const std::string Shader::BACKWARD_SHADER_HEADER("");
const std::string Shader::FORWARD_SHADER_HEADER("");

Shader::Shader(const string resource_identifier, Shader::context ogl_context) {
    // Assume failure.
    state = Shader::INVALID;

    // Instantiate a ResourceFactory in order to obtain the
    // text of the shader.
    ResourceFactory factory;

    Resource* txt = factory.createInstance(resource_identifier);
    if(txt != NULL) {
        const vector<unsigned char>* contents = txt.data();
        for(unsigned int index = 0; index < contents->size(); ++index) {
            text += (*contents)[index];
            }
        switch(ogl_context) {
            case Shader::BACKWARD :
                text = Shader::BACKWARD_SHADER_HEADER + text;
                break;
            case Shader::FORWARD :
                text = Shader::FORWARD_SHADER_HEADER + text;
                break;
            default:
                // Should not get here!!
                ErrorHandler::record("Shader::Shader() : bad switch case reached (default)",
                                     ErrorHandler::FATAL);
                break;
            }


        state = Shader::EXISTS;
        }
    else {
        std::stringstream msg;
        msg << "Shader::Shader() : no resource to construct shader available "
            << "for resource identifier = '" << resource_identifier << "'";
        ErrorHandler::record(msg, ErrorHandler::WARN);
        }
    }

Shader::~Shader() {
    }

const std::string& Shader::contents(void) const {
    return text;
    }

Shader::status Shader::getStatus(void) const {
    return state;
    }
