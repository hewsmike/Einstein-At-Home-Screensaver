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
 
#ifndef SHAPE_H_
#define SHAPE_H_

#include "Vector3D.h"
 
/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Gives position information along a path.
 *
 * \author Mike Hewson\n
 */

class Shape {
   public:
		/**
	    * \brief Constructor
	    *
	    * \param start : the point at the begining
	    *
	    * \param finish : the point at the end
	    */
		Shape(Vector3D start, Vector3D finish);

		Shape(void);

		/**
		 * \brief Destructor
		 */
		~Shape();

		/*
		 * \brief The vector to the point parameterised by the given rho. In this
		 * 		 base class a simple straight line is emulated.
		 *
		 * \param rho : a value b/w zero and one indicating the desired point.
		 */
		virtual Vector3D value(float rho) const;

		void setStart(Vector3D start);
		void setFinish(Vector3D finish);
   
   private:
		Vector3D begin;
		Vector3D end;
   };

/**
 * @}
 */

#endif // SHAPE_H_
