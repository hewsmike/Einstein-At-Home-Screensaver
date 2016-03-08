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

#include "TextureBuffer.h"

#include "ErrorHandler.h"

#include <iostream>
#include <sstream>

const GLuint TextureBuffer::DEFAULT_MIPMAP_BASE_LEVEL(0);
const GLuint TextureBuffer::DEFAULT_IMAGE_BORDER_WIDTH(0);
const GLsizei TextureBuffer::MIN_TEX_WIDTH(2);
const GLsizei TextureBuffer::MIN_TEX_HEIGHT(2);

TextureBuffer::TextureBuffer(const GLvoid* texture_data,
                             GLuint bytes,
                             GLsizei width,
                             GLsizei height,
                             GLenum format,
                             GLenum data_type,
                             GLenum wrap_type_s,
                             GLenum wrap_type_t,
                             bool mipmaps) :
                                Buffer(texture_data, bytes),
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

    // Ensure compliance with acceptable parameter types for color format.
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

    // Ensure compliance with acceptable parameter types for data binary representation.
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

    // Ensure compliance with acceptable parameter types for horizontal texture coordinate wrapping.
    if((wrap_type_s == GL_REPEAT) ||
       (wrap_type_s == GL_CLAMP) ||
       (wrap_type_s == GL_CLAMP_TO_EDGE) ||
       (wrap_type_s == GL_CLAMP_TO_BORDER)) {
        m_wrap_type_s = wrap_type_s;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Bad horizontal wrap type provided.",
                             ErrorHandler::FATAL);
        }

    // Ensure compliance with acceptable parameter types for vertical texture coordinate wrapping.
    if((wrap_type_t == GL_REPEAT) ||
       (wrap_type_t == GL_CLAMP) ||
       (wrap_type_t == GL_CLAMP_TO_EDGE) ||
       (wrap_type_t == GL_CLAMP_TO_BORDER)) {
        m_wrap_type_t = wrap_type_t;
        }
    else {
        ErrorHandler::record("TextureBuffer::TextureBuffer() : Bad vertical wrap type provided.",
                             ErrorHandler::FATAL);
        }
    }

TextureBuffer::~TextureBuffer() {
    TextureBuffer::release();
    }

bool TextureBuffer::acquire(void) {
    GLuint handle;
    glGenTextures(1, &handle);
    this->set_ID(handle);
    this->setAcquisitionState(true);
    return true;
    }

void TextureBuffer::release(void) {
    GLuint handle(this->ID());
    glDeleteTextures(1, &handle);
    this->reset_ID();
    this->setAcquisitionState(false);
    }

void TextureBuffer::bind(void) {
    glBindTexture(GL_TEXTURE_2D, this->ID());
    }

void TextureBuffer::unbind(void) {
    glBindTexture(GL_TEXTURE_2D, OGL_ID::NO_ID);
    }

bool TextureBuffer::isBound(void) const {
    // Assume failure.
    bool ret_val = false;

    // Discover which 2D texture array buffer, if any, is bound to the OpenGL state.
    GLuint temp;

    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &temp);

    if((this->ID() == temp) && (this->ID() != OGL_ID::NO_ID)) {
        ret_val = true;
        }

    return ret_val;
    }

void TextureBuffer::loadBuffer(void) {
    this->bind();

    // The 'm_format' parameter appears twice in the following parameter
    // list as we don't change/convert for this application.
    glTexImage2D(GL_TEXTURE_2D,
                 DEFAULT_MIPMAP_BASE_LEVEL,     // Mipmap level zero.
                 m_format,                      // Storage format ( to be stored in state machine ) .
                 m_width,                       // image width in texels.
                 m_height,                      // image height in texels.
                 DEFAULT_IMAGE_BORDER_WIDTH,    // Usually no image border.
                 m_format,                      // Storage format ( from data source ).
                 m_data_type,                   // Binary data representation.
                 this->data());                 // The actual data.

    // Specify 'reasonable' values for simple 'decal' like application.

    // 'S' is the 'horizontal' texture coordinate direction.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_type_s);

    // 'T' is the 'vertical' texture coordinate direction.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_type_t);

    // How it maps when texels and fragments/pixels areas don't match
    // when we do minification and magnification. That partly depends upon
    // whether we are mipmapping. GL_NEAREST ( Manhattan distance ) chosen
    // as faster than GL_LINEAR ( Pythagorus to hypotenuse ).
    /// TODO - make these choices dependent upon rendering quality selection ?
    if(m_mipmaps == true) {
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
    if(m_mipmaps == true) {
        glGenerateMipmap(GL_TEXTURE_2D);
        }

    /// TODO - Use glHint here to specify quality ?
    unbind();
    }
