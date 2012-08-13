/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

#ifndef STAR_DATA_H_
#define STAR_DATA_H_

#include "Libxml2Adapter.h"

#include <string>
#include <vector>

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class performs retrieval of star data from XML sources.
 *
 * \author Mike Hewson\n
 */

class StarDataAdapter : public Libxml2Adapter {
    public:
        /**
         * \brief Constructor
         */
        StarDataAdapter(const string& xmlFile, const string xmlURL);

        /**
         * \brief Destructor
         */
        virtual ~StarDataAdapter();

    // protected:
        /**
         * \brief Obtain data for the first star from the data source.
         *
         * \return A list of star parameters, empty if none available.
         */
        std::vector<std::string> getFirstStar(void);

        /**
         * \brief Obtain the next star from the data source.
         *
         * \return A list of star parameters, empty if none available.
         */
        std::vector<std::string> getNextStar(void);

    private:
        /// These depend upon the XML document's data model,
        /// being used to construct an XPath expression for
        /// actual data retrieval.
        static const std::string XML_BASE_PATH;
        static const std::string STAR_ELEMENT_NAME;
        static const std::string STAR_DATA_NAME;

        std::string file;
        std::string url;

        /// The index of the current star's
        /// data 'record' being accessed.
        unsigned int star_index;
    };

/**
 * @}
 */

#endif /* STAR_DATA_H_ */
