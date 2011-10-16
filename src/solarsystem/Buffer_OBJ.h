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

#ifndef BUFFER_OBJ_H_
#define BUFFER_OBJ_H_

#include "OGL_ID.h"

/// Not a pretty solution for a purist, but it gets the pointer type right.
/// Used to represent an offset into an OpenGL buffer server side object.
#define BUFFER_OFFSET(bytes)  ((GLubyte*)NULL + (bytes))

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 * buffer objects. It's a wrapper.
 *
 * \author Mike Hewson\n
 */

class Buffer_OBJ : public OGL_ID {
   public :
      /**
      * \brief Constructor
      */
      Buffer_OBJ(void);

      /**
       * \brief Destructor
       */
      virtual ~Buffer_OBJ();

      /**
       * \brief Obtains the buffer object resources.
       */
      virtual void acquire(void);

      /**
       * \brief Releases the buffer object resources.
       */
      virtual void release(void);
   };

/**
 * @}
 */

#endif /*BUFFER_OBJ_H_*/
