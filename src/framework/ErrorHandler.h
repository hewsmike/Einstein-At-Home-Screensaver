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

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include "framework.h"

#include <string>

#include "SDL_ttf.h"

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief %Framework This static class provides common error handling
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
         * truncates existing log files.
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

        /**
         * \brief Check the current OpenGL context for error.
         */
        static void check_OpenGL_Error(void);

        /**
         * \brief Check the current OpenGL context for error.
         *
         * \param file : the filename ( typically compile time generated using __FILE__ )
         * \param line : the line number ( typically compile time generated using __LINE__ )
         */
        static void check_OpenGL_Error(char* file, GLint line);

        /**
         * \brief Check the current SDL2 state for error, and return any error
         * message.
         *
         *      Only the most recent, if any, error is reported upon since
         * the last call to SDL_ClearError(). Return value will be an empty string
         * if there is no error to report. Before using this function I recommended
         * testing of any return value of a given SDL function to see if any error
         * occurred at all, before/if calling this method.
         *
         * \return : an std::string version of any SDL error message.
         */
        static const std::string& check_SDL2_Error(void);

        /**
         * \brief Check the current SDL2 TTF state for error, and return any error
         * message.
         *
         *      Only the most recent, if any, error is reported upon since
         * the last call to TTF_SetError(). Return value will be an empty string
         * if there is no error to report. Before using this function I recommended
         * testing of any return value of a given SDL TTF function to see if any error
         * occurred at all, before/if calling this method.
         *
         * \return : an std::string version of any SDL TTF error message.
         */
        static const std::string& check_SDL2_TTF_Error(void);

        /**
         * \brief Transform an OpenGL string type to an STL string type.
         *
         * \param glstring : the OpenGL string
         *
         * \return : an std::string version
         */
        static std::string convertGLstring(const GLubyte* glstring);

    private:
        /// Identifiers of program exit codes returned to the OS.
        enum exit_type {EH_NORMAL = 0, EH_ERROR};

        /// Identifiers of file stream readiness for use.
        enum stream_ready {EH_READY, EH_NOT_READY};

        /// Name of log file to record messages in.
        static const std::string LOG_FILE_NAME;

        /// Holders for timestamp manipulation.
        static time_t current;
        static struct tm* local;

        /// Most recently polled SDL2 error.
        static std::string last_SDL2_error;

        /// Most recently polled SDL2 TTF error.
        static std::string last_SDL2_TTF_error;

        /**
         * \brief Constructor ( private since this a static class )
         */
        ErrorHandler(void);
    };

/**
 * @}
 */

#endif // ERRORHANDLER_H_
