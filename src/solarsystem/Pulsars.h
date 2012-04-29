/***************************************************************************
 *   Copyright (C) 2010 by Mike Hewson                                     *
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

#ifndef PULSARS_H_
#define PULSARS_H_

#include <vector>

#include "Buffer_OBJ.h"
#include "Pulsar.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Vector3D.h"

using namespace std;

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class manages the rendering of pulsar data
 *
 * \author Mike Hewson\n
 */

class Pulsars : public Renderable {
   private:
      /// The displayed size and color parameters of generic pulsars
      static const GLfloat MAG_SIZE;
      static const GLfloat RGB_RED;
      static const GLfloat RGB_GREEN;
      static const GLfloat RGB_BLUE;

      static const GLint ARRAY_START;
      static const GLsizei ARRAY_STRIDE;
      static const GLuint BYTE_OFFSET;
      static const GLint COORDS_PER_VERTEX;

      /// Store pulsar data here
      std::vector<Pulsar> pulsar_list;

      /// At what radius from the origin do the pulsars lie?
      vec_t p_rad;

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
       * \param rad The radius from the origin where the pulsars map to.
       */
      Pulsars(vec_t rad);

      /**
       * \brief Destructor, virtual
       */
      virtual ~Pulsars();
	};

/**
 * @}
 */

#endif /*PULSARS_H_*/
