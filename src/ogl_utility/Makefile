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
CPPFLAGS += -I$(UTILITY_SRC)
CPPFLAGS += -I$(FRAMEWORK_SRC)
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/GL
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/SDL2

DEPS = Makefile
OBJS = Buffer.o
OBJS += BufferVertexFetch.o
OBJS += OGL_ID.o
OBJS += Program.o
OBJS += Shader.o
OBJS += Texture.o
OBJS += VertexFetch.o

# primary role based tagets
default: release
debug: liboglutility.a
memcheck: clean debug
callgrind: clean debug
release: clean liboglutility.a

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum

# file based targets
liboglutility.a: $(OBJS) $(UTILITY_SRC)/vmath.h
	ar rcs liboglutility.a $(OBJS)

Buffer.o: $(DEPS) $(UTILITY_SRC)/Buffer.cpp $(UTILITY_SRC)/Buffer.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Buffer.cpp

BufferVertexFetch.o: $(DEPS) $(UTILITY_SRC)/BufferVertexFetch.cpp $(UTILITY_SRC)/BufferVertexFetch.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/BufferVertexFetch.cpp

OGL_ID.o: $(DEPS) $(UTILITY_SRC)/OGL_ID.cpp $(UTILITY_SRC)/OGL_ID.h
	$(CXX) -g $(CPPFLAGS) -c $(UTILITY_SRC)/OGL_ID.cpp

Program.o: $(DEPS) $(UTILITY_SRC)/Program.cpp $(UTILITY_SRC)/Program.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Program.cpp

Shader.o: $(DEPS) $(UTILITY_SRC)/Shader.cpp $(UTILITY_SRC)/Shader.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Shader.cpp

Texture.o: $(DEPS) $(UTILITY_SRC)/Texture.cpp $(UTILITY_SRC)/Texture.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Texture.cpp

VertexFetch.o: $(DEPS) $(UTILITY_SRC)/Vertexfetch.cpp $(UTILITY_SRC)/VertexFetch.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/VertexFetch.cpp

# tools
install:
	cp liboglutility.a $(UTILITY_INSTALL)/lib

clean:
	rm -f $(OBJS) liboglutility.a
