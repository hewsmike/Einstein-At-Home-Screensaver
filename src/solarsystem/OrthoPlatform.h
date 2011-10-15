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

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <iostream>
#include <cmath>

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem dynamical platform state
 *
 * This class comprises the viewpoint state data only.
 * It has no dynamical model.
 *
 * \author Mike Hewson\n
 * hewsmike@iinet.net.au
 */

class OrthoPlatform{
   private:
		static const Vector3D INIT_LOOK;
		static const Vector3D INIT_UP;

      // Three components of the point we are looking towards in space.
      Vector3D look_dir;

		// Three components defining the 'up' view direction.
      Vector3D up_dir;

   protected:
      // Mutators.
		void set_look(const Vector3D& n_look);
      void set_up(const Vector3D& n_up);

      virtual void reset(void);

	public:
      OrthoPlatform();
      virtual ~OrthoPlatform();

      // Accessors.
      // Cross vector is derived from the other two via lazy evaluation.
      // NOTE : Accumulated rounding error can be a source of 'platform drift'
      // with respect to orientation axes. That is, there is no check that
      // the first two vectors are orthogonal or will remain so .....
      Vector3D cross(void) const;

      Vector3D look(void) const;
      Vector3D up(void) const;

      // Mutators.
      void pitch_down(vec_t angle);
      void pitch_up(vec_t angle);
      void roll_left(vec_t angle);
      void roll_right(vec_t angle);
      void yaw_left(vec_t angle);
      void yaw_right(vec_t angle);

      void show_invariants(void) const;
	};

#endif // PLATFORM_H_
