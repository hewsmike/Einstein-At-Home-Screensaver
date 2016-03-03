/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#include "ogl_utility.h"

#include "Buffer.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        texture objects.
 *
 *        There are many restrictions compared with a full OpenGL texture
 * facility, for reasons of efficiency and simplicity.
 *        - no multitexturing, thus no explicit reference to any active texture
 *          unit ie. GL_TEXTURE0.
 *        - texture data is provided by pointer only eg. no file sources.
 *        - minimum texure dimensions is 2 x 2.
 *        - the target is GL_TEXTURE_2D ie. planar surface mapping.
 *        - no change is made to internal formats on loading ie. the state
 *          machine storage format is whatever the data source has.
 *        - there are no image borders assumed.
 *        - if mipmaps are generated the default base level is the original
 *          scale.
 *        - basic filtering is set to NEAREST ( Manhattan distance ).
 *        - anisotropic filtering not handled.
 *        - texture compression not handled.
 *
 * \see Buffer
 *
 * \author Mike Hewson\n
 */

class TextureBuffer : public Buffer {
    public :
        /**
         * \brief Constructor of an OpenGL borderless GL_TEXTURE_2D
         *        texture type.
         *
         * \param texture_data : a pointer to the image data
         * \param bytes : the number of bytes of data.
         * \param width : in texels, must be a strictly positive power of 2
         * \param height : in texels, must be a strictly positive power of 2
         * \param format : one of the following allowed enumerants which
         *                 indicate the ORDERING of the color components.
         *                      GL_ALPHA
         *                      GL_LUMINANCE
         *                      GL_LUMINANCE_ALPHA
         *                      GL_RGB
         *                      GL_RGBA
         * \param data_type : one of the following allowed enumerants which
         *                    indicate the PER COLOR BINARY REPRESENTATION.
         *                      GL_UNSIGNED_BYTE
         *                      GL_UNSIGNED_SHORT_5_6_5
         *                      GL_UNSIGNED_SHORT_4_4_4_4
         *                      GL_UNSIGNED_SHORT_5_5_5_1
         * \param wrap_type_s : one of the following allowed enumerants
         *                      GL_REPEAT
         *                      GL_CLAMP
         *                      GL_CLAMP_TO_EDGE
         *                      GL_CLAMP_TO_BORDER
         * \param wrap_type_t : one of the following allowed enumerants
         *                      GL_REPEAT
         *                      GL_CLAMP
         *                      GL_CLAMP_TO_EDGE
         *                      GL_CLAMP_TO_BORDER
         * \param mipmaps : if true, all mipmaps generated down to 1 x 1
         */
        TextureBuffer(const GLvoid* texture_data,
                      GLuint bytes,
                      GLsizei width,
                      GLsizei height,
                      GLenum format,
                      GLenum data_type,
                      GLenum wrap_type_s,
                      GLenum wrap_type_t,
                      bool mipmaps = true);

        /**
         * \brief Destructor
         */
        virtual ~TextureBuffer();

        /**
         * \brief Perform any binding to the OpenGL pipeline.
         */
        virtual void bind(void);

        /**
         * \brief Remove any binding to the OpenGL pipeline.
         */
        virtual void unbind(void);

        /**
         * \brief The binding state of the underlying OpenGL buffer
         *        object to the OpenGL state machine. This MUST be a
         *        dynamic inquiry.
         *
         * \return a boolean indicating binding state :
         *          true - the object is bound
         *          false - the object is not bound
         */
        virtual bool isBound(void) const;

    private :
        static const GLuint DEFAULT_MIPMAP_BASE_LEVEL;
        static const GLuint DEFAULT_IMAGE_BORDER_WIDTH;
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

        /// The wrapping mode along the texture horizontal axis.
        GLenum m_wrap_type_s;

        /// The wrapping mode along the texture vertical axis.
        GLenum m_wrap_type_t;

        /// Whether or not mipmaps are to be generated.
        bool m_mipmaps;

        /**
         * \brief Get an OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void acquire_ID(GLuint* handle);

        /**
         * \brief Release to pool the OpenGL handle for the texture.
         *
         * \param handle : pointer to a handle.
         */
        virtual void release_ID(GLuint* handle);

        /**
         * \brief Populate the buffer with texture data.
         */
        virtual void loadBuffer(void);
    };

/**
 * @}
 */

#endif /*TEXTURE_BUFFER_H_*/
