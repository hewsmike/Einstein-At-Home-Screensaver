/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

#include "HUDImage.h"

#include <sstream>

#include "ErrorHandler.h"
#include "ResourceFactory.h"

// In 3D space there are three position coordinate values per vertex.
const GLuint HUDImage::POSITION_COORDS_PER_VERTEX(3);

// For 2D texturing there are two coordinate values per vertex.
const GLuint HUDImage::TEXTURE_COORDS_PER_VERTEX(2);

// Thus each vertex has the sum of the above components to specify.
const GLuint HUDImage::ELEMENTS_PER_VERTEX(POSITION_COORDS_PER_VERTEX +
                                           TEXTURE_COORDS_PER_VERTEX);

// Vertex data begins from the first byte into the array.
const GLuint HUDImage::ARRAY_START(0);

// No padding between vertex data entries
const GLsizei HUDImage::ARRAY_STRIDE(0);

// Call base class constructor to set dimensions.
HUDImage::HUDImage(std::string resourceName,
                   GLuint horizontalMargin,
                   GLuint verticalMargin) :
                        HUDContent(horizontalMargin, verticalMargin),
                        image_resource_name(resourceName),
                        image_width(0),
                        image_height(0) {
    // Call base class to set minima for the given image content.
    setMinimumDimensions(image_width + 2*horzMargin(),
                         image_height + 2*vertMargin());
    }

HUDImage::~HUDImage() {
    release();
    std::stringstream msg;
        msg << "HUDImage::~HUDImage() - released resources for : "
            << image_resource_name;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);
    }

void HUDImage::prepare(SolarSystemGlobals::render_quality rq) {
    // Policy is to set size when rendering is implied.
    // Ensure one has a texture ( pixel map ) available
    // to paste on the surface.
    loadTexture();

    // Create vertex data knowing the image's position.
    createVertexData();

    // Load vertex data to a server side buffer.
    loadVertexBuffer();

    // Call base class to reset minima for the given image content.
    setMinimumDimensions(image_width + 2*horzMargin(),
                         image_height + 2*vertMargin());

    // Preparation of image implies any enclosing container
    // ought be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer !=NULL) {
        outer->adjust();
        }
    }

void HUDImage::release(void) {
    std::stringstream msg;
        msg << "HUDImage::release() - releasing resources for : "
            << image_resource_name;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);
    // Policy is to set size to zero when rendering is not implied.

    // Discard server side resources.
    texture.release();
    buff_obj_points.release();

    // Call base class to reset minima to no content.
    setMinimumDimensions(0, 0);

    // Release of image implies any enclosing container
    // ought be made aware of size change.
    HUDContainer* outer = getEnclosingContainer();
    if(outer !=NULL) {
        outer->adjust();
        }
    }

void HUDImage::render(void) {
    // Enable the texture and vertex arrays as being active
    // for this bout of rendering.
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Make our texture identifier OpenGL's current one.
    glBindTexture(GL_TEXTURE_2D, texture.ID());

    // Make our vertex buffer identifier OpenGL's current one.
    glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

    // The untextured polygonal ( triangles ) color will be opaque and white.
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // If all goes well we won't see the lines at the edge of the
    // polygons, but we have to define something.
    glLineWidth(0.1f);

    // This announces that the pattern of data storage, per vertex, is
    // 2 texture and 3 position coordinates in that order. They
    // are all of the same floating point data type.
    glInterleavedArrays(GL_T2F_V3F, ARRAY_STRIDE, ARRAY_START);

    glPushMatrix();

    glTranslatef(horzBase(), vertBase(), 0);

    // This is one actual rendering call that all preparations have been aiming at.
    glDrawArrays(GL_QUADS, ARRAY_START, 4);

    glPopMatrix();

    // Unbind the buffers and the texture.
    glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
    glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);

    // Disable the texture and vertex arrays.
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

