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

#include "ErrorHandler.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "WindowManager.h"

// The error messsage file's name, within local runtime directory.
const std::string ErrorHandler::LOG_FILE_NAME("eahss_stderr.txt");

std::string ErrorHandler::last_SDL2_error;
std::string ErrorHandler::last_SDL2_TTF_error;

ErrorHandler::ErrorHandler(void) {
    }

ErrorHandler::~ErrorHandler() {
    }

void ErrorHandler::initialise(void) {
    // The file stream to use.
    std::ofstream output_file;

    // Prepare a stream for output to the file and truncate ( contents gone ).
    output_file.open(ErrorHandler::LOG_FILE_NAME.c_str(),
                     std::ios::out | std::ios::trunc);

    // Close the file.
    output_file.close();
    }

void ErrorHandler::record(std::string msg, message_type mt) {
    // Prepare log file for append.
    std::ofstream output_file;
    output_file.open(ErrorHandler::LOG_FILE_NAME.c_str(),
                     std::ios::out | std::ios::app);

    // Check for access, if not emit message on std::cerr only ( recursion ).
    bool output_ready = ErrorHandler::EH_READY;
    if(!output_file.is_open()) {
       std::cerr << "Could not open '" << ErrorHandler::LOG_FILE_NAME
                 << "' for writing !!" << std::endl;
       output_ready = ErrorHandler::EH_NOT_READY;
       }

    std::string message("");

   // First include a system derived local time stamp.
   /// TODO - write this !
   //time(&current);
   //local = localtime(&current);
   //message += local.tm_hour;
   //message += ":";
   //message += local.tm_min;
   //message += ":";
   //message += local.tm_sec;
   //message += " ";

    // Dispose as per message type.
    switch(mt) {
        case ErrorHandler::INFORM :
            message += "INFO : " + msg;
            std::cout << message << std::endl;
            if(output_ready == ErrorHandler::EH_READY) {
                output_file << message << std::endl;
                }
            break;
        case ErrorHandler::NORMAL_EXIT :
            message += "NORMAL EXIT : " + msg;
            std::cout << message << std::endl;
            if(output_ready == ErrorHandler::EH_READY) {
                output_file << message << std::endl;
                }
            output_file.close();
            exit(ErrorHandler::EH_NORMAL);
            break;
        case ErrorHandler::WARN :
            message += "WARN : " + msg;
            std::cerr << message << std::endl;
            std::cout << message << std::endl;
            if(output_ready == ErrorHandler::EH_READY) {
                output_file << message << std::endl;
                }
            break;
        case ErrorHandler::FATAL :
            message += "FATAL EXIT: " + msg;
                std::cerr << msg << std::endl;
            std::cout << msg << std::endl;
            if(output_ready == ErrorHandler::EH_READY) {
                output_file << message << std::endl;
                }

            // Unload any retained OpenGL error messages.
            check_OpenGL_Error();

            // Then we'll be leaving the program, in mild shame .....
            output_file.close();
            exit(ErrorHandler::EH_ERROR);
            break;
        default:
            // We should NEVER get here, but in case we do ......
            // Construct a string describing this error state.
            message += "ErrorHandler::record() - default switch case reached ! \n";
            message += "switch variable value = ";
            message += mt;
            message += "\nMessage string passed = '";
            message += msg;
            message += "'\n";
            // This is an error, so goes to cerr.
            std::cerr << message << std::endl;
            std::cout << message << std::endl;
            // But also record error in file,
            // provided the output file stream is available for use.
            if(output_ready == ErrorHandler::EH_READY) {
                // Then record to our file.
                output_file << message << std::endl;
                }
            // This indicates a faulty error handling routine.
            // Deserves to die. In deep shame.
            exit(ErrorHandler::EH_ERROR);
            break;
        }

    // Close the file between invocations of this routine.
    // Preserves readability if program falls over elsewhere
    // without exit through this routine.
    output_file.close();

    return;
    };

