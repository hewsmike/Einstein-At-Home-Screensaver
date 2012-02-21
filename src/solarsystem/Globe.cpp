/***************************************************************************
* Copyright (C) 2011 by Mike Hewson                                        *
* hewsmike@iinet.net.au                                                    *
*                                                                          *
* This file is part of Einstein@Home.                                      *
*                                                                          *
* Einstein@Home is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published        *
* by the Free Software Foundation, version 2 of the License.               *
*                                                                          *
* Einstein@Home is distributed in the hope that it will be useful,         *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.    *
*                                                                          *
***************************************************************************/

#include "Globe.h"

#include <iostream>
#include <sstream>
#include <vector>


// In 3D space there are three position coordinate values per vertex.
const GLuint Globe::POS_COORDS_PER_VERTEX(3);

// In 3D space there are three coordinate values per normal vector, per vertex.
const GLuint Globe::NORM_COORDS_PER_VERTEX(3);

// For 2D texturing there are two coordinate values per vertex.
const GLuint Globe::TEXTURE_COORDS_PER_VERTEX(2);

// Thus each vertex has the sum of the above components to specify.
const GLuint Globe::ELEMENTS_PER_VERTEX(POS_COORDS_PER_VERTEX +
                                        NORM_COORDS_PER_VERTEX +
                                        TEXTURE_COORDS_PER_VERTEX);

// Vertex data begins from the first byte into the array.
const GLuint Globe::ARRAY_START(0);

// No padding between vertex data entries
const GLsizei Globe::ARRAY_STRIDE(0);

// Don't ever change these !!
const bool Globe::STAGGERING(true);
const bool Globe::STITCHING(true);

Globe::Globe(std::string name,
             std::string image_file_name,
             GLfloat radius,
             GLuint stacks,
             GLuint slices,
             GLfloat zero_longitude_offset) :
                 nm(name),
                 ifn(image_file_name),
                 zlo(zero_longitude_offset),
                 sp(radius, slices, stacks, STAGGERING, STITCHING),
                 surface(NULL),
                 verts_per_lat(slices + 1) {
   }

Globe::~Globe() {
   release();
   }

void Globe::loadTexture(void) {
   // Get an OpenGL texture object.
   texture.acquire();

   // Load the image from file to an SDL_Surface object.
   surface = SolarSystemGlobals::loadImage(ifn, &image_format);

   // Did that work?
   if(surface != NULL) {
      // Make our texture object OpenGL's current one.
      glBindTexture(GL_TEXTURE_2D, texture.ID());

      // The target for the following specifying calls is GL_TEXTURE_2D. We're
      // gonna let the Graphics Utility Library do the hard work of mipmap
      // production.
      gluBuild2DMipmaps(GL_TEXTURE_2D,                   // it's a 2D texture
                        surface->format->BytesPerPixel,  // the number of RGBA components we will use
                        surface->w,                      // width in pixels
                        surface->h,                      // height in pixels
                        image_format,                    // the bitmap format type as discovered
                        GL_UNSIGNED_BYTE,                // how we are packing the pixels
                        surface->pixels);                // the actual image data from an SDL surface

      // Set the texture's stretching properties for minification and
      // magnification.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Do we repeat the map? Yes, but only in the 'horizontal' direction,
      // meaning that texture coordinates outside of [0.0f, 1.0f] will
      // imply rendering continuing onto the 'other' side of the pixel map.
      // This will give east/west continuity on the globe, if needed, but
      // we don't need north pole to south pole continuity do we ??
      // Don't you just luv topology ?? :-)
      // 'S' is the 'horizontal' texture coordinate direction.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      // 'T' is the 'vertical' texture coordinate direction.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      // How it maps when texels and fragments/pixels areas don't match
      // when we do minification and magnification.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      // You want to paste the image on, with no show-through of what's beneath.
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

      // Bless the texture as most important.
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0f);

      // Unbind the texture from the state machine - but don't delete it!
      glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);
      }
   else {
      // Nope, the loading into a texture object failed. This is not fatal, as
      // with later rendering OpenGL will simply use the 'default' texture ie.
      // nothing. The only visual result will be to see whatever background
      // color(s) have been assigned ( or not ! ) to the polygon(s) in question.
      ErrorHandler::record("Globe::loadTexture() - texture object NOT loaded ", ErrorHandler::WARN);
      }

   // Delete the SDL_surface once we've loaded the pixels to the texture object.
   SDL_FreeSurface(surface);
   }

