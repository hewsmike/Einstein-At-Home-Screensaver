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
    star_index = 0;
    return getNextStar();
    }

std::vector<std::string> StarDataAdapter::getNextStar(void) {
    std::vector<std::string> ret_val;

    unsigned int param_index = 0;
    std::string param_value;
    do{
        std::stringstream xpath_query;
        xpath_query << XML_BASE_PATH
                    << STAR_ELEMENT_NAME
                    << "["
                    << star_index
                    << "]/"
                    << STAR_DATA_NAME
                    << "["
                    << param_index
                    << "]/";

        param_value = getSingleNodeContentByXPath(xpath_query.str());
        //library/book/title[2]/text()
        if(param_value.size() != 0){
            ret_val.push_back(param_value);
            ++param_index;
            }
        }while(param_value.size() != 0);
    ++star_index;
    return ret_val;
    }
