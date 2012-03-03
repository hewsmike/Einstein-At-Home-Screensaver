/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#include "OpenGLExts.h"

#include "ErrorHandler.h"
#include "SolarSystemGlobals.h"

#include <iomanip>
#include <iostream>
#include <string>

glBindBuffer_fptr OpenGLExts::BindBuffer_ptr(NULL);
glBufferData_fptr OpenGLExts::BufferData_ptr(NULL);
glDeleteBuffers_fptr OpenGLExts::DeleteBuffers_ptr(NULL);
glGenBuffers_fptr OpenGLExts::GenBuffers_ptr(NULL);
glMapBuffer_fptr OpenGLExts::MapBuffer_ptr(NULL);
glUnmapBuffer_fptr OpenGLExts::UnmapBuffer_ptr(NULL);
glGetStringi_fptr OpenGLExts::GetStringi_ptr(NULL);

OpenGLExts::OpenGLExts() {
   }

OpenGLExts::~OpenGLExts() {
   }

void OpenGLExts::acquire(void) {
   BindBuffer_ptr = reinterpret_cast<glBindBuffer_fptr>(SDL_GL_GetProcAddress("glBindBuffer"));
   if(BindBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glBindBuffer!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   BufferData_ptr = reinterpret_cast<glBufferData_fptr>(SDL_GL_GetProcAddress("glBufferData"));
   if(BufferData_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glBufferData!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   DeleteBuffers_ptr = reinterpret_cast<glDeleteBuffers_fptr>(SDL_GL_GetProcAddress("glDeleteBuffers"));
   if(DeleteBuffers_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glDeleteBuffers!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   GenBuffers_ptr = reinterpret_cast<glGenBuffers_fptr>(SDL_GL_GetProcAddress("glGenBuffers"));
   if(GenBuffers_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   MapBuffer_ptr = reinterpret_cast<glMapBuffer_fptr>(SDL_GL_GetProcAddress("glMapBuffer"));
   if(MapBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glMapBuffer!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   UnmapBuffer_ptr = reinterpret_cast<glUnmapBuffer_fptr>(SDL_GL_GetProcAddress("glUnmapBuffer"));
   if(UnmapBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glUnmapBuffer!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   GetStringi_ptr = reinterpret_cast<glGetStringi_fptr>(SDL_GL_GetProcAddress("glGetStringi"));
   if(GetStringi_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glGetStringi!";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }
   }

void OpenGLExts::ExtGLBindBuffer(GLenum target, GLuint buffer) {
   (*BindBuffer_ptr)(target, buffer);
   }

void OpenGLExts::ExtGLBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {
   (*BufferData_ptr)(target, size, data, usage);
   }

void OpenGLExts::ExtGLDeleteBuffers(GLsizei num, const GLuint* buff) {
   (*DeleteBuffers_ptr)(num, buff);
   }

void OpenGLExts::ExtGLGenBuffers(GLsizei num, GLuint* buff) {
   (*GenBuffers_ptr)(num, buff);
   }

void* OpenGLExts::ExtGLMapBuffer(GLenum target, GLenum access) {
   return (*MapBuffer_ptr)(target, access);
   }

GLboolean OpenGLExts::ExtGLUnmapBuffer(GLenum target) {
   return (*UnmapBuffer_ptr)(target);
   }

const GLubyte* OpenGLExts::ExtGLGetStringi(GLenum name, GLuint index) {
   return (*GetStringi_ptr)(name, index);
   }
