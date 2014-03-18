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

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Program.h"
#include "VertexFetch.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with Pipeline
 *        objects. This encompasses a given pipeline state ie. a specified
 *        OpenGL program with it's vertex fetch capability.
 *
 * \see Program
 * \see VertexFetch
 *
 * \author Mike Hewson\n
 */

class Pipeline {
    public :
        /**
         * \brief Constructor.
         *
         * \param program : reference to an existing Program object.
         * \param vertex_fetch : reference to an existing VertexFetch object.
         */
        Pipeline(Program& program, VertexFetch& vertex_fetch);

        /**
         * \brief Destructor.
         */
        virtual ~Pipeline();

    private:
        // These are merely set during construction.
        /// The Program reference.
        Program& m_program;

        /// The VertexFetch reference.
        VertexFetch& m_vertex_fetch;
    };

/**
 * @}
 */

#endif /*PIPELINE_H_*/

