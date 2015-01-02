/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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
 * \see main.cpp ( as regards use of SDL_SetMainReady() with SDL_MAIN_HANDLED )
 *
 * \author Mike Hewson\n
 */

#define GLEW_STATIC
#include <GL/glew.h>

// To circumvent potential later failure of SDL_Init(), as we are not
// not using SDL_main() as a program entry point. The SDL2 Wiki entries
// ( as of 05 Jan 2014 ) are :
//
// Under 'Initialization and Shutdown' : "It should be noted that on some
// operating systems, SDL_Init() will fail if SDL_main() has not been defined
// as the entry point for the program. Calling SDL_SetMainReady() prior to
// SDL_Init() will circumvent this failure condition, however, users should be
// careful when calling SDL_SetMainReady() as improper initalization may cause
// crashes and hard to diagnose problems."
//
// Under 'SDL_SetMainReady' : "This function is defined in SDL_main.h, along
// with the preprocessor rule to redefine main() as SDL_main(). Thus to ensure
// that your main() function will not be changed it is necessary to define
// SDL_MAIN_HANDLED before including SDL.h."
//
// Err ..... some each-way bet here ??? :-O
#ifndef SDL_MAIN_HANDLED
    #define SDL_MAIN_HANDLED
#endif

#include <SDL2/SDL.h>

#include <SDL_ttf.h>

/**
 * @}
 */

#endif /*FRAMEWORK_H_*/
