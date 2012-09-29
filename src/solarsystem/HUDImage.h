/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#ifndef HUD_IMAGE_H_
#define HUD_IMAGE_H_

#include "framework.h"

#include <string>

#include "Buffer_OBJ.h"
#include "HUDContainer.h"
#include "HUDContent.h"
#include "Texture_OBJ.h"
#include "Vector3D.h"
#include "Vertex.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapslates an image to display upon the HUD.
 *
 * \see HUDContent
 * \see Vector3D
 * \see Vertex
 *
 * \author Mike Hewson\n
 */

class HUDImage : public HUDContent {
    public:
        /**
         * \brief Constructor
         *
         * \param resourceName : the name of the image resource
         *
         * \param horizontalMargin : the horizontal pixel margin to be applied
         *                           either side of the enclosed content
         * \param verticalMargin : the vertical pixel margin to be applied
         *                         above and below the enclosed content
         */
        HUDImage(std::string resourceName, GLuint horizontalMargin,
                 GLuint verticalMargin);

        /**
         * \brief Destructor
         */
        virtual ~HUDImage();

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// The number of position coordinates per OpenGL vertex.
        static const GLuint POSITION_COORDS_PER_VERTEX;

        /// The number of texture coordinates per OpenGL vertex.
        static const GLuint TEXTURE_COORDS_PER_VERTEX;

        /// The total number of coordinates per OpenGL vertex.
        static const GLuint ELEMENTS_PER_VERTEX;

        /// Offset of the vertex data into the server-side buffer
        static const GLuint ARRAY_START;

        /// The number of bytes in the server-side buffer between each vertex
        static const GLsizei ARRAY_STRIDE;

        /**
         * \brief Load a pixel map into a server-side texture buffer.
         */
        void loadTexture(void);

        /**
         * \brief Create vertex and texture coordinate data for the rectangular
         *        area to be textured upon.
         */
        void createVertexData(void);

        /**
         * \brief Load vertex data into a server-side buffer.
         */
        void loadVertexBuffer(void);

        /**
         * \brief Load a single vertex's data into a server-side buffer
         *
         * \param vert : the vertex
         *
         * \param buffer : the buffer
         */
        void vertex2buffer(const Vertex& vert, vec_t* buffer) const;

        /// Listing of vertices.
        std::vector<Vertex> verts;

        /// An OpenGL texture object identifier
        Texture_OBJ texture;

        /// An OpenGL buffer object ( in server-side memory ) holding
        /// the vertex associated data for a quad.
        Buffer_OBJ buff_obj_points;

        /// Name to resource to load image from.
        std::string image_resource_name;

        /// Dimensions of loaded image.
        GLuint image_width;
        GLuint image_height;
    };

/**
 * @}
 */

#endif /* HUD_IMAGE_H_ */
