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
wglCreateContextAttribsARB_fptr OpenGLExts::wCreateContextAttribsARB_ptr(NULL);
wglGetCurrentContext_fptr OpenGLExts::wGetCurrentContext_ptr(NULL);
wglGetCurrentDC_fptr OpenGLExts::wGetCurrentDC_ptr(NULL);
wglDeleteContext_fptr OpenGLExts::wDeleteContext_ptr(NULL);
wglMakeCurrent_fptr OpenGLExts::wMakeCurrent_ptr(NULL);

OpenGLExts::OpenGLExts() {
   }

OpenGLExts::~OpenGLExts() {
   }

void OpenGLExts::acquire(void) {
   BindBuffer_ptr = reinterpret_cast<glBindBuffer_fptr>(SDL_GL_GetProcAddress("glBindBuffer"));
   if(BindBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glBindBuffer !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   BufferData_ptr = reinterpret_cast<glBufferData_fptr>(SDL_GL_GetProcAddress("glBufferData"));
   if(BufferData_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glBufferData !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   DeleteBuffers_ptr = reinterpret_cast<glDeleteBuffers_fptr>(SDL_GL_GetProcAddress("glDeleteBuffers"));
   if(DeleteBuffers_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glDeleteBuffers !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   GenBuffers_ptr = reinterpret_cast<glGenBuffers_fptr>(SDL_GL_GetProcAddress("glGenBuffers"));
   if(GenBuffers_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glGenBuffers !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   MapBuffer_ptr = reinterpret_cast<glMapBuffer_fptr>(SDL_GL_GetProcAddress("glMapBuffer"));
   if(MapBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glMapBuffer !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   UnmapBuffer_ptr = reinterpret_cast<glUnmapBuffer_fptr>(SDL_GL_GetProcAddress("glUnmapBuffer"));
   if(UnmapBuffer_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glUnmapBuffer !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   GetStringi_ptr = reinterpret_cast<glGetStringi_fptr>(SDL_GL_GetProcAddress("glGetStringi"));
   if(GetStringi_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for glGetStringi !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
      }
      
   wCreateContextAttribsARB_ptr = reinterpret_cast<wglCreateContextAttribsARB_fptr>(SDL_GL_GetProcAddress("wglCreateContextAttribsARB"));
      if(wCreateContextAttribsARB_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for wglCreateContextAttribsARB !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
      }
      
   wGetCurrentContext_ptr = reinterpret_cast<wglGetCurrentContext_fptr>(SDL_GL_GetProcAddress("wglGetCurrentContext"));
      if(wGetCurrentContext_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for wglGetCurrentContext !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
      }
      
   wGetCurrentDC_ptr = reinterpret_cast<wglGetCurrentDC_fptr>(SDL_GL_GetProcAddress("wglGetCurrentDC"));
      if(wGetCurrentDC_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for wglGetCurrentDC !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
      }
      
   wDeleteContext_ptr = reinterpret_cast<wglDeleteContext_fptr>(SDL_GL_GetProcAddress("wglDeleteContext"));
      if(wDeleteContext_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for wglDeleteContext !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
      }
      
   wMakeCurrent_ptr = reinterpret_cast<wglMakeCurrent_fptr>(SDL_GL_GetProcAddress("wglMakeCurrent"));
      if(wMakeCurrent_ptr == NULL) {
      std::string msg = "OpenGLExts::acquire() : null pointer obtained for wglMakeCurrent !";
      std::cout << msg << std::endl;
      ErrorHandler::record(msg, ErrorHandler::WARN);
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
   
HGLRC OpenGLExts::ExtWGLCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList) {
   return (*wCreateContextAttribsARB_ptr)(hDC, hShareContext, attribList);
   }
   
HGLRC OpenGLExts::ExtWGLGetCurrentContext(void) {
   return (*wGetCurrentContext_ptr)();
   }

HDC OpenGLExts::ExtWGLGetCurrentDC(void) {
   return (*wGetCurrentDC_ptr)();
   }
   
BOOL OpenGLExts::ExtWGLDeleteContext(HGLRC hglrc) {
   return (*wDeleteContext_ptr)(hglrc);
   }
   
BOOL OpenGLExts::ExtWGLMakeCurrent(HDC hdc, HGLRC hglrc) {
   return (*wMakeCurrent_ptr)(hdc, hglrc);
   }
