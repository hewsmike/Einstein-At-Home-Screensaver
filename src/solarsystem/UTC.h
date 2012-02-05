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

#ifndef UTC_H_
#define UTC_H_

#include <ctime>

#include "ErrorHandler.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief Wrapper class for system supplied UTC values.
 *
 * \author Mike Hewson\n
 */

class UTC {
   public:
      /**
       * \brief Constructor
       */
      UTC(void);

      /**
       * \brief Destructor
       */
      ~UTC();

      /**
       * \brief Inquire of the system clock for UTC values
       *
       * \param hours24 : pointer to integer, value to be set to integral hours
       *                  on a 24 hour clock ie. range is 0 - 23
       *
       * \param days366 : pointer to integer, value to be set to integral days
       *                  since January 1st ie. range 0 - 365
       *                  thus including February 29th if in a leap year
       */
      static void getTime(int* minutes60, int* hours24, int* days366);
   };

/**
 * @}
 */

#endif /* UTC_H_*/
