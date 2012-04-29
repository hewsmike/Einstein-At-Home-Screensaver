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

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief Enforce the order of inclusions. BTW glew includes gl/glu ...
 *
 * \author Mike Hewson\n
 */

#include <GL/glew.h>
#include <GL/glfw.h>

#ifdef WIN_OGL_WORKAROUND
#include <GL/wglew.h>
#endif

/**
 * @}
 */

#endif /*FRAMEWORK_H_*/