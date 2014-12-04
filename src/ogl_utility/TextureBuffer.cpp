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

#include "TextureBuffer.h"

#include "ErrorHandler.h"

const GLsizei TextureBuffer::MIN_TEX_WIDTH(2);
const GLsizei TextureBuffer::MIN_TEX_HEIGHT(2);

TextureBuffer::TextureBuffer(const GLvoid* texture_data,
                             GLsizei width,
                             GLsizei height,
                             GLenum format,
                             GLenum data_type,
                             bool mipmaps) :
                                Buffer(texture_data),
                                m_mipmaps(mipmaps) {
    // Ensure sufficient width.
    if(width >= TextureBuffer::MIN_TEX_WIDTH) {
        m_width = width;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Insufficient texel width.",
                             ErrorHandler::FATAL);
        }

    // Ensure sufficient height.
    if(height >= TextureBuffer::MIN_TEX_HEIGHT) {
        m_height = height;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Insufficient texel height.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((format == GL_ALPHA) ||
       (format == GL_LUMINANCE) ||
       (format == GL_LUMINANCE_ALPHA) ||
       (format == GL_RGB) ||
       (format == GL_RGBA)) {
        m_format = format;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Bad format type provided.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with OpenGL ES 2.x acceptable parameter types.
    if((data_type == GL_UNSIGNED_BYTE) ||
       (data_type == GL_UNSIGNED_SHORT_5_6_5) ||
       (data_type == GL_UNSIGNED_SHORT_4_4_4_4) ||
       (data_type == GL_UNSIGNED_SHORT_5_5_5_1)) {
        m_data_type = data_type;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Bad data type provided.",
                             ErrorHandler::FATAL);
        }
    }

TextureBuffer::~TextureBuffer() {
    Buffer::release();
    }

void TextureBuffer::acquire_ID(GLuint* handle) const {
    glGenTextures(1, handle);
    OGL_DEBUG
    }

void TextureBuffer::release_ID(GLuint* handle) const {
    glDeleteTextures(1, handle);
    OGL_DEBUG
    }

void TextureBuffer::loadBuffer(void) {
    // Bind the texture ( of GL_TEXTURE_2D type ) to our identifier.
    glBindTexture(GL_TEXTURE_2D, this->ID());
    OGL_DEBUG

    glTexImage2D(GL_TEXTURE_2D,
                 0,                      // Mipmap level zero.
                 m_format,               // Storage format.
                 m_width,                // image width in texels.
                 m_height,               // image height in texels.
                 0,                      // No image border.
                 m_format,               // Storage format.
                 m_data_type,            // Binary data representation.
                 this->data());          // The actual data.
    OGL_DEBUG

    // Specify 'reasonable' values for simple 'decal' like application.

    /// TODO - should we parameterise through the interface here ?
    // 'S' is the 'horizontal' texture coordinate direction. GL_REPEAT
    // chosen to cope with globes ie. the longitude = 0 modulus.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    OGL_DEBUG
    // 'T' is the 'vertical' texture coordinate direction.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    OGL_DEBUG

    // How it maps when texels and fragments/pixels areas don't match
    // when we do minification and magnification. That partly depends upon
    // whether we are mipmapping. GL_NEAREST ( Manhattan distance ) chosen
    // as faster than GL_LINEAR ( Pythagorus to hypotenuse ).
    /// TODO - make these choices dependent upon rendering quality selection ?
    if(m_mipmaps == true) {
        // With mipmapping intended.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        OGL_DEBUG
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        OGL_DEBUG
        }
    else {
        // Without mipmaps involved.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        OGL_DEBUG
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        OGL_DEBUG
        }

    // Make mipmaps if requested.
    if(m_mipmaps == true) {
        glGenerateMipmap(GL_TEXTURE_2D);
        OGL_DEBUG
        }

    /// TODO - Use glHint here to specify quality ?
    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, OGL_ID::NO_ID);
    OGL_DEBUG
    }
