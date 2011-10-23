/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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
#include "Earth.h"

const GLuint Earth::EARTH_RADIUS(100);
const GLfloat Earth::EARTH_GRID_RADIUS(Earth::EARTH_RADIUS);
const GLuint Earth::EARTH_GRID_SLICES(72);
const GLuint Earth::EARTH_GRID_STACKS(37);

const char* Earth::TEXTURE_FILE_NAME("worldmap.bmp");

const GLuint Earth::POINT_COORDS_PER_VERTEX(3);
const GLuint Earth::NORM_COORDS_PER_VERTEX(3);
const GLuint Earth::TEXTURE_COORDS_PER_VERTEX(2);
const GLuint Earth::ELEMENTS_PER_VERTEX(POINT_COORDS_PER_VERTEX +
                                        NORM_COORDS_PER_VERTEX +
                                        TEXTURE_COORDS_PER_VERTEX);

// This accounts for the extra 'stitch' vertex
// on non-polar latitudinal bands.
const GLuint Earth::VERTICES_PER_LATITUDE(EARTH_GRID_SLICES + 1);

const GLuint Earth::ARRAY_START(0);
const GLsizei Earth::ARRAY_STRIDE(0);

const GLfloat Earth::GREENWICH_TEXTURE_OFFSET(-0.5);

Earth::Earth() : sp(EARTH_GRID_RADIUS, EARTH_GRID_SLICES, EARTH_GRID_STACKS, Sphere::STAGGERED),
                 surface(NULL),
                 texture_format(0),
                 nOfColors(0) {
   }

Earth::~Earth() {
   release();
   }

void Earth::prepare(SolarSystemGlobals::render_quality rq) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();
   // Preparations may depend upon the requested rendering quality level ?
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST :
         loadVertexBuffer();
         loadIndexBuffers();
         loadTexture();
         break;
      default :
         // Ought not get here !!
         std::string msg = "Earth::prepare() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void Earth::release(void) {
   }

void Earth::render(void) {
   // Enable the texture, normal and vertex arrays.
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);

   // Enable the use of 2D textures.
   glEnable(GL_TEXTURE_2D);

   // Only want to look at and/or render the outside of the Earth.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT, GL_FILL);

   // Make our texture identifier OpenGL's current one.
   glBindTexture(GL_TEXTURE_2D, texture.ID());

   // Make our vertex buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // Draw northern cap
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glLineWidth(0.5f);
   glInterleavedArrays(GL_T2F_N3F_V3F, ARRAY_STRIDE, ARRAY_START);

   // Draw north polar region
   // Make our northern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, north_cap_indices.ID());
   glDrawElements(GL_TRIANGLE_FAN,
                  VERTICES_PER_LATITUDE + 1,
                  GL_UNSIGNED_INT,
                  BUFFER_OFFSET(ARRAY_START));

   // Draw waist
   if(EARTH_GRID_STACKS > 3) {
      // Make the waist index buffer identifier OpenGL's current one.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

      for(GLuint stack = 1; stack <= EARTH_GRID_STACKS - 2; stack++) {
         // Herewith the number of bytes into the vertex buffer
         // to begin this strip for this stack.
         GLuint strip_index_start = ARRAY_START +
                                    ((stack - 1) *
                                    sizeof(GLuint) *
                                    VERTICES_PER_LATITUDE *
                                    2);

         glDrawElements(GL_TRIANGLE_STRIP,
                        VERTICES_PER_LATITUDE*2,
                        GL_UNSIGNED_INT,
                        BUFFER_OFFSET(strip_index_start));
         }
      }

   // Draw south polar region
   // Make our southern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, south_cap_indices.ID());
   glDrawElements(GL_TRIANGLE_FAN,
                  VERTICES_PER_LATITUDE + 1,
                  GL_UNSIGNED_INT,
                  ARRAY_START);

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

void Earth::loadTexture(void) {
   // Get an OpenGL texture object.
   texture.acquire();

   // Ensure the image on file gets loaded into the SDL_Surface object.
   if(loadImage()) {
      // Make our texture object OpenGL's current one.
      glBindTexture(GL_TEXTURE_2D, texture.ID());

      // The target for the following specifying calls is GL_TEXTURE_2D.
      glTexImage2D(GL_TEXTURE_2D,         // it's a 2D texture
                   0,                     // only one resolution level ( not a mip-map )
                   nOfColors,             // the number of RGBA components we will use
                   surface->w,            // width in pixels
                   surface->h,            // height in pixels
                   0,                     // no border
                   texture_format,        // the bitmap format type as discovered
                   GL_UNSIGNED_BYTE,      // how we are packing the pixels
                   surface->pixels );     // the actual image data from an SDL surface

      // Set the texture's stretching properties
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Do we repeat the map? Yes, but only in the 'horizontal' direction,
      // meaning that texture coordinates outside of [0.0f, 1.0f] will
      // imply rendering continuing onto the 'other' side of the pixel map.
      // This will give east/west continuity on the globe, if needed, but
      // we don't need north pole to south pole continuity do we ??
      // Don't you just luv topology ?? :-)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      // How it maps when texels and fragments/pixels areas don't match.
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
      std::cout << "Earth::loadTexture() - couldn't load image from file!! " << std::endl;
      }

   // Delete the SDL_surface once we've loaded the pixels to the texture object.
   void SDL_FreeSurface(SDL_Surface* surface);
   }

