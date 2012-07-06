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

#include "LookOut.h"
#include "Path.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates a list of camera states with descriptions
 *        ( called Lookouts ) linked by paths.
 *
 *      A sequence of Lookouts may be progressively added, and when at least two
 * are present then one can extract a sequence of Paths that connect them. For
 * Path extraction initially call getFirstPath and then getNextPath for a cyclical
 * sequence. If fewer than two Lookouts are present before a Path retrieval then
 * a fatal error occurs. If getFirstPath wasn't called prior to getNextPath, then
 * the result is not defined ( though a Path is returned ). I recommend restarting
 * any Path traverse with getFirstPath if one or more Lookouts are added during a
 * series of Path fetches.
 *
 * \see CameraState
 * \see Lookout
 * \see Path
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

        /**
         * \brief Empty the traverse of all LookOuts
         */
		void clear(void);

		/**
		 * \brief Add a LookOut onto the tail of the list
		 *
		 * \param cam : the camera state to add
		 */
		void addLookout(const LookOut& cam);

        /**
		 * \brief Obtain the current number of stored LookOuts
		 *
		 * \return the number of Lookouts
		 */
		unsigned int numLookouts(void) const;

        /**
         * \brief Obtain the initial path in the list
         *
         *      If there are fewer than two Lookouts present then
         * a fatal error is triggered.
         *
         * \return the first Path in the list
         */
		Path getFirstPath(void);

		/**
		 * \brief Obtain the next path from the traverse
		 *
		 *      This occurs in a cyclic fashion where the most
		 * recently added lookout is connected to the first
		 * added lookout. If there are fewer than two Lookouts
		 * present then a fatal error is triggered.
		 *
		 * \return the next Path in the list
		 */
		Path getNextPath(void);

	private:
        static unsigned int MIN_LOOKOUTS;

		std::deque<LookOut> cam_states;

		unsigned int current_path_index;

		Path makePath();
	};

/**
 * @}
 */

#endif // TRAVERSE_H_
