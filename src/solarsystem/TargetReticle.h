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
        TargetReticle(void);

        /**
         * \brief Destructor.
         */
        virtual ~TargetReticle();

        void reset(void);

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        unsigned int phase;
    };

/**
 * @}
 */

#endif // TARGET_RETICLE_H_
