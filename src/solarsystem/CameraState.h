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

#ifndef CAMERA_STATE_H_
#define CAMERA_STATE_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief View state information
 *
 * \author Mike Hewson\n
 */

class CameraState {
   public:
      CameraState(void);

		/**
	    * \brief Constructor
	    *
	    * \param position : the position of the camera
	    * \param focus : the point the camera is looking at
	    * \param orientation : the up direction for the view
	    */
		CameraState(const Vector3D& position, const Vector3D& focus, const Vector3D& orientation);

		CameraState(const CameraState& other);

		CameraState& operator=(const CameraState& other);

		/**
		 * \brief Destructor
		 */
		~CameraState();

		void setPosition(const Vector3D& position);

		void setFocus(const Vector3D& focus);

		void setOrientation(const Vector3D& orientation);

		const Vector3D& position(void) const;

		const Vector3D& focus(void) const;

		const Vector3D& orientation(void) const;

   private:
		Vector3D where;
		Vector3D look_at;
		Vector3D up_dir;
   };

/**
 * @}
 */

#endif // CAMERA_STATE_H_
