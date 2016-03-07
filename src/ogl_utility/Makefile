###########################################################################
#   Copyright (C) 2014 by Mike Hewson                                     #
#   hewsmike[AT]iinet.net.au                                              #
#                                                                         #
#   This file is part of Einstein@Home.                                   #
#                                                                         #
#   Einstein@Home is free software: you can redistribute it and/or modify #
#   it under the terms of the GNU General Public License as published     #
#   by the Free Software Foundation, version 2 of the License.            #
#                                                                         #
#   Einstein@Home is distributed in the hope that it will be useful,      #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. #
#                                                                         #
###########################################################################

# path settings
UTILITY_SRC?=$(PWD)
UTILITY_INSTALL?=$(PWD)

# config values
CXX?=g++

# variables
CPPFLAGS += -std=c++11
CPPFLAGS += -I$(UTILITY_SRC)
CPPFLAGS += -I$(FRAMEWORK_SRC)
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/GL
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/SDL2
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm/gtc

DEPS = Makefile
# OBJS += Acquirable.o
OBJS = AttributeInputAdapter.o
# OBJS += Bindable.o
OBJS += Buffer.o
OBJS += FragmentShader.o
OBJS += IndexBuffer.o
OBJS += OGL_ID.o
OBJS += Pipeline.o
OBJS += Program.o
OBJS += RenderTask.o
OBJS += Shader.o
OBJS += TextureBuffer.o
OBJS += TexturedParallelogram.o
OBJS += TextString.o
OBJS += TransformGlobals.o
OBJS += VertexBuffer.o
OBJS += VertexFetch.o
OBJS += VertexShader.o

# primary role based tagets
default: release
debug: liboglutility.a
release: clean liboglutility.a

memcheck: clean debug
callgrind: clean debug

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum

# file based targets
liboglutility.a: $(DEPS) $(OBJS)
	ar rcs liboglutility.a $(OBJS)

# pattern rule for object files
%.o: $(UTILITY_SRC)/%.cpp $(UTILITY_SRC)/%.h
	$(CXX) -g ${CPPFLAGS} -c $< -o $@

# tools
install:
	cp liboglutility.a $(UTILITY_INSTALL)/lib

clean:
	rm -f $(OBJS) liboglutility.a
