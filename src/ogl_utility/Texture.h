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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OGL_ID.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        texture objects.
 *
 *      It's a wrapper class to especially manage acquisition and
 * release of OpenGL resources.
 *
 * \see OGL_ID
 *
 * \author Mike Hewson\n
 */

class Texture : public OGL_ID {
    public :
        /**
         * \brief Constructor of an OpenGL ES 2.0 borderless GL_TEXTURE_2D
         *        texture type.
         *
         * \param mipmaps : if true, all mipmaps generated down to 1x1
         *
         * \param format : one of the following ES 2.0 allowed enumerants
         *                      GL_ALPHA
         *                      GL_LUMINANCE
         *                      GL_LUMINANCE_ALPHA
         *                      GL_RGB
         *                      GL_RGBA
         * \param width : in texels, must be at least 64
         * \param height : in texels, must be at least 64
         * \param data_type : one of the following ES 2.0 allowed enumerants
         *                      GL_UNSIGNED_BYTE
         *                      GL_UNSIGNED_SHORT_5_6_5
         *                      GL_UNSIGNED_SHORT_4_4_4_4
         *                      GL_UNSIGNED_SHORT_5_5_5_1
         * \param data : a pointer to the image data
         */
        Texture(bool mipmap,
                GLint format,
                GLsizei width,
                GLsizei height,
                GLenum data_type,
                const GLvoid* data);

        /**
         * \brief Destructor
         */
        virtual ~Texture();

        /**
         * \brief Acquire the texture object's OpenGL resources.
         *
         * \return a boolean indicating success of acquisition
         *              TRUE - resources acquired without error
         *              FALSE - resources were not acquired
         */
        virtual bool acquire(void);

        /**
         * \brief Release the texture object's OpenGL resources.
         */
        virtual void release(void);

    private :
        // These are merely set during construction, though utilised during acquisition.
        /// Whether or not mipmaps are to be generated.
        bool m_mipmap;

        /// What the texture data represents in terms of rendering intent.
        GLint m_format;

        /// The width of the texture.
        GLsizei m_width;

        /// The height of the texture.
        GLsizei m_height;

        /// The binary representation of the data.
        GLenum m_data_type;

        /// A pointer to the data.
        const GLvoid* m_data;

        /**
         * \brief Write data to the texture with the characteristics given at construction.
         */
        void loadTexture(void);
    };

/**
 * @}
 */

#endif /*TEXTURE_H_*/
