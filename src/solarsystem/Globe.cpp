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

#include "Globe.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "ErrorHandler.h"

// In 3D space there are three position coordinate values per vertex.
const GLuint Globe::POS_COORDS_PER_VERTEX(3);

// In 3D space there are three coordinate values per normal vector, per vertex.
const GLuint Globe::NORM_COORDS_PER_VERTEX(3);

// For 2D texturing there are two coordinate values per vertex.
const GLuint Globe::TEXTURE_COORDS_PER_VERTEX(2);

// Thus each vertex has the sum of the above components to specify.
const GLuint Globe::ELEMENTS_PER_VERTEX(POS_COORDS_PER_VERTEX +
                                        NORM_COORDS_PER_VERTEX +
                                        TEXTURE_COORDS_PER_VERTEX);

// Vertex data begins from the first byte into the array.
const GLuint Globe::ARRAY_START(0);

// No padding between vertex data entries
const GLsizei Globe::ARRAY_STRIDE(0);

// Don't ever change these !!
const bool Globe::STAGGERING(true);
const bool Globe::STITCHING(true);

Globe::Globe(std::string name,
             std::string resource_name,
             GLfloat radius,
             GLuint stacks,
             GLuint slices,
             GLfloat zero_longitude_offset) :
                 nm(name),
                 image_resource_name(resource_name),
                 zlo(zero_longitude_offset),
                 sp(radius, slices, stacks, STAGGERING, STITCHING),
                 verts_per_lat(slices + 1),
                 num_slices(slices),
                 num_stacks(stacks) {
    }

Globe::~Globe() {
    release();
    }

void Globe::loadTexture(void) {
    // Get an OpenGL texture object.
    texture.acquire();

    // Make our texture object OpenGL's current one.
    glBindTexture(GL_TEXTURE_2D, texture.ID());

    ResourceFactory factory;
    // Create texture resource instance.
    const Resource* textureResource = factory.createInstance(image_resource_name.c_str());

    // We're gonna let the GLFW do the hard work of mipmap production.
    // This implicitly operates on the GL_TEXTURE_2D target.
    int load_success = GL_FALSE;
    if(textureResource != NULL) {
        load_success = glfwLoadMemoryTexture2D(&(textureResource->data()->front()),
                                               textureResource->data()->size(),
                                               GLFW_BUILD_MIPMAPS_BIT);
        }
    else {
        std::stringstream msg;
        msg << "Globe::loadTexture() - texture resource NOT available : ";
        msg << image_resource_name;
        ErrorHandler::record(msg.str(), ErrorHandler::WARN);
        }

    // Did that work?
    if(load_success == GL_TRUE) {
        // Set the texture's stretching properties
        // for minification and magnification.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Do we repeat the map? Yes, but only in the 'horizontal' direction,
        // meaning that texture coordinates outside of [0.0f, 1.0f] will
        // imply rendering continuing onto the 'other' side of the pixel map.
        // This will give east/west continuity on the globe, if needed, but
        // we don't need north pole to south pole continuity do we ??
        // Don't you just luv topology ?? :-)
        // 'S' is the 'horizontal' texture coordinate direction.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // 'T' is the 'vertical' texture coordinate direction.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        // How it maps when texels and fragments/pixels areas don't match
        // when we do minification and magnification.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // You want to paste the image on, with no show-through of what's
        // beneath.
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        // Bless the texture as most important.
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0f);
        }
    else {
        // Nope, the loading into a texture object failed. This is not fatal,
        // as with later rendering OpenGL will simply use the 'default' texture
        // ie. nothing. The only visual result will be to see whatever
        // background color(s) have been assigned ( or not ! ) to the polygon(s)
        // in question.
        ErrorHandler::record("Globe::loadTexture() - texture object NOT loaded ",
                             ErrorHandler::WARN);
        }

    // Get rid of any heap resource allocation.
    delete textureResource;

    // Unbind the texture from the state machine - but don't delete it!
    glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);
    }

