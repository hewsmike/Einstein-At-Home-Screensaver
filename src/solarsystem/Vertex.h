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

#ifndef VERTEX_H_
#define VERTEX_H_

#include "framework.h"

#include <utility>

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates data relevant to an OpenGL vertex.
 *
 * It contains a position vector in 3D space, a normal vector and a
 * pair of co-ordinate values for 2D texture mapping.
 *
 * \see HUDImage
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class Vertex {
    public:
        /**
         * \brief Constructor ( no argument )
         */
        Vertex(void);

        /**
         * \brief Constructor ( fully qualified )
         *
         * \param position The position of the vertex
         *
         * \param normal The normal to the vertex
         *
         * \param texture_coords The texture coordinates of the vertex
         */
        Vertex(const Vector3D& position, const Vector3D& normal,
               const std::pair<GLfloat, GLfloat>& texture_coords);

        /**
         * \brief Constructor ( copy )
         *
         * \param A reference to the vertex to be copied
         */
        Vertex(const Vertex& other);

        /// Destructor
        virtual ~Vertex();

        /**
         * \brief Obtains the position of the vertex
         *
         * \return A position vector to the vertex
         */
        const Vector3D& position(void) const;

        /**
         * \brief Obtains the normal to the vertex
         *
         * \return A vector normal to the vertex
         */
        const Vector3D& normal(void) const;

        /**
         * \brief Obtains texture coordinates of the vertex
         *
         * \return A pair of texture co-ordinates
         */
        const std::pair<vec_t, vec_t>& texture_co_ords(void) const;

    private:
        /// Position in 3D space.
        Vector3D pos;

        /// Normal vector.
        Vector3D norm;

        /// Coordinate value pair for 2D texturing.
        std::pair<vec_t, vec_t> text_cds;
    };

/**
 * @}
 */

#endif // VERTEX_H_
