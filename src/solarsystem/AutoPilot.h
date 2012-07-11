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

#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

#include <vector>

#include "CameraState.h"
#include "Path.h"
#include "Traversable.h"
#include "Traverse.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates automatic viewpoint generation.
 *
 *      Animation proceeds frame by frame and for unattended behaviours
 * a prescription for viewpoint evolution is desired. This is done via
 * the concept of a Traverse, being a series of waypoints or Lookouts
 * separate by Paths. When the Autopilot is engaged a Traverse is created
 * by querying a Traversable object and combining the received listing
 * with the current CameraState. That yields and endless loop of viewpoints
 * to visit.
 *
 * \see CameraState
 * \see Path
 * \see Traverse
 * \see Traversable
 *
 * \author Mike Hewson\n
 */

class AutoPilot {
    public :
        /**
         * \brief Constructor
         */
        AutoPilot(void);

        /**
         * \brief Destructor
         */
        virtual ~AutoPilot();

        /**
         * \brief Activate the autopilot
         *
         * \param trav : a Traversable object to query
         * \param cam : the current viewpoint
         */
        void activate(const Traversable& trav, const CameraState& cam);

        /**
         * \brief Inactivate the autpilot
         */
        void inactivate(void);

        /**
         * \brief Query the activity state
         *
         * \return a boolean indicating :
         *              TRUE if active
         *              FALSE if not active
         */
        bool isActive(void) const;

        /**
         * \brief Obtain the current view state for rendering.
         *        Also moves the camera viewpoint along a notional
         *        amount.
         *
         * \return a camera state for animation
         *
         */
        const CameraState& viewState(void);

        /**
         * \brief Obtain the descriptive strings for the nearest Lookout.
         *
         *      Along each Path in the Traverse the available current
         * description changes. So just prior to, and just after, visiting
         * a LookOut a description is available. In the midsection of a
         * Path between LookOut visits no description is available.
         *
         * \return a list of descriptive strings
         */
        const std::vector<std::string>& getDescription(void) const;

    private:
        // Enumerants for Path stages.
        enum path_stage {EARLY, MIDDLE, LATE};

        // Lambda boundaries for Path stages.
        static const float PATH_EARLY_BOUNDARY;
        static const float PATH_LATE_BOUNDARY;

        // Distance in notional units to progress
        // the view/camera state between frames.
        // This is implies that the evolution is
        // frame based and not ( wall clock ) based.
        static const float LENGTH_PER_FRAME;

        // Of the three curves that define a Path,
        // what is the length of the shortest? This
        // is used to govern the rate of change of
        // camera state, and thus reduces the possibility
        // of overly rapid movement and/or slewing.
        static const float LEAST_PATH_LENGTH;

        // The number of frames to delay viewpoint
        // evolution while at a Lookout position.
        static const unsigned int PAUSE_FRAME_COUNT;

        // Flag for activation state.
        bool active_flag;

        // The
        CameraState view;

        Traverse current_traverse;

        Path current_path;

        float lambda;

        float current_delta_lambda;

        std::vector<std::string> current_description;

        path_stage path_stage_flag;

        bool description_change_flag;

        void set_delta_lambda(void);

        void getTraverse(const Traversable& trav, const CameraState& cam);
    };

/**
 * @}
 */

#endif /*AUTOPILOT_H_*/
