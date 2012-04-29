/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

// Only include the specific header for this cpp file, with any
// any other header inclusion done within that. One doesn't ever compile
// this file by the way!! :-)
#include "DocumentationExample.h"

DocumentationExample::DocumentationExample(sometype1 parameter1,
                                           sometype2 parameter2) {
   // Constructor stuff.
   }

DocumentationExample::~DocumentationExample() {
   // Destructor stuff.
   }

return_type DocumentationExample::publicMemberFunction() {
   // Whatever.
   }

return_type DocumentationExample::privateMemberFunction() {
   // Whatever.
   }
