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

#ifndef GRID_GLOBE_H_
#define GRID_GLOBE_H_

#include "Buffer_OBJ.h"
#include "Line.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 *  @{
 */

/**
 * \brief This class defines a renderable object of spherical shape with
 *        grid features, primarily used as for coordinate display.
 *
 * \see Buffer_OBJ
 * \see Line
 * \see Renderable
 * \see SolarSystemGlobals
 * \see Sphere
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class GridGlobe : public Renderable {
    public:
        /// Enumerant of line features.
        enum lineType {MAIN, EQUATOR, PRIME_MERIDIAN};

        /// Which side will text be readable on?
        enum textFacing {INSIDE, OUTSIDE};

        /// Units for right ascension display
        enum ra_units {DEGREES, HOURS};

        /**
         * \brief Constructor.
         *
         * \param rad : the radius of the sphere.
         * \param slices : the number of longitudinal partitions.
         * \param stacks : the number of latitudinal partitions.
         * \param tf : the side ( in or out ) from which text ought be read.
         */
        GridGlobe(vec_t rad, GLuint slices, GLuint stacks, GridGlobe::textFacing tf);

        /**
         * \brief Destructor.
         */
        virtual ~GridGlobe();

        /**
         * \brief Cycles the activation state of the object
         */
        virtual void cycleActivation(void);

        /**
         * \brief Set the rendering features of a given line feature
         *
         * \param type : the line feature to set the values of.
         * \param line : the line features to render using
         */
        void setLine(lineType type, Line line);

        /**
         * \brief Set the units for right ascension display
         *
         * \param units : an enumerant indicating the units choice
         */
        void setRAUnits(GridGlobe::ra_units units);

    private:
        /// Cyclic rendering states
        enum state {ALL_OFF, GRID, ALL_ON};

        /// Initial cycle state.
        static const state INITIAL_CYCLE_STATE;

        /// OpenGL server side buffer parameters.
        static const GLuint ARRAY_START;
        static const GLsizei ARRAY_STRIDE;
        static const GLuint BYTE_OFFSET;
        static const GLuint COORDS_PER_VERTEX;
        static const GLuint VERTICES_PER_LINK;

        /// Whether adjacent latitudes have a longitude offset.
        static const bool STAGGERING;

        /// Whether a prime meridian longitude is reproduced.
        static const bool STITCHING;

        /// Scaling and positioning factors.
        static const GLfloat TEXT_RATIO;
        static const GLfloat TEXT_OTHER_RATIO;
        static const GLfloat TEXT_OFFSET;
        static const GLfloat TEXT_UNITS_RATIO;

        /// Structure for vertex position data.
        struct Vert {
            vec_t x_pos;
            vec_t y_pos;
            vec_t z_pos;
            };

        Line main;
        Line equator;
        Line prime_meridian;

        Sphere* sp;

        vec_t radius;
        GLuint slices;
        GLuint stacks;

        bool hasEquator;

        bool textInside;

        bool ra_is_hours;

        state current_cycle_state;

        GLuint grid_links;

        GLuint prime_meridian_links;

        GLuint celestial_equator_links;

        /// The OpenGL buffer object wrapper for vertex data.
        Buffer_OBJ buff_obj_points;

        /// The OpenGL buffer object wrapper for grid vertex indices.
        Buffer_OBJ buff_obj_grid_links;

        /// The OpenGL buffer object wrapper for grid vertex indices.
        Buffer_OBJ buff_obj_prime_meridian_links;

        /// The OpenGL buffer object wrapper for grid vertex indices.
        Buffer_OBJ buff_obj_celestial_equator_links;

        /**
         * \brief Load the vertex list into a server side buffer.
         */
        void loadVertexBuffer(void);

        /**
         * \brief Load the vertex indice list for the grid
         *        into a server side buffer.
         */
        void loadGridIndexBuffer(void);

        /**
         * \brief Load the vertex indice list for the prime
         *        meridian into a server side buffer.
         */
        void loadPrimeMeridianIndexBuffer(void);

        /**
         * \brief Load the vertex indice list for the celestial
         *        equator into a server side buffer.
         */
        void loadCelestialEquatorIndexBuffer(void);

        /**
         * \brief Create display lists for coordinate markers.
         */
        void createMarkerLists(void);

        /**
         * \brief Clears the OpenGL display lists for the grid marking symbols.
         *
         *      Releases OpenGL display list resources associated with the grid
         * markers, and empties the marker_lists STL container. Used when cycling
         * the display components and overall resource release with inactivation.
         */
        void clearMarkerLists(void);

        /// Provide OpenGL code to prepare for rendering.
        void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        void release(void);

        /// Provide OpenGL code to render the object.
        void render(void);

        /// The listing by latitude/stacks of the coordinate markers.
        std::vector<std::vector<GLuint> > marker_lists;
    };

/**
* @}
*/

#endif // GRID_GLOBE_H_
