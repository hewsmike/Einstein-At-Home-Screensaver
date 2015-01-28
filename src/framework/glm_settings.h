/***************************************************************************
 *   Copyright (C) 2015 by Mike Hewson                                     *
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

#ifndef GLM_SETTINGS_H_
#define GLM_SETTINGS_H_

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief Common header to enforce project wide glm library settings.
 *
 * \see framework.h
 * \see ogl_utility.h
 *
 * \author Mike Hewson\n
 */

// Enable forward declaration handling for the glm library headers ( compile time efficiency ).
// GLM is a header only library that makes a heavy usage of C++ templates. This design
// may significantly increase the compile time for files that use GLM.
#include "fwd.hpp"

// Primary glm header
#include "glm.hpp"

// Matrix transforms
#include "matrix_transform.hpp"

// Handle the interaction between pointers and vector, matrix types.
// This extension defines an overloaded function, glm::value_ptr , which takes any of the
// core template types ( vec3 , mat4 , etc.). It returns a pointer to the memory layout of
// the object. Matrix types store their values in column-major order.
#include "type_ptr.hpp"

/**
 * @}
 */

#endif /*FRAMEWORK_H_*/
