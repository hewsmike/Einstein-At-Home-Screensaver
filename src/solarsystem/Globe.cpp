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
#include "Globe.h"

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

// Four color channels with the alpha, three without.
const GLuint Globe::ALPHA_CHANNEL(4);
const GLuint Globe::NO_ALPHA_CHANNEL(3);

Globe::Globe(std::string name,
             std::string image_file_name,
             GLfloat radius,
             GLuint stacks,
             GLuint slices,
             GLfloat zero_longitude_offset) :
                 nm(name),
                 ifn(image_file_name),
                 zlo(zero_longitude_offset),
                 sp(radius, slices, stacks, Sphere::STAGGERED),
                 surface(NULL),
                 image_format(0),
                 num_colors(0) {
   // Because of the extra 'stitch' vertex to get full wrap of the
   // texture around the globe, this is the number of vertices per
   // latitudinal band.
   verts_per_lat = sp.slices() + 1;
   }

Globe::~Globe() {
   release();
   }

void Globe::loadImage(void) {
   // If we can access the image file, then we need to also check a number of
   // it's parameters.
   // TODO - write better error paths for this ??
   ostringstream os;

   // See if SDL will load the image file into a surface struct ....
   surface = SDL_LoadBMP(ifn.c_str());

   // Did that work?
   if(surface != NULL) {
      // Ok, managed to load something. Record in the log file.
      os.str("");
      os << "Globe::loadImage() - successfully loaded "
         << ifn;
      ErrorHandler::record(os.str(), ErrorHandler::INFORM);

      // Record the image width and height - mainly for debug purposes
      os.str("");
      os << "Globe::loadImage() - width is "
         << surface->w
         << " pixels and height is "
         << surface->h
         << " pixels";
      ErrorHandler::record(os.str(), ErrorHandler::INFORM);

      // Get the number of color channels
      num_colors = surface->format->BytesPerPixel;

      // Record the number of color channels - again mainly for debugging
      os.str("");
      os << "Globe::loadImage() - number of color channels is "
         << num_colors;
      ErrorHandler::record(os.str(), ErrorHandler::INFORM);

      // Get the pixel storage format, record to log and store for later use.
      os.str("");
      os << "Globe::loadImage() - the pixel storage format is ";
      switch (num_colors) {
         case ALPHA_CHANNEL:
            if(surface->format->Rmask == 0x000000ff) {
               os << "RGBA";
               image_format = GL_RGBA;
               }
            else {
               os << "BGRA";
               image_format = GL_BGRA;
               }
            break;
         case NO_ALPHA_CHANNEL:
            if(surface->format->Rmask == 0x000000ff) {
               os << "RGB";
               image_format = GL_RGB;
               }
            else {
               os << "BGR";
               image_format = GL_BGR;
               }
            break;
         default:
            os << "not recognised";
            break;
         }
      ErrorHandler::record(os.str(), ErrorHandler::INFORM);
      }
   else {
      // No, the image did not load into a surface struct.
      ErrorHandler::record("Globe::loadImage() - image file NOT loaded ", ErrorHandler::WARN);
      }
   }

void Globe::loadTexture(void) {
   // Get an OpenGL texture object.
   texture.acquire();

   // Load the image from file to an SDL_Surface object.
   loadImage();

   // Did that work?
   if(surface != NULL) {
      // Yep, so make our texture object OpenGL's current one.
      glBindTexture(GL_TEXTURE_2D, texture.ID());

      // The target for the following specifying calls is GL_TEXTURE_2D. We're
      // gonna let the Graphics Utility Library do the hard work of mipmap
      // production.
      gluBuild2DMipmaps(GL_TEXTURE_2D,          // it's a 2D texture
                        num_colors,             // the number of RGBA components we will use
                        surface->w,             // width in pixels
                        surface->h,             // height in pixels
                        image_format,           // the bitmap format type as discovered
                        GL_UNSIGNED_BYTE,       // how we are packing the pixels
                        surface->pixels);       // the actual image data from an SDL surface

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
      ErrorHandler::record("Globe::Texture() - texture object NOT loaded ", ErrorHandler::WARN);
      }

   // Delete the SDL_surface once we've loaded the pixels to the texture object.
   SDL_FreeSurface(surface);
   }

