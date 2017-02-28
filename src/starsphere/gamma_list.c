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

/***************************************************************************
 *   List of gamma pulsars discovered by Einstein At Home.                 *
 ***************************************************************************/

#include "framework.h"

#define N_GAMMA_ITEMS 2

extern float gamma_info[][N_GAMMA_ITEMS] = {
{ 0.0334f, 62.2667f },
{ 3.9834f, 54.2334f },
{ 5.9000f, 31.1167f },
{ 6.5167f, 6.7667f },
{ 10.9500f, -58.8500f },
{ 11.0834f, -60.6167f },
{ 13.8334f, -62.4167f },
{ 14.3667f, -61.6334f },
{ 15.3667f, -57.5834f },
{ 15.4667f, -58.6334f },
{ 16.4334f, -50.0834f },
{ 16.4000f, -40.6834f },
{ 16.8334f, -46.0167f },
{ 18.4500f, -14.7667f },
{ 18.7334f, -3.7667f },
{ 19.1000f, 7.3667f },
{ 19.5334f, 19.2667f },
{ 20.2834f, 36.4167f }
};

extern GLuint Ngammas = sizeof(gamma_info)/(N_GAMMA_ITEMS*sizeof(float));

// EOF