void HUDImage::loadTexture() {
    // Get an OpenGL texture object.
    texture.acquire();

    // Make our texture object OpenGL's current one.
    glBindTexture(GL_TEXTURE_2D, texture.ID());

    ResourceFactory factory;
    // Create texture resource instance.
    const Resource* textureResource = factory.createInstance(image_resource_name.c_str());

    // This implicitly operates on the GL_TEXTURE_2D target.
    GLFWimage gli;
    int texture_load_success = GL_FALSE;
    if(textureResource != NULL) {
        int resource_load_success = glfwReadMemoryImage(&(textureResource->data()->front()),
                                                        textureResource->data()->size(),
                                                        &gli, 0);
        if(resource_load_success == GL_TRUE) {
            texture_load_success = glfwLoadMemoryTexture2D(&(textureResource->data()->front()),
                                                           textureResource->data()->size(),
                                                           0);
            }
        else {
            std::stringstream msg;
            msg << "HUDImage::loadTexture() - texture resource did NOT load to GLFWimage : ";
            msg << image_resource_name;
            ErrorHandler::record(msg.str(), ErrorHandler::WARN);
            }
        }
    else {
        std::stringstream msg;
        msg << "HUDImage::loadTexture() - texture resource NOT available : ";
        msg << image_resource_name;
        ErrorHandler::record(msg.str(), ErrorHandler::WARN);
        }

    if(texture_load_success == GL_TRUE) {
        // Remember the image dimensions.
        image_width = gli.Width;
        image_height = gli.Height;

        // Make our texture object OpenGL's current one.
        glBindTexture(GL_TEXTURE_2D, texture.ID());

        // Set the texture's stretching properties
        // for minification and magnification.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Do we repeat the map in either direction? No.
        // 'S' is the 'horizontal' texture coordinate direction.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        // 'T' is the 'vertical' texture coordinate direction.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        // How it maps when texels and fragments/pixels areas
        // don't match when we do minification and magnification.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // You want to paste the image on, with no
        // show-through of what's beneath.
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        // Bless the texture as most important.
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0f);
        }
    else {
        std::stringstream msg;
        msg << "HUDImage::loadTexture() - texture memory did NOT load from GLFWimage : ";
        msg << image_resource_name;
        ErrorHandler::record(msg.str(), ErrorHandler::WARN);
        }

    // Get rid of any heap resource allocation.
    delete textureResource;

    // Unbind the texture from the state machine - but don't delete it!
    glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);
    }

void HUDImage::createVertexData(void) {
    // We won't use the 'normal' vector component of a Vertex
    // object so allow the default Vector3D construction value
    // for the second parameter of the Vertex constructor, and
    // then ignore. Attending to margin offsets, make a
    // counterclockwise listing of vertices begining at
    // the lower left corner. This seats the subsequent quad
    // rendering with lower left corner at (0, 0, 0), so one
    // ought transform the desired position prior to render.

    Vertex lower_left = Vertex(Vector3D(horzMargin(), vertMargin(), 0.0f),
                               Vector3D(),
                               std::pair<GLfloat, GLfloat>(0.0f, 0.0f));

    verts.push_back(lower_left);

    Vertex lower_right = Vertex(Vector3D(horzMargin() + image_width, vertMargin(), 0.0f),
                                Vector3D(),
                                std::pair<GLfloat, GLfloat>(1.0f, 0.0f));

    verts.push_back(lower_right);

    Vertex upper_right = Vertex(Vector3D(horzMargin() + image_width, vertMargin() + image_height, 0.0f),
                                Vector3D(),
                                std::pair<GLfloat, GLfloat>(1.0f, 1.0f));

    verts.push_back(upper_right);

    Vertex upper_left = Vertex(Vector3D(horzMargin(), vertMargin() + image_height, 0.0f),
                               Vector3D(),
                               std::pair<GLfloat, GLfloat>(0.0f, 1.0f));

    verts.push_back(upper_left);
    }

void HUDImage::loadVertexBuffer(void) {
    // Get an OpenGL buffer object.
    buff_obj_points.acquire();

    // Make our buffer identifier OpenGL's current one.
    glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

    // What size allocation are we after? There are four vertices,
    // one for each corner of a quadrilateral. Each vertex has
    // position and texture data ( fortunately those have the
    // same type : float = vec_t )
    GLsizeiptr size = sizeof(vec_t) *            // Number of bytes in the base data type
                      ELEMENTS_PER_VERTEX *      // The number of base data types per vertex
                      verts.size();              // How many vertices we have.

    // Allocate buffer memory but don't store, yet.
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    // Get write access to the buffer area.
    vec_t* buffer_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

    // Check for failure, as we don't want to dereference a NULL later on,
    // ... MAKE IT A FATAL ERROR.
    if(buffer_ptr == NULL) {
        std::stringstream msg;
        msg << "HUDImage::loadVertexBuffer() - can't acquire buffer pointer : "
            << image_resource_name;
        ErrorHandler::record(msg.str(), ErrorHandler::WARN);
        }

    for(std::vector<Vertex>::const_iterator vt = verts.begin();
        vt != verts.end();
        ++vt) {
        // Transfer the vertex data to the buffer.
        vertex2buffer(*vt, buffer_ptr);

        // Update the buffer pointer.
        buffer_ptr += ELEMENTS_PER_VERTEX;
        }

    // Disconnect the mapping and the buffer from OpenGL.
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
    }

void HUDImage::vertex2buffer(const Vertex& vert, vec_t* buffer) const {
    // Texture co-ordinates.
    buffer[0] = vert.texture_co_ords().first;
    buffer[1] = vert.texture_co_ords().second;

    // Position data.
    buffer[2] = vert.position().x();
    buffer[3] = vert.position().y();
    buffer[4] = vert.position().z();
    }