void Globe::prepare(SolarSystemGlobals::render_quality rq) {
   // Preparations may depend upon the requested rendering quality level ?
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST :
         // Get the vertex data into the server-side buffer.
         loadVertexBuffer();

         // For each pole and in between define and store the indices
         // ( into the above vertex data array ) in correct sequence
         // for later use when rendering.
         loadPolarIndexBuffer(north_cap_indices, NORTH);
         loadWaistIndexBuffer();
         loadPolarIndexBuffer(south_cap_indices, SOUTH);

         // Finally make sure one has a texture ( pixel map ) available
         // to paste on the surface which approximates a sphere.
         loadTexture();
         break;
      default :
         // Ought not get here !!
         ErrorHandler::record("Globe::prepare() - bad switch case reached (default)", ErrorHandler::FATAL);
         break;
      }
   }

void Globe::release(void) {
   // Normally this is already freed up after an OpenGL texture object has been
   // successfully created from the surface, but it's OK to pass a NULL here.
   SDL_FreeSurface(surface);

   // Free up the various buffer and texture objects on the server side.
   north_cap_indices.release();
   waist_buffer.release();
   south_cap_indices.release();
   buff_obj_points.release();
   texture.release();
   }

void Globe::render(void) {
   // Enable the texture, normal and vertex arrays as being active
   // for this bout of rendering.
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);

   // Enable the use of 2D textures.
   glEnable(GL_TEXTURE_2D);

   // Only want to look at and/or render the outside of the Globe,
   // for the sake of efficiency.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   // glPolygonMode(GL_FRONT, GL_FILL);

   // Make our texture identifier OpenGL's current one.
   glBindTexture(GL_TEXTURE_2D, texture.ID());

   // Make our vertex buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // The untextured polygonal ( triangles ) color will be opaque and white.
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   // If all goes well we won't see the lines at the edge of the
   // polygons, but we have to define something.
   glLineWidth(0.1f);

   // This announces that the pattern of data storage, per vertex, is
   // 2 texture, 3 normal, and 3 position coordinates in that order. They
   // are all of the same floating point data type.
   glInterleavedArrays(GL_T2F_N3F_V3F, ARRAY_STRIDE, ARRAY_START);

   // Draw north polar region
   // Make our northern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, north_cap_indices.ID());
   // This is one actual rendering call that all preparations have been aiming at.
   glDrawElements(GL_TRIANGLE_FAN,
                  verts_per_lat + 1,            // The pole, and one stack's worth of vertices
                  GL_UNSIGNED_INT,
                  BUFFER_OFFSET(ARRAY_START));

   // Make the waist index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

   for(GLuint stack = 1; stack < sp.stacks() - 2; ++stack) {
      // Herewith the number of bytes into the index buffer
      // to begin this strip for this stack.
      GLuint strip_index_start = ARRAY_START +           // Start at the beginning
                                 sizeof(GLuint) *        // size of base type
                                 (stack - 1) *           // number of non-polar stacks so far
                                 verts_per_lat *         // indices per non-polar stack
                                 2;                      // this stack and the next

      glDrawElements(GL_TRIANGLE_STRIP,
                     verts_per_lat * 2,
                     GL_UNSIGNED_INT,
                     BUFFER_OFFSET(strip_index_start));
      }

   // Draw south polar region
   // Make our southern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, south_cap_indices.ID());
   glDrawElements(GL_TRIANGLE_FAN,
                  verts_per_lat + 1,            // The pole, and one stack's worth of vertices
                  GL_UNSIGNED_INT,
                  BUFFER_OFFSET(ARRAY_START));

   // Unbind the buffers and the texture.
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);

   // Disable polygon face culling.
   glDisable(GL_CULL_FACE);

   // Disable the use of 2D textures.
   glDisable(GL_TEXTURE_2D);

   // Disable the texture, normal and vertex arrays.
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }

void Globe::loadVertexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // What size allocation are we after? There are two pole vertices, and
   // a number of vertices at each stack of latitude. Each vertex has
   // position, normal and texture data ( fortunately those have the same
   // type : float = vec_t )
   GLsizeiptr size = sizeof(vec_t) *            // Number of bytes in the base data type
                     ELEMENTS_PER_VERTEX *      // The number of base data types per vertex
                     sp.vertices().size();      // How many vertices we have.

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get write access to the buffer area.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("Globe::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   for(std::vector<Vertex>::const_iterator vt = sp.vertices().begin();
       vt != sp.vertices().end();
       ++vt) {
      // Transfer the vertex data to the buffer.
      vertex2buffer(*vt, buffer_ptr);
      // Update the buffer pointer.
      buffer_ptr += ELEMENTS_PER_VERTEX;
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Globe::loadWaistIndexBuffer(void) {
   // Get a valid buffer object ( server-side ) identifier.
   waist_buffer.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

   // What size allocation are we after?
   GLsizeiptr waist_size = sizeof(GLuint) *           // Size of base data type
                           verts_per_lat *            // Number of vertices per stack
                           (sp.stacks() - 3) *       // Number of non-polar stacks less one
                           2;                         // Two stacks involved per GL_TRIANGLE_STRIP.

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, waist_size, NULL, GL_STATIC_DRAW);

   // Get write access to the buffer area.
   GLuint* buffer_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   // The indices of points are obtained by interrogating the sphere object.
   // Vertices are suitably listed for use within a GL_TRIANGLE_STRIP.
   for(std::vector<std::vector<GLuint> >::const_iterator stack = sp.stackIndices().begin() + 1;
       stack <= sp.stackIndices().end() - 3;
       ++stack) {
      for(GLuint slice = 0; slice < stack->size(); ++slice) {
         // Interleave this vertex with ...
         *buffer_ptr = (*stack)[slice];
         ++buffer_ptr;
         // ... the corresponding vertex on the next stack.
         *buffer_ptr = (*(stack + 1))[slice];
         ++buffer_ptr;
         }
      }

   // Discard the mapping and unbind the buffer.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Globe::loadPolarIndexBuffer(Buffer_OBJ& buff, enum pole p) {
   // Get a valid buffer object ( server-side ) identifier.
   buff.acquire();

   // Make our buffer identifier OpenGL's current one for indices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.ID());

   // What size byte allocation are we after for this array of indices? For
   // each point we have sizeof(GLuint) worth. What is the point count ?
   // - one for the pole (+1) , plus
   // - one for each vertex within the stack just adjacent the pole.
   GLsizeiptr polar_size = sizeof(GLuint) * (1 + verts_per_lat);

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, polar_size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   GLuint* buffer_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Index of point on sphere which begins a sequence of
   // points for later use within a GL_TRIANGLE_FAN pattern.
   // Default starting index is zero for the north pole, and then counting up.
   GLuint pole_index = 0;
   GLint increment = 1;
   if(p == SOUTH) {
      // The south polar index is however many vertex entries there
      // are for the entire sphere minus one. Count down from that.
      pole_index = sp.vertices().size() - 1;
      increment = -1;
      }

   // First entry in buffer is the polar point's index.
   *buffer_ptr = pole_index;
   // Move to next buffer position.
   ++buffer_ptr;

   // The indices of points on sphere at a latitude just one stack nearby
   // the pole, listed in sequence suitable for later use within
   // GL_TRIANGLE_FAN pattern. The way this 'winds around' makes the convex side
   // the 'outside' for OpenGL purposes, and the southern cap is necessarily of
   // the opposite sense to the northern cap.
   // Alas, you have to sit and think on it .... :-)

   for(GLuint i = 0; i < verts_per_lat; i++) {
      // Index of buffer position to insert the point's index upon the sphere.
      // While our loop index 'i' counts up, the index
      // counts up or down depending on which pole.
      *buffer_ptr = pole_index + increment*i;
      // Move to next buffer position.
      ++buffer_ptr;
      }

   // Discard the mapping and unbind ( but not release ) the buffer.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Globe::vertex2buffer(const Vertex& vert, vec_t* buffer) const {
   // Texture co-ordinates, noting an adjustment to line
   // the map up to the Greenwich meridian.
   buffer[0] = vert.texture_co_ords().first - zlo;
   buffer[1] = vert.texture_co_ords().second;

   // Normal data.
   buffer[2] = vert.normal().x();
   buffer[3] = vert.normal().y();
   buffer[4] = vert.normal().z();

   // Position data.
   buffer[5] = vert.position().x();
   buffer[6] = vert.position().y();
   buffer[7] = vert.position().z();
   }