void Globe::prepare(SolarSystemGlobals::render_quality rq) {
    // Preparations may depend upon the requested rendering quality level ??
    switch (rq) {
        case SolarSystemGlobals::RENDER_LOWEST :
        case SolarSystemGlobals::RENDER_MEDIUM :
        case SolarSystemGlobals::RENDER_HIGHEST :
            // Get the vertex data into the server-side buffer.
            loadVertexBuffer();

            // For each pole and in between define and store the indices
            // ( into the above vertex data array ) in correct sequence
            // for later use when rendering.
            loadPolarIndexBuffer(north_cap_indices, NORTH);
            loadWaistIndexBuffer();
            loadPolarIndexBuffer(south_cap_indices, SOUTH);

            // Finally make sure one has a texture ( pixel map ) available
            // to paste onto the surface which approximates a sphere.
            loadTexture();
            break;
        default :
            // Ought not get here !!
            ErrorHandler::record("Globe::prepare() - bad switch case reached (default)",
                                 ErrorHandler::FATAL);
            break;
        }
    }

void Globe::release(void) {
    // Free up the various buffer and texture objects on the server side.
    north_cap_indices.release();
    waist_indices.release();
    south_cap_indices.release();
    vertex_buffer.release();
    texture.release();
    }

void Globe::render(void) {
    // Enable the texture, normal and vertex arrays as being active
    // for this bout of rendering.
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Enable the use of 2D textures.
    glEnable(GL_TEXTURE_2D);

    // Only want to look at and/or render the outside of the Globe,
    // for the sake of efficiency.
    glCullFace(GL_BACK);

    // Make our texture identifier OpenGL's current one.
    glBindTexture(GL_TEXTURE_2D, texture.ID());

    // Make our vertex buffer identifier OpenGL's current one.
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.ID());

    // The untextured polygonal ( triangles ) color will be opaque and white.
    // You'd only see that if texture application failed.
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // If all goes well we won't see the lines at the edge of the
    // polygons, but we ought define something.
    glLineWidth(0.1f);

    // This announces that the pattern of data storage, per vertex, is
    // 2 texture, 3 normal, and 3 position coordinates in that order. They
    // are all of the same floating point data type.
    glInterleavedArrays(GL_T2F_N3F_V3F, ARRAY_STRIDE, ARRAY_START);

    // Draw north polar region.
    // Make our northern index buffer identifier OpenGL's current one.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, north_cap_indices.ID());

    // This is one actual rendering call that all preparations have been aiming
    // at.
    glDrawElements(GL_QUAD_STRIP,
                   verts_per_lat*2,
                   GL_UNSIGNED_INT,
                   BUFFER_OFFSET(ARRAY_START));

    // Make the waist index buffer identifier OpenGL's current one.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waist_indices.ID());

    for(GLuint stack = 1; stack < num_stacks - 2; ++stack) {
        // Herewith the number of bytes into the index buffer
        // to begin this strip for this stack.
        GLuint strip_index_start = ARRAY_START +        // Start at the beginning
                                   sizeof(GLuint) *     // size of base type
                                   (stack - 1) *        // number of non-polar stacks so far
                                   verts_per_lat *      // indices per non-polar stack
                                   2;                   // this stack and the next

        // Actual rendering call for this stack.
        glDrawElements(GL_TRIANGLE_STRIP,
                       verts_per_lat * 2,
                       GL_UNSIGNED_INT,
                       BUFFER_OFFSET(strip_index_start));
        }

    // Draw south polar region.
    // Make our southern index buffer identifier OpenGL's current one.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, south_cap_indices.ID());

    // Another actual rendering call that all preparations have been aiming
    // at.
    glDrawElements(GL_QUAD_STRIP,
                   verts_per_lat*2,
                   GL_UNSIGNED_INT,
                   BUFFER_OFFSET(ARRAY_START));

    // Unbind the buffers and the texture.
    glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
    glBindTexture(GL_TEXTURE_2D, Texture_OBJ::NO_ID);

    // Disable the use of 2D textures.
    glDisable(GL_TEXTURE_2D);

    // Disable the texture, normal and vertex arrays.
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

