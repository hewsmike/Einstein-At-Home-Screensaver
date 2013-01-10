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

#ifndef ORTHO_NORMAL_PLATFORM_H_
#define ORTHO_NORMAL_PLATFORM_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Right-handed Cartesian orthonormal vector set.
 *
 *      Consider a mutually orthogonal set of three vectors in space
 * such that each is of unit length. The vector cross product of the
 * 'first' vector with the 'second' yields a third vector. Here 'look'
 * is designated as the first vector, with the meaning that it is the
 * direction along which a viewpoint is aimed. 'Up' is designated as
 * the second vector meaning that it is perpendicular to the line of
 * sight defined by 'look' and specifies which way should be deemed as
 * us upwards. The remaining vector, here called 'cross' is the final
 * vector formed by the vector cross product 'look X up' and would in
 * the above described viewpoint be going to the right and perpendicular
 * to the first two.
 *
 * \see Vector3D
 *
 * TODO Include testing and if necessary adjustment to maintain mutual
 * orthogonality of the three axes ? HINT Set 'look' and 'up' as a pair
 * atomically and have normalisation followed by adjustment of either/both
 * by the addition to them of ( small ) corrective vectors that lie within
 * their mutual plane. FATAL if 'look' and 'up' are ( anti- ) parallel.
 *
 * \author Mike Hewson\n
 */

class OrthoNormalPlatform {
    public:
        /**
         * \brief Constructor
         */
        OrthoNormalPlatform(void);

        /**
         * \brief Destructor
         */
        virtual ~OrthoNormalPlatform();

        /**
         * \brief return the 'cross' vector
         *
         *      This is obtained by forming the cross product of the 'look' and
         * 'up' vectors in that order. Once formed then if one is situated at
         * the intersection of these three vectors, looking along the positive
         * direction of the 'look' axis ( from the tail to the head of the 'look'
         * vector ), then the 'cross' vector can be obtained by turning the 'up'
         * vector 90 degrees clockwise around that 'look' axis.
         */
        Vector3D cross(void) const;

        /**
         * \brief Retrieve the 'look' vector
         */
        Vector3D look(void) const;

        /**
         * \brief Retrieve the 'up' vector
         */
        Vector3D up(void) const;

   protected:
        /**
         * \brief Set the direction of the 'look' and 'up' vectors
         *
         * Protected use only as need to ensure orthogonality of the vector set
         * ie. can't have arbitrary assignment of the 'look' or 'up' vectors.
         * Also want the changes as atomic.
         *
         * \param new_look : the desired 'look' vector
         * \param new_up : the desired 'up' vector
         */
        void setOrientation(Vector3D new_look,
                            Vector3D new_up);

        /**
         * \brief Reset the platform in orientation
         */
        virtual void reset(void);

    private:
        /// Initial 'look' vector
        static const Vector3D INITIAL_LOOK;

        /// Initial 'up' vector
        static const Vector3D INITIAL_UP;

        /// The current 'look' vector.
        Vector3D look_dir;

        /// The current 'up' vector.
        Vector3D up_dir;
    };

/**
 * @}
 */

#endif // ORTHO_NORMAL_PLATFORM_H_
