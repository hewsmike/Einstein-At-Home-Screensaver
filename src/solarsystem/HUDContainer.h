/***************************************************************************
 * Copyright (C) 2013 by Mike Hewson                                       *
 * hewsmike[AT]iinet.net.au                                                *
 *                                                                         *
 * This file is part of Einstein@Home.                                     *
 *                                                                         *
 * Einstein@Home is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published       *
 * by the Free Software Foundation, version 2 of the License.              *
 *                                                                         *
 * Einstein@Home is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef HUD_CONTAINER_H_
#define HUD_CONTAINER_H_

#include <map>
#include <utility>

#include "HUDItem.h"

/**
* \addtogroup solarsystem Solarsystem
* @{
*/

/**
* \brief Base container type for display of HUD content.
*
* The key design feature here is that a HUDContainer
* may be one/both of container and contained. That is, any
* HUDContainer object may contain another HUDContainer
* object.
*
* Any contained object(s) will be activated for rendering
* when the container itself is activated, but only those
* present within at such time of activation.
*
* \see HUDItem
*
* \author Mike Hewson\n
*/

class HUDContainer : public HUDItem {
    public:
        enum Mode{RETAIN, DESTROY};

        /**
         * \brief Constructor
         */
        HUDContainer(Mode mode);

        /**
         * \brief Destructor
         */
        virtual ~HUDContainer();

        /**
         * \brief Resize this container, at or above it's minimum dimensions
         *
         * \param newWidth : the desired width
         * \param newHeight : the desired height
         *
         * \return bool : whether the resize occured
         * - true if a resize was allowed
         * - false if below minimum was requested
         */
        virtual bool requestResize(GLuint newWidth, GLuint newHeight);

        /**
         * \brief Set the lower left corner window coordinates
         *
         * \param newHorz - the desired horizontal coordinate
         * \param newVert - the desired vertical coordinate
         */
        virtual void reBase(GLuint newHorz, GLuint newVert);

        /**
         * \brief Re-evaluate this container's sizing and contained items'
         * positions. Triggered by a change within the container.
         */
        void adjust(void);

        /**
         * \brief Empty the underlying container.
         */
        void erase(void);

        /**
         * \brief Obtain the count of contained items
         *
         * \return the count
         */
        GLuint itemCount(void) const;

        /**
         * \brief Query whether anything is contained within
         *
         * \return boolean : indicating true if empty, false otherwise
         */
        bool isEmpty(void) const;

        /**
         * \brief Get a pointer to a contained item
         *
         * \param handle : the identifier used when the item was added
         *
         * \return HUDItem* : a pointer to the item ( NULL if not present )
         */
        HUDItem* getItem(int handle);

        /**
         * \brief Add an item to this container.
         *
         * IF the given pointer was not NULL, then
         * - the item was inserted, and
         * - any existing item with the same handle was removed, and
         * - this container's minimum dimensions were adjusted
         *
         * \param handle : an identifier for the item
         * \param obj : the item's pointer
         */
        void addItem(int handle, HUDItem* item);

        /**
         * \brief Remove an item from this container.
         *
         * Any existing item with the same handle is removed, and
         * this container's minimum dimensions are adjusted
         *
         * \param handle : an identifier for the item
         */
        void removeItem(int handle);

    protected:
        std::map<int, HUDItem*>& getMap(void);

        /**
         * \brief Re-assess the minimal width and height.
         *
         * Pure virtual as this depends upon the container's
         * internal layout, and a subclass must enact this.
         *
         * \return std::pair composed of ( in order )
         * - the minimum width
         * - the minimum height
         */
        virtual std::pair<GLuint, GLuint> reassessMinimumDimensions(void) = 0;

        /**
         * \brief Determine and set the base positions of each item.
         *
         * Pure virtual as only a subclass will know how to do this.
         */
        virtual void allocateItemBases(void) = 0;

        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// Associative array b/w item pointers and integral labels.
        std::map<int, HUDItem*> container;

        Mode mode_type;
    };

/**
* @}
*/

#endif /* HUD_CONTAINER_H_ */
