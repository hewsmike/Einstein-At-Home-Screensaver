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

#ifndef ATTRIBUTE_INPUT_ADAPTER_H_
#define ATTRIBUTE_INPUT_ADAPTER_H_

#include "ogl_utility.h"

#include <string>
#include <vector>

#include "VertexBuffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with the OpenGL
 *        interface between vertex data buffers and the vertex shader. It
 *        stores the naming, data type and location correspondences.
 *
 * \author Mike Hewson\n
 */

class AttributeInputAdapter {
    public :
		struct attribute_spec {
			// The index of the attribute in the OpenGL vertex array object (VAO).
			GLuint attrib_index;
			// Name of the corresponding vertex shader variable.
			std::string name;
			// Number of components for this attribute eg. 3 for x, y and z coordinates,
			// 4 for RBGA values.
			GLint multiplicity;
			// The data type of each component of this attribute.
			// Valid values are :
			// 		GL_BYTE
			// 		GL_UNSIGNED_BYTE
			// 		GL_SHORT
			// 		GL_UNSIGNED_SHORT
			// 		GL_FIXED
			// 		GL_FLOAT
			GLenum type;
			// For integer data types only, whether values are normalised
			// with respect to the type range.
			// GL_TRUE signed integers map to [-1, +1]
			// unsigned integers map to [0, +1]
			// GL_FALSE no normalisation.
			GLboolean normalised;
			};

        /**
         * \brief Constructor
         */
		AttributeInputAdapter(void);

        /**
         * \brief Destructor
         */
        virtual ~AttributeInputAdapter();

        /**
         * \brief Add another correspondence between vertex buffer and the vertex shader.
         */
        void addSpecification(attribute_spec spec);

        /**
		 * \brief Retrieve an attribute specification at a given index into this store.
		 *
		 * \param index - a positive integer ranging from 0 to [attributeCount() - 1]. This is
		 * 				  NOT the location field of a given attribute specification.
		 * \param spec - a reference to an attribute specification to fill.
		 *
		 * \return a boolean indicating success of the retrieval
		 *              TRUE - attribute specification has been filled with valid data.
		 *              FALSE - attribute specification has not been altered,
		 *               		for instance if the index is out of range.
		 */
        bool getAttributeSpecAt(GLuint index, attribute_spec& spec) const;

        /**
		 * \brief How many attribute specifications are stored ?
		 */
        GLuint size(void) const;

    private :
        std::vector<struct attribute_spec> m_matchings;
    };

/**
 * @}
 */

#endif /* ATTRIBUTE_INPUT_ADAPTER_H_ */
