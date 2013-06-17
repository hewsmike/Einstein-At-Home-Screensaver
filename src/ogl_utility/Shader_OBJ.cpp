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

#include "Shader_OBJ.h"

#include <sstream>

#include "ErrorHandler.h"
#include "Resource.h"
#include "ResourceFactory.h"

const std::string Shader_OBJ::BACKWARD_SHADER_HEADER("");
const std::string Shader_OBJ::FORWARD_SHADER_HEADER("");

Shader_OBJ::Shader_OBJ(const string resource_identifier, Shader_OBJ::context ogl_context) {
    // Assume failure.
    state = Shader_OBJ::INVALID;

    // Instantiate a ResourceFactory in order to obtain the
    // text of the Shader_OBJ.
    ResourceFactory factory;

    Resource* txt = factory.createInstance(resource_identifier);
    if(txt != NULL) {
        const vector<unsigned char>* contents = txt.data();
        for(unsigned int index = 0; index < contents->size(); ++index) {
            text += (*contents)[index];
            }
        switch(ogl_context) {
            case Shader_OBJ::BACKWARD :
                text = Shader_OBJ::BACKWARD_SHADER_HEADER + text;
                break;
            case Shader_OBJ::FORWARD :
                text = Shader_OBJ::FORWARD_SHADER_HEADER + text;
                break;
            default:
                // Should not get here!!
                ErrorHandler::record("Shader_OBJ::Shader_OBJ() : bad switch case reached (default)",
                                     ErrorHandler::FATAL);
                break;
            }


        state = Shader_OBJ::EXISTS;
        }
    else {
        std::stringstream msg;
        msg << "Shader_OBJ::Shader_OBJ() : no resource to construct Shader_OBJ available "
            << "for resource identifier = '" << resource_identifier << "'";
        ErrorHandler::record(msg, ErrorHandler::WARN);
        }
    }

Shader_OBJ::~Shader_OBJ() {
    }

const std::string& Shader_OBJ::contents(void) const {
    return text;
    }

Shader_OBJ::status Shader_OBJ::getStatus(void) const {
    return state;
    }
