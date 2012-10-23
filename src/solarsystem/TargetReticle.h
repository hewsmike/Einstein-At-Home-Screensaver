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
         */
        TargetReticle(std::string resource_base_name, unsigned int frames);

        /**
         * \brief Destructor.
         */
        virtual ~TargetReticle();

        /**
         * \brief Set reticle animation to first frame
         */
        void reset(void);

        virtual void SSGUpdate(void);

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

        std::string base_name;
        unsigned int frame_count;

        /// The frame/phase of the reticle animation.
        unsigned int phase;

        std::vector<HUDImage*> frames;

        void clearFrames(void);
    };

/**
 * @}
 */

#endif // TARGET_RETICLE_H_
