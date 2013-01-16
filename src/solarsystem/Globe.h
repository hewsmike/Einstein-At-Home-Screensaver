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
#ifndef GLOBE_H_
#define GLOBE_H_

#include "framework.h"

#include <string>

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "ResourceFactory.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Texture_OBJ.h"

/**
* \addtogroup solarsystem Solarsystem
* @{
*/

/**
* \brief A Renderable entity of spherical shape with texture, to be
*        instantiated in detail for the Earth, the Sun etc.
*
* \see Buffer_OBJ
* \see Renderable
* \see ResourceFactory
* \see SolarSystemGlobals
* \see Sphere
* \see Texture_OBJ
*
* \author Mike Hewson\n
*/

class Globe : public Renderable {
    public:
        /**
         * \brief Constructor.
         *
         * \param name : the label of the globe ( Earth, Sun .... ).
         * \param image_file_name : the resource name identifying the image for
         *        use as a texture.
         * \param radius : the radius of the globe.
         * \param stacks : the number of latitudinal values ( including poles )
         *        to use in approximating a spherical surface.
         * \param slices : the number of longitudinal values to use in
         *        approximating a spherical surface.
         * \param zero_longitude_offset : displacement, expressed as an OpenGL
         *        horizontal texture coordinate value, into the pixel map where
         *        the zero of longitude is deemed to lie eg. Greenwich meridian
         *        on Earth.
         */
        Globe(std::string name,
              std::string resource_name,
              GLfloat radius,
              GLuint stacks,
              GLuint slices,
              GLfloat zero_longitude_offset);

        /**
         * \brief Destructor
         */
        ~Globe();

    private:
        /// Enumeration to select either pole.
        enum pole {SOUTH, NORTH};

        /// The number of position coordinates per OpenGL vertex.
        static const GLuint POS_COORDS_PER_VERTEX;

        /// The number of normal vector coordinates per OpenGL vertex.
        static const GLuint NORM_COORDS_PER_VERTEX;

        /// The number of texture coordinates per OpenGL vertex.
        static const GLuint TEXTURE_COORDS_PER_VERTEX;

        /// The total number of coordinates per OpenGL vertex.
        static const GLuint ELEMENTS_PER_VERTEX;

        /// Offset of the vertex data into the server-side buffer
        static const GLuint ARRAY_START;

        /// The number of bytes in the server-side buffer between each vertex
        static const GLsizei ARRAY_STRIDE;

        /// Whether adjacent latitudes have a longitude offset.
        static const bool STAGGERING;

        /// Whether longitude zero is represented by two vertex lists with
        /// differing horizontal texture coordinates ie. ensuring the right
        /// edge of the texture map is identifed with the left edge for proper
        /// wrapping of the image.
        static const bool STITCHING;

        /// Structure for texture coordinates.
        struct Text {
            vec_t s;
            vec_t t;
            };

        /// Structure for normal coordinates.
        struct Norm {
            vec_t x;
            vec_t y;
            vec_t z;
            };

        /// Structure for position coordinates.
        struct Position {
            vec_t x;
            vec_t y;
            vec_t z;
            };

        /// Structure combining the above three to represent vertex data, in
        /// the correct manner for the OpenGL GL_T2F_N3F_V3F interleaved
        /// array pattern.
        struct Vert {
            Text text;
            Norm norm;
            Position pos;
            };

        /// The globe's name eg. Earth
        std::string nm;

        /// The resource name of the image to use as a texture upon the globe.
        std::string image_resource_name;

        /// The horizontal texture coordinate offset - into the pixel map -
        /// that indicates the zero of 'geographical' longitude.
        GLfloat zlo;

        /// The buffer object for the northern polar cap.
        Buffer_OBJ north_cap_indices;

        /// The buffer object for waist rendering.
        Buffer_OBJ waist_indices;

        /// The buffer object for the southern polar cap.
        Buffer_OBJ south_cap_indices;

        /// The underlying geometric model approximating a sphere.
        Sphere sp;

        /// An OpenGL buffer object ( in server-side memory ) holding
        /// the vertex associated data for an entire sphere approximation.
        Buffer_OBJ vertex_buffer;

        /// An OpenGL texture object identifier
        Texture_OBJ texture;

        /// The number of vertices per latitude/stack.
        GLuint verts_per_lat;

        /// The number of longitude values used.
        GLuint num_slices;

        /// The number of latitude values used, including one per pole.
        GLuint num_stacks;

        /// Vertex index counter.
        GLuint last_vertex_index;

        /**
         * \brief Load a pixel map into a server-side texture with mipmap
         *        generation.
         */
        void loadTexture(void);

        /// Provide OpenGL code to prepare for rendering.
        void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        void release(void);

        /// Provide OpenGL code to render the object.
        void render(void);

        /**
         * \brief Load all of the vertex data into a server-side
         *        buffer.
         */
        void loadVertexBuffer(void);

        /**
         * \brief Load an immediate peri-polar region's worth of
         *        indices into a server-side buffer.
         *
         * \param buff : the buffer object to load.
         *
         * \param po : which pole is adjacent ie. north or south.
         */
        void loadPolarIndexBuffer(Buffer_OBJ& buff, enum pole po);

        /**
         * \brief Load the ( typically many latitudes worth of ) non-peri-polar
         *        regions indices into a server-side buffer.
         */
        void loadWaistIndexBuffer(void);

        /**
         * \brief Load a single vertex's data into a server-side buffer.
         *
         *      This function exists so that no assumptions need be made about
         * member ordering/padding within the Vertex class, when transferring
         * data to the server buffer which does have specific ordering, padding
         * and alignment requirements.
         *
         * \param vert : the Vertex to transfer data from.
         *
         * \param buffer : the buffer location to transfer data to.
         */
        void vertex2buffer(const Vertex& vert, Vert* buffer) const;
    };

/**
* @}
*/

#endif // GLOBE_H_
