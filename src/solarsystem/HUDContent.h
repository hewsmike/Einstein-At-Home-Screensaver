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

#ifndef HUD_CONTENT_H_
#define HUD_CONTENT_H_

#include "HUDItem.h"

/**
 * \addtogroup  solarsystem Solarsystem
 * @{
 */

/**
 * \brief Base class for holders of images and text with fixed horizontal
 *        and vertical margins.
 *
 * \see HUDItem
 *
 * \author Mike Hewson\n
 */

class HUDContent : public HUDItem {
    public:
        /**
         * \brief Constructor.
         *
         * \param horizontalMargin : the horizontal pixel margin to be applied
         *                           either side of the enclosed content
         * \param verticalMargin : the vertical pixel margin to be applied
         */
        HUDContent(GLuint horizontalMargin, GLuint verticalMargin);

        /**
         * \brief Destructor
         */
        virtual ~HUDContent();

        /**
         * \brief Callback target for the static SolarSystemGlobals instance.
         *
         *      This routine is called when there is a change in the window
         * size. We need to trigger reacquisition of server side resources on
         * Windows systems if the window is resized.
         */
        void resizeUpdate(GLuint width, GLuint height);

        /**
         * \brief Get the size of the fixed horizontal margin
         *
         * \return the horizontal margin
         */
        GLuint horzMargin(void) const;

        /**
         * \brief Get the size of the fixed vertical margin
         *
         * \return the vertical margin
         */
        GLuint vertMargin(void) const;

    private:
        /// Current margins
        GLuint hz_marg;
        GLuint vt_marg;
    };

/**
 * @}
 */

#endif /* HUD_CONTENT_H_ */
