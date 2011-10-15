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

#include "ErrorHandler.h"

// The error messsage file's name, within local runtime directory.
const std::string ErrorHandler::LOG_FILE_NAME("solarsystem_stderr.txt");

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
   bool output_ready = ErrorHandler::READY;

   // The file stream to use.
   std::ofstream output_file;

   // Prepare said stream for appending output to the file that records
   // the messages.
   output_file.open(ErrorHandler::LOG_FILE_NAME.c_str(),
                    std::ios::out | std::ios::app);

   // Could we open it ?
   if(!output_file.is_open()) {
      // No, so emit message on std::cerr ONLY !!
      // Don't invoke this routine from within, as we don't want
      // any silly recursion! :-)
      std::cerr << "Could not open '" << ErrorHandler::LOG_FILE_NAME
                << "' for writing !!" << std::endl;

      // We can't use this stream.
      output_ready = ErrorHandler::NOT_READY;
      }

   // Start with an empty message string to record and output.
   std::string message("");

   // First include a system derived local time stamp.
   //time(&current);
   //local = localtime(&current);
   //message += local.tm_hour;
   //message += ":";
   //message += local.tm_min;
   //message += ":";
   //message += local.tm_sec;
   //message += " ";

   // Now what sort of message have we been instructed to emit ?
   switch(mt) {
      case ErrorHandler::INFORM :
         message += "INFORMATIVE : " + msg;
         std::cout << message << std::endl;
         // But also record error in file,
         // provided the output file stream is available for use.
         if(output_ready == ErrorHandler::READY) {
            // Then record to our file.
            output_file << message << std::endl;
            }
         break;
      case ErrorHandler::NORMAL_EXIT :
         message += "NORMAL EXIT : " + msg;
         // This isn't an error, so only goes to cout.
         std::cout << message << std::endl;
         // But also record error in file,
         // provided the output file stream is available for use.
         if(output_ready == ErrorHandler::READY) {
            // Then record to our file.
            output_file << message << std::endl;
            }
         // But we'll be leaving the program.
         output_file.close();
         exit(ErrorHandler::NORMAL);
         break;
      case ErrorHandler::WARN :
         message += "WARNING : " + msg;
         // This is an error, so goes to cerr.
         std::cerr << msg << std::endl;
         // But also record error in file,
         // provided the output file stream is available for use.
         if(output_ready == ErrorHandler::READY) {
            // Then record to our file.
            output_file << message << std::endl;
            }
         break;
      case ErrorHandler::FATAL :
         message += "FATAL : " + msg;
         // This is an error, so goes to cerr.
         std::cerr << msg << std::endl;
         // But also record error in file,
         // provided the output file stream is available for use.
         if(output_ready == ErrorHandler::READY) {
            // Then record to our file.
            output_file << message << std::endl;
            }
         // Then we'll be leaving the program, in mild shame .....
         output_file.close();
         exit(ErrorHandler::ERROR);
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
         // But also record error in file,
         // provided the output file stream is available for use.
         if(output_ready == ErrorHandler::READY) {
            // Then record to our file.
            output_file << message << std::endl;
            }
         // This indicates a faulty error handling routine.
         // Deserves to die. In deep shame.
         exit(ErrorHandler::ERROR);
         break;
      }

   // Close the file between invocations of this routine.
   // Preserves readability if program falls over elsewhere
   // without exit through this routine.
   output_file.close();

   return;
   };