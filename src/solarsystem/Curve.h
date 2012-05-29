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
 
#ifndef CURVE_H_
#define CURVE_H_

#include "Vector3D.h"
 
/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Gives position information along, and thus defines the precise
 *        Curve of, a curve in 3D space.
 *
 *    A class invariant is that the 'start' and 'finish' vectors will
 * be referenced by parameter values of 0.0 and 1.0 respectively.
 *
 * \author Mike Hewson\n
 */

class Curve {
   public:
		/**
	    * \brief Constructor
	    *
	    * \param start : the vector to the begining
	    *
	    * \param finish : the vector to the end
	    */
		Curve(const Vector3D& start, const Vector3D& finish);

		/**
		 * \brief Destructor
		 */
		virtual ~Curve();

		/*
		 * \brief The vector to the point parameterised by the given rho. In this
		 * 		 base class a simple straight line is emulated.
		 *
		 * \param rho : a value indicating the desired point.
		 *              The start vector can be retrieved by a value of zero,
		 *              and the finish vector by a value of one. Rho values outside
		 *              of the interval [0.0, 1.0] will still give an evaluated
		 *              vector according to said value.
		 *
		 * \return a vector indicating the point referred to by the given rho.
		 */
		virtual Vector3D value(float rho) const;

   private:
		Vector3D begin;
		Vector3D end;
   };

/**
 * @}
 */

#endif // CURVE_H_
