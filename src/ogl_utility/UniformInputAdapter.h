/***************************************************************************
 *   Copyright (C) 2015 by Mike Hewson                                     *
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

#ifndef UNIFORM_INPUT_ADAPTER_H_
#define UNIFORM_INPUT_ADAPTER_H_

#include "ogl_utility.h"

#include <vector>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief
 *
 * \author Mike Hewson\n
 */

class UniformInputAdapter {
    public :
		struct uniform_spec {
			// Name of the uniform variable as known within an OpenGL program object.
			std::string name;
			// Untyped pointer to location of persistent data in client space
			// which will be used to refresh the in-program value.
			GLvoid* client_where;
			};

        /**
         * \brief Constructor
         */
		UniformInputAdapter(void);

        /**
         * \brief Destructor
         */
        virtual ~UniformInputAdapter();

        /**
         * \brief Add another correspondence between uniform variable and program vertex buffer and the vertex shader.
         */
        void addSpecification(uniform_spec spec);

        /**
		 * \brief Retrieve a uniform specification at a given index into this store.
		 *
		 * \param index - a positive integer ranging from 0 to [size() - 1].
		 * \param spec - a uniform specification to fill.
		 *
		 * \return a boolean indicating success of the retrieval
		 *              TRUE - uniform specification has been filled with valid data.
		 *              FALSE - uniform specification has not been altered,
		 *               		for instance if the index is out of range.
		 */
        bool getUniformSpecAt(GLuint index, uniform_spec spec) const;

        /**
		 * \brief How many attribute specifications are stored ?
		 */
        GLuint size(void) const;

    private :
        struct full_uniform_spec {
        	//
        	uniform_spec uniform;
			//
			GLenum type;
			};

        std::vector<struct full_uniform_spec> m_matchings;
    };

/**
 * @}
 */

#endif /* UNIFORM_INPUT_ADAPTER_H_ */
