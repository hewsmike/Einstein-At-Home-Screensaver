/***************************************************************************
 *   Copyright (C) 2014 by Mike Hewson                                     *
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

#ifndef BOUND_H_
#define BOUND_H_

#include "ogl_utility.h"

#include "framework.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        objects that have binding semantics with the OpenGL pipeline.
 *
 *      Many OpenGL types have a common functionality for which this
 * class is a base of. The detailed binding and unbinding to the pipeline
 * resources is to be provided in subclasses.
 *
 * \author Mike Hewson\n
 */

class Bound {
    public:
        /**
         * \brief Constructor ( no argument )
         */
        Bound(void);

        /**
         * \brief Destructor - do NOT call release() here, a suitable derived
         *        class destructor MUST call it.
         */
        virtual ~Bound();

        /**
		 * \brief Perform any binding to the OpenGL pipeline.
		 */
		virtual void bind(void) = 0;

		/**
		 * \brief Remove any binding to the OpenGL pipeline.
		 */
		virtual void unbind(void) = 0;

		/**
		 * \brief The binding state of the underlying OpenGL buffer
		 *        object to the OpenGL state machine. This MUST be a
		 *        dynamic inquiry.
		 *
		 * \return a boolean indicating binding state :
		 *          true - the object is bound
		 *          false - the object is not bound
		 */
		virtual bool isBound(void) const = 0;
    };

/**
 * @}
 */

#endif /*BOUND_H_*/
