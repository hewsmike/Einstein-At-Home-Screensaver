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

const GLfloat GridGlobe::GRID_LINE_WIDTH(0.5f);
const GLfloat GridGlobe::GRID_RED(0.12f);
const GLfloat GridGlobe::GRID_GREEN(0.17f);
const GLfloat GridGlobe::GRID_BLUE(0.12f);

const GLfloat GridGlobe::PRIME_MERIDIAN_LINE_WIDTH(1.0f);
const GLfloat GridGlobe::PRIME_MERIDIAN_RED(0.36f);
const GLfloat GridGlobe::PRIME_MERIDIAN_GREEN(0.51f);
const GLfloat GridGlobe::PRIME_MERIDIAN_BLUE(0.36f);

const GLfloat GridGlobe::CELESTIAL_EQUATOR_LINE_WIDTH(1.0f);
const GLfloat GridGlobe::CELESTIAL_EQUATOR_RED(0.24f);
const GLfloat GridGlobe::CELESTIAL_EQUATOR_GREEN(0.34f);
const GLfloat GridGlobe::CELESTIAL_EQUATOR_BLUE(0.24f);

const GLuint GridGlobe::ARRAY_START(0);
const GLsizei GridGlobe::ARRAY_STRIDE(0);
const GLuint GridGlobe::BYTE_OFFSET(0);
const GLuint GridGlobe::COORDS_PER_VERTEX(3);
const GLuint GridGlobe::VERTICES_PER_LINK(2);

// Don't ever change these !!
const bool GridGlobe::STAGGERING(false);
const bool GridGlobe::STITCHING(false);

const GLfloat GridGlobe::TEXT_RATIO(1000);
const GLfloat GridGlobe::TEXT_OTHER_RATIO(0.5f);
const GLfloat GridGlobe::TEXT_OFFSET(2);
const GLfloat GridGlobe::TEXT_UNITS_RATIO(0.5f);

// Don't alter this initial state !!
const GridGlobe::state GridGlobe::INITIAL_CYCLE_STATE(ALL_ON);

GridGlobe::GridGlobe(vec_t rad, GLuint slices,
                     GLuint stacks) :
                     radius(rad),
                     slices(slices),
                     stacks(stacks) {
   // We only have an equatorial slice if an odd number of stacks.
   equator = ((stacks % 2) == 1) ? true : false;

   // Set initial display cycle state.
   current_cycle_state = INITIAL_CYCLE_STATE;
   }

GridGlobe::~GridGlobe() {
   }

void GridGlobe::cycleActivation(void) {
   // Mimic three state cycling switch.
   switch (current_cycle_state) {
      case ALL_OFF :
         current_cycle_state = GRID;
         activate();
         break;
      case GRID :
         current_cycle_state = ALL_ON;
         break;
      case ALL_ON :
         current_cycle_state = ALL_OFF;
         inactivate();
         break;
      default:
         ErrorHandler::record("GridGlobe::cycleActivation() - bad switch case reached (default)", ErrorHandler::FATAL);
         break;
      }
   }

void GridGlobe::prepare(SolarSystemGlobals::render_quality rq) {
   // Make a sphere on the heap.
   sp = new Sphere(radius, slices, stacks, STAGGERING, STITCHING);
   if(sp == NULL) {
      ErrorHandler::record("GridGlobe::GridGlobe() - can't acquire heap for sphere", ErrorHandler::FATAL);
      }

   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST : {
         loadVertexBuffer();
         loadGridIndexBuffer();
         loadPrimeMeridianIndexBuffer();
         // Only prepare an equatorial circle if there is one.
         if(equator) {
            loadCelestialEquatorIndexBuffer();
            }
         createMarkerLists();
         break;
         }
      default :
         // Ought not get here !!
         std::string msg = "GridGlobe::prepare() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }

   // Get rid of the sphere now that we have everything in place, server side.
   delete sp;
   }

