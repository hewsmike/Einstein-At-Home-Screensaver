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

const Glsizei TextureBuffer::MIN_TEX_WIDTH(64);
const Glsizei TextureBuffer::MIN_TEX_HEIGHT(64);

TextureBuffer::TextureBuffer(const GLvoid* texture_data,
                             GLsizei width,
                             GLsizei height,
                             GLint usage,
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
    if((usage == GL_ALPHA) ||
       (usage == GL_LUMINANCE) ||
       (usage == GL_LUMINANCE_ALPHA) ||
       (usage == GL_RGB) ||
       (usage == GL_RGBA)) {
        m_usage = usage;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Bad usage type provided.",
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

GLuint TextureBuffer::acquire_ID(GLuint* handle) const {
    glGenTextures(1, handle);
    }

GLuint TextureBuffer::release_ID(GLuint* handle) const {
    glDeleteTextures(1, handle);
    }

void TextureBuffer::loadBuffer(void) {
    // Bind the texture ( of GL_TEXTURE_2D type ) to our identifier.
    glBindTexture(GL_TEXTURE_2D, this->ID());

    glTexImage2D(GL_TEXTURE_2D,
                 0,                      // Mipmap level zero.
                 m_usage,                // Rendering intent.
                 m_width,                // image width in texels.
                 m_height,               // image height in texels.
                 0,                      // No image border.
                 m_usage,                // Rendering intent.
                 m_data_type,            // Binary data represetnation.
                 this->data());          // The actual data.

    // Specify 'reasonable' values for simple 'decal' like application.

    /// TODO - should we parameterise through the interface here ?
    // 'S' is the 'horizontal' texture coordinate direction. GL_REPEAT
    // chosen to cope with globes ie. the longitude = 0 modulus.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 'T' is the 'vertical' texture coordinate direction.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // How it maps when texels and fragments/pixels areas don't match
    // when we do minification and magnification. That partly depends upon
    // whether we are mipmapping. GL_NEAREST ( Manhattan distance ) chosen
    // as faster than GL_LINEAR ( Pythagorus to hypotenuse ).
    /// TODO - make these choices dependent upon rendering quality selection ?
    if(m_mipmap == true) {
        // With mipmapping intended.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
    else {
        // Without mipmaps involved.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }

    // Make mipmaps if requested.
    if(m_mipmap == true) {
        glGenerateMipmap(GL_TEXTURE_2D);
        }

    /// TODO - Use glHint here to specify quality ?
    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, OGL_ID::NO_ID);
    }
