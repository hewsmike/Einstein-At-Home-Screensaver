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

#include "Stars.h"

#include <exception>
#include <iostream>
#include <string>

#include "ErrorHandler.h"
#include "VectorSP.h"

// Ordinary Stars
template<class T> const GLfloat Stars<T>::DEFAULT_MAG_SIZE(1.0f);
template<class T> const GLfloat Stars<T>::DEFAULT_RGB_RED(1.00f);
template<class T> const GLfloat Stars<T>::DEFAULT_RGB_GREEN(1.00f);
template<class T> const GLfloat Stars<T>::DEFAULT_RGB_BLUE(1.00f);

template<class T> const GLint Stars<T>::ARRAY_START(0);
template<class T> const GLsizei Stars<T>::ARRAY_STRIDE(0);
template<class T> const GLuint Stars<T>::BYTE_OFFSET(0);
template<class T> const GLint Stars<T>::COORDS_PER_VERTEX(3);

template<class T> Stars<T>::Stars(vec_t rad = SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS,
				 GLfloat magsize = DEFAULT_MAG_SIZE,
				 GLclampf red = DEFAULT_RGB_RED,
				 GLclampf green = DEFAULT_RGB_GREEN,
				 GLclampf blue = DEFAULT_RGB_BLUE) :
				 	 star_rad(rad),
				 	 mag_size(magsize),
				 	 rgb_red(red),
				 	 rgb_green(green),
				 	 rgb_blue(blue) {
	}

template<class T> Stars<T>::~Stars() {
   }

template<class T> void Stars<T>::add(T star) {
	star_list.push_back(star);
	}

template<class T> vec_t Stars<T>::getRadius(void) const {
	return star_rad;
	}

template<class T> void Stars<T>::prepare(SolarSystemGlobals::render_quality rq) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();
   /// Preparations may depend upon the requested rendering quality level.
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST : {
         // What size allocation are we after? The size of a
         // Star's worth times how many Stars.
         GLsizeiptr buffer_size = sizeof(Position) * star_list.size();

         Position* buffer_base_ptr = new Position[star_list.size()];
         Position* buffer_ptr = buffer_base_ptr;

         // Traverse the listing of Star positions and
         // store such vertices in the buffer.
         for(typename std::vector<T>::iterator st = star_list.begin();
             st < star_list.end();
             ++st) {
         	Star stc = static_cast<Star>(*st);
            // Take each Star in spherical polar co-ordinate representation.
            VectorSP star = VectorSP(stc.right_ascension(), stc.declination(), star_rad);

            // But store in Cartesian co-ordinate representation..
            *(buffer_ptr) = {star.x(), star.y(), star.z()};
            buffer_ptr++;
            }

         buff_obj_points.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);
         delete[] buffer_base_ptr;
         break;
         }
      default :
         // Ought not get here !!
         std::string msg = "Stars::loadbuffer() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

template<class T> void Stars<T>::release(void) {
   // Release the buffer object's resources.
   buff_obj_points.release();
   }

template<class T> void Stars<T>::render(void) {
   // Set the point size and colour for rendering supernovae.
   glPointSize(mag_size);
   glColor3f(rgb_red, rgb_green, rgb_blue);

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // We will use a vertex array within that buffer.
   glEnableClientState(GL_VERTEX_ARRAY);

   // The vertex array pointer indicates the start of the buffer.
   glVertexPointer(COORDS_PER_VERTEX, GL_FLOAT, ARRAY_STRIDE, BUFFER_OFFSET(BYTE_OFFSET));

   // Finally we get to render the points.
   glDrawArrays(GL_POINTS, ARRAY_START, star_list.size());

   // Stop using vertex arrays.
   glDisableClientState(GL_VERTEX_ARRAY);

   // Unbind the texture from the state machine - but don't delete it!
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }
