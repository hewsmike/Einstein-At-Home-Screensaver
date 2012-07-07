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

#ifndef SUPERNOVA_H_
#define SUPERNOVA_H_

#include "Star.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Supernova data type.
 *
 *      This class comprises the astronomical data relating to a specific supernova.
 * TODO maybe include a name field in constructor, passed through to base class?
 *
 * \see Star
 *
 * \author Mike Hewson\n
 */

class Supernova : public Star {
    public:
        /**
         * \brief Constructor
         *
         * \param ra - right ascension of the supernova
         *
         * \param dec - declination of the supernova
         */
        Supernova(starpos_t ra, starpos_t dec);

        /**
         * \brief Destructor
         */
        virtual ~Supernova();
    };

/**
 * @}
 */

#endif /*SUPERNOVA_H_*/
