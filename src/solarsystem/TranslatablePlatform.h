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

#ifndef TRANSLATABLE_PLATFORM_H_
#define TRANSLATABLE_PLATFORM_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Translatable Cartesian orthonormal vector set
 *
 * This class comprises position state data, accessors and mutators thereof.
 *
 * \author Mike Hewson\n
 */

class TranslatablePlatform {
	public:
      /**
       * \brief Constructor
       */
      TranslatablePlatform(void);

      /**
       * \brief Destructor
       */
      virtual ~TranslatablePlatform();

      /**
       * \brief Get the position
       */
      Vector3D position(void) const;

      /**
       * \brief Set the position
       */
      void set_position(const Vector3D& vc);

   protected:
      /**
       * \brief Reset the platform in position and rotation
       */
      virtual void reset(void);

   private:
      /// Fiducial position
      static const Vector3D INIT_POS;

      /// Current position in space.
      Vector3D pos;
   };

/**
 * @}
 */

#endif // TRANSLATABLE_PLATFORM_H_
