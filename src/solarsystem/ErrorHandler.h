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

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem This static class provides common error handling
 * and message recording for the %Solarsystem project.
 *
 * All program console messages, and exits due to error, are handled here.
 * Each message has a type, all are emitted to a log file if accessible,
 * and to console cout or cerr depending. Messages marked as :
 *
 *    - INFORM result in message copied to cout.
 *
 *    - WARN result in message copied to cerr.
 *
 *    - FATAL result in program termination with non-zero exit status returned
 *      to the OS, and the message copied to cerr.
 *
 *    - NORMAL_EXIT result in program termination with zero exit status
 *      returned to the OS, and the message copied to cout.
 *
 * \author Mike Hewson\n
 */

class ErrorHandler {
   private:
      /// Identifiers of program exit codes returned to the OS.
      enum exit_type {NORMAL = 0, ERROR};

      /// Identifiers of file stream readiness for use.
      enum stream_ready {READY, NOT_READY};

      /// Name of file to record messages in.
      static const std::string LOG_FILE_NAME;
      
      /// Holders for timestamp manipulation.
      static time_t current;
      static struct tm* local;
      
      /**
       * \brief Constructor ( private since this a static class )
       *
       */
      ErrorHandler(void);

   public:
      /// Identifiers of message types.
      enum message_type {INFORM, WARN, FATAL, NORMAL_EXIT};

      /**
       *  \brief Destructor
       */
      ~ErrorHandler();

      /**
       * \brief This method is called once to initialise this error
       * handling and message recording system. At present it simply
       * truncates the existing log file.
       */
      static void initialise(void);
      
      /**
       * \brief This method is called when an ( error ) message is to
       * be recorded and/or for the program to be exited.
       *
       * \param msg The message to be recorded
       * \param mt  The type of message
       *
       * \see message_type
       */
      static void record(std::string msg, message_type mt);
   };

/**
 * @}
 */

#endif // ERRORHANDLER_H_