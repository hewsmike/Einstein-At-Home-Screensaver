/***************************************************************************
* Copyright (C) 2012 by Mike Hewson                                        *
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

#include "HUDImage.h"

#include "ErrorHandler.h"

//#ifdef WIN32_GLEXT_LINKS
#include "OpenGLExts.h"
//#endif

// In 3D space there are three position coordinate values per vertex.
const GLuint HUDImage::POS_COORDS_PER_VERTEX(3);

// For 2D texturing there are two coordinate values per vertex.
const GLuint HUDImage::TEXTURE_COORDS_PER_VERTEX(2);

// Thus each vertex has the sum of the above components to specify.
const GLuint HUDImage::ELEMENTS_PER_VERTEX(POS_COORDS_PER_VERTEX +
                                           TEXTURE_COORDS_PER_VERTEX);

// Vertex data begins from the first byte into the array.
const GLuint HUDImage::ARRAY_START(0);

// No padding between vertex data entries
const GLsizei HUDImage::ARRAY_STRIDE(0);

const GLint HUDImage::NO_MIPMAP(0);
const GLint HUDImage::NO_BORDER(0);

// Call base class constructor to set dimensions.
HUDImage::HUDImage(std::string fileName,
                   GLuint horizontalMargin,
                   GLuint verticalMargin) :
                     HUDContent(horizontalMargin, verticalMargin),
                     image_file_name(fileName),
                     image_width(0),
                     image_height(0) {
   // Ensure one has a texture ( pixel map ) available
   // to paste on the surface.
   loadTexture();

   // Call base class to set minima for the given image content.
   setMinimumDimensions(image_width + 2*horzMargin(),
                        image_height + 2*vertMargin());
   }

HUDImage::~HUDImage() {
   release();
   }

void HUDImage::prepare(SolarSystemGlobals::render_quality rq) {
   // Policy is to set size when rendering is implied.

   // Create vertex data knowing the image's position.
   createVertexData();

   // Load vertex data to a server side buffer.
   loadVertexBuffer();

   // Call base class to reset minima for the given image content.
   setMinimumDimensions(image_width + 2*horzMargin(),
                        image_height + 2*vertMargin());

   // Preparation of image implies any enclosing container
   // ought be made aware of size change.
   HUDContainer* outer = getEnclosingContainer();
   if(outer !=NULL) {
      outer->adjust();
      }
   }

void HUDImage::release(void) {
   // Policy is to set size to zero when rendering is not implied.

   // Discard server side resources.
   texture.release();
   buff_obj_points.release();

   // Call base class to reset minima to no content.
   setMinimumDimensions(0, 0);

   // Release of image implies any enclosing container
   // ought be made aware of size change.
   HUDContainer* outer = getEnclosingContainer();
   if(outer !=NULL) {
      outer->adjust();
      }
   }

void HUDImage::render(void) {
   // Enable the texture and vertex arrays as being active
   // for this bout of rendering.
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);

   // Make our texture identifier OpenGL's current one.
   glBindTexture(GL_TEXTURE_2D, texture.ID());

   // Make our vertex buffer identifier OpenGL's current one.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
#endif

   // The untextured polygonal ( triangles ) color will be opaque and white.
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   // If all goes well we won't see the lines at the edge of the
   // polygons, but we have to define something.
   glLineWidth(0.1f);

   // This announces that the pattern of data storage, per vertex, is
   // 2 texture and 3 position coordinates in that order. They
   // are all of the same floating point data type.
   glInterleavedArrays(GL_T2F_V3F, ARRAY_STRIDE, ARRAY_START);

   glPushMatrix();

   glTranslatef(horzBase(), vertBase(), 0);

   // This is one actual rendering call that all preparations have been aiming at.
   glDrawArrays(GL_QUADS, ARRAY_START, 4);

   glPopMatrix();

   // Unbind the buffers and the texture.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
#endif
   glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);

   // Disable the texture and vertex arrays.
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }

void HUDImage::loadTexture() {
   // Get an OpenGL texture object.
   texture.acquire();

   // Load the image from file to an SDL_Surface object.
   SDL_Surface* surface = SolarSystemGlobals::loadImage(image_file_name, &image_format);

   // Did that work?
   if(surface != NULL) {
      // Remember the image dimensions.
      image_width = surface->w;
      image_height = surface->h;

      // Make our texture object OpenGL's current one.
      glBindTexture(GL_TEXTURE_2D, texture.ID());

      // The target for the following specifying calls is GL_TEXTURE_2D.
      glTexImage2D(GL_TEXTURE_2D,                   // it's a 2D texture
                   NO_MIPMAP,                       // level zero ie. not a mipmap
                   surface->format->BytesPerPixel,  // the number of RGBA components we will use
                   surface->w,                      // width in pixels
                   surface->h,                      // height in pixels
                   NO_BORDER,                       // don't want a border
                   image_format,                    // the bitmap format type as discovered
                   GL_UNSIGNED_BYTE,                // how we are packing the pixels
                   surface->pixels);                // the actual image data from an SDL surface

      // Set the texture's stretching properties for minification and
      // magnification.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Do we repeat the map in either direction? No.
      // 'S' is the 'horizontal' texture coordinate direction.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
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
      ErrorHandler::record("HUDImage::loadTexture() - texture object NOT loaded ", ErrorHandler::WARN);
      }

   // Discard the SDL_surface once we've loaded the pixels to the texture object.
   SDL_FreeSurface(surface);
   }

void HUDImage::createVertexData(void) {
   // We won't use the normal vector component of a Vertex
   // object so allow the default construction value and
   // then ignore. Attending to margin offsets, make a
   // counterclockwise listing of vertices begining at
   // the lower left corner. This seats the subsequent quad
   // rendering with lower left corner at (0, 0, 0), so one
   // ought transform the the desired position prior to render.

   Vertex lower_left = Vertex(Vector3D(horzMargin(), vertMargin(), 0.0f),
                              Vector3D(),
                              std::pair<GLfloat, GLfloat>(0.0f, 0.0f));

   verts.push_back(lower_left);

   Vertex lower_right = Vertex(Vector3D(horzMargin() + image_width, vertMargin(), 0.0f),
                               Vector3D(),
                               std::pair<GLfloat, GLfloat>(1.0f, 0.0f));

   verts.push_back(lower_right);

   Vertex upper_right = Vertex(Vector3D(horzMargin() + image_width, vertMargin() + image_height, 0.0f),
                               Vector3D(),
                               std::pair<GLfloat, GLfloat>(1.0f, 1.0f));

   verts.push_back(upper_right);

   Vertex upper_left = Vertex(Vector3D(horzMargin(), vertMargin() + image_height, 0.0f),
                              Vector3D(),
                              std::pair<GLfloat, GLfloat>(0.0f, 1.0f));

   verts.push_back(upper_left);
   }

void HUDImage::loadVertexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

   // Make our buffer identifier OpenGL's current one.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
#endif

   // What size allocation are we after? There are four vertices,
   // one for each corner of a quadrilateral. Each vertex has
   // position and texture data ( fortunately those have the
   // same type : float = vec_t )
   GLsizeiptr size = sizeof(vec_t) *            // Number of bytes in the base data type
                     ELEMENTS_PER_VERTEX *      // The number of base data types per vertex
                     verts.size();              // How many vertices we have.

   // Allocate buffer memory but don't store, yet.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
#endif

   // Get write access to the buffer area.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
#else
   // Indirection with win32 build.
   vec_t* buffer_ptr = static_cast<vec_t*>(OpenGLExts::ExtGLMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
#endif

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("HUDImage::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   for(std::vector<Vertex>::const_iterator vt = verts.begin();
       vt != verts.end();
       ++vt) {
      // Transfer the vertex data to the buffer.
      vertex2buffer(*vt, buffer_ptr);

      // Update the buffer pointer.
      buffer_ptr += ELEMENTS_PER_VERTEX;
      }

   // Disconnect the mapping and the buffer from OpenGL.
#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glUnmapBuffer(GL_ARRAY_BUFFER);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLUnmapBuffer(GL_ARRAY_BUFFER);
#endif

#ifndef WIN32_GLEXT_LINKS
   // Non win32 build so call directly.
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
#else
   // Indirection with win32 build.
   OpenGLExts::ExtGLBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
#endif
   }

void HUDImage::vertex2buffer(const Vertex& vert, vec_t* buffer) const {
   // Texture co-ordinates.
   buffer[0] = vert.texture_co_ords().first;
   buffer[1] = vert.texture_co_ords().second;

   // Position data.
   buffer[2] = vert.position().x();
   buffer[3] = vert.position().y();
   buffer[4] = vert.position().z();
   }
