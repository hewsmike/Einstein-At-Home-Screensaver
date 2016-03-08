/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#ifndef TRIGGERABLE_H_
#define TRIGGERABLE_H_

#include "ogl_utility.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with objects
 *        that involve triggering of the OpenGL pipeline.
 *
 * \author Mike Hewson\n
 */

class Triggerable {
    public:
        /**
         * \brief Constructor.
         */
        Triggerable(void);

        /**
         * \brief Destructor.
         */
        virtual ~Triggerable();

        /**
         * \brief Trigger pipeline activity.
         *
         * \param primitive : one of the OpenGL primitives
         *          GL_POINTS
         *          GL_LINE_STRIP
         *          GL_LINE_LOOP
         *          GL_LINES
         *          GL_TRIANGLE_STRIP
         *          GL_TRIANGLE_FAN
         *          GL_TRIANGLES
         * \param count : how many times to invoke the vertex shader.
         */
        virtual void trigger(GLenum primitive, GLsizei count) = 0;

    private:
    };

/**
 * @}
 */

#endif /*TRIGGERABLE_H_*/