bool ErrorHandler::check_OpenGL_Error() {
    bool ret_val = false;

    GLenum error_code = glGetError();

    // Hit glGetError() until it runs out of errors.
    while(error_code != GL_NO_ERROR) {
        ret_val = true;
        std::stringstream message;
        message << "ErrorHandler::check_OpenGL_Error() - Reported OpenGL error with code : "
                << error_code
                << " - "
 //               << ErrorHandler::convertGLstring(gluErrorString(glGetError()))
                << " : ";
        switch(error_code) {
            case GL_INVALID_ENUM :
                message << "An unacceptable value is specified for an enumerated argument";
                break;
            case GL_INVALID_VALUE :
                message << "A numeric argument is out of range";
                break;
            case GL_INVALID_OPERATION :
                message << "The specified operation is not allowed in the current state";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION :
                message << "The framebuffer object is not complete";
                break;
            case GL_OUT_OF_MEMORY :
                message << "There is not enough memory left to execute the command";
                break;
            case GL_STACK_UNDERFLOW :
                message << "An attempt has been made to perform an operation that would cause an internal stack to underflow";
                break;
            case GL_STACK_OVERFLOW :
                message << "An attempt has been made to perform an operation that would cause an internal stack to overflow";
                break;
            default:
                // We should NEVER get here, but in case we do ......
                // Construct a string describing this error state.
                string message_fail;
                message_fail += "\nErrorHandler::check_OpenGL_Error() - default switch case reached ! \n";
                message_fail += "switch variable value = ";
                message_fail += error_code;
                // This is an error, so goes to cerr.
                std::cerr << message_fail << std::endl;
                std::cout << message_fail << std::endl;
                // This indicates faulty error handling, a significant problem.
                // Deserves to die. For shame.
                exit(ErrorHandler::EH_ERROR);
                break;
            }
        // Record any message string obtained from OpenGL.
        ErrorHandler::record(message.str(), ErrorHandler::WARN);
        // Fetch any/next error code.
        error_code = glGetError();
        }
    return ret_val;
    }

void ErrorHandler::check_OpenGL_Error(const char* file, GLint line) {
    ErrorHandler::check_OpenGL_Error();

    std::stringstream message;
    message << "FILE = "
            << file
            << "\t LINE = "
            << line;
    ErrorHandler::record(message.str(), ErrorHandler::WARN);
    }

const std::string& ErrorHandler::check_SDL2_Error(void) {
    // Retrieve and store a message about the most recent error
    // since call to SDL_ClearError(). Repeated calls to SDL_GetError
    // will only ever return the same last error.
    last_SDL2_error = SDL_GetError();

    // Clearout any message.
    SDL_ClearError();

    return last_SDL2_error;
    }

const std::string& ErrorHandler::check_SDL2_TTF_Error(void) {
    // Retrieve and store a message about the most recent error
    // since call to SDL_SetError(). Repeated calls to TTF_GetError
    // will only ever return the same last error.
    last_SDL2_TTF_error = TTF_GetError();

    // Clearout any message.
    TTF_SetError("");

    return last_SDL2_TTF_error;
    }

std::string ErrorHandler::convertGLstring(const GLubyte* glstring) {
    // gluErrorString(), and others, return 'const GLubyte *' - essentially
    // devolving to 'const unsigned char *'  - but string expects
    // 'const char *'. Trouble is that direct type conversion/cast is problematic,
    // so one has to traverse an OpenGL 'string' ( fortunately null terminated )
    // in order to construct a workable C++ STL string version. This
    // assumes that the typedef 'khronos_uint8_t' won't change in future ...

    // Start with an empty string as the return value.
    std::string ret_val;

    // Check that we weren't given a NULL.
    if(glstring != NULL) {
        // Traverse the OpenGL 'string' from it's begining.
        int i = 0;
        char current = 0;
        // Assuming we haven't reached the null character terminator.
        while((current = *(glstring + i)) != 0) {
            // Add the current character onto the string.
            ret_val += current;
            // Increment to the next character position.
            ++i;
            }
        }

    // Return any string accumulated ( which may be empty ).
    return ret_val;
    }
