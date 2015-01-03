/***************************************************************************
 *   Copyright (C) 2014 by Mike Hewson                                     *
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

#include <iostream>
#include <sstream>

#include "AttributeInputAdapter.h"

#include "ErrorHandler.h"

AttributeInputAdapter::AttributeInputAdapter(void) {

    }

AttributeInputAdapter::~AttributeInputAdapter() {

    }

void AttributeInputAdapter::addMatching(struct attribute_spec& spec) {
	m_matchings.push_back(spec);
	}

bool AttributeInputAdapter::getAttributeSpecAt(GLuint index, struct attribute_spec* spec) const {
    bool ret_val = false;

    spec = m_matchings[index];

    return ret_val;
	}

GLuint AttributeInputAdapter::size(void) const {
	return m_matchings.size();
	}
