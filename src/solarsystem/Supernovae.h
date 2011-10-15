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

#ifndef SUPERNOVAE_H_
#define SUPERNOVAE_H_

#include <vector>

#include "Buffer_OBJ.h"
#include "ErrorHandler.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Supernova.h"
#include "Vector3D.h"
#include "VectorSP.h"

class Supernovae : public Renderable {
   private:
      /// The displayed size and color parameters of generic supernovae
		static const GLfloat MAG_SIZE;
  	   static const GLfloat RGB_RED;
	   static const GLfloat RGB_GREEN;
	   static const GLfloat RGB_BLUE;

	   static const GLint ARRAY_START;
 	   static const GLsizei ARRAY_STRIDE;
 	   static const GLvoid* BYTE_OFFSET;
	   static const GLint COORDS_PER_VERTEX;

	   /// Store Supernova data here
      std::vector<Supernova> supernova_list;

      /// At what radius from the origin do the supernovae lie?
      vec_t s_rad;

      /// The OpenGL buffer object wrapper for vertex data.
      Buffer_OBJ buff_obj_points;

      /// Provide OpenGL code to prepare for rendering.
      void prepare(SolarSystemGlobals::render_quality rq);

      /// Provide OpenGL code to release any resources used.
      void release(void);

      /// Provide OpenGL code to render the object.
      void render(void);

   public:
      /**
       * \brief Constructor
       *
       * \param rad The radius from the origin where the supernovae map to.
       */
	   Supernovae(vec_t rad);

      /**
       * \brief Destructor, virtual
       */
      virtual ~Supernovae();
   };

#endif /* SUPERNOVAE_H_ */
