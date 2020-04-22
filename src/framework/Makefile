###########################################################################
#   Copyright (C) 2008 by Oliver Bock                                     #
#   oliver.bock[AT]aei.mpg.de        					  #
#                                                                         #
#   Copyright (C) 2020 by Mike Hewson	                                  #
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
FRAMEWORK_SRC?=$(PWD)
FRAMEWORK_INSTALL?=$(PWD)

$(info FRAMEWORK_SRC is $(FRAMEWORK_SRC))
$(info FRAMEWORK_INSTALL is $(FRAMEWORK_INSTALL))

# Product specific.
# Linux 'tr' command translates, here converting lower case alphabetical
# characters to uppercase eg. 'starsphere' -> 'STARSPHERE'
PRODUCT_UPPER := $(shell echo $(PRODUCT) | tr a-z A-Z)
# Triggers conditional compilation within GraphicsEngineFactory class.
CPPFLAGS += -D$(PRODUCT_UPPER)

# config values
CXX?=g++

# variables
CPPFLAGS += -std=c++11
CPPFLAGS += $(shell $(FRAMEWORK_INSTALL)/bin/freetype-config --cflags)
CPPFLAGS += $(shell $(FRAMEWORK_INSTALL)/bin/xml2-config --cflags)
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/boinc
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/GL
CPPFLAGS += -I$(FRAMEWORK_INSTALL)
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/SDL2
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm
CPPFLAGS += -I$(FRAMEWORK_INSTALL)/include/glm/gtc
CPPFLAGS += -I/usr/include

DEPS = Makefile
OBJS = AbstractGraphicsEngine.o
OBJS += ErrorHandler.o
OBJS += GraphicsEngineFactory.o
OBJS += glew.o
OBJS += WindowManager.o
OBJS += Resource.o
OBJS += ResourceFactory.o
OBJS += BOINCClientAdapter.o
OBJS += Libxml2Adapter.o

# TODO: GraphicsEngineFactory obviously depends on the actual implementations (here solarsystem & starsphere)!
# need to change the structure! what about plugins?
CPPFLAGS += -I$(FRAMEWORK_SRC)
CPPFLAGS += -I$(FRAMEWORK_SRC)/../solarsystem
CPPFLAGS += -I$(FRAMEWORK_SRC)/../starsphere
CPPFLAGS += -I$(FRAMEWORK_SRC)/../ogl_utility
CPPFLAGS += -I$(FRAMEWORK_SRC)/../framework
CPPFLAGS += -I$(SOLARSYSTEM_INSTALL)/include/boinc

# primary role based tagets
default: release
debug: libframework.a
memcheck: clean debug
callgrind: clean debug
release: clean libframework.a

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum

$(info CPPFLAGS is $(CPPFLAGS))

# file based targets
libframework.a: $(OBJS) $(FRAMEWORK_SRC)/framework.h
	ar rcs libframework.a $(OBJS)

AbstractGraphicsEngine.o: $(DEPS) $(FRAMEWORK_SRC)/AbstractGraphicsEngine.cpp $(FRAMEWORK_SRC)/AbstractGraphicsEngine.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/AbstractGraphicsEngine.cpp

ErrorHandler.o: $(DEPS) $(FRAMEWORK_SRC)/ErrorHandler.cpp $(FRAMEWORK_SRC)/ErrorHandler.h
	$(CXX) -g $(CPPFLAGS) -c $(FRAMEWORK_SRC)/ErrorHandler.cpp

GraphicsEngineFactory.o: $(DEPS) $(FRAMEWORK_SRC)/GraphicsEngineFactory.cpp $(FRAMEWORK_SRC)/GraphicsEngineFactory.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/GraphicsEngineFactory.cpp

glew.o: $(DEPS) $(FRAMEWORK_SRC)/glew.c $(FRAMEWORK_SRC)/glew.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/glew.c

WindowManager.o: $(DEPS) $(FRAMEWORK_SRC)/WindowManager.cpp $(FRAMEWORK_SRC)/WindowManager.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/WindowManager.cpp

BOINCClientAdapter.o: $(DEPS) $(FRAMEWORK_SRC)/BOINCClientAdapter.cpp $(FRAMEWORK_SRC)/BOINCClientAdapter.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/BOINCClientAdapter.cpp

ResourceFactory.o: $(DEPS) $(FRAMEWORK_SRC)/ResourceFactory.cpp $(FRAMEWORK_SRC)/ResourceFactory.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/ResourceFactory.cpp

Resource.o: $(DEPS) $(FRAMEWORK_SRC)/Resource.cpp $(FRAMEWORK_SRC)/Resource.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/Resource.cpp

Libxml2Adapter.o: $(DEPS) $(FRAMEWORK_SRC)/Libxml2Adapter.cpp $(FRAMEWORK_SRC)/Libxml2Adapter.h $(FRAMEWORK_SRC)/XMLProcessorInterface.h
	$(CXX) -g ${CPPFLAGS} -c $(FRAMEWORK_SRC)/Libxml2Adapter.cpp

# tools
install:
	cp libframework.a $(FRAMEWORK_INSTALL)/lib

clean:
	rm -f $(OBJS) libframework.a
