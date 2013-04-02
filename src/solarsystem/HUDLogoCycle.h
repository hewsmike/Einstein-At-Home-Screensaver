/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#ifndef HUD_LOGO_CYCLE_H_
#define HUD_LOGO_CYCLE_H_

#include "HUDContainer.h"

#include <utility>

#include "HUDImage.h"

/**
 * \addtogroup  solarsystem Solarsystem
 * @{
 */

/**
 * \brief Container of logo/images to be held and displayed sequentially at
 *        a given screen location.
 *
 * \see HUDContainer
 *
 * \author Mike Hewson\n
 */

class HUDLogoCycle : public HUDContainer {
    public:
         /**
         * \brief Constructor.
         *
         * \param frames : the number of frames between image refreshes
         */
        HUDLogoCycle(GLuint frames = DEFAULT_FRAME_GAP);

        /**
         * \brief Destructor
         */
        virtual ~HUDLogoCycle();

        void erase(void);

        void addItem(HUDImage* logo);

        GLuint getFrameGap(void) const;

        void setFrameGap(GLuint gap);

    protected:
        /**
         * \brief Determine the miminum dimensions as per the internal
         *        layout of items within. Enacts HUDContainer interface.
         *
         * \return std::pair composed of ( in order )
         *          - the desired width
         *          - the desired height
         */
        virtual std::pair<GLuint, GLuint> reassessMinimumDimensions(void);

        /**
         * \brief Determine and set the base positions of each item.
         *        Enacts HUDContainer interface.
         */
        virtual void allocateItemBases(void);

        /// This routine satisfies the Renderable interface.

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        static GLuint DEFAULT_FRAME_GAP;

        GLuint frame_gap;
        GLuint frame_count;

        GLuint current_image_index;
    };

/**
 * @}
 */

#endif /* HUD_LOGO_CYCLE_H_ */
