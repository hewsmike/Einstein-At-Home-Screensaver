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

#include "VertexFetch.h"

#include "OGL_ID.h"

VertexFetch::VertexFetch(void) {
    }

VertexFetch::~VertexFetch() {
    }

void VertexFetch::attach(void) {
    // Ensure that the pipeline vertex fetch stage is NOT
    // bound at all, regardless of any prior bindings.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_ID::NO_ID);
    }

void VertexFetch::trigger(GLenum primitive, GLsizei count) {
	// Provokes vertex shader activity for count invocations.
	glDrawArrays(primitive, 0, count);
	}

void VertexFetch::detach(void) {
    // Ensure that the pipeline vertex fetch stage is NOT
    // bound at all.
    glBindBuffer(GL_ARRAY_BUFFER, OGL_ID::NO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_ID::NO_ID);
    }
