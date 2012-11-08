/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#include "HUDContent.h"

HUDContent::HUDContent(GLuint horizontalMargin, GLuint verticalMargin) :
                        HUDItem(),
                        hz_marg(horizontalMargin),
                        vt_marg(verticalMargin) {
    }

HUDContent::~HUDContent() {
    }

void HUDContent::resizeUpdate(GLuint width, GLuint height) {
    // We need do nothing for non-Windows executables.
//#ifdef WIN_OGL_WORKAROUND
  //  #pragma message("Yup, we've put in the Win32 resizeUpdate behaviour ... ")
    // For Windows builds only, re-acquire assets in the server.
    // Not especially interested in the actual dimensions here,
    // merely that they have changed and the OpenGL context was lost.
    std::cout << "HUDImage::resizeUpdate()" << std::endl;
    // inactivate();
    // activate();
//#endif
    }

GLuint HUDContent::horzMargin(void) const {
    return hz_marg;
    }

GLuint HUDContent::vertMargin(void) const {
    return vt_marg;
    }
