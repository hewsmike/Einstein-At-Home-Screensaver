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

#ifndef INERTIALPLATFORM_H_
#define INERTIALPLATFORM_H_

#include "OrthoPlatform.h"

class InertialPlatform : public OrthoPlatform {
   private:
      static const Vector3D NULL_VECTOR;
      static const Vector3D INIT_POS;
      static const Vector3D INIT_VEL;

      // Position in space.
      Vector3D pos;

      // Velocity in space.
      Vector3D vel;

	public:
      InertialPlatform();
      virtual ~InertialPlatform();

      // Accessors.
      Vector3D position(void) const;
      Vector3D velocity(void) const;

      // Mutators.
      void set_position(const Vector3D& vc);
		void set_velocity(const Vector3D& vc);
      void stop(void);
      void translate(void);

      virtual void reset(void);
   };

#endif // INERTIALPLATFORM_H_
