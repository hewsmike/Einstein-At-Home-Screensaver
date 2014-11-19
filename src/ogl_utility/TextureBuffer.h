/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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

#ifndef TEXTURE_BUFFER_H_
#define TEXTURE_BUFFER_H_

#include "Buffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        texture objects.
 *
 * \see Buffer
 *
 * \author Mike Hewson\n
 */

class TextureBuffer : public Buffer {
    public :
        /**
         * \brief Constructor of an OpenGL ES 2.0 borderless GL_TEXTURE_2D
         *        texture type.
         *
         * \param texture_data : a pointer to the image data
         * \param width : in texels, must be a strictly positive power of 2
         * \param height : in texels, must be a strictly positive power of 2
         * \param format : one of the following ES 2.0 allowed enumerants
         *                      GL_ALPHA
         *                      GL_LUMINANCE
         *                      GL_LUMINANCE_ALPHA
         *                      GL_RGB
         *                      GL_RGBA
         * \param data_type : one of the following ES 2.0 allowed enumerants
         *                      GL_UNSIGNED_BYTE
         *                      GL_UNSIGNED_SHORT_5_6_5
         *                      GL_UNSIGNED_SHORT_4_4_4_4
         *                      GL_UNSIGNED_SHORT_5_5_5_1
         * \param mipmaps : if true, all mipmaps generated down to 1x1
         */
        TextureBuffer(const GLvoid* texture_data,
                      GLsizei width,
                      GLsizei height,
                      GLenum format,
                      GLenum data_type,
                      bool mipmaps = true);

        /**
         * \brief Destructor
         */
        virtual ~TextureBuffer();

    private :
        static const GLsizei MIN_TEX_WIDTH;
        static const GLsizei MIN_TEX_HEIGHT;

        /// What the texture data represents in terms of rendering intent.
        GLint m_format;

        /// The width of the texture.
        GLsizei m_width;

        /// The height of the texture.
        GLsizei m_height;

        /// The binary representation of the data.
        GLenum m_data_type;

        /// Whether or not mipmaps are to be generated.
        bool m_mipmaps;

        /**
         * \brief Get an OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void acquire_ID(GLuint* handle) const;

        /**
         * \brief Release to pool the OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void release_ID(GLuint* handle) const;

        /**
         * \brief Populate the buffer with texture data.
         */
        virtual void loadBuffer(void);
    };

/**
 * @}
 */

#endif /*TEXTURE_BUFFER_H_*/
