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

#ifndef CONSTELLATIONS_H_
#define CONSTELLATIONS_H_

#include <vector>

#include "Buffer_OBJ.h"
#include "Constellation.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Traversable.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Container type which holds and renders constellations
 *        for 3D display.
 *
 *      Also satisfies the Traversable interface in order to generate upon
 * request a series of Lookouts to visit.
 *
 * \see Buffer_OBJ
 * \see Constellation
 * \see Lookout
 * \see Renderable
 * \see SolarSystemGlobals
 * \see Traversable
 *
 * \author Mike Hewson\n
 */

class Constellations : public Renderable, public Traversable {
   public:
        /**
         * \brief Constructor.
         *
         * \param rad - the radius at which to draw
         */
        Constellations(vec_t rad);

        /**
         * \brief Destructor.
         */
        ~Constellations();

        /**
         * \brief Obtain the number of waypoints available.
         *
         * \return the number of waypoints.
         */
        virtual unsigned int numberOfWayPoints(void) const;

        /**
         * \brief Obtain a Lookout in the sequence.
         *
         * \param sequence : an index into series of waypoints, with a value
         *                   from 0 to that returned by numberOfWayPoints().
         *
         * \return a Lookout representing that waypoint.
         */
        virtual LookOut getView(unsigned int sequence) const;

        /**
         * \brief Cycles the activation state of the object.
         */
        virtual void cycleActivation(void);

    private:
        /// Enumerants defining features to render.
        enum state {ALL_OFF, STARS, STARS_N_LINKS, STARS_N_NAMES, ALL_ON};

        /// Initial rendering state.
        static const state INITIAL_CYCLE_STATE;

        /// Line width for links.
        static const GLfloat LINK_WIDTH;
        /// Stippling pattern for links.
        static const GLushort LINK_STIPPLE_PATTERN;
        /// Scale factor for link stippling.
        static const GLint LINK_STIPPLE_FACTOR;
        /// RGB color components for link.
        static const GLfloat LINK_RGB_RED;
        static const GLfloat LINK_RGB_GREEN;
        static const GLfloat LINK_RGB_BLUE;

        /// The 'dot' size to render a star with.
        static const GLfloat MAG_SIZE;

        /// How many indices in a linking line.
        static const GLuint INDICES_PER_LINK;
        /// How many color components per rendered vertex.
        static const GLuint COLORS_PER_VERTEX;
        /// How many position coordinates per vertex.
        static const GLuint COORDS_PER_VERTEX;
        /// How many bytes, in the buffer, representing both vertex position
        /// and color.
        static const GLuint BYTE_STRIDE_PER_VERTEX;

        /// A fudge factor to vary the displayed text size independently of
        /// font settings.
        static const GLfloat TEXT_RATIO;

        /// How far back ( from the centroid ) along a radial ( from the
        /// origin ) to view a constellation from.
        static const float VIEW_OFFSET;

        static const Vector3D VIEW_UP;

        /// Inner structs for buffer population use.
        struct colors {
            GLfloat red;
            GLfloat green;
            GLfloat blue;
            };

        struct Vert {
            struct colors col;
            vec_t x_pos;
            vec_t y_pos;
            vec_t z_pos;
            };

        /// Store constellation star data here.
        std::vector<Constellation> cons_list;

        /// The radius from the origin at which the constellations will be drawn.
        vec_t radius;

        /// The present drawing state.
        state current_cycle_state;

        /// The OpenGL buffer object wrapper for vertex data.
        Buffer_OBJ buff_obj_points;

        /// The OpenGL buffer object wrapper for index data.
        Buffer_OBJ buff_obj_indices;

        /// Provide OpenGL code to prepare for rendering.
        void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        void release(void);

        /// Provide OpenGL code to render the object.
        void render(void);

        /// This is set during object construction, by summing the number
        /// of stars within each constellation.
        unsigned long int tot_stars;

        /// This is set during object construction, by summing the number
        /// of links within each constellation.
        unsigned long int tot_links;

        /**
         * \brief Load a server buffer with vertex information.
         */
        void loadVertexBuffer(void);

        /**
         * \brief Load a server buffer with index information.
         */
        void loadIndexBuffer(void);

        /**
         * \brief Populate a master listing of display list for constellation
         *        name rendering.
         */
        void createMarkerLists(void);

        void clearMarkerLists(void);

        /**
         * \brief Convert a star's spectral class to an RGB color trio.
         *
         * \param spectral_class - the spectral type of the star
         * \param vt - pointer to a Vert structure where the RGB values will
         *             be placed
         */
        static void class_color(OrdStar::spectral_type spectral_class, Vert* vt);

        /// The master list of display lists used when rendering the
        /// constellation names.
        std::vector<GLuint> marker_lists;
    };

/**
 * @}
 */

#endif // CONSTELLATIONS_H_