void Earth::loadVertexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // What size allocation are we after? There are two pole vertices, and
   // a number of vertices at each stack of latitude. Each vertex has
   // position, normal and texture data ( fortunately those have the same
   // type : float )
   GLsizeiptr size = sizeof(vec_t) *
                     ELEMENTS_PER_VERTEX *
                     (2 + VERTICES_PER_LATITUDE * (EARTH_GRID_STACKS - 2));

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("Earth::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   // Iterator/pointer to the sphere vertex listing.
   std::vector<Vertex>::const_iterator vt = sp.vertices().begin();

   GLuint vertex_index = 0;

   // Store the vertex position data in the buffer. Do the north pole first.

   Vertex north_pole = *vt;

   vertex2buffer(*vt, buffer_ptr);
   buffer_ptr += ELEMENTS_PER_VERTEX;
   vt++;
   vertex_index++;

   // Store the vertex position data in the buffer, for non-polar points.
   for(unsigned int stack = 1; stack <= EARTH_GRID_STACKS - 2; stack++) {
      // Remember the first vertex at a given latitude.
      Vertex first_in_stack = *vt;

      for(unsigned int slice = 0; slice < EARTH_GRID_SLICES; slice++) {
         Vertex this_one = *vt;

         // Transfer the vertex data to the buffer.
         vertex2buffer(this_one, buffer_ptr);
         // Update both buffer pointer and vertex listing iterator.
         buffer_ptr += ELEMENTS_PER_VERTEX;
         vt++;
         vertex_index++;
         }

      // Insert the 'stitch' vertex, by creating a new vertex via
      // alteration of the first for that latitiude. The only vertex
      // attribute to change is the 'horizontal' texture coordinate, by
      // setting it to 1.0f.
      std::pair<GLfloat, GLfloat> text = first_in_stack.texture_co_ords();
      text.first = 1.0f;
      Vertex last_in_stack = Vertex(first_in_stack.position(),
                                    first_in_stack.normal(),
                                    text);

      // Now pop this new extra vertex into the buffer.
      vertex2buffer(last_in_stack, buffer_ptr);
      // Update only the buffer pointer not the vertex listing iterator.
      buffer_ptr += ELEMENTS_PER_VERTEX;
      vertex_index++;
      }

   // Do the south pole last. Being the last point to be processed
   // we don't update any pointers/iterators.

   Vertex south_pole = *vt;

   vertex2buffer(*vt, buffer_ptr);

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

bool Earth::loadImage(void) {
   // If we can access the image file, then we need to check a number of
   // it's parameters to establish whether it is suitable for our use.
   // TODO - write better error paths for this.
   bool returnFlag = true;

   if((surface = SDL_LoadBMP(Earth::TEXTURE_FILE_NAME))) {
      std::cout << "Earth::loadImage() - "
                << "Successfully loaded image '" << Earth::TEXTURE_FILE_NAME
                << "'" << std::endl;
      std::cout << "Width is " << surface->w << " pixels" << std::endl;
      // Check that the image's width is a power of 2
      if((surface->w & (surface->w - 1)) != 0) {
         std::cout << "warning : width is not a power of 2" << std::endl;
         }

      std::cout << "Height is " << surface->w << " pixels" << std::endl;
      // Also check if the height is a power of 2
      if((surface->h & (surface->h - 1)) != 0) {
         std::cout << "warning : height is not a power of 2" << std::endl;
         }

      // get the number of channels in the SDL surface
      nOfColors = surface->format->BytesPerPixel;
      std::cout << "Number of channels is " << nOfColors << std::endl;
      if(nOfColors == 4) {
         // contains an alpha channel
         std::cout << "\twe have an alpha channel and the";
         if(surface->format->Rmask == 0x000000ff) {
            std::cout << "format is RGBA" << std::endl;
            texture_format = GL_RGBA;
            }
         else {
            std::cout << "format is BGRA" << std::endl;
            texture_format = GL_BGRA;
            }
         }
      else if(nOfColors == 3) {
         // no alpha channel
         std::cout << "\twe don't have an alpha channel and the ";
         if(surface->format->Rmask == 0x000000ff) {
            std::cout << "format is RGB" << std::endl;
            texture_format = GL_RGB;
            }
         else {
            std::cout << "format is BGR" << std::endl;
            texture_format = GL_BGR;
            }
      }
      else {
         std::cout << "warning : the image is not truecolor ... it has "
                   << nOfColors << " colors." << std::endl;
         // TODO this error should not go unhandled
         }
      }
   else {
      std::cout << "Earth::loadImage() - SDL could not load "
                << Earth::TEXTURE_FILE_NAME
                << " image!" << std::endl;
      returnFlag = false;
	   }
   return returnFlag;
   }

void Earth::loadIndexBuffers(void) {
   // NB Potential for huge confusion is that there are two indices used with
   // each call to glDrawElements() during rendering. One is that into the
   // server-side buffer denoted by the target GL_ELEMENT_ARRAY_BUFFER,
   // the other being that into the list of points upon the sphere denoted by
   // the target GL_ARRAY_BUFFER. The key point of the method is to have a
   // server-side buffer of indices into another server-side buffer of
   // vertices. At render time OpenGL will look up what's bound to
   // GL_ELEMENT_ARRAY_BUFFER in order to retrieve a value which is hence
   // interpreted as an index used to look up within whatever is bound to
   // GL_ARRAY_BUFFER. We have many index buffers which will be consecutively
   // bound to GL_ELEMENT_ARRAY_BUFFER at render time, one for each polar region
   // producing GL_TRIANGLE_FAN patterns and several for everything in between
   // that each produce a GL_TRIANGLE_STRIP pattern. There will only be one
   // ( vertex containing ) buffer bound to GL_ARRAY_BUFFER at render time.
   // Did you get all that ? :-) :-)

   // Northern cap.
   loadNorthBuffer();

   // Equatorial waist.
   loadWaistBuffer();

   // Southern cap.
   loadSouthBuffer();
   }

void Earth::loadNorthBuffer(void) {
   // Northern cap, get a valid buffer object ( server-side ) identifier.
   north_cap_indices.acquire();

   // Make our buffer OpenGL's current one for indices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, north_cap_indices.ID());

   // What size byte allocation are we after for this array of indices? For
   // each point we have sizeof(GLuint) worth, so what is the point count ?
   //       - one for the North pole (+1) , plus
   //       - one for each longitudinal slice (+EARTH_GRID_SLICES), but
   //       - there is a final point ( 'horizontal' texture coordinate of 1.0f ) at
   //         the same position as first one ( 'horizontal' texture coordinate of 0.0f ),
   //         both within the slice just below the pole.
   GLsizeiptr north_size = sizeof(GLuint) * (2 + EARTH_GRID_SLICES);

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, north_size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   GLuint* north_cap_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Index of point on sphere for North pole, which begins a sequence of
   // points for later use within a GL_TRIANGLE_FAN pattern. BTW what follows
   // assumes that you know of the point ordering within the vertex buffer and
   // thus in turn have knowledge of ordering within a sphere object. A necessary
   // coupling but uncomfortable from a strict OO view.
   GLuint north_pole_index = 0;

   // First entry in buffer is the North polar point's index.
   north_cap_ptr[0] = north_pole_index;

   // The indices of points on sphere at a latitude just one stack below
   // the north pole, listed in sequence suitable for later use within
   // GL_TRIANGLE_FAN pattern. The way this 'winds around' makes the convex side
   // the 'outside' for OpenGL purposes. As it should be. NB OpenGL considers
   // the 'front face' of a polygon to be what I have called 'outside', the
   // sense of the winding is right handed : list in order a single
   // polygon's vertices and they will form a right-handed system with the
   // normal pointing out of the 'front face' of the polygon. Curved fingers
   // of one's right hand follow the vertex order, the thumb points along the
   // normal vector to the mutual plane defined by said vertices. Or if you
   // look down the normal vector from it's tip to the tail embedded upon the
   // plane, the vertices are listed in counter-clockwise order.
   // Got that ? :-) :-) :-)
   GLuint stack_first_point_index = north_pole_index + 1;
   for(GLuint i = 0; i <= EARTH_GRID_SLICES; i++) {
      // Index of buffer position to insert the point's index upon the sphere.
      GLuint buffer_index = i + 1;
      north_cap_ptr[buffer_index] = stack_first_point_index + i;
      }

   // Note that we had to include an index to the repeated vertex at the
   // longitudinal 'stitch' line ( meridian of zero longitude ).

   // Discard the mapping and unbind ( but not release ) the buffer.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Earth::loadWaistBuffer(void) {
   // NB As we are only going to render a GL_TRIANGLE_STRIP at one per
   // latitude band then the staggering of vertices longitudinally
   // from one latitude band value to the next isn't relevant for
   // vertex ordering.
   if(EARTH_GRID_STACKS > 3) {
      waist_buffer.acquire();

      // Make our buffer identifier OpenGL's current one.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

      // What size allocation are we after?
      GLsizeiptr waist_size = sizeof(GLuint) *
                              (VERTICES_PER_LATITUDE * 2) *
                              (EARTH_GRID_STACKS - 3);

      // Allocate buffer memory but don't store, yet.
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, waist_size, NULL, GL_STATIC_DRAW);

      // Get an access pointer to the buffer area, of correct type,
      // for the purpose of writing.
      GLuint* waist_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

      // Index of the first non-polar point ( the north pole pole has index 0 )
      GLuint first_point_index = 1;

      // The indices of points ......
      // listed in sequence suitable for later use within GL_TRIANGLE_STRIP pattern.
      for(GLuint stack = 1; stack <= EARTH_GRID_STACKS - 3; stack++) {
         GLuint stack_first_point_index = first_point_index + (stack - 1) * VERTICES_PER_LATITUDE;

         GLuint stack_first_buffer_index = (stack - 1) * VERTICES_PER_LATITUDE * 2;

         for(GLuint vert = 0; vert < VERTICES_PER_LATITUDE; vert++) {
            waist_ptr[stack_first_buffer_index + vert * 2] = stack_first_point_index + vert;

            waist_ptr[stack_first_buffer_index + vert * 2 + 1] = stack_first_point_index + vert + VERTICES_PER_LATITUDE;
            }
         }
      // Discard the mapping and unbind the buffer.
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
      }
   }

