/***************************************************************************
 *   Copyright (C) 2014 by Mike Hewson                                     *
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

#include "TestProgram.h"

TestProgram::TestProgram(VertexShader* vertex_shader,
                 	 	 FragmentShader* fragment_shader,
						 AttributeInputAdapter* adapter,
						 UniformInputAdapter* uniforms,
						 Program::shaderDisposition dispose) :
				Program(vertex_shader,
						fragment_shader,
						adapter,
						uniforms,
						dispose){
    }

TestProgram::~TestProgram() {
    }

void TestProgram::frameCallback(void) {

	}
