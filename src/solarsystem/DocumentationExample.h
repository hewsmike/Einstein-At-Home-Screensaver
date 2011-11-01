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

#ifndef DOCUMENTATION_EXAMPLE_H_
#define DOCUMENTATION_EXAMPLE_H_

#include <a_standard_library_header>

#include "a_homegrown_header.h"

/**
 * \addtogroup group_label
 * @{
 */

/**
 * \brief A short explanation of as many lines as you like until an empty line
 *        just like below. One doesn't ever compile this file by the way!! :-)
 *
 * A somewhat longer explanation with further detail
 *
 * \see SomeKnownClassOfInterest
 *
 * \author Mike Hewson\n
 */

class DocumentationExample {
   public:
      /**
       * \brief Constructor
       *
       * \param parameter1 : a brief note about parameter1
       * \param parameter2 : a brief note about parameter2
       */
      DocumentationExample(sometype parameter1, sometype parameter2);

      /**
       * \brief Destructor
       */
      ~DocumentationExample();

      /**
       * \brief A short explanation of some public member function
       *
       * The more detailed longer explanation.
       *
       * \see someMemberFunctionOfInterest
       */
      return_type publicMemberFunction();

   private:
      /**
       * \brief A short explanation of some private member function
       *
       * The more detailed longer explanation.
       *
       * \see someMemberFunctionOfInterest
       */
      return_type privateMemberFunction();

      /// Comment upon some private data member
      sometype some_private_data_member;
   };

/**
 * @}
 */

#endif /*DOCUMENTATION_EXAMPLE_H_*/
