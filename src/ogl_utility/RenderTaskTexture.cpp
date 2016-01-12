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

#include "RenderTaskTexture.h"

RenderTaskTexture::RenderTaskTexture(RenderTask::shader_group s_group,
        			   	   	   	     RenderTask::index_buffer_group i_group,
									 RenderTask::vertex_buffer_group v_group,
									 TextureBuffer* texture_buffer) :
									 RenderTask(s_group, i_group, v_group) {
    // Is the texture buffer pointer meaningful ?
    if(texture_buffer == NULL) {
    	 ErrorHandler::record("RenderTaskTexture::RenderTaskTexture() : - no TextureBuffer object supplied!",
    	                       ErrorHandler::FATAL);
    	}
    m_texture_buffer = texture_buffer;
    }

RenderTaskTexture::~RenderTaskTexture() {
    }
