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
#ifndef TRAVERSABLE_H_
#define TRAVERSABLE_H_

#include "CameraState.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares all mandatory public methods to be provided by

 * \author Mike Hewson\n
 */

class Traversable {
   public:
		/**
		 * \brief Constructor
		 */
	   Traversable(void);

      /// Virtual destructor
      virtual ~Traversable();

      virtual unsigned int numberOfWayPoints(void) const = 0;

      virtual CameraState getView(unsigned int sequence) const = 0;
   };

/**
 * @}
 */

#endif /*TRAVERSABLE_H_*/
