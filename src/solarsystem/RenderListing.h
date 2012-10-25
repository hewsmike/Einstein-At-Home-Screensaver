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
#ifndef RENDER_LISTING_H_
#define RENDER_LISTING_H_

#include <vector>

#include "Renderable.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A wrapper for holding a listing of pointers to heap based Renderable
 *        objects. Semantics of storage and retrieval of pointers assume a
 *        linear container.
 *
 * \author Mike Hewson\n
 */

template<class T> class RenderListing {
    public:
        /**
         * \brief Constructor.
         */
        RenderListing();

        /**
         * \brief Destructor.
         */
        virtual ~RenderListing();

        /**
         * \brief Add a pointer to a heap based Renderable object.
         */
        void add(T* renderable_ptr);

        /**
         * \brief Empty the pointer listing, while freeing any OpenGL
         *  resources and the heap allocation itself.
         */
        void clear(void);

        /**
         * \brief Retrieve a pointer by index ( assumes knowledge of the
         *        ordering of insertion )
         *
         * \param index - the index into the underlying container. Bounds
         *                checking occurs with NULL returned for out of range,
         *                thus including the case where the container is empty.
         *
         * \return A pointer to the Renderable object stored at the given
         *         index, MAY BE NULL.
         */
        const T* at(unsigned int index) const;

        /**
         * \brief Obtain the number of pointer entries
         *
         * \return the number of pointer entries in the listing
         */
        unsigned int size(void) const;

    private:
        /// Storage for the heap allocation pointers.
        std::vector<T*> r_ptrs;
    };

/**
 * @}
 */

#endif // RENDER_LISTING_H_
