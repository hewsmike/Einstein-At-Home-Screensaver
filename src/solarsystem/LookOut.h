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

#ifndef LOOK_OUT_H_
#define LOOK_OUT_H_

#include <string>
#include <vector>

#include "CameraState.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A camera view state with descriptive components.
 *
 * \see CameraState
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class LookOut : public CameraState {
    public:
        /**
         * \brief Constructor
         *
         * \param position : a vector to the camera's position
         * \param focus : a vector to the camera's focus point
         * \param orientation : a vector to be the camera's orientation axis
         */
		LookOut(const Vector3D& position,
                const Vector3D& focus,
                const Vector3D& orientation);

		/**
		 * \brief Destructor
		 */
		virtual ~LookOut();

        /**
         * \brief Obtain the description for this camera state
         *
         * \return a reference to a set of descriptive strings
         */
		const std::vector<std::string>& getDescription(void) const;

        /**
         * \brief Add a description to this camera state
         *
         * \param a descriptive string reference
         */
		void addToDescription(const std::string& description);

	private:
        // The set of descriptive strings for this LookOut
		std::vector<std::string> desc;
    };

/**
 * @}
 */

#endif // LOOK_OUT_H_
