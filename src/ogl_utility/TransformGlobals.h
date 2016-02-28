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

#ifndef TRANSFORM_GLOBALS_H_
#define TRANSFORM_GLOBALS_H_

#include "framework.h"

/**
 * \addtogroup  ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief %OGL_Utility This class provides a common point for setting and
 *        accessing some global variables relevant to transforms.
 *
 *     This static/singleton class was created to prevent pollution of
 * function interfaces with the values of certain widely needed variables :
 *
 *      - the location of the 4x4 float matrix representing the model/view
 * perspective transform. It is a matter of programmer rigour to assure that :
 *          - inadvertent dereferencing does not occur ie. such a location
 *            ought be valid for all usage instances. Only one action of
 *            setting the value is allowed. A fatal error is generated if
 *            accessed before being set.
 *          - have suitable and consistent GLSL typing with respect to
 *            shader code as used.
 *
 *      - the dimensions of the client screen area ie. it's width and height.
 * These may be set to new values as often as desired eg. after screen resizing.
 *
 *      - I include a helper function to precalculate the uniform variable that
 * would be used for proper scaling to the screen client area in Normalised
 * Device Coordinates. This could be useful for orthographic projections.
 *
 * \author Mike Hewson\n
 */

class TransformGlobals {
    public:
        /**
         *  \brief Destructor
         */
        ~TransformGlobals();

        /**
         * \brief Set the location of the camera transform matrix.
         *
         * \param load_point : an untyped address of the matrix in client code.
         *                     Obviously that should be persistent and
         *                     available before first dereference.
         */
        static void setCameraTransformMatrix(glm::mat4* load_point);

        /**
         * \brief Get the location of the camera transform matrix.
         *
         * \return the address of the matrix in client code as stored here.
         *         Obviously that should be set to a valid location prior
         *         to the first dereference. A FATAL error will be generated
         *         if this is called before value setting has occured via
         *         setPerspectiveTransformMatrix().
         */
        static glm::mat4* getCameraTransformMatrix(void);

        /**
		 * \brief Set the location of the orthographic transform matrix.
		 *
		 * \param load_point : an untyped address of the matrix in client code.
		 *                     Obviously that should be persistent and
		 *                     available before first dereference.
		 */
		static void setOrthographicTransformMatrix(glm::mat4* load_point);

		/**
		 * \brief Get the location of the orthographic transform matrix.
		 *
		 * \return the address of the matrix in client code as stored here.
		 *         Obviously that should be set to a valid location prior
		 *         to the first dereference. A FATAL error will be generated
		 *         if this is called before value setting has occured via
		 *         setOrthographicTransformMatrix().
		 */
		static glm::mat4* getOrthographicTransformMatrix(void);

        /**
         * \brief Set the width and the height of the client screen area.
         *
         * \param height : the height of the client screen area in pixels.
         * \param width : the width of the client screen area in pixels.
         */
        static void setClientScreenDimensions(GLuint height, GLuint width);

        /**
         * \brief Get the height of the client screen area.
         *
         * \return the height of the client screen area in pixels.
         */
        static GLuint getClientScreenHeight(void);

        /**
         * \brief Get the width of the client screen area.
         *
         * \return the width of the client screen area in pixels.
         */
        static GLuint getClientScreenWidth(void);

    private:
        /// The 3D perspective transform matrix.
        static glm::mat4* m_transform_matrix_camera;

        /// The 2D orthographic transform matrix.
        static glm::mat4* m_transform_matrix_orthographic;

        /// The current width and height values of the client screen area.
        /// This can be set/reset many times
        static GLuint m_height;
        static GLuint m_width;

        /**
         * \brief Constructor ( private since this a static class )
         */
        TransformGlobals(void);
    };

/**
 * @}
 */

#endif // TRANSFORM_GLOBALS_H_