void Globe::loadVertexBuffer(void) {
    // Get an OpenGL buffer object.
    vertex_buffer.acquire();

    // What size allocation are we after? There are two pole vertices,
    // but for proper texture rendering each pole point has a number
    // of ( near ) duplicates only differing in longitudal/horizontal
    // texture coordinate values.
    GLuint num_buffer_vertices = 2*(verts_per_lat);

    // plus the number of vertices at each stack of non-polar latitude.
    num_buffer_vertices += sp.vertices().size() - 2;

    // Calculate & keep index of the last vertex.
    last_vertex_index = num_buffer_vertices - 1;

    // Total size of buffer, in bytes, containing all of the required vertices.
    GLsizeiptr buffer_size = sizeof(Vert) * (num_buffer_vertices);

    // Allocate temporary heap buffer of correct size.
    Vert* buffer_base_ptr = new Vert[num_buffer_vertices];
    Vert* buffer_ptr = buffer_base_ptr;

     // What is the step in the longitudinal texture co-ordinate?
    vec_t polar_s_texture_step = 1.0f/num_slices;

    // Create new northern polar vertices. Get the north polar
    // vertex as constructed by the Sphere class instance.
    Vertex north = sp.vertices().at(0);
    Vector3D north_norm = Vector3D(north.normal().x(),
                                   north.normal().y(),
                                   north.normal().z());

    Vector3D north_position = Vector3D(north.position().x(),
                                       north.position().y(),
                                       north.position().z());

    std::pair<vec_t, vec_t> north_texture;
    north_texture.second = north.texture_co_ords().second;

    // Step along in longitude across the polar region, including
    // a 'stitch' vertex at the end.
    for(GLuint slice = 0; slice < verts_per_lat; ++slice) {
        // Set the vertex's longitudinal texture co-ordinate as
        // appropriate for a step in longitude. Keep all other
        // vertex attributes as per the north pole ( these are
        // the afore-mentioned near duplicates ).
        north_texture.first = polar_s_texture_step*slice;

        Vertex current = Vertex(north_position, north_norm, north_texture);

        // Transfer the vertex data to the buffer.
        vertex2buffer(current, buffer_ptr);
        // Update the buffer pointer.
        ++buffer_ptr;
        }

    // Intermediate vertices, directly copied from
    // non-polar vertices constructed by Sphere class instance.
    for(std::vector<Vertex>::const_iterator vt = sp.vertices().begin() + 1;
        vt != sp.vertices().end() - 1;
        ++vt) {
        // Transfer the vertex data to the buffer.
        vertex2buffer(*vt, buffer_ptr);
        // Update the buffer pointer.
        ++buffer_ptr;
        }

    // Create new southern polar vertices. Get the south polar
    // vertex as constructed by the Sphere class instance.
    Vertex south = sp.vertices().at(sp.vertices().size() - 1);
    Vector3D south_norm = Vector3D(south.normal().x(),
                                   south.normal().y(),
                                   south.normal().z());

    Vector3D south_position = Vector3D(south.position().x(),
                                       south.position().y(),
                                       south.position().z());

    std::pair<vec_t, vec_t> south_texture;
    south_texture.second = south.texture_co_ords().second;

    // Step along in longitude across the polar region, including
    // a 'stitch' vertex at the end.
    for(GLuint slice = 0; slice < verts_per_lat; ++slice) {
        // Set the vertex's longitudinal texture co-ordinate as
        // appropriate for a step in longitude. Keep all other
        // vertex attributes as per the south pole ( these are
        // the afore-mentioned near duplicates ).
        south_texture.first = polar_s_texture_step*slice;

        Vertex current = Vertex(south_position, south_norm, south_texture);

        // Transfer the vertex data to the buffer.
        vertex2buffer(current, buffer_ptr);
        // Update the buffer pointer.
        ++buffer_ptr;
        }

    // Now load the server side buffer with our heap contents.
    vertex_buffer.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
                             buffer_size, buffer_base_ptr);
    // Delete the temporary heap Vertex array.
    delete[] buffer_base_ptr;
    }

