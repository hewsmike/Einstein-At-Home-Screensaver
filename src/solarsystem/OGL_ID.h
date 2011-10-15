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

#ifndef OGL_ID_H_
#define OGL_ID_H_

#include "SDL_opengl.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        identifiers.
 *
 * Display lists, buffer objects and texture objects have a common
 * functionality for which this class is a wrapper of. The detailed
 * acquisition and release of OpenGL resources is to be provided in
 * subclasses.
 *
 * \author Mike Hewson\n
 */

class OGL_ID {
   public:
      /// Default initialiser for the identifier.
      static const GLuint NO_ID;

   private:
      void cleanup(void);

   protected:
      // The identifier as allocated by OpenGL.
      GLuint ident;

   public:
      /**
       * \brief Constructor
       */
      OGL_ID();

      /**
       * \brief Destructor
       */
      ~OGL_ID();

      /**
       * \brief Obtains the OpenGL resources.
       */
      virtual void acquire(void) = 0;

      /**
       * \brief Releases the OpenGL resources.
       */
      virtual void release(void) = 0;

      /**
       * \brief Get the OpenGL resource identifier.
       */
      GLuint ID(void) const;
   };

/**
 * @}
 */

#endif /*OGL_ID_H_*/
