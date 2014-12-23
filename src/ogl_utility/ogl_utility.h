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

#include <iostream>
#include <string>

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
// Make sure we know of the ErrorHandler class.
//

#include "ErrorHandler.h"

//
// If NDEBUG is NOT defined then the OGL_DEBUG; macro is replaced with
// a call to the check_OpenGL_Error static member function of the
// the ErrorHandler class - which is in the 'framework' group. It provides
// the file and line compiler macro values. Recommend placing after each
// and every OpenGL function invocation.
//

#ifdef NDEBUG
    #define OGL_DEBUG(A) A
	#define SDL_DEBUG(B) B
#else
    #define OGL_DEBUG(A) A;\
						 ErrorHandler::check_OpenGL_Error((std::string(#A) + std::string("\t") + std::string(__FILE__).substr(std::string(__FILE__).rfind('/') + 1)).c_str(), __LINE__);\
						 //getchar()
	#define SDL_DEBUG(B) B;\
						 ErrorHandler::check_SDL2_Error((std::string(#B) + std::string("\t") + std::string(__FILE__).substr(std::string(__FILE__).rfind('/') + 1)).c_str(), __LINE__);\
						 OGL_DEBUG();\
						 getchar()
#endif

/**
 * @}
 */

#endif /*OGL_UTILITY_H_*/
