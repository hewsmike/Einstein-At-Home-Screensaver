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

#ifndef TEXTURE_OBJ_H_
#define TEXTURE_OBJ_H_

#include "OGL_ID.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 * texture objects. It's a wrapper.
 *
 * \author Mike Hewson\n
 */

class Texture_OBJ : public OGL_ID {
   public :
      /**
      * \brief Constructor
      */
      Texture_OBJ(void);

      /**
       * \brief Destructor
       */
      virtual ~Texture_OBJ();

      /**
       * \brief Obtains the texture object resources.
       */
      virtual void acquire(void);

      /**
       * \brief Releases the texture object resources.
       */
      virtual void release(void);
   };

/**
 * @}
 */

#endif /*TEXTURE_OBJ_H_*/
