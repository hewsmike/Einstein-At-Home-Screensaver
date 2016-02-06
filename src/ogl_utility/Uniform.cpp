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

#include "Uniform.h"

#include <sstream>
#include <string>

#include "ErrorHandler.h"

Uniform::Uniform(const std::string& name, GLvoid* load_point) :
                    m_name(name),
                    m_load_point(load_point) {
	}

Uniform::Uniform(const Uniform& other) {
	m_name = other.m_name;
	m_load_point = other.m_load_point;
	}

Uniform& Uniform::operator=(const Uniform& other) {
	m_name = other.m_name;
	m_load_point = other.m_load_point;
	return *this;
	}

Uniform::~Uniform() {
    }

std::string Uniform::name(void) const {
    return m_name;
    }

GLvoid* Uniform::loadPoint(void) const {
    return m_load_point;
    }

std::string Uniform::type_name(GLenum type) {
    std::string ret_val("");

	// Somewhat over-killed here, but covers all possible OpenGL v3.3 types.
	switch(type) {
		case GL_FLOAT:
			ret_val = "GL_FLOAT";
			break;
		case GL_FLOAT_VEC2:
			ret_val = "GL_FLOAT_VEC2";
			break;
		case GL_FLOAT_VEC3:
			ret_val = "GL_FLOAT_VEC3";
			break;
		case GL_FLOAT_VEC4:
			ret_val = "GL_FLOAT_VEC4";
			break;
		case GL_DOUBLE:
			ret_val = "GL_DOUBLE";
			break;
		case GL_DOUBLE_VEC2:
			ret_val = "GL_DOUBLE_VEC2";
			break;
		case GL_DOUBLE_VEC3:
			ret_val = "GL_DOUBLE_VEC3";
			break;
		case GL_DOUBLE_VEC4:
			ret_val = "GL_DOUBLE_VEC4";
			break;
		case GL_INT:
			ret_val = "GL_INT";
			break;
		case GL_INT_VEC2:
			ret_val = "GL_INT_VEC2";
			break;
		case GL_INT_VEC3:
			ret_val = "GL_INT_VEC3";
			break;
		case GL_INT_VEC4:
			ret_val = "GL_INT_VEC4";
			break;
		case GL_UNSIGNED_INT:
			ret_val = "GL_UNSIGNED_INT";
			break;
		case GL_UNSIGNED_INT_VEC2:
			ret_val = "UNSIGNED_INT_VEC2";
			break;
		case GL_UNSIGNED_INT_VEC3:
			ret_val = "UNSIGNED_INT_VEC3";
			break;
		case GL_UNSIGNED_INT_VEC4:
			ret_val = "UNSIGNED_INT_VEC4";
			break;
		case GL_BOOL:
			ret_val = "GL_BOOL";
			break;
		case GL_BOOL_VEC2:
			ret_val = "GL_BOOL_VEC2";
			break;
		case GL_BOOL_VEC3:
			ret_val = "GL_BOOL_VEC3";
			break;
		case GL_BOOL_VEC4:
			ret_val = "GL_BOOL_VEC4";
			break;
		case GL_FLOAT_MAT2:
			ret_val = "GL_FLOAT_MAT2";
			break;
		case GL_FLOAT_MAT3:
			ret_val = "GL_FLOAT_MAT3";
			break;
		case GL_FLOAT_MAT4:
			ret_val = "GL_FLOAT_MAT4";
			break;
		case GL_FLOAT_MAT2x3:
			ret_val = "GL_FLOAT_MAT2x3";
			break;
		case GL_FLOAT_MAT2x4:
			ret_val = "GL_FLOAT_MAT2x4";
			break;
		case GL_FLOAT_MAT3x2:
			ret_val = "GL_FLOAT_MAT3x2";
			break;
		case GL_FLOAT_MAT3x4:
			ret_val = "GL_FLOAT_MAT3x4";
			break;
		case GL_FLOAT_MAT4x2:
			ret_val = "GL_FLOAT_MAT4x2";
			break;
		case GL_FLOAT_MAT4x3:
			ret_val = "GL_FLOAT_MAT4x3";
			break;
		case GL_DOUBLE_MAT2:
			ret_val = "GL_DOUBLE_MAT2";
			break;
		case GL_DOUBLE_MAT3:
			ret_val = "GL_DOUBLE_MAT3";
			break;
		case GL_DOUBLE_MAT4:
			ret_val = "GL_DOUBLE_MAT4";
			break;
		case GL_DOUBLE_MAT2x3:
			ret_val = "GL_DOUBLE_MAT2x3";
			break;
		case GL_DOUBLE_MAT2x4:
			ret_val = "GL_DOUBLE_MAT2x4";
			break;
		case GL_DOUBLE_MAT3x2:
			ret_val = "GL_DOUBLE_MAT3x2";
			break;
		case GL_DOUBLE_MAT3x4:
			ret_val = "GL_DOUBLE_MAT3x4";
			break;
		case GL_DOUBLE_MAT4x2:
			ret_val = "GL_DOUBLE_MAT4x2";
			break;
		case GL_DOUBLE_MAT4x3:
			ret_val = "GL_DOUBLE_MAT4x3";
			break;
		case GL_SAMPLER_1D:
			ret_val = "GL_SAMPLER_1D";
			break;
		case GL_SAMPLER_2D:
			ret_val = "GL_SAMPLER_2D";
			break;
		case GL_SAMPLER_3D:
			ret_val = "SAMPLER_3D";
			break;
		case GL_SAMPLER_CUBE:
			ret_val = "GL_SAMPLER_CUBE";
			break;
		case GL_SAMPLER_1D_SHADOW:
			ret_val = "GL_SAMPLER_1D_SHADOW";
			break;
		case GL_SAMPLER_2D_SHADOW:
			ret_val = "GL_SAMPLER_2D_SHADOW";
			break;
		case GL_SAMPLER_1D_ARRAY:
			ret_val = "GL_SAMPLER_1D_ARRAY";
			break;
		case GL_SAMPLER_2D_ARRAY:
			ret_val = "GL_SAMPLER_2D_ARRAY";
			break;
		case GL_SAMPLER_1D_ARRAY_SHADOW:
			ret_val = "GL_SAMPLER_1D_ARRAY_SHADOW";
			break;
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			ret_val = "GL_SAMPLER_2D_ARRAY_SHADOW";
			break;
		case GL_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_SAMPLER_CUBE_SHADOW:
			ret_val = "GL_SAMPLER_CUBE_SHADOW";
			break;
		case GL_SAMPLER_BUFFER:
			ret_val = "GL_SAMPLER_BUFFER";
			break;
		case GL_SAMPLER_2D_RECT:
			ret_val = "GL_SAMPLER_2D_RECT";
			break;
		case GL_SAMPLER_2D_RECT_SHADOW:
			ret_val = "GL_SAMPLER_2D_RECT_SHADOW";
			break;
		case GL_INT_SAMPLER_1D:
			ret_val = "GL_INT_SAMPLER_1D";
			break;
		case GL_INT_SAMPLER_2D:
			ret_val = "GL_INT_SAMPLER_2D";
			break;
		case GL_INT_SAMPLER_3D:
			ret_val = "GL_INT_SAMPLER_3D";
			break;
		case GL_INT_SAMPLER_CUBE:
			ret_val = "GL_INT_SAMPLER_CUBE";
			break;
		case GL_INT_SAMPLER_1D_ARRAY:
			ret_val = "GL_INT_SAMPLER_1D_ARRAY";
			break;
		case GL_INT_SAMPLER_2D_ARRAY:
			ret_val = "GL_INT_SAMPLER_2D_ARRAY";
			break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_INT_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_INT_SAMPLER_2D_MULTISMAPLE_ARRAY";
			break;
		case GL_INT_SAMPLER_BUFFER:
			ret_val = "GL_INT_SAMPLER_BUFFER";
			break;
		case GL_INT_SAMPLER_2D_RECT:
			ret_val = "GL_INT_SAMPLER_2D_RECT";
			break;
		case GL_UNSIGNED_INT_SAMPLER_1D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_1D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_3D:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_3D";
			break;
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_CUBE";
			break;
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
			ret_val = "GL_DOUBLE_UNSIGNED_INT_SAMPLER_2D_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_BUFFER";
			break;
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
			ret_val = "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
			break;
		case GL_IMAGE_1D:
			ret_val = "GL_IMAGE_1D";
			break;
		case GL_IMAGE_2D:
			ret_val = "GL_IMAGE_2D";
			break;
		case GL_IMAGE_3D:
			ret_val = "GL_IMAGE_3D";
			break;
		case GL_IMAGE_2D_RECT:
			ret_val = "GL_IMAGE_2D_RECT";
			break;
		case GL_IMAGE_CUBE:
			ret_val = "GL_IMAGE_CUBE";
			break;
		case GL_IMAGE_BUFFER:
			ret_val = "GL_IMAGE_BUFFER";
			break;
		case GL_IMAGE_1D_ARRAY:
			ret_val = "GL_IMAGE_1D_ARRAY";
			break;
		case GL_IMAGE_2D_ARRAY:
			ret_val = "GL_IMAGE_2D_ARRAY";
			break;
		case GL_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_INT_IMAGE_1D:
			ret_val = "GL_INT_IMAGE_1D";
			break;
		case GL_INT_IMAGE_2D:
			ret_val = "GL_INT_IMAGE_2D";
			break;
		case GL_INT_IMAGE_3D:
			ret_val = "GL_INT_IMAGE_3D";
			break;
		case GL_INT_IMAGE_2D_RECT:
			ret_val = "GL_INT_IMAGE_2D_RECT";
			break;
		case GL_INT_IMAGE_CUBE:
			ret_val = "GL_INT_IMAGE_CUBE";
			break;
		case GL_INT_IMAGE_BUFFER:
			ret_val = "GL_INT_IMAGE_BUFFER";
			break;
		case GL_INT_IMAGE_1D_ARRAY:
			ret_val = "GL_INT_IMAGE_1D_ARRAY";
			break;
		case GL_INT_IMAGE_2D_ARRAY:
			ret_val = "GL_INT_IMAGE_2D_ARRAY";
			break;
		case GL_INT_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_INT_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_1D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_1D";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D";
			break;
		case GL_UNSIGNED_INT_IMAGE_3D:
			ret_val = "GL_UNSIGNED_INT_IMAGE_3D";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_RECT:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_RECT";
			break;
		case GL_UNSIGNED_INT_IMAGE_CUBE:
			ret_val = "GL_UNSIGNED_INT_IMAGE_CUBE";
			break;
		case GL_UNSIGNED_INT_IMAGE_BUFFER:
			ret_val = "GL_UNSIGNED_INT_IMAGE_BUFFER";
			break;
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_1D_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_ARRAY";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE";
			break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			ret_val = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
			break;
		case GL_UNSIGNED_INT_ATOMIC_COUNTER:
			ret_val = "UNSIGNED_INT_ATOMIC_COUNTER";
			break;
		default:
			std::stringstream chk_uni_msg;
			chk_uni_msg << "Uniform::type_name() : unknown type ( default case ) = "
						<< type;
			ErrorHandler::record(chk_uni_msg.str(), ErrorHandler::WARN);
			ret_val = "UNKNOWN";
			break;
		}

	return ret_val;
    }
