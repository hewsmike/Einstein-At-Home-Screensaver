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

#ifndef PATH_H_
#define PATH_H_

#include "CameraState.h"
#include "Curve.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A path in 3D space with a camera state retrievable
 *        for any position along it.
 *
 * \author Mike Hewson\n
 */

class Path {
   public:
		/**
	    * \brief Constructor
	    *
	    * \param pos : the curve for determining camera position
	    *
	    * \param view : the curve for determining camera focus point
	    *
	    * \param orient : the curve for determining camera orientation
	    */
		Path(const Curve& pos, const Curve& focus, const Curve& orient);

		/**
		 * \brief Destructor
		 */
		~Path();

		/*
		 * \brief Obtain the camera state parameterised by the given lambda.
		 *
		 * \param lambda : a value indicating the desired point. This is
		 *             clamped to the real number interval [0.0, 1.0],
		 *             meaning that if given values less than 0.0 that leads
		 *             to an evaluation at 0.0, likewise for given values
		 *             greater than 1.0 will evaluate at 1.0 ....
		 *
		 * \return The camera state at the given lambda
		 */
		CameraState value(float lambda) const;

	private:
		Curve where;
		Curve look_at;
		Curve up_dir;
	};

/**
 * @}
 */

#endif // PATH_H_