void GridGlobe::release(void) {
   // Release the buffer object's resources.
   buff_obj_points.release();

   clearMarkerLists();
   }

void GridGlobe::render(void) {
   // Provided we should be showing anything at all.
   if(current_cycle_state != ALL_OFF) {
      // Make our vertex buffer identifier OpenGL's current one.
      glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
      // We will use a vertex array within that buffer.
      glEnableClientState(GL_VERTEX_ARRAY);
      // The vertex array pointer points to the start of the buffer.
      glVertexPointer(COORDS_PER_VERTEX, GL_FLOAT, ARRAY_STRIDE, BUFFER_OFFSET(BYTE_OFFSET));

      // Do the grid. Start by setting the line width and colour.
      glLineWidth(GRID_LINE_WIDTH);
      glColor3f(GRID_RED, GRID_GREEN, GRID_BLUE);

      // Bind the grid index array.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_grid_links.ID());

      // Finally we get to render the lines.
      glDrawElements(GL_LINES, grid_links * VERTICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(ARRAY_START));

      // Do the prime meridian. Start by setting the line width and colour.
      glLineWidth(PRIME_MERIDIAN_LINE_WIDTH);
      glColor3f(PRIME_MERIDIAN_RED, PRIME_MERIDIAN_GREEN, PRIME_MERIDIAN_BLUE);

      // Bind the prime meridian index array.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_prime_meridian_links.ID());

      // Finally we get to render the lines.
      glDrawElements(GL_LINES, prime_meridian_links * VERTICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(ARRAY_START));

      // Only draw an equatorial circle if there is one.
      if(equator) {
         // Do the celestial equator. Start by setting the line width and colour.
         glLineWidth(CELESTIAL_EQUATOR_LINE_WIDTH);
         glColor3f(CELESTIAL_EQUATOR_RED, CELESTIAL_EQUATOR_GREEN, CELESTIAL_EQUATOR_BLUE);

         // Bind the celestial equator index array.
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_celestial_equator_links.ID());

         // Finally we get to render the lines.
         glDrawElements(GL_LINES, celestial_equator_links * VERTICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(ARRAY_START));
         }

      // Stop using vertex arrays.
      glDisableClientState(GL_VERTEX_ARRAY);

      // Unbind the buffers.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
      glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);

      // Now display the grid markers, if activation state agrees.
      if(current_cycle_state == ALL_ON) {
         glEnable(GL_TEXTURE_2D);
         glDisable(GL_CULL_FACE);

         // Go through marker_lists and call display lists.
         for(std::vector<std::vector<GLuint> >::const_iterator lists = marker_lists.begin();
             lists != marker_lists.end();
             ++lists) {
            // Only call the first, as others are called by it.
            glCallList((*lists)[0]);
            }

         glDisable(GL_TEXTURE_2D);
         }
      }
   }

