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

#ifndef TRAVERSE_H_
#define TRAVERSE_H_

#include <deque>

#include "CameraState.h"
#include "Path.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A list of camera states ( here called waypoints ) linked by paths.
 *
 * \author Mike Hewson\n
 */

class Traverse {
   public:
      /**
	    * \brief Constructor
	    */
		Traverse(void);

		/**
		 * \brief Destructor
		 */
		~Traverse();

		void clear(void);

		/**
		 * \brief Add a waypoint onto the tail of the list.
		 *
		 * \param cam : the camera state to add
		 */
		void addWayPoint(const CameraState& cam);

		unsigned int numWayPoints(void) const;

      /**
       * \brief Obtain the initial path in the list.
       *
       * \return the first Path in the list
       */
		Path getFirstPath(void);

		/**
		 * \brief Obtain the next path from the traverse. This
		 *        occurs modulo the number of waypoints ie.
		 *        in a cyclic fashion.
		 *
		 * \return the next Path in the list
		 */
		Path getNextPath(void);

	private:
		std::deque<CameraState> cam_states;

		unsigned int current_path_index;

		Path makePath(void);
	};

/**
 * @}
 */

#endif // TRAVERSE_H_
