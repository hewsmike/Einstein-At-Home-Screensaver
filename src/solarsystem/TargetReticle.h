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
#ifndef TARGET_RETICLE_H_
#define TARGET_RETICLE_H_

#include <string>
#include <vector>

#include "HUDImage.h"
#include "Renderable.h"
#include "RenderListing.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A target reticle that appears on the centre of the HUD.
 *
 * \author Mike Hewson\n
 */

class TargetReticle : public Renderable {
    public:
        /**
         * \brief Constructor.
         *
         * \param resource_base_name - the base name of the resource
         *                             which itself is generally of the form
         *                             BASE_NAMEx.TGA with x denoting
         *                             digit(s) from 0 thru to frame_count
         *                             minus one eg. 'reticle0TGA' thru
         *                             'reticle3TGA' ( 'reticle' is this base
         *                             name and the frame_count is 4 )
         * \param screen_width - the width in pixels of the current total client
         *                       display area allocated by OpenGL
         * \param screen_height - the height in pixels of the current total client
         *                        display area allocated by OpenGL
         * \param frame_count - the total number of frames to be animated, which
         *                      MUST at least match the available resources as
         *                      given by the aforementioned naming scheme
         */
        TargetReticle(std::string resource_base_name,
                      GLuint screen_width,
                      GLuint screen_height,
                      unsigned int frame_count);

        /**
         * \brief Destructor.
         */
        virtual ~TargetReticle();

        /**
         * \brief Set reticle animation to first frame
         */
        void reset(void);

        void resize(GLuint screen_width, GLuint screen_height);

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        static const std::string FRAME_RESOURCE_END_NAME;
        static const unsigned int INITIAL_PHASE;

        std::string base_name;

        /// The frame/phase of the reticle animation.
        unsigned int phase;

        /// The current screen height as advised through resize()
        GLuint scr_width;

        /// The current screen width as advised through resize()
        GLuint scr_height;

        unsigned int max_frames;

        /// Storage for the sequence of frames to animate the reticle
        RenderListing frames;
    };

/**
 * @}
 */

#endif // TARGET_RETICLE_H_
