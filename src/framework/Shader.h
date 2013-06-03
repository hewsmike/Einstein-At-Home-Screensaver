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

#ifndef SHADER_H_
#define SHADER_H_

#include <string>

class Shader {
    public:
        enum status {EXISTS, COMPILED, VALID};

        /**
         * Constructor
         *
         * \param a resource identifier, so that a ResourceFactory may
         *        instantiate.
         */
        Shader(const string resource_identifier);

        /**
         * Destructor
         */
        ~Shader();

        /**
         * Obtain the text contents of the shader.
         *
         * \return - an std::string reference representing the text
         */
        const std::string& contents(void) const;

    private:
        std::string text;
    }

#endif /*SHADER_H_*/
