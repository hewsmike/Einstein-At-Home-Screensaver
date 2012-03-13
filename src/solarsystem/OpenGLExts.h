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

#ifndef OPENGL_EXTS_H_
#define OPENGL_EXTS_H_

#include <map>

#include "SDL.h"
#include "SDL_opengl.h"

// For convenience the function pointers are hereby given type definitions.
// To be used when casting return values ......

typedef void (APIENTRY *glBindBuffer_fptr)(GLenum, GLuint);
typedef void (APIENTRY *glBufferData_fptr)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
typedef void (APIENTRY *glDeleteBuffers_fptr)(GLsizei, const GLuint*);
typedef void (APIENTRY *glGenBuffers_fptr)(GLsizei, GLuint*);
typedef void* (APIENTRY *glMapBuffer_fptr)(GLenum, GLenum);
typedef GLboolean (APIENTRY *glUnmapBuffer_fptr)(GLenum);
typedef const GLubyte* (APIENTRY *glGetStringi_fptr)(GLenum, GLuint);
typedef HGLRC (APIENTRY *wglCreateContextAttribsARB_fptr)(HDC, HGLRC, const int*);
typedef HGLRC (APIENTRY *wglGetCurrentContext_fptr)(void);
typedef HDC (APIENTRY *wglGetCurrentDC_fptr)(void);
typedef BOOL (APIENTRY *wglDeleteContext_fptr)(HGLRC);
typedef BOOL (APIENTRY *wglMakeCurrent_fptr)(HDC, HGLRC);

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 * post v1.1 extensions ie. just about everything since ~ 1996. This
 * exists purely to allow Windows calls via a function pointer mechanism,
 * to be used dynamically at runtime, in the absence of static linking
 * to opengl32.dll exported functions .....
 *
 * Note : no conditional compilation within this class, as inclusion is
 * decided upon at makefile level.
 *
 * \author Mike Hewson\n
 */

class OpenGLExts {
   public :
      /**
       * \brief Destructor
       */
      ~OpenGLExts();

      static void ExtGLBindBuffer(GLenum target, GLuint buffer);

      static void ExtGLBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

      static void ExtGLDeleteBuffers(GLsizei num, const GLuint* buff);

      static void ExtGLGenBuffers(GLsizei num, GLuint* buff);

      static void* ExtGLMapBuffer(GLenum target, GLenum access);

      static GLboolean ExtGLUnmapBuffer(GLenum target);

      static const GLubyte* ExtGLGetStringi(GLenum name, GLuint index);

      static HGLRC ExtWGLCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList);

      static HGLRC ExtWGLGetCurrentContext(void);

      static HDC ExtWGLGetCurrentDC(void);

      static BOOL ExtWGLDeleteContext(HGLRC hglrc);

      static BOOL ExtWGLMakeCurrent(HDC hDC, HGLRC hglrc);

      /**
       * \brief Obtains all the OpenGL extension function
       *        pointers, one for each within the above enumeration.
       *        Note that this has to be called AFTER an OpenGL
       *        context has been acquired, and repeated with any
       *        new acquisition.
       */
      static void acquire(void);

   private :
      /**
       * \brief Constructor, private as static
       */
      OpenGLExts(void);

      static glBindBuffer_fptr BindBuffer_ptr;
      static glBufferData_fptr BufferData_ptr;
      static glDeleteBuffers_fptr DeleteBuffers_ptr;
      static glGenBuffers_fptr GenBuffers_ptr;
      static glMapBuffer_fptr MapBuffer_ptr;
      static glUnmapBuffer_fptr UnmapBuffer_ptr;
      static glGetStringi_fptr GetStringi_ptr;
      static wglCreateContextAttribsARB_fptr wCreateContextAttribsARB_ptr;
      static wglGetCurrentContext_fptr wGetCurrentContext_ptr;
      static wglGetCurrentDC_fptr wGetCurrentDC_ptr;
      static wglDeleteContext_fptr wDeleteContext_ptr;
      static wglMakeCurrent_fptr wMakeCurrent_ptr;
   };

/**
 * @}
 */

#endif /*OPENGL_EXTS_H_*/
