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
 * \brief A sequence of camera states linked by paths.
 *
 * \author Mike Hewson\n
 */

class Traverse {
   public:
		/**
	    * \brief Constructor, ensures a minimum of two way points
	    *
	    * \param first : the initial point ( camera state ) in the traverse
	    * \param second : the second point ( camera state ) in the traverse
	    */
		Traverse(const CameraState& first, const CameraState& second);

		/**
		 * \brief Destructor
		 */
		~Traverse();

		void addWayPoint(const CameraState& cam);

		bool getSegment(Path& pt, unsigned int seq) const;

		unsigned int numberOfSegments(void) const;

		unsigned int numberOfWayPoints(void) const;

	private:
		std::deque<CameraState> cam_states;
	};

/**
 * @}
 */

#endif // TRAVERSE_H_
