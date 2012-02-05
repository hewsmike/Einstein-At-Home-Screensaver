/***************************************************************************
* Copyright (C) 2011 by Mike Hewson *
* hewsmike@iinet.net.au *
* *
* This file is part of Einstein@Home. *
* *
* Einstein@Home is free software: you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published *
* by the Free Software Foundation, version 2 of the License. *
* *
* Einstein@Home is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. *
* *
***************************************************************************/
#ifndef GLOBE_H_
#define GLOBE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Buffer_OBJ.h"
#include "Renderable.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Sphere.h"
#include "SolarSystemGlobals.h"
#include "Texture_OBJ.h"

/**
* \addtogroup solarsystem Solarsystem
* @{
*/

/**
* \brief A renderable object of spherical shape with texture, to be
* instantiated in detail for the Earth, the Sun etc
*
* \author Mike Hewson\n
*/

class Globe : public Renderable {
   public:
      /**
       * \brief Constructor
       *
       * \param name : the English label of the globe ( Earth, Sun .... )
       * \param image_file_name : the OS path identifying the image file for
       *        use as a texture
       * \param radius : the radius of the globe
       * \param stacks : the number of latitudinal values to use in
       *        approximating a spherical surface
       * \param slices : the number of longitudinal values to use in
       *        approximating a spherical surface
       * \param zero_longitude_offset : displacement, expressed as an OpenGL
       *        horizontal texture coordinate value, into the pixel map where
       *        the zero of 'geographical' longitude is deemed to lie
       *        ( eg. Greenwich meridian on Earth )
       */
      Globe(std::string name,
            std::string image_file_name,
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

      static const bool STAGGERING;
      static const bool STITCHING;

      /// The globe's name eg. Earth
      std::string nm;

      /// The filename of the image to use as a texture upon the globe.
      std::string ifn;

      /// The horizontal texture coordinate offset - into the pixel map -
      /// that indicates the zero of 'geographical' longitude.
      GLfloat zlo;

      /// The buffer object for the northern polar cap.
      Buffer_OBJ north_cap_indices;

      /// The buffer object for waist rendering.
      Buffer_OBJ waist_buffer;

      /// The buffer object for the southern polar cap.
      Buffer_OBJ south_cap_indices;

      /// The geometric model approximating a sphere.
      Sphere sp;

      /// The SDL surface to temporarily hold a pixel map, ultimately
      /// for texture use.
      SDL_Surface* surface;

      /// The OpenGL image format type enumerator.
      GLenum image_format;

      /// The number of color channels in the pixel map
      GLuint num_colors;

      /// An OpenGL buffer object ( in server-side memory ) holding
      /// the vertex associated data for an entire sphere approximation.
      Buffer_OBJ buff_obj_points;

      /// An OpenGL texture object identifier
      Texture_OBJ texture;

      /// The number of vertices per latitude/stack
      GLuint verts_per_lat;

      /// Load the image file into an SDL surface.
      void loadImage(void);

      /// Load an SDL surface pixel map into a server-side texture buffer,
      /// with mipmap generation.
      void loadTexture(void);

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

      /// Load all of the vertex data into a server-side buffer
      void loadVertexBuffer(void);

      /// Load an immediate peri-polar region's worth of indices into a
      /// server-side buffer.
      void loadPolarIndexBuffer(Buffer_OBJ& buff, enum pole p);

      /// Load the ( many latitudes worth of ) non-peri-polar regions indices
      /// into a server-side buffer.
      void loadWaistIndexBuffer(void);

      /// Load a single vertex's data into a server-side buffer
      void vertex2buffer(const Vertex& vert, vec_t* buffer) const;
   };

/**
* @}
*/

#endif // GLOBE_H_