void Globe::prepare(SolarSystemGlobals::render_quality rq) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

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
         loadPolarBuffer(north_cap_indices, NORTH);
         loadWaistBuffer();
         loadPolarBuffer(south_cap_indices, SOUTH);

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
   glPolygonMode(GL_FRONT, GL_FILL);

   // Make our texture identifier OpenGL's current one.
   glBindTexture(GL_TEXTURE_2D, texture.ID());

   // Make our vertex buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // The untextured polygonal ( triangles ) color will be opaque and white.
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   // If all goes well you won't see the lines at the edge of the
   // polygons, but you have to define something.
   glLineWidth(0.5f);

   // This announces that the pattern of data storage, per vertex, is
   // 2 texture, 3 normal, and 3 position coordinates in that order. They
   // are all of the same floating point data type.
   glInterleavedArrays(GL_T2F_N3F_V3F, ARRAY_STRIDE, ARRAY_START);

   // Draw north polar region
   // Make our northern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, north_cap_indices.ID());
   // This is one actual rendering call that all preparations have been aiming at.
   glDrawElements(GL_TRIANGLE_FAN,
                  verts_per_lat + 1,
                  GL_UNSIGNED_INT,
                  BUFFER_OFFSET(ARRAY_START));

   // Draw waist - if there is one to draw that is.
   if(sp.stacks() > Sphere::MIN_STACKS) {
      // Make the waist index buffer identifier OpenGL's current one.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

      for(GLuint stack = 1; stack <= sp.stacks() - 2; stack++) {
         // Herewith the number of bytes into the vertex buffer
         // to begin this strip for this stack.
         GLuint strip_index_start = ARRAY_START +
                                    ((stack - 1) *
                                    sizeof(GLuint) *
                                    verts_per_lat *
                                    2);

         glDrawElements(GL_TRIANGLE_STRIP,
                        verts_per_lat*2,
                        GL_UNSIGNED_INT,
                        BUFFER_OFFSET(strip_index_start));
         }
      }

   // Draw south polar region
   // Make our southern index buffer identifier OpenGL's current one.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, south_cap_indices.ID());
   glDrawElements(GL_TRIANGLE_FAN,
                  verts_per_lat + 1,
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
   GLsizeiptr size = sizeof(vec_t) *
                     ELEMENTS_PER_VERTEX *
                     (2 + verts_per_lat * (sp.stacks() - 2));

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("Globe::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
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
   for(unsigned int stack = 1; stack <= sp.stacks() - 2; stack++) {
      // Remember the first vertex at a given latitude.
      Vertex first_in_stack = *vt;

      for(unsigned int slice = 0; slice < sp.slices(); slice++) {
         // Transfer the vertex data to the buffer.
         vertex2buffer(*vt, buffer_ptr);
         // Update both buffer pointer and vertex listing iterator.
         buffer_ptr += ELEMENTS_PER_VERTEX;
         vt++;
         vertex_index++;
         }

      // Insert the 'stitch' vertex, by creating a new vertex via simple
      // alteration of the first for that latitiude. The only vertex
      // attribute to change is the 'horizontal' texture coordinate, by
      // setting it from 0.0f to 1.0f
      std::pair<GLfloat, GLfloat> text = first_in_stack.texture_co_ords();
      text.first = 1.0f;
      Vertex last_in_stack = Vertex(first_in_stack.position(),
                                    first_in_stack.normal(),
                                    text);

      // Now pop the data for this new extra vertex into the buffer.
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

void Globe::loadWaistBuffer(void) {
   // NB As we are only going to render a GL_TRIANGLE_STRIP at one per
   // latitude band then the staggering of vertices longitudinally
   // from one latitude band value to the next isn't relevant for
   // vertex ordering.
   if(sp.stacks() > Sphere::MIN_STACKS) {
      // Get a valid buffer object ( server-side ) identifier.
      waist_buffer.acquire();

      // Make our buffer identifier OpenGL's current one.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_buffer.ID());

      // What size allocation are we after?
      GLsizeiptr waist_size = sizeof(GLuint) *
                              (verts_per_lat * 2) *
                              (sp.stacks() - 3);

      // Allocate buffer memory but don't store, yet.
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, waist_size, NULL, GL_STATIC_DRAW);

      // Get an access pointer to the buffer area, of correct type,
      // for the purpose of writing.
      GLuint* waist_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

      // Index of the first non-polar point ( the north pole pole has index 0 )
      GLuint first_point_index = 1;

      // The indices of points ...... stack by stack ....
      // listed in sequence suitable for later use within GL_TRIANGLE_STRIP pattern.
      for(GLuint stack = 1; stack <= sp.stacks() - 3; stack++) {
         GLuint stack_first_point_index = first_point_index + (stack - 1) * verts_per_lat;

         GLuint stack_first_buffer_index = (stack - 1) * verts_per_lat * 2;

         for(GLuint vert = 0; vert < verts_per_lat; vert++) {
            waist_ptr[stack_first_buffer_index + vert * 2] = stack_first_point_index + vert;

            waist_ptr[stack_first_buffer_index + vert * 2 + 1] = stack_first_point_index + vert + verts_per_lat;
            }
         }

      // Discard the mapping and unbind the buffer.
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
      }
   }

void Globe::loadPolarBuffer(Buffer_OBJ& buff, enum pole p) {
   // Get a valid buffer object ( server-side ) identifier.
   buff.acquire();

   // Make our buffer identifier OpenGL's current one for indices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.ID());

   // What size byte allocation are we after for this array of indices? For
   // each point we have sizeof(GLuint) worth. What is the point count ?
   //       - one for the pole (+1) , plus
   //       - one for each longitudinal slice [ sp.slices() ], but
   //       - there is a final point ( 'horizontal' texture coordinate of 1.0f ) at
   //         the same position as first one ( 'horizontal' texture coordinate of 0.0f ),
   //         both within the stack just adjacent the pole.
   GLsizeiptr polar_size = sizeof(GLuint) * (2 + sp.slices());

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, polar_size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   GLuint* polar_ptr = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Index of point on sphere which begins a sequence of
   // points for later use within a GL_TRIANGLE_FAN pattern.
   // Default starting index is zero for the north pole, and then counting up.
   GLuint pole_index = 0;
   GLint increment = 1;
   if(p == SOUTH) {
      // The south polar index is however many vertex entries there
      // are for the entire globe = (sp.stacks() - 2) * verts_per_lat + 2,
      // minus one. One counts down from that.
      pole_index = (sp.stacks() - 2) * verts_per_lat + 1;
      increment = -1;
      }

   // First entry in buffer is the polar point's index.
   polar_ptr[0] = pole_index;

   // The indices of points on sphere at a latitude just one stack nearby
   // the pole, listed in sequence suitable for later use within
   // GL_TRIANGLE_FAN pattern. The way this 'winds around' makes the convex side
   // the 'outside' for OpenGL purposes, and the southern cap is necessarily of
   // the opposite sense to the northern cap.
   // Alas, you have to sit and think on it .... :-)

   for(GLuint i = 0; i <= verts_per_lat; i++) {
      // Index of buffer position to insert the point's index upon the sphere.
      // While our loop index 'i' counts up, the index into the indices buffer
      // counts up or down depending on which pole.
      polar_ptr[i] = pole_index + increment*i;
      }

   // Note that we had to include an index to the repeated vertex
   // at the longitudinal 'stitch' line ( meridian of zero longitude ).

   // Discard the mapping and unbind ( but not release ) the buffer.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Globe::vertex2buffer(const Vertex& vert, vec_t* buffer) const {
   // Texture co-ordinates, noting an adjustment to line
   // the map up to the Greenwich meridian.
   buffer[0] = vert.texture_co_ords().first + zlo;
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
