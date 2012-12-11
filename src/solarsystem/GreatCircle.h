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

#ifndef GREAT_CIRCLE_H_
#define GREAT_CIRCLE_H_

#include "framework.h"

#include <string>
#include <utility>
#include <vector>

#include :Line.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class renders a great circle centered upon the coordinate origin.
 *
 *      The combination of the normal ( N ) to the plane of the great circle
 * and the vector ( L ) to the point designated as of zero longitude defines
 * a sense of increase in the longitude value as follows : N x L is in the
 * direction of said increase.
 *
 * \see Line
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class GreatCircle public Renderable {
    public:
        /**
         * \brief Constructor
         *
         * \param normal : vector perpendicular to the plane of the great circle.
         *
         * \param zero_long : vector to the point on the great circle where
         *                    the zero of coordinate longitude is deemed to lie.
         *
         * \param line : rendering characteristics for the great circle.
         *
         * \param radius : the radius of the great circle from the coordinate
         *                 origin.
         */
        GreatCircle(const Vector3D& normal, const Vector3D& zero_long,
                    Line line, GLfloat radius);

        /**
         * Destructor
         */
        virtual ~GreatCircle();

        /**
         * \brief Adds a descriptive comment to a position of longitude
         *        upon the great circle.
         *
         * \param longitude : the angle from longitude zero to place the comment.
         *
         * \param description : the comment to place there.
         */
        void addComment(GLfloat longitude, std::string description);

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// Unit normal perpendicular to plane of great circle.
        Vector3D norm;

        /// Unit vector pointing to the point of zero longitude.
        Vector3D z_long;

        /// Line characterising renderable features.
        Line gc_line;

        /// Radius of the circle.
        GLfloat rad;

        /// Optional commenting on great circle features,
        /// eg. galactic centre or summer solstice.
        std::vector<std::pair<GLfloat, std::string> > features;
    };

/**
 * @}
 */

#endif // GREAT_CIRCLE_H_
