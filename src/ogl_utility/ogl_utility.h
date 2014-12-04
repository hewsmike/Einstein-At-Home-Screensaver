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

#ifndef OGL_UTILITY_H_
#define OGL_UTILITY_H_

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief Debug macro(s) for the ogl_utility group of classes.
 *
 * \author Mike Hewson\n
 */

//
// If NDEBUG is NOT defined then the OGL_DEBUG macro is replaced with
// a call to the check_OpenGL_Error static member function of the
// the ErrorHandler class - which is in the 'framework' group. It provides
// the file and line compiler macro values. Recommend placing after each
// and every OpenGL function invocation.
//
#ifndef NDEBUG
    #define OGL_DEBUG ErrorHandler::check_OpenGL_Error(__FILE_, __LINE__);
#endif

/**
 * @}
 */

#endif /*OGL_UTILITY_H_*/