void Earth::loadSouthBuffer(void) {
   // Southern cap, get a valid buffer object ( server-side ) identifier.
   south_cap_indices.acquire();

   // Make our buffer identifier OpenGL's current one for indices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, south_cap_indices.ID());

   // What size byte allocation are we after for this array of indices? For
   // each point we have sizeof(GLuint) worth. What is the point count ?
   //       - one for the South pole (+1) , plus
   //       - one for each longitudinal slice (+EARTH_GRID_SLICES), but
   //       - there is a final point ( 'horizontal' texture coordinate of 1.0f ) at
   //         the same position as first one ( 'horizontal' texture coordinate of 0.0f ),
   //         both within the slice just above the pole.
   GLsizeiptr south_size = sizeof(GLuint) * (2 + EARTH_GRID_SLICES);

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, south_size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   GLuint* south_cap_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Index of point on sphere for South pole, which begins a sequence of
   // points for later use within a GL_TRIANGLE_FAN pattern. Of course the
   // indexing of the vertex array ( bound to GL_ARRAY_BUFFER ) begins, as
   // always with C++, numbering at zero. The final index is hence however
   // many vertex entries ie. (EARTH_GRID_STACKS - 2) * VERTICES_PER_LATITUDE + 2,
   // there are minus one.
   GLuint south_pole_index = (EARTH_GRID_STACKS - 2) * VERTICES_PER_LATITUDE + 1;

   // First entry in buffer is the South polar point's index.
   south_cap_ptr[0] = south_pole_index;

   // The indices of points on sphere at a latitude just one stack above
   // the south pole, listed in sequence suitable for later use within
   // GL_TRIANGLE_FAN pattern. The way this 'winds around' makes the convex side
   // the 'outside' for OpenGL purposes, and is necessarily of the opposite
   // sense to the northern cap. Alas, you have to sit and think on it .... :-)
   GLuint stack_first_point_index = south_pole_index - 1;
   for(GLuint i = 0; i <= EARTH_GRID_SLICES; i++) {
      // Index of buffer position to insert the point's index upon the sphere.
      // While our loop index 'i' counts up, the index into the indices buffer
      // counts down.
      GLuint buffer_index = EARTH_GRID_SLICES - i + 1;
      south_cap_ptr[buffer_index] = stack_first_point_index + (i%EARTH_GRID_SLICES);
      }

   // Note that we had to include an index to the repeated vertex
   // at the longitudinal 'stitch' line ( meridian of zero longitude ).

   // Discard the mapping and unbind ( but not release ) the buffer.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Earth::vertex2buffer(const Vertex& vert, vec_t* buffer) const {
   // Texture co-ordinates, noting an adjustment to line
   // the map up to the Greenwich meridian.
   buffer[0] = vert.texture_co_ords().first + GREENWICH_TEXTURE_OFFSET;
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
