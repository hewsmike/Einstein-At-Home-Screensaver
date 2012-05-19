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

#include "Globe.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "ErrorHandler.h"

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
             std::string image_resource_name,
             GLfloat radius,
             GLuint stacks,
             GLuint slices,
             GLfloat zero_longitude_offset) :
                 nm(name),
                 irn(image_resource_name),
                 zlo(zero_longitude_offset),
                 sp(radius, slices, stacks, STAGGERING, STITCHING),
                 verts_per_lat(slices + 1) {
   }

Globe::~Globe() {
   release();
   }

void Globe::loadTexture(void) {
   // Get an OpenGL texture object.
   texture.acquire();
   
   // Make our texture object OpenGL's current one.
   glBindTexture(GL_TEXTURE_2D, texture.ID());
   
   ResourceFactory factory;
   // Create texture resource instance.
   const Resource* textureResource = factory.createInstance(irn.c_str());

   // We're gonna let the GLFW do the hard work of mipmap production.
   // This implicitly operates on the GL_TEXTURE_2D target.
   int load_success = GL_FALSE;
   if(textureResource != GL_FALSE) {
		load_success = glfwLoadMemoryTexture2D(&(textureResource->data()->front()),
															 textureResource->data()->size(),
															 GLFW_BUILD_MIPMAPS_BIT);
		}
	else {
      ErrorHandler::record("Globe::loadTexture() - texture resource not available", ErrorHandler::WARN);
		}

   // Did that work?
   if(load_success == GL_TRUE) {
      // Set the texture's stretching properties
		// for minification and magnification.
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
		}
	else {
      // Nope, the loading into a texture object failed. This is not fatal, as
      // with later rendering OpenGL will simply use the 'default' texture ie.
      // nothing. The only visual result will be to see whatever background
      // color(s) have been assigned ( or not ! ) to the polygon(s) in question.
      ErrorHandler::record("Globe::loadTexture() - texture object not loaded ", ErrorHandler::WARN);
      }
      
   // Unbind the texture from the state machine - but don't delete it!
   glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);
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
   // Free up the various buffer and texture objects on the server side.
   north_cap_indices.release();
   waist_indices.release();
   south_cap_indices.release();
   vertex_buffer.release();
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
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.ID());

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
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_indices.ID());

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
   vertex_buffer.acquire();

   // What size allocation are we after? There are two pole vertices, and
   // a number of vertices at each stack of latitude.
   GLsizeiptr buffer_size = sizeof(Vert) * sp.vertices().size();

   Vert* buffer_base_ptr = new Vert[sp.vertices().size()];
   Vert* buffer_ptr = buffer_base_ptr;

   for(std::vector<Vertex>::const_iterator vt = sp.vertices().begin();
       vt != sp.vertices().end();
       ++vt) {
      // Transfer the vertex data to the buffer.
      vertex2buffer(*vt, buffer_ptr);
      // Update the buffer pointer.
      ++buffer_ptr;
      }

   vertex_buffer.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);
   delete[] buffer_base_ptr;
   }

void Globe::loadWaistIndexBuffer(void) {
   // Get a valid buffer object ( server-side ) identifier.
   waist_indices.acquire();

   // What size allocation are we after?
   GLsizeiptr waist_size = sizeof(GLuint) *           // Size of base data type
                           verts_per_lat *            // Number of vertices per stack
                           (sp.stacks() - 3) *        // Number of non-polar stacks less one
                           2;                         // Two stacks involved per GL_TRIANGLE_STRIP.

   GLuint* buffer_base_ptr = new GLuint[verts_per_lat * (sp.stacks() - 3) * 2];
   GLuint* buffer_ptr = buffer_base_ptr;

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

   waist_indices.loadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, waist_size, buffer_base_ptr);
   delete[] buffer_base_ptr;
   }

void Globe::loadPolarIndexBuffer(Buffer_OBJ& polar_buffer, enum pole p) {
   // Get a valid buffer object ( server-side ) identifier.
   polar_buffer.acquire();

   // What size byte allocation are we after for this array of indices? For
   // each point we have sizeof(GLuint) worth. What is the point count ?
   // - one for the pole (+1) , plus
   // - one for each vertex within the stack just adjacent the pole.
   GLsizeiptr polar_size = sizeof(GLuint) * (1 + verts_per_lat);

   GLuint* buffer_base_ptr = new GLuint[1 + verts_per_lat];
   GLuint* buffer_ptr = buffer_base_ptr;

   // Index of point on sphere which begins a sequence of
   // points for later use within a GL_TRIANGLE_FAN pattern.
   // Default starting index is zero for the north pole,.
   GLuint pole_index = 0;
   GLuint peri_polar_index = pole_index + 1;
   GLuint delta = 1;
   if(p == SOUTH) {
      // The south polar index is however many vertex entries there
      // are for the entire sphere minus one.
      pole_index = sp.vertices().size() - 1;
      peri_polar_index = pole_index - 1;
      delta = -1;
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

   for(GLuint i = 0; i < verts_per_lat - 1; i++) {
      *buffer_ptr = peri_polar_index + delta*i;
      // Move to next buffer position.
      ++buffer_ptr;
      }

   *buffer_ptr = peri_polar_index;

   polar_buffer.loadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, polar_size, buffer_base_ptr);
   delete[] buffer_base_ptr;
   }

void Globe::vertex2buffer(const Vertex& vert, Vert* buffer) const {
   // Texture co-ordinates, noting an adjustment to line
   // the map up to the Greenwich meridian.
   buffer->text.s = vert.texture_co_ords().first - zlo;
   buffer->text.t = vert.texture_co_ords().second;

   // Normal data.
   buffer->norm.x = vert.normal().x();
   buffer->norm.y = vert.normal().y();
   buffer->norm.z = vert.normal().z();

   // Position data.
   buffer->pos.x = vert.position().x();
   buffer->pos.y = vert.position().y();
   buffer->pos.z = vert.position().z();
   }