void GridGlobe::loadVertexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // What size allocation are we after?
   GLsizeiptr size = sizeof(vec_t) * COORDS_PER_VERTEX * sp->vertices().size();

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get write access to the buffer area.
   vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("GridGlobe::loadVertexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   // Store the vertex position data in the buffer.
   for(std::vector<Vertex>::const_iterator vt = sp->vertices().begin();
      vt < sp->vertices().end();
      ++vt) {
      *buffer_ptr = vt->position().x();
      ++buffer_ptr;
      *buffer_ptr = vt->position().y();
      ++buffer_ptr;
      *buffer_ptr = vt->position().z();
      ++buffer_ptr;
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void GridGlobe::loadGridIndexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_grid_links.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_grid_links.ID());

   // The number of line links contained in slices.
   GLuint slice_links = (slices - 1) *    // Number of slices excluding the prime meridian
                        (stacks - 1);     // links per slice

   // The number of line links contained in stacks, accounting for any possible
   // equatorial stacks.
   GLuint stack_links = 2 * (stacks/2 - 1) *    // non-polar and non-equatorial stacks ( integral divide )
                        slices;                 // links per stack

   // Total links for the grid rendering.
   grid_links = slice_links + stack_links;

   // What size allocation are we after?
   GLsizeiptr size = sizeof(GLuint) * grid_links * VERTICES_PER_LINK;

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get write access to the buffer area.
   GLuint* buffer_ptr = static_cast<GLuint*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("GridGlobe::loadGridIndexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   // Do rings of latitude/declination first. Don't need a ring at either pole! Exempt
   // any equatorial slice, if present.
   GLuint stack_count = 0;
   GLuint equator_stack_count = (sp->stacks() - 1)/2 - 1;
   for(std::vector<std::vector<GLuint> >::const_iterator stack = sp->stackIndices().begin() + 1;
       stack != sp->stackIndices().end() - 1;
       ++stack) {
      for(GLuint slice = 0; slice < sp->slices(); ++slice) {
         // If we have an equator and we have reached it ...
         if(equator && (stack_count == equator_stack_count)) {
            // ... then skip it.
            continue;
            }
         // Linking the current vertex to ...
         *buffer_ptr = (*stack)[slice];
         ++buffer_ptr;
         // ... the next, but accounting for wrap around to start of list
         *buffer_ptr = (*stack)[(slice + 1) % sp->slices()];
         ++buffer_ptr;
         }
      ++stack_count;
      }

   // Now do half-rings of longitude/right-ascension. Exempt the prime
   // meridian.
   for(std::vector<std::vector<GLuint> >::const_iterator slice = sp->sliceIndices().begin() + 1;
       slice != sp->sliceIndices().end();
       ++slice) {
      for(GLuint stack = 0; stack < sp->stacks() - 1; ++stack) {
         // Linking the current vertex to ...
         *buffer_ptr = (*slice)[stack];
         ++buffer_ptr;
         // ... the next
         *buffer_ptr = (*slice)[stack + 1];
         ++buffer_ptr;
         }
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void GridGlobe::loadPrimeMeridianIndexBuffer(void) {
   // Get an OpenGL buffer object.
   buff_obj_prime_meridian_links.acquire();

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_prime_meridian_links.ID());

   prime_meridian_links = sp->stacks() - 1;

   // What size allocation are we after?
   GLsizeiptr size = sizeof(GLuint) * prime_meridian_links * VERTICES_PER_LINK;

   // Allocate buffer memory but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

   // Get write access to the buffer area.
   GLuint* buffer_ptr = static_cast<GLuint*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   // Check for failure, as we don't want to dereference a NULL later on,
   // ... MAKE IT A FATAL ERROR.
   if(buffer_ptr == NULL) {
      ErrorHandler::record("GridGlobe::loadPrimeMeridianIndexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
      }

   // Now do the prime meridian.
   std::vector<std::vector<GLuint> >::const_iterator prime_slice = sp->sliceIndices().begin();
   for(std::vector<GLuint>::const_iterator vertex = prime_slice->begin();
       vertex != prime_slice->end() - 1;
       ++vertex) {
      // Linking the current vertex to ...
      *buffer_ptr = *vertex;
      ++buffer_ptr;
      // ... the next
      *buffer_ptr = *(vertex + 1);
      ++buffer_ptr;
      }

   // Disconnect the mapping and the buffer from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void GridGlobe::loadCelestialEquatorIndexBuffer(void) {
   // There'll be no links in the equator unless ....
   celestial_equator_links = 0;

   // There is an equator to draw ?
   if(equator) {
      // Yes, so it will have as many links as there are slices.
      celestial_equator_links = sp->slices();

      // Get an OpenGL buffer object.
      buff_obj_celestial_equator_links.acquire();

      // Make our buffer identifier OpenGL's current one.
      glBindBuffer(GL_ARRAY_BUFFER, buff_obj_celestial_equator_links.ID());

      // What size allocation are we after?
      GLsizeiptr size = sizeof(GLuint) * celestial_equator_links * VERTICES_PER_LINK;

      // Allocate buffer memory but don't store, yet.
      glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

      // Get write access to the buffer area.
      GLuint* buffer_ptr = static_cast<GLuint*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

      // Check for failure, as we don't want to dereference a NULL later on,
      // ... MAKE IT A FATAL ERROR.
      if(buffer_ptr == NULL) {
         ErrorHandler::record("GridGlobe::loadCelestialEquatorIndexBuffer() - can't acquire buffer pointer", ErrorHandler::FATAL);
         }

      // Find the stack index listing for the equator.
      std::vector<std::vector<GLuint> >::const_iterator equator_stack = sp->stackIndices().begin() + (sp->stacks() - 1)/2;
      // Find the start of this listing.
      std::vector<GLuint>::const_iterator vertices = equator_stack->begin();
      // Now go around the circle.
      for(GLuint i = 0; i < celestial_equator_links; ++i) {
         // Linking the current vertex to ...
         *buffer_ptr = *(vertices + i);
         ++buffer_ptr;
         // ... the next, but accounting for wrap around to start of list
         *buffer_ptr = *(vertices + ((i + 1) % celestial_equator_links));
         ++buffer_ptr;
         }

      // Disconnect the mapping and the buffer from OpenGL.
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
      }
   }

void GridGlobe::createMarkerLists(void) {
   clearMarkerLists();

   static const string HOUR_UNITS("h");
   static const string DEGREE_UNITS("o");

   OGLFT_ft* myFont = this->getFont();

   GLfloat text_scale_factor = radius/TEXT_RATIO;

   GLfloat slice_step = SolarSystemGlobals::FULL_CIRCLE_DEG/slices;

   GLfloat stack_step = SolarSystemGlobals::HALF_CIRCLE_DEG/(stacks - 1);

   hour_glyph = myFont->compile(HOUR_UNITS.c_str());
   degree_glyph = myFont->compile(DEGREE_UNITS.c_str());

   for(GLuint slice = 0; slice < slices; ++slice) {
      GLint right_asc = slice * slice_step;

      std::string ra_token = "";
      char ra_tens = '0' + (slice/10);

      // Only show a second digit if it's non-zero.
      if(slice >= 10) {
         ra_token += ra_tens;;
         }

      char ra_ones = '0' + (slice % 10);
      ra_token += ra_ones;

      GLuint ra_draw_ID = myFont->compile(ra_token.c_str());

      for(GLuint stack = 1; stack < stacks - 1; ++stack) {
         std::vector<GLuint> temp;

         GLint declin = SolarSystemGlobals::QUARTER_CIRCLE_DEG - stack * stack_step;

         std::string dec_token = "";

         char dec_tens = '0' + (abs(declin)/10);
         char dec_ones = '0' + (abs(declin) % 10);

         char dec_sign = (declin > 0) ? '+' : '-';

         // Only show a sign for non-zero values.
         if(declin != 0) {
            dec_token += dec_sign;
            }

         // Only show a second digit if it's non-zero.
         if(abs(declin) >= 10) {
            dec_token += dec_tens;
            }

         dec_token += dec_ones;

         GLuint dec_draw_ID = myFont->compile(dec_token.c_str());

         // Text scaling.
         GLfloat t_scale;

         if((equator && (stack == (stacks - 1)/2)) || (slice == 0)) {
            // On the celestial equator (if present) or the prime meridian.
            t_scale = text_scale_factor;
            }
         else {
            // Not upon either the celestial equator or the prime meridian.
            t_scale = text_scale_factor * TEXT_OTHER_RATIO;
            }

         GLuint transform_ID = glGenLists(1);

         glNewList(transform_ID, GL_COMPILE);
            // Isolate the transforms.
            glPushMatrix();
               // Transforms ... in reverse order if "world space" description.
               // Or in forward order if thinking in terms of "local coordinates".

               // Rotate to desired right ascension.
               // Rotate anti-clockwise, seen looking from +ve z-axis to origin.
               glRotatef(right_asc, 0, 0, 1);

               // Rotate to desired declination.
               // Rotate anti-clockwise, seen looking from -ve y-axis to origin.
               glRotatef(declin, 0, -1, 0);

               // Shift out to grid radius nearby the First Point of Aries.
               glTranslatef(radius, 0, 0);

               // Rotate 90 degrees clockwise, seen looking from +ve y-axis to origin.
               glRotatef(-90, 0, 1, 0);

               // Rotate 90 degrees clockwise, seen looking from +ve z-axis to origin.
               glRotatef(-90, 0, 0, 1);

               // When lying on the x-y plane, expand the text.
               glScalef(t_scale, t_scale, 1);

               // Isolate the transform.
               glPushMatrix();
                  // Move up and to the right of the origin.
                  glTranslatef(+TEXT_OFFSET, +TEXT_OFFSET, 0);

                  // Render the current right ascension.
                  glCallList(ra_draw_ID);

                  // Find the dimensions of what we just rendered.
                  OGLFT::BBox ra_box = myFont->measure(ra_token.c_str());

                  // How much to advance towards the right.
                  OGLFT::Advance ra_adv = ra_box.advance_;

                  // Shift across and up to 'superscript' position.
                  glTranslatef(ra_adv.dx_, ra_box.y_max_/2, 0);

                  // Resize for 'hour' symbol.
                  glScalef(TEXT_UNITS_RATIO, TEXT_UNITS_RATIO, 1);

                  // Render the 'hour' symbol.
                  glCallList(hour_glyph);
               glPopMatrix();

               // Find the dimensions of the 'degree' symbol.
               OGLFT::BBox degree_box = myFont->measure(DEGREE_UNITS.c_str());

               // Find the dimensions of what we are about to render.
               OGLFT::BBox dec_box = myFont->measure(dec_token.c_str());

               // How much will it advance towards the right?
               OGLFT::Advance dec_adv = dec_box.advance_;

               // Move down and to the left of the origin, using the above dimensions
               // as a guide, so tha we have the 'degree' symbol suitably abut the
               // intersection of the grid lines.
               glTranslatef(-(TEXT_OFFSET + dec_box.x_max_ + degree_box.x_max_),
                            -(TEXT_OFFSET + dec_box.y_max_ + degree_box.y_max_),
                            0);

               // Render the current declination.
               glCallList(dec_draw_ID);

               // Shift across and up to 'superscript' position.
               glTranslatef(dec_adv.dx_, 7 * dec_box.y_max_/8, 0);

               // Resize for 'degree' symbol.
               glScalef(TEXT_UNITS_RATIO, TEXT_UNITS_RATIO, 1);

               // Render the 'degree' symbol.
               glCallList(degree_glyph);
            glPopMatrix();
         glEndList();

         temp.push_back(transform_ID);
         temp.push_back(ra_draw_ID);
         temp.push_back(dec_draw_ID);

         marker_lists.push_back(temp);
         }
      }
   }

void GridGlobe::clearMarkerLists(void) {
   // Nothing bad will happen if these display list ID's are
   // not current, or invalid, or zero.
   glDeleteLists(hour_glyph, 1);
   glDeleteLists(degree_glyph, 1);

   // Go through the display lists, and ask OpenGL to delete each of them.
   for(std::vector<std::vector<GLuint> >::iterator lists = marker_lists.begin();
       lists != marker_lists.end();
       ++lists) {
      for(std::vector<GLuint>::const_iterator list = lists->begin();
          list != lists->end();
          ++list) {
         glDeleteLists(*list, 1);
         }
      // Empty this list, to be sure.
      lists->clear();
      }

   // This destroys any contained vectors too ...
   marker_lists.clear();
   }
