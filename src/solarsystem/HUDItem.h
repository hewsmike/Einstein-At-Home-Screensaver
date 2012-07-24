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

#ifndef HUD_ITEM_H_
#define HUD_ITEM_H_

#include "Renderable.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Base type for all HUD classes.
 *
 *      Especially note that the option exists of storing a pointer to
 * a HUDContainer type. This is used, if non-NULL, in order to alert
 * an enclosing HUDContainer instance that an enclosed HUDItem
 * instance has changed it's size, and hence that the enclosing
 * HUDContainer ought recalculate it's dimensions etc to account
 * for that. This also includes cases where said HUDItems are added or
 * deleted from a container, and where the container is activated or
 * inactivated.
 *
 * \see Renderable
 *
 * \author Mike Hewson\n
 */

/// Forward declaration ie. HUDContainer is a type
/// for which we will hold a pointer to.
class HUDContainer;

class HUDItem : public Renderable {
    public:
        /**
         * \brief Constructor
         */
        HUDItem();

        /**
         * \brief Destructor
         */
        virtual ~HUDItem();

        /**
         * \brief Obtain the minimum width
         *
         * \return the minimum width
         */
        GLuint minWidth(void) const;

        /**
         * \brief Obtain the minimum height
         *
         * \return the minimum height
         */
        GLuint minHeight(void) const;

        /**
         * \brief Obtain the current lower left corner
         *        horizontal coordinate.
         *
         * \return the horizontal coordinate
         */
        GLuint horzBase(void) const;

        /**
         * \brief Obtain the current lower left corner
         *        vertical coordinate.
         *
         * \return the vertical coordinate
         */
        GLuint vertBase(void) const;

        /**
         * \brief Set the lower left corner window coordinates
         *
         * \param newHorz - the desired horizontal coordinate
         * \param newVert - the desired vertical coordinate
         */
        void virtual reBase(GLuint newHorz, GLuint newVert);

        /**
         * \brief Set the enclosing container.
         *
         * \param enclosing : a pointer to the enclosing container
         */
        void setEnclosingContainer(HUDContainer* enclosing);

        /**
         * \brief Get the enclosing container, if any.
         *
         * \return a pointer to the container ( may be NULL )
         */
        HUDContainer* getEnclosingContainer(void) const;

    protected:
        /**
         * \brief Unconditionally set the minimum dimensions.
         *
         *      Note a derived class must set the minimum
         * dimensions via this function upon construction or
         * change that implies a resize.
         *
         * \param minWidth - the minimum width
         * \param minHeight - the minimum height
         */
        void setMinimumDimensions(GLuint minWidth, GLuint minHeight);

    private:
        /// Current minimum dimensions.
        GLuint wd_min;
        GLuint ht_min;

        /// Current coordinates of lower left corner.
        GLuint hz_base;
        GLuint vt_base;

        /// Which enclosing container to call if that container
        /// needs to resize upon an alteration to this item.
        HUDContainer* surround;
    };

/**
 * @}
 */

#endif /* HUD_ITEM_H_ */
