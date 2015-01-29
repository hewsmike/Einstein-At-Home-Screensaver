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
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm/gtc

DEPS = Makefile
OBJS = AttributeInputAdapter.o
OBJS += Buffer.o
OBJS += FragmentShader.o
OBJS += IndexBuffer.o
OBJS += OGL_ID.o
OBJS += Pipeline.o
OBJS += Program.o
OBJS += Shader.o
OBJS += TextureBuffer.o
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
liboglutility.a: $(OBJS)
	ar rcs liboglutility.a $(OBJS)	

AttributeInputAdapter.o: $(DEPS) $(UTILITY_SRC)/AttributeInputAdapter.cpp $(UTILITY_SRC)/AttributeInputAdapter.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/AttributeInputAdapter.cpp	

Buffer.o: $(DEPS) $(UTILITY_SRC)/Buffer.cpp $(UTILITY_SRC)/Buffer.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Buffer.cpp

FragmentShader.o: $(DEPS) $(UTILITY_SRC)/FragmentShader.cpp $(UTILITY_SRC)/FragmentShader.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/FragmentShader.cpp

IndexBuffer.o: $(DEPS) $(UTILITY_SRC)/IndexBuffer.cpp $(UTILITY_SRC)/IndexBuffer.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/IndexBuffer.cpp

OGL_ID.o: $(DEPS) $(UTILITY_SRC)/OGL_ID.cpp $(UTILITY_SRC)/OGL_ID.h
	$(CXX) -g $(CPPFLAGS) -c $(UTILITY_SRC)/OGL_ID.cpp

Pipeline.o: $(DEPS) $(UTILITY_SRC)/Pipeline.cpp $(UTILITY_SRC)/Pipeline.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Pipeline.cpp

Program.o: $(DEPS) $(UTILITY_SRC)/Program.cpp $(UTILITY_SRC)/Program.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Program.cpp

Shader.o: $(DEPS) $(UTILITY_SRC)/Shader.cpp $(UTILITY_SRC)/Shader.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Shader.cpp

Texture.o: $(DEPS) $(UTILITY_SRC)/Texture.cpp $(UTILITY_SRC)/Texture.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/Texture.cpp

TextureBuffer.o: $(DEPS) $(UTILITY_SRC)/TextureBuffer.cpp $(UTILITY_SRC)/TextureBuffer.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/TextureBuffer.cpp

VertexBuffer.o: $(DEPS) $(UTILITY_SRC)/VertexBuffer.cpp $(UTILITY_SRC)/VertexBuffer.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/VertexBuffer.cpp	

VertexFetch.o: $(DEPS) $(UTILITY_SRC)/VertexFetch.cpp $(UTILITY_SRC)/VertexFetch.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/VertexFetch.cpp

VertexShader.o: $(DEPS) $(UTILITY_SRC)/VertexShader.cpp $(UTILITY_SRC)/VertexShader.h
	$(CXX) -g ${CPPFLAGS} -c $(UTILITY_SRC)/VertexShader.cpp

# tools
install:
	cp liboglutility.a $(UTILITY_INSTALL)/lib

clean:
	rm -f $(OBJS) liboglutility.a
