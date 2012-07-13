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
 * \brief This class encapsulates view state information for
 *        the purpose of orientating an OpenGL rendering frame.
 *
 *      To begin rendering an OpenGL frame one must first
 * define the 'camera' viewpoint. This is comprised of a position
 * in 3D space to site the camera, a point to aim the camera at,
 * and an axis to define the direction from the bottom to the top
 * of the frame.
 *
 * \see Craft
 * \see SolarSystem::render
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class CameraState {
    public:
        /**
         * \brief Constructor ( no argument )
         *
         *      The camera state will be set to that which
         * is defined by the initial/default view of the Craft class.
         */
        CameraState(void);

        /**
         * \brief Constructor
         *
         * \param position : the position of the camera
         * \param focus : the point the camera is looking at
         * \param orientation : the up direction for the view
         */
        CameraState(const Vector3D& position,
                    const Vector3D& focus,
                    const Vector3D& orientation);

        /**
         * \brief Constructor (copy)
         *
         * \param other : camera state to copy
         */
        CameraState(const CameraState& other);

        /**
         * \brief Assignment operator
         *
         * \param other : camera state to copy from
         */
        CameraState& operator=(const CameraState& other);

        /**
         * \brief Destructor
         */
        virtual ~CameraState();

        /**
         * \brief Set the camera position
         *
         * \param position : the point to place the camera at
         */
        void setPosition(const Vector3D& position);

        /**
         * \brief Set the camera focus
         *
         * \param focus : the point to focus the camera toward
         */
        void setFocus(const Vector3D& focus);

        /**
         * \brief Set the camera orientation
         *
         * \param orientation : the axis to align the camera
         */
        void setOrientation(const Vector3D& orientation);

        /**
         * \brief Obtain the camera position
         *
         * \return a vector pointing to where the camera is
         */
        const Vector3D& position(void) const;

        /**
         * \brief Obtain the camera focus point
         *
         * \return a vector pointing to where the camera is focussed
         */
        const Vector3D& focus(void) const;

        /**
         * \brief Obtain the camera orientation
         *
         * \return a vector defining the orientation of the camera
         */
        const Vector3D& orientation(void) const;

        void print(void) const;

   private:
        // The camera's position
        Vector3D where;

        // The camera's focus point
        Vector3D look_at;

        // The camera's orientation axis
        Vector3D up_dir;
    };

/**
 * @}
 */

#endif // CAMERA_STATE_H_