void Globe::loadWaistIndexBuffer(void) {
    // Get a valid buffer object ( server-side ) identifier.
    waist_indices.acquire();

    // What size byte allocation are we after for this array of indices? What
    // is the vertex count?
    GLuint num_waist_indices = verts_per_lat *      // number of vertices per latitude band
                               (num_stacks - 3) *   // all non-polar stacks bar one
                               2;                   // this stack and the next

    // Size of base data type times the total number of vertices.
    GLsizeiptr waist_size = sizeof(GLuint) *
                            num_waist_indices;

    GLuint* buffer_base_ptr = new GLuint[num_waist_indices];
    GLuint* buffer_ptr = buffer_base_ptr;

    // Each set of vertices are to be suitably listed for use within
    // a GL_TRIANGLE_STRIP.
    for(GLuint stack = 1; stack < num_stacks - 2; ++stack) {
        for(GLuint slice = 0; slice < verts_per_lat; ++slice) {
            // Interleave this vertex with ...
            *buffer_ptr = stack*verts_per_lat + slice;
            ++buffer_ptr;
            // ... the corresponding vertex on the next stack.
            *buffer_ptr = (stack + 1)*verts_per_lat + slice;
            ++buffer_ptr;
            }
        }

    waist_indices.loadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
                             waist_size, buffer_base_ptr);
    delete[] buffer_base_ptr;
    }

void Globe::loadPolarIndexBuffer(Buffer_OBJ& polar_buffer, enum pole po) {
    // Get a valid buffer object ( server-side ) identifier.
    polar_buffer.acquire();

    // What size byte allocation are we after for this array of indices? For
    // each vertex we have sizeof(GLuint) worth. What is the vertex count ?
    //      - two vertices per longitude value including stitching line.
    GLuint num_polar_indices = 2*verts_per_lat;

    // Total size of buffer, in bytes, containing all of the required indices.
    GLsizeiptr polar_size = sizeof(GLuint)*num_polar_indices;

    // Temporary heap buffer of correct size.
    GLuint* buffer_base_ptr = new GLuint[num_polar_indices];
    GLuint* buffer_ptr = buffer_base_ptr;

    // Index of point on sphere which begins a sequence of points for later use
    // within a GL_QUAD_STRIP pattern. Default starting index is zero for the
    // north pole,.
    GLuint pole_index = 0;
    // Next ( QUAD ) point not on the pole.
    GLuint peri_polar_index = pole_index + verts_per_lat;
    // We increment vertex indices.
    GLuint delta = 1;
    if(po == SOUTH) {
        // The south polar index is however many vertex entries there are for
        // the entire sphere minus one.
        pole_index = last_vertex_index;
        // Next ( QUAD ) point not on the pole.
        peri_polar_index = pole_index - verts_per_lat;
        // We decrement vertex indices.
        delta = -1;
        }

    // The indices of points on sphere at a latitude just one stack nearby the
    // pole, listed in sequence suitable for later use within a GL_QUAD_STRIP
    // pattern. The way this 'winds around' makes the convex side the 'outside'
    // for OpenGL purposes, and the southern cap is necessarily of the opposite
    // sense to the northern cap.
    for(GLuint i = 0; i < verts_per_lat; ++i) {
        // Polar points, differing only in 'horizontal' texture value.
        *buffer_ptr = pole_index + delta*i;
        ++buffer_ptr;

        // The peri-polar band.
        *buffer_ptr = peri_polar_index + delta*i;
        ++buffer_ptr;
        }

    // Now load the server side buffer with our heap contents.
    polar_buffer.loadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
                            polar_size, buffer_base_ptr);
    // Delete the temporary heap indicial array.
    delete[] buffer_base_ptr;
    }

void Globe::vertex2buffer(const Vertex& vert, Vert* buffer) const {
    // Copy to a buffer in OpenGL GL_T2F_N3F_V3F interleaved array pattern.
    // Texture co-ordinates are adjusted to line the map up to the
    // 'Greenwich meridian'.
    buffer->text.s = vert.texture_co_ords().first - zlo;
    buffer->text.t = vert.texture_co_ords().second;

    // Normal data.
    buffer->norm.x = vert.normal().x();
    buffer->norm.y = vert.normal().y();
    buffer->norm.z = vert.normal().z();

    // Position data.
    buffer->pos.x = vert.position().x();
    buffer->pos.y = vert.position().y();
    buffer->pos.z = vert.position().z();
    }
