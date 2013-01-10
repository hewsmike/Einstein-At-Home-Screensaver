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

#ifndef SPHERE_H_
#define SPHERE_H_

#include "framework.h"

#include <vector>

#include <oglft/OGLFT.h>

#include "SolarSystemGlobals.h"
#include "Vector3D.h"
#include "VectorSP.h"
#include "Vertex.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class creates OpenGL vertex data for a spherical shape.
 *
 *      The sphere is approximated polygonally by a mesh of points at
 * specific latitude and longitude values. Stacks refers to how many
 * distinct values of latitude ( counting each pole as one ) are desired,
 * and slices as to how many distinct values of longitude. I recommend an
 * odd number of stacks so that one will have points on the equator. The
 * radius being the common distance of all points from the origin.
 *
 *      Each vertex ( in the OpenGL sense ) is given a unit normal in the
 * radial direction.
 *
 *      Staggering refers to whether or not the longitude values b/w
 * consecutive latitudes are identical ( non-staggered ) or mid-way
 * ( staggered ). There are two effects of this. The main one is that
 * triangles can be used ( staggered case ) rather than quadrilaterals
 * ( non-staggered case ) when constructing primitives to render upon.
 * The other effect is visual, being a closer approximation to a spherical
 * appearance in the staggered case ( but this is arguable ).
 *
 *      Stitching refers to potential usage with OpenGL texturing, as
 * points along the prime meridian are given two vertices, each with
 * equal position coordinates, the former with a horizontal texture
 * coordinate of 0.0f and the latter with a value of 1.0. This is
 * required to sucessfully apply the full texture map in a decal like
 * fashion.
 *
 *      It is also highly recommended that the following call be made
 * prior to subsequent texturing upon the vertices :
 *
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 *
 * on account of the behaviour of the texture coordinate values at the
 * prime meridian ( s = 0.0f ). This, for 2D tectures, sets the 'horizontal'
 * texture coordinate (s) to cyclically repeat at 0.0f and 1.0f. For example
 * a coordinate of -0.05 will map to 0.95, and 1.05 will map to 0.05.
 *
 * \author Mike Hewson\n
 */

class Sphere {
    public:
        /// Minimal sphere parameters.
        static const GLfloat MIN_RADIUS;
        static const GLuint MIN_SLICES;
        static const GLuint MIN_STACKS;

        /**
         * \brief Constructor
         *
         * \param radius : the radius
         *
         * \param slices : the number of longitudinal meridians
         *
         * \param stacks : the number of latitudinal bands
         *
         * \param stagger : whether or not longitude values stagger
         *
         * \param stitch : whether or not we create 'stitch' vertices
         */
        Sphere(vec_t radius, GLuint slices, GLuint stacks, bool stagger, bool stitch);

        /**
         * \brief Destructor
         */
        ~Sphere();

        /**
         * \brief Read only access to vertex listing
         *
         * \return the entire listing of vertices
         */
        const std::vector<Vertex>& vertices(void) const;

        /**
         * \brief Read only access to slice vertices index listing
         *
         * \return an array of vertex indices organised by slice
         */
        const std::vector<std::vector<GLuint> >& sliceIndices(void) const;

        /**
         * \brief Read only access to stack vertices index listing
         *
         * \return an array of vertex indices organised by stack
         */
        const std::vector<std::vector<GLuint> >& stackIndices(void) const;

        /**
         * \brief Obtain the radius
         *
         * \return the radius
         */
        GLfloat radius(void) const;

        /**
         * \brief Obtain the total number of slices
         *
         * \return the number of slices
         */
        GLuint slices(void) const;

        /**
         * \brief Obtain the total stack number of stacks
         *
         * \return the number of stacks
         */
        GLuint stacks(void) const;

        /**
         * \brief Obtain the stagger setting
         *
         * \return the stagger setting
         *              true, if staggered
         *              false, if not
         */
        bool isStaggered(void) const;

        /**
         * \brief Obtain the stitch setting
         *
         * \return the stitch setting
         *              true, if stitched
         *              false, if not
         */
        bool isStitched(void) const;

    private:
        /// Radius of the sphere.
        vec_t rad;

        /// The number of longitudinal values.
        GLuint num_slices;

        /// The number of latitudinal values ( counting each pole as one band )
        GLuint num_stacks;

        /// Flag indicating whether we stagger the vertices or not.
        bool stagger_flag;

        /// Flaf indicating whether we insert stitch vertices or not.
        bool stitch_flag;

        /// Master listing of vertices.
        std::vector<Vertex> verts;

        /// Listing of vertex indices by slices.
        std::vector<std::vector<GLuint> > slice_indices;

        /// Listing of vertex indices by stacks.
        std::vector<std::vector<GLuint> > stack_indices;

        /**
         * \brief Vertex creator
         *
         *      Construct a master list of vertices, beginning at the
         * north pole and thence by latitude ( which are each listed
         * from lowest longitude to highest ) until the south pole.
         */
        void makeVertices(void);

        /**
         * \brief Stack index creator
         *
         *      Construct an array of vertex indices organised by stacks.
         * The first stack is the north pole alone, followed by intermediate
         * latitudes and finally the south pole alone.
         */
        void makeStackIndices(void);

        /**
         * \brief Slice index creator
         *
         *      Construct an array of vertex indices organised by slices.
         * The first slice is the prime meridian ( including those positively
         * adjacent in longitiude if staggered ), then each longitude mutatis
         * mutandis, and finally the longitude negatively adjacent to the
         * prime meridian.
         */
        void makeSliceIndices(void);
    };

/**
 * @}
 */

#endif // SPHERE_H_
