/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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
#include "HUDImage.h"
#include "Path.h"
#include "TargetReticle.h"
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
 * ( without keyborad or mouse input ) a prescription for viewpoint
 * evolution is desired. This is done via the concept of a Traverse, being
 * a series of Lookouts ( waypoints ) separated by Paths. Each Path links
 * two Lookouts, and is composed of three distinct Curves : one to prescribe
 * evolution of camera position ( where it is situated ), one to prescribe
 * evolution of camera focus point ( where it is pointed ) and one to prescribe
 * camera orientation ( which way is 'up' or top of window ). Thus at any
 * time ( notional units ) a CameraState is available to base a frame render
 * upon.
 *
 *      When the Autopilot is activated a Traverse is created by querying the
 * given Traversable object to obtain a list of Lookouts, which is combined
 * with a Lookout representing the CameraState at the time of autopilot
 * activation. That yields a set of Lookouts of interest that may be
 * sequentially visited along a closed loop of Paths. That is : the last
 * Lookout in the list is succeeded by the first ( the one representing
 * the camera view at activation ) cyclically. Each Lookout may be queried
 * to yield descriptive strings and/or images specific to that point of view.
 *
 *      A pointer to a TargetReticle object needs to be provided on activation
 * in order to display or hide it appropriately.
 *
 * \see CameraState
 * \see Curve
 * \see HUDimage
 * \see Lookout
 * \see Path
 * \see TargetReticle
 * \see Traversable
 * \see Traverse
 *
 * \author Mike Hewson\n
 */

class AutoPilot {
    public :
        enum description_change {NONE, ADDED, DELETED};

        /**
         * \brief Constructor.
         */
        AutoPilot(void);

        /**
         * \brief Destructor.
         */
        virtual ~AutoPilot();

        /**
         * \brief Evolve the autopilot by one notional time step.
         */
        void step(void);

        /**
         * \brief Activate the autopilot, which also constructs a Traverse
         *        based upon the given parameters.
         *
         * \param trav : a Traversable object to query.
         * \param cam : the current viewpoint.
         * \param reticle : a pointer to a TargetReticle
         */
        void activate(const Traversable& trav, const CameraState& cam, TargetReticle* reticle);

        /**
         * \brief Inactivate the autpilot.
         */
        void inactivate(void);

        /**
         * \brief Query the activation state.
         *
         * \return a boolean indicating :
         *              TRUE if active
         *              FALSE if not active
         */
        bool isActive(void) const;

        /**
         * \brief Obtain the current view state for rendering.
         *
         * \return A CameraState for animation.
         */
        CameraState viewState(void) const;

        /**
         * \brief Obtain the descriptive strings for the nearest Lookout.
         *
         *      Along each Path in the Traverse the available current
         * description changes. So just prior to, and just after, visiting
         * a LookOut a description is available. In the midsection of a
         * Path between LookOut visits no description is available.
         *
         * \return A reference to a list of descriptive strings.
         */
        const std::vector<std::string>& getDescription(void) const;

        /**
         * \brief Obtain the image resource name for the nearest Lookout.
         *
         *      Along each Path in the Traverse the available images change.
         * So just prior to, and just after, visiting a LookOut image is
         * available. In the midsection of a Path between LookOut visits no
         * image is available.
         *
         * \return An image resource name.
         */
        std::string getImageResourceName(void) const;

        /**
         * \brief Determine if the description/images have changed since the
         *        last step of the autopilot.
         *
         * \return enumerant : NONE             ( no change )
         *                     ADDED            ( a description has been added )
         *                     DELETED          ( a description has been removed )
         */
        AutoPilot::description_change hasDescriptionChanged(void) const;

    private:
        // Enumerants for Path stages.
        enum path_stage {EARLY, MIDDLE, LATE};

        // Lambda boundaries for Path stages.
        // The lambda value that divides the EARLY from the MIDDLE stage.
        static const float PATH_EARLY_BOUNDARY;
        // The lambda value that divides the MIDDLE from the LATE stage.
        static const float PATH_LATE_BOUNDARY;

        // Distance in notional units within simulation/world space to
        // progress the view/camera state between frames. This is implies
        // that the evolution is frame based and not time based.
        static const float LENGTH_PER_FRAME;

        // If a Path has the length of its longest Curve any
        // LESS than this amount, then only one frame will
        // be rendered. Prevents ( allegedly ) jerkiness or stutter
        // with really short Paths - including those of zero length.
        static const float LEAST_PATH_LENGTH;

        // The number of frames to delay viewpoint evolution while
        // sitting at a Lookout position.
        static const unsigned int PAUSE_FRAME_COUNT;

        // Flag for activation state.
        bool active_flag;

        // The current CameraState which is progressively evolved.
        CameraState view;

        // The Traverse currently being used in a cyclical fashion.
        Traverse current_traverse;

        // The current Path within the current Traverse.
        Path current_path;

        // The paramater used to evolve the CameraState along the current Path.
        GLfloat lambda;

        // The increment in lambda to use which yields ( fairly closely )
        // the LENGTH_PER_FRAME distance between successive CameraStates.
        GLfloat current_delta_lambda;

        // The current set of descriptive strings.
        std::vector<std::string> current_description;

        // The current HUD displayable image resource name.
        std::string current_image;

        // A flag indicating the stage of the current Path that we are in.
        path_stage path_stage_flag;

        // Flag indicating whether there has been a change of descriptive
        // strings and/or images.
        AutoPilot::description_change description_change_flag;

        TargetReticle* target;

        /**
         * \brief For the current Path decide upon the best increment in lambda
         *        to use.
         *
         *      Essentially the following occurs : the Curves within the Path
         * are examined to find the longest one, the length of that Curve is
         * divided by LENGTH_PER_FRAME thus yielding some number of frames to
         * render along said Path. As all Curves are parameterised by a lambda
         * going from 0.0f ( start ) to 1.0f ( finish ), then the reciprocal of
         * that calculated number of frames becomes the increment in the
         * lambda's to use in progression along the entire current Path. The
         * reason the longest Curve is chosen for division is in order to
         * reduce any stuttering and/or slew rates.
         *      Note that the same lambda increment is used for all three
         * Curves, and so those other two ( shorter ) Curves will have smaller
         * length increments than LENGTH_PER_FRAME.
         */
        void set_delta_lambda(void);

        /**
         * \brief Construct a Traverse which is populated by Paths and Lookouts
         *        consistent with the given Traversable object and CameraState.
         *
         *      The given CameraState becomes the first point in the Traverse.
         *
         * \param trav : a Traversable object to query.
         * \param cam : the current viewpoint.
         */
        void getTraverse(const Traversable& trav, const CameraState& cam);
    };

/**
 * @}
 */

#endif /*AUTOPILOT_H_*/
