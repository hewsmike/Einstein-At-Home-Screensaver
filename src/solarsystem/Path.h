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

#ifndef PATH_H_
#define PATH_H_

#include "Shape.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A path in 3D space.
 *
 * \author Mike Hewson\n
 */

class Path {
   public:
		enum evolve_type {CLAMPED, CYCLIC};

		enum param_type {POSITION, FOCUS, ORIENTATION};

		Path(void);

		/**
	    * \brief Constructor
	    *
	    * \param start : the position at the begining
	    * \param finish : the position at the end
	    * \param first_look : the focus at the begining
	    * \param last_look : the focus at the end
	    * \param first_up : the orientation at the begining
	    * \param first_look : the orientation at the end
	    * \param type : one of the curve type enumerants
	    */
		Path(const Shape& pos, const Shape& view, const Shape& orient, evolve_type e_type);

		/**
		 * \brief Destructor
		 */
		~Path();

		/*
		 * \brief Obtain the vector parameterised by the given lambda.
		 *
		 * \param lambda : a value indicating the desired point.
		 * \param pt : one of the parameter type enumerants
		 *
		 * \return The vector of said parameter type at the given lambda
		 */
		Vector3D value(float lambda, param_type pt) const;

		/*
		 * \brief Set the curve for a given parameter type.
		 *
		 * \param curve : a shape indicating the desired variance.
		 * \param pt : one of the parameter type enumerants
		 */
		void set(const Shape& curve, param_type pt);

	private:
		Shape where;
		Shape look_at;
		Shape up_dir;

		evolve_type et;
	};

/**
 * @}
 */

#endif // PATH_H_
