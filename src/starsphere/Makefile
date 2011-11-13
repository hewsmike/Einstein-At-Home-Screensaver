###########################################################################
#   Copyright (C) 2008 by Oliver Bock                                     #
#   oliver.bock[AT]aei.mpg.de                                             #
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
STARSPHERE_SRC?=$(PWD)
STARSPHERE_INSTALL?=$(PWD)

# config values
CXX ?= g++

# variables
LIBS += -Wl,-Bstatic -lframework -loglft -L$(STARSPHERE_INSTALL)/lib
LIBS += $(shell $(STARSPHERE_INSTALL)/bin/freetype-config --libs)
LIBS += $(shell $(STARSPHERE_INSTALL)/bin/xml2-config --libs)
LIBS += -lboinc_api -lboinc
LIBS += -lstdc++
LIBS += -L/usr/lib
LIBS += -Wl,-Bdynamic $(shell $(STARSPHERE_INSTALL)/bin/sdl-config --static-libs)
LIBS += -lGL -lGLU
LIBS += -lpthread -lm -lc

LDFLAGS += -static-libgcc

CPPFLAGS += -I$(STARSPHERE_INSTALL)/include
CPPFLAGS += $(shell $(STARSPHERE_INSTALL)/bin/sdl-config --cflags)
CPPFLAGS += $(shell $(STARSPHERE_INSTALL)/bin/freetype-config --cflags)
CPPFLAGS += $(shell $(STARSPHERE_INSTALL)/bin/xml2-config --cflags)
CPPFLAGS += -I$(STARSPHERE_INSTALL)/include/boinc -I/usr/include

DEPS = Makefile
OBJS = Starsphere.o StarsphereS5R3.o StarsphereRadio.o EinsteinS5R3Adapter.o
OBJS += EinsteinRadioAdapter.o starlist.o snr_list.o pulsar_list.o $(RESOURCESPEC).o
OBJS += Vector3D.o VectorSPR.o
RESOURCESPEC = resources

# TODO: GraphicsEngineFactory obviously depends on the actual implementations (here starsphere)! need to change the structure! what about plugins?
CPPFLAGS += -I$(STARSPHERE_SRC) -I$(STARSPHERE_SRC)/../framework

# primary role based tagets
default: release
debug: starsphere
release: clean starsphere
starsphere: starsphere_Einstein_LVC starsphere_Einstein_ABP
memcheck: clean debug starsphere
callgrind: clean debug starsphere

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum
starsphere_Einstein_LVC: CPPFLAGS += -D SCIENCE_APP=EinsteinS5R3
starsphere_Einstein_ABP: CPPFLAGS += -D SCIENCE_APP=EinsteinRadio
memcheck: CPPFLAGS += $(DEBUGFLAGSCPP) -D DEBUG_VALGRIND
callgrind: CPPFLAGS += $(DEBUGFLAGSCPP) -D DEBUG_VALGRIND

# file based targets
starsphere_Einstein_LVC: $(DEPS) $(STARSPHERE_SRC)/main.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(STARSPHERE_SRC)/main.cpp -o "graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu" $(OBJS) $(LIBS)

starsphere_Einstein_ABP: $(DEPS) $(STARSPHERE_SRC)/main.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(STARSPHERE_SRC)/main.cpp -o "graphics_app=einsteinbinary_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu" $(OBJS) $(LIBS)

Starsphere.o: $(DEPS) $(STARSPHERE_SRC)/Starsphere.cpp $(STARSPHERE_SRC)/Starsphere.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/Starsphere.cpp

StarsphereS5R3.o: $(DEPS) $(STARSPHERE_SRC)/StarsphereS5R3.cpp $(STARSPHERE_SRC)/StarsphereS5R3.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/StarsphereS5R3.cpp

StarsphereRadio.o: $(DEPS) $(STARSPHERE_SRC)/StarsphereRadio.cpp $(STARSPHERE_SRC)/StarsphereRadio.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/StarsphereRadio.cpp

EinsteinS5R3Adapter.o: Makefile $(STARSPHERE_SRC)/EinsteinS5R3Adapter.cpp $(STARSPHERE_SRC)/EinsteinS5R3Adapter.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/EinsteinS5R3Adapter.cpp

EinsteinRadioAdapter.o: Makefile $(STARSPHERE_SRC)/EinsteinRadioAdapter.cpp $(STARSPHERE_SRC)/EinsteinRadioAdapter.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/EinsteinRadioAdapter.cpp

starlist.o: $(DEPS) $(STARSPHERE_SRC)/starlist.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/starlist.C

snr_list.o: $(DEPS) $(STARSPHERE_SRC)/snr_list.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/snr_list.C

pulsar_list.o: $(DEPS) $(STARSPHERE_SRC)/pulsar_list.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/pulsar_list.C

Vector3D.o: $(DEPS) $(STARSPHERE_SRC)/Vector3D.cpp $(STARSPHERE_SRC)/Vector3D.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/Vector3D.cpp

VectorSPR.o: $(DEPS) $(STARSPHERE_SRC)/VectorSPR.cpp $(STARSPHERE_SRC)/VectorSPR.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/VectorSPR.cpp

# resource compiler
$(RESOURCESPEC).o: $(STARSPHERE_SRC)/$(RESOURCESPEC).orc
	$(STARSPHERE_INSTALL)/bin/orc $(STARSPHERE_SRC)/$(RESOURCESPEC).orc $(RESOURCESPEC).cpp
	$(CXX) -g $(CPPFLAGS) -c $(RESOURCESPEC).cpp -o $(RESOURCESPEC).o


# tools
memcheck:
	valgrind --tool=memcheck --track-fds=yes --time-stamp=yes --log-file=$(PWD)/memcheck.out.%p --leak-check=full $(PWD)/graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu

callgrind:
	valgrind --tool=callgrind --track-fds=yes --time-stamp=yes $(PWD)/graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu

install:
	mkdir -p $(STARSPHERE_INSTALL)/../dist
	cp graphics_app* $(STARSPHERE_INSTALL)/../dist

clean:
	rm -f $(RESOURCESPEC).cpp $(OBJS) graphics_app*
