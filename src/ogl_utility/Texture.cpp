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

#include "Texture.h"

#include <iostream>

#include "ErrorHandler.h"

Texture::Texture(bool mipmap,
                 GLint format,
                 GLsizei width,
                 GLsizei height,
                 GLenum data_type,
                 const GLvoid* data) :
                    m_mipmap(mipmap),
                    m_format(format),
                    m_width(width),
                    m_height(height),
                    m_data_type(data_type),
                    m_data(data) {
    }

Texture::~Texture() {
    // Must call this here in this derived class.
    Texture::release();
    }

bool Texture::acquire(void) {
    // Assume failure to acquire.
    bool ret_val = false;

    // Ask OpenGL to assign a texture object.
    GLuint temp;
    glGenTextures(1, &temp);
    set_ID(temp);

    // Failure to acquire a handle should be FATAL.
    if(this->ID() == OGL_ID::NO_ID) {
        ErrorHandler::record("Texture::acquire() : failure to obtain identifier",
                             ErrorHandler::FATAL);
        }
    else {
        // Use the handle and load the texture data.
        loadTexture();
        ret_val = true;
        }

    return ret_val;
    }

void Texture::release(void) {
    // Ask OpenGL to release the texture object.
    GLuint temp = this->ID();
    glDeleteTextures(1, &temp);
    // Set our handle store to safe value.
    set_ID(OGL_ID::NO_ID);
    }

void Texture::loadTexture(void) {
    // Bind the texture ( of GL_TEXTURE_2D type ) to our identifier.
    glBindTexture(GL_TEXTURE_2D, this->ID());

    glTexImage2D(GL_TEXTURE_2D,
                 0,                      // Mipmap level zero.
                 m_format,               // Rendering intent.
                 m_width,                // image width in texels.
                 m_height,               // image height in texels.
                 0,                      // No image border.
                 m_format,               // Rendering intent.
                 m_data_type,            // Binary data represetnation.
                 m_data);                // The actual data.

    // Specify 'reasonable' values for simple 'decal' like application.

    // 'S' is the 'horizontal' texture coordinate direction. GL_REPEAT
    // chosen to cope with globes ie. the longitude = 0 modulus.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 'T' is the 'vertical' texture coordinate direction.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // How it maps when texels and fragments/pixels areas don't match
    // when we do minification and magnification. That partly depends upon
    // whether we are mipmapping. GL_NEAREST ( Manhattan distance ) chosen
    // as faster than GL_LINEAR ( Pythagorus to hypotenuse ).
    /// TODO - make these choices dependent upon rendering quality selection.
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

    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, OGL_ID::NO_ID);
    }
