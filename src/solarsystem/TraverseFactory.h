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
#ifndef TRAVERSE_FACTORY_H_
#define TRAVERSE_FACTORY_H_

#include "CameraState.h"
#include "Traverse.h"
#include "Traversable.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief

 * \author Mike Hewson\n
 */

class TraverseFactory {
   public:
		/**
		 * \brief Constructor
		 */
	   TraverseFactory(void);

      /// Virtual destructor
      virtual ~TraverseFactory();

      Traverse* getInstance(const Traversable& trav, const CameraState& cam);
   };

/**
 * @}
 */

#endif /*TRAVERSE_FACTORY_H_*/