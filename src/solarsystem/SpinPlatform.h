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

#ifndef SPIN_PLATFORM_H_
#define SPIN_PLATFORM_H_

#include "OrthoNormalPlatform.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Cartesian orthonormal vector set
 *
 * This class comprises angular velocity state data, accessors and mutators thereof.
 *
 * \author Mike Hewson\n
 */

class SpinPlatform : public OrthoNormalPlatform {
   public:
      /**
       * \brief Constructor
       */
      SpinPlatform(void);

      /**
       * \brief Destructor
       */
      virtual ~SpinPlatform();

      /**
       * \brief Pitch, positive angle is up
       *
       * \param angle in radians
       */
      void pitch(vec_t angle);

       /**
       * \brief Roll, positive angle is to the right
       *
       * \param angle in radians
       */
      void roll(vec_t angle);

       /**
       * \brief Yaw, positive angle is to the right
       *
       * \param angle in radians
       */
      void yaw(vec_t angle);

   protected:
      /**
       * \brief Reset the platform in spin
       */
      virtual void reset(void);

      /**
       * \brief Evolve the platform in spin
       */
      virtual void step(void);

      // Current pitch rate.
      vec_t pitch_rate;

      vec_t roll_rate;

      vec_t yaw_rate;
   private:
      static const vec_t NULL_RATE;
	};

/**
 * @}
 */

#endif // SPIN_PLATFORM_H_
