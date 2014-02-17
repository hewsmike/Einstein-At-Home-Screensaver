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

#ifndef BUFFER_VERTEX_FETCH_
#define BUFFER_VERTEX_FETCH_

#include "Buffer.h"

#include <map>

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This class declares public methods to deal with the OpenGL ES 2.0
 *        pipeline vertex fetch functionality using buffers.
 *
 *      This is the case where vertex attributes are supplied to the pipeline
 * sequentially from a buffer.
 *
 * \author Mike Hewson\n
 */

class BufferVertexFetch
    public :
        enum data_mix {BY_VERTEX,
                       BY_ATTRIBUTE};

        struct attribute_spec {
            // Number of components for this attribute.
            GLint size;
            // The data type of each component of this attribute.
            // Valid values are :
            //      GL_BYTE
		    //      GL_UNSIGNED_BYTE
		    //      GL_SHORT
		    //      GL_UNSIGNED_SHORT
		    //      GL_FIXED
            //      GL_FLOAT
            GLenum type;
            // For integer data types only, whether values are normalised
            // with respect to the type range.
            //      GL_TRUE signed integers map to [-1, +1]
            //              unsigned integers map to [0, +1]
            //      GL_FALSE no normalisation, directly map to floats.
            GLboolean normalised;
            };

        /**
         * \brief Constructor.
         *
         * \param mix : the type of mixing of data
         *                  BY_VERTEX, per vertex storage
         *                  BY_ATTRIBUTE, per attribute storage
         */
        BufferVertexFetch(data_mix mix);

        /**
         * \brief Destructor.
         */
        virtual ~BufferVertexFetch();

        /**
         * \brief Perform any data binding to the pipeline input.
         *
         * \param vertex_buffer : the ogl_utility buffer object containing
         *                        the vertex data.
         */
        void attach(const Buffer& vertex_buffer);

        /**
         * \brief Add a vertex attribute.
         *
         * \param shader_index : the index within the shader that this attribute
         *                       loads into with each shader invocation.
         * \param specification : structure indicating data layout within
         *                        the attached buffer object & behaviour.
         */
        void addVertexAttribute(GLuint shader_index,
                                attribute_spec specification);

    private :
        data_mix m_mix;

        struct attribute_record {attribute_spec a_spec;
                                 GLsizei stride;
                                 GLvoid* pointer;
                                };

        // Storage for all the attribute specifications, sorted
        // by shader index eg. within the vertex shader's GLSL code
        //
        // layout (location = index_0) in attribute_type0 attribute_name_in_shader0;
        // layout (location = index_1) in attribute_type1 attribute_name_in_shader1;
        // layout (location = index_2) in attribute_type2 attribute_name_in_shader2;
        //
        std::map<attribute_record> m_attribute_specs;

        /**
         * \brief Create full/detailed mapping of attribute positions within
         *        the buffer, based upon any given vertex attribute specifications
         *        choice of data mixing.
         */
        void mapAttributes(void);
    };

/**
 * @}
 */

#endif /*BUFFER_VERTEX_FETCH_*/
