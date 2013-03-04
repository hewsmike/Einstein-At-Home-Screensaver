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
        /// Enumerants to specify the mode of justification
        /// along the vertical axis.
        enum verticalJustification {TOP, MIDDLE, BOTTOM};

        /// Enumerants to specify the mode of justification
        /// along the horizontal axis.
        enum horizontalJustification {LEFT, MIDDLE, RIGHT};

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

        void addLogo(HUDImage* logo);

        GLuint getFrameGap(void) const;

        void setFrameGap(GLuint frame_gap);

        void erase(void);

        GLuint getLogoCount(void) const;

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

        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        GLuint DEFAULT_FRAME_GAP;

        GLuint frame_gap;
        GLuint frame_count;

        GLuint current_image_index;
        GLuint total_image_count;

        verticalJustification vert_just;
        horizontalJustification horz_just;
    };

/**
 * @}
 */

#endif /* HUD_LOGO_CYCLE_H_ */
