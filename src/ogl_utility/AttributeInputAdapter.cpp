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

void AttributeInputAdapter::addSpecification(const attribute_spec& spec) {
	// Add this to the store.
	m_matchings.push_back(spec);
	}

bool AttributeInputAdapter::getAttributeSpecAt(GLuint index, attribute_spec* spec) const {
	// Assume failure.
    bool ret_val = false;

    // Is the index within range ?
    if(index < m_matchings.size()) {
    	// Yes, copy to the given structure.
    	spec->attrib_index = m_matchings[index].attrib_index;
    	ret_val = true;
    	}
    else {
    	// No, emit a warning.
    	ErrorHandler::record("AttributeInputAdapter::getAttributeSpecAt() : out of range index to attribute store.",
    	                     ErrorHandler::WARN);
    	}

    return ret_val;
	}

GLuint AttributeInputAdapter::size(void) const {
	return m_matchings.size();
	}
