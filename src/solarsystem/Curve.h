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

#ifndef CURVE_H_
#define CURVE_H_

#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates a geometric curve in 3D space.
 *
 *      Take any two points in 3D space ( defined by vectors from the
 * origin to each of them ) and create a continuous sequence of points
 * in between them to form a curve. Use a parameter, a real number here
 * called 'rho', with an appropriate rule or function to determine the
 * position of points on the curve.
 *      A class invariant is that the 'start' and 'finish' vectors will
 * be referenced by rho values of 0.0 and 1.0 respectively. This
 * class will still evaluate for any rho value, that is outside of the
 * range [0.0, 1.0] as well ie. extrapolate the curve.
 *
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class Curve {
   public:
        /**
         * Constructor.
         */
        Curve(void);

        /**
         * \brief Constructor.
         *
         * \param start : the vector to the begining point.
         *
         * \param finish : the vector to the end point.
         */
        Curve(const Vector3D& start, const Vector3D& finish);

        /**
         * \brief Destructor.
         */
        virtual ~Curve();

        /**
         * \brief The vector to the point parameterised by the given rho. In
         *        this base class a simple straight line is emulated.
         *
         * \param rho : a value indicating the desired point.
         *
         * \return a Vector3D indicating the point referred to by the given rho.
         */
        virtual Vector3D value(float rho) const;

        /**
         * \brief Obtain the length along the curve ( in notional units ) from
         *        the start to the finish point.
         *
         * \return the length along the curve.
         */
        virtual float length(void) const;

   private:
        Vector3D begin;
        Vector3D end;
    };

/**
 * @}
 */

#endif // CURVE_H_
