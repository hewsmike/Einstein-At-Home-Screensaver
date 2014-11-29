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

#ifndef VERTEX_ARRAY_OBJ_H_
#define VERTEX_ARRAY_OBJ_H_

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        Vertex Array objects. It's a wrapper.
 *
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class VertexArrayObject : public OGL_ID {
    public :
        /**
         * \brief Constructor.
         */
        VertexArrayObject(void);

        /**
         * \brief Destructor.
         */
        virtual ~VertexArrayObject();

        /**
         * \brief Obtains the vertex array object resources.
         *
         * \return a boolean indicating success of acquisition
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        bool acquire(void);

        /**
         * \brief Releases the vertex array object resources.
         */
        void release(void);

    private:

    };

/**
 * @}
 */

#endif /*VERTEX_ARRAY_OBJ_H_*/
