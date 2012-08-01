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

#include "StarDataAdapter.h"

#include <iostream>
#include <sstream>

const std::string StarDataAdapter::XML_BASE_PATH("/RESOURCE/TABLE/DATA/TABLEDATA/");
const std::string StarDataAdapter::STAR_ELEMENT_NAME("TR");
const std::string StarDataAdapter::STAR_DATA_NAME("TD");

StarDataAdapter::StarDataAdapter(const string xml, const string url) {
    setXmlDocument(xml, url);
    star_index = 0;
    }

StarDataAdapter::~StarDataAdapter() {
    }

std::vector<std::string> StarDataAdapter::getFirstStar(void) {
    // Reset index to zero.
    star_index = 0;

    // Then get the data.
    return getNextStar();
    }

std::vector<std::string> StarDataAdapter::getNextStar(void) {
    // An array of strings to return a single star's
    // stored data parameters with.
    std::vector<std::string> ret_val;

    // Assume no parameters.
    unsigned int param_index = 0;
    std::string param_value;
    do{
        // Constuct an XPath query string, using the current
        // indicies for star and parameter. Context node for
        // the query is the document's root node.
        std::stringstream xpath_query;
        xpath_query << XML_BASE_PATH
                    << STAR_ELEMENT_NAME
                    << "["
                    << star_index
                    << "]/"
                    << STAR_DATA_NAME
                    << "["
                    << param_index
                    << "]/text()";

        // Use the constructed string to find any content.
        param_value = getSingleNodeContentByXPath(xpath_query.str());

        // If the query succeeded and content returned ...
        if(param_value.size() != 0) {
            std::cout << "Yeey hha" << std::endl;
            // Then save said content to our string array.
            ret_val.push_back(param_value);
            // Assume another parameter might be available.
            ++param_index;
            }

        // Keep queries going until no content returned.
        }while(param_value.size() != 0);

    // Nudge the index and return what ( if any ) was found.
    ++star_index;
    return ret_val;
    }
