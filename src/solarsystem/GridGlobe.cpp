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

#include "GridGlobe.h"

const GLfloat GridGlobe::LINE_WIDTH(0.5f);
const GLfloat GridGlobe::RGB_RED(0.12f);
const GLfloat GridGlobe::RGB_GREEN(0.17f);
const GLfloat GridGlobe::RGB_BLUE(0.12f);

const GLuint GridGlobe::ARRAY_START(0);
const GLsizei GridGlobe::ARRAY_STRIDE(0);
const GLuint GridGlobe::BYTE_OFFSET(0);
const GLuint GridGlobe::COORDS_PER_VERTEX(3);
const GLuint GridGlobe::VERTICES_PER_LINK(2);

GridGlobe::GridGlobe(vec_t rad, GLuint slices, GLuint stacks) :
                     sp(rad, slices, stacks, Sphere::NO_STAGGER),
                     slices(slices),
                     stacks(stacks) {
   }

GridGlobe::~GridGlobe() {
   }

void GridGlobe::prepare(SolarSystemGlobals::render_quality rq) {
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST : {
         loadVertexBuffer();
         loadIndexBuffer();
         break;
         }
      default :
         // Ought not get here !!
         std::string msg = "GridGlobe::prepare() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void GridGlobe::release(void) {
   // Release the buffer object's resources.
   buff_obj_points.release();
   }

void GridGlobe::render(void) {
   // Set the line width and colour for rendering the grid.
   glLineWidth(LINE_WIDTH);
   glColor3f(RGB_RED, RGB_GREEN, RGB_BLUE);

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_links.ID());

   // We will use a vertex array within that buffer.
   glEnableClientState(GL_VERTEX_ARRAY);

   // The vertex array pointer points to the start of the buffer.
   glVertexPointer(COORDS_PER_VERTEX, GL_FLOAT, ARRAY_STRIDE, BUFFER_OFFSET(BYTE_OFFSET));

   // Finally we get to render the lines.
   glDrawElements(GL_LINES, total_links * VERTICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(ARRAY_START));

   // Stop using vertex arrays.
   glDisableClientState(GL_VERTEX_ARRAY);
   }

void GridGlobe::loadVertexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // What size allocation are we after?
   GLsizeiptr size = sizeof(vec_t) * COORDS_PER_VERTEX * sp.vertices().size();

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("GridGlobe::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   GLuint index_current = 0;
   /// Store the vertex position data in the buffer.
   for(std::vector<Vertex>::const_iterator vt = sp.vertices().begin();
      vt < sp.vertices().end();
      vt++ ) {
      *(buffer_ptr) = vt->position().x();
      buffer_ptr++;
      *(buffer_ptr) = vt->position().y();
      buffer_ptr++;
      *(buffer_ptr) = vt->position().z();
      buffer_ptr++;

      index_current++;
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void GridGlobe::loadIndexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_links.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_links.ID());

   total_links = slices*(2*stacks - 3);

   // What size allocation are we after?
   GLsizeiptr size = sizeof(unsigned int) * total_links * VERTICES_PER_LINK;

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get an access pointer to the buffer area, of correct type,
   // for the purpose of writing.
   unsigned int* buffer_ptr = static_cast<unsigned int*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("GridGlobe::loadIndexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   /// Do rings of latitude/declination first.
   for(unsigned int stack = 1; stack < stacks - 1; stack++) {
      /// Start counting at the second vertex, as the first is a pole.
      unsigned int base_vertex_index = 1 + (stack - 1)*slices;

      for(unsigned int slice = 0; slice < slices; slice++) {
         *(buffer_ptr) = base_vertex_index + slice;
         buffer_ptr++;

         *(buffer_ptr) = base_vertex_index + ((slice + 1) % slices);
         buffer_ptr++;
         }
      }

   /// Now do half-rings of longitude/right-ascension.
   for(unsigned int slice = 0; slice < slices; slice++) {
      /// Index of north pole vertex.
      *(buffer_ptr) = 0;
      buffer_ptr++;

      /// Index of point one latitude below north.
      *(buffer_ptr) = 1 + slice;
      buffer_ptr++;

      for(unsigned int stack = 1; stack <= stacks - 3; stack++) {
         *(buffer_ptr) = 1 + slice + (stack - 1) * slices;
         buffer_ptr++;
         *(buffer_ptr) = 1 + slice + stack * slices;
         buffer_ptr++;
         }

      /// Index of point one latitude above south.
      *(buffer_ptr) = 1 + (stacks - 3) * slices + slice;
      buffer_ptr++;

      /// Index of south pole vertex.
      *(buffer_ptr) = slices * (stacks - 2) + 1;
      buffer_ptr++;
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }
