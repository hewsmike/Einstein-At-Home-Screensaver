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

#ifndef PTRHOLDER_H_
#define PTRHOLDER_H_

#include "framework.h"

#include <vector>

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Generic container for pointers to heap objects
 * of Renderable type.
 *
 *      This class encapsulates a variable length array of pointers
 * to heap based objects of Renderable type. Ensures proper cleanup
 * of both OpenGL and heap resources.
 *
 * \author Mike Hewson\n
 */

class PtrHolder {
    public:
        /**
         * \brief Constructor
         */
        PtrHolder(void);

        /**
         * \brief Destructor
         */
        virtual ~PtrHolder();

        /**
         * \brief Add a pointer to a heap object.
         */
        void add(const Renderable* pointer);

        /**
         * \brief Empty the container of pointers while releasing
         * OpenGL resources and heap memory.
         */
        void clear(void);

        /**
         * \brief Obtain access to the pointer listing.
         *
         * \return a reference to the pointer listing.
         */
        const std::vector<Renderable*>& pointers(void);

    private:
        // The pointer listing.
        std::vector<Renderable*> ptr_list;
    };

/**
 * @}
 */

#endif /* PTRHOLDER_H_*/
