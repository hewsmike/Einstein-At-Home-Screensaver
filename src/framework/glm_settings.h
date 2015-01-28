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

// The optional defines as subsequently listed must be mentioned PRIOR to the inclusion
// of ANY glm include file.

// Enabling swizzle operators will massively increase the size of compiled files and the
// compilation time. Swizzle is disabled by default.
// #define GLM_SWIZZLE

// Available defines for floating point types (glm::vec*, glm::mat*).
// High precision is the default :
// #define GLM_PRECISION_LOWP_FLOAT
// #define GLM_PRECISION_MEDIUMP_FLOAT
// #define GLM_PRECISION_HIGHP_FLOAT

// Available defines for double floating point types (glm::dvec* , glm::dmat*).
// High precision is the default :
// #define GLM_PRECISION_LOWP_DOUBLE
// #define GLM_PRECISION_MEDIUMP_DOUBLE
// #define GLM_PRECISION_HIGHP_DOUBLE

// Available defines for signed integer types (glm::ivec*).
// High precision is the default :
// #define GLM_PRECISION_LOWP_INT
// #define GLM_PRECISION_MEDIUMP_INT
// #define GLM_PRECISION_HIGHP_INT

// Available defines for unsigned integer types (glm::uvec*).
// High precision is the default :
// #define GLM_PRECISION_LOWP_UINT
// #define GLM_PRECISION_MEDIUMP_UINT
// #define GLM_PRECISION_HIGHP_UINT

// This gives the default GLSL 4.10 precision.
#define GLM_PRECISION_MEDIUMP_INT
#define GLM_PRECISION_HIGHP_FLOAT

// GLM includes a notification system which can display some information at build
// time. The messages are generated only by compiler supporting '#program message'
// and only once per project build. This system is disabled by default.
// #define GLM_MESSAGES

// GLM will automatically take advantage of compilers’ language extensions when
// enabled. To increase cross platform compatibility and to avoid compiler extensions,
// a programmer can define GLM_FORCE_CXX98 to restrict the language feature set C++98:
#define GLM_FORCE_CXX98
// GLM_FORCE_CXX11 overrides GLM_FORCE_CXX98
// #define GLM_FORCE_CXX11
// GLM_FORCE_CXX14 overrides GLM_FORCE_CXX11 and GLM_FORCE_CXX98
// #define GLM_FORCE_CXX14

// GLM provides some SIMD optimizations based on compiler intrinsics. These
// optimizations will be automatically utilized based on the compiler arguments.
// GLM provides specialized vec4 and mat4 through two extensions,
// #define GLM_GTX_simd_vec4
// #define GLM_GTX_simd_mat4
// A programmer can restrict or force instruction sets used by GLM using the following :
// #define GLM_FORCE_SSE2
// #define GLM_FORCE_SSE3
// #define GLM_FORCE_SSE4
// #define GLM_FORCE_AVX
// #define GLM_FORCE_AVX2
// A programmer can discard the use of intrinsics by defining GLM_FORCE_PURE.
// If GLM_FORCE_PURE is defined, then including a SIMD extension will generate a build error.
// #define GLM_FORCE_PURE

// To push further the software performance, a programmer can define GLM_FORCE_INLINE to
// force the compiler to inline GLM code.
// #define GLM_FORCE_INLINE

// Vector and matrix static size ( see http://glm.g-truc.net/glm.pdf for complete discussion )
// #define GLM_FORCE_SIZE_T_LENGTH
// #define GLM_FORCE_SIZE_FUNC

// By default and following GLSL specifications, vector and matrix default constructors
// initialize the components to zero. This is a reliable behavior but initialization has a
// cost and it’s not always necessary. This behavior can be disabled at compilation time.
// #define GLM_FORCE_NO_CTOR_INIT

// GLSL supports implicit conversions of vector and matrix types. For example, an ivec4
// can be implicitly converted into vec4. Often, this behaviour is not desirable but
// following the spirit of the library, this behavior is supported in GLM. However,
// GLM 0.9.6 introduced a define to require explicit conversion for GLM types.
// #define GLM_FORCE_EXPLICIT_CTOR

// Core glm header, which rounds up all of the following :
// <glm/vec2.hpp>: vec2, bvec2, dvec2, ivec2 and uvec2
// <glm/vec3.hpp>: vec3, bvec3, dvec3, ivec3 and uvec3
// <glm/vec4.hpp>: vec4, bvec4, dvec4, ivec4 and uvec4
// <glm/mat2x2.hpp>: mat2, dmat2
// <glm/mat2x3.hpp>: mat2x3, dmat2x3
// <glm/mat2x4.hpp>: mat2x4, dmat2x4
// <glm/mat3x2.hpp>: mat3x2, dmat3x2
// <glm/mat3x3.hpp>: mat3, dmat3
// <glm/mat3x4.hpp>: mat3x4, dmat2
// <glm/mat4x2.hpp>: mat4x2, dmat4x2
// <glm/mat4x3.hpp>: mat4x3, dmat4x3
// <glm/mat4x4.hpp>: mat4, dmat4
// <glm/common.hpp>: all the GLSL common functions
// <glm/exponential.hpp>: all the GLSL exponential functions
// <glm/geometry.hpp>: all the GLSL geometry functions
// <glm/integer.hpp>: all the GLSL integer functions
// <glm/matrix.hpp>: all the GLSL matrix functions
// <glm/packing.hpp>: all the GLSL packing functions
// <glm/trigonometric.hpp>: all the GLSL trigonometric functions
// <glm/vector_relational.hpp>: all the GLSL vector relational functions
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
