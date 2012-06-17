###########################################################################
#   Copyright (C) 2011 by Mike Hewson                                     #
#   hewsmike@iinet.net.au                                                 #
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

# product name
PRODUCT = SolarSystem
PRODUCT_LOWER := $(shell echo $(PRODUCT) | tr A-Z a-z)

# product variants
GRAV = S5R3
RADIO = Radio
# GAMMA = Gamma  - for later on! :-)

# path settings
PRODUCT_SRC = $(PROJECT_ROOT)/src/$(PRODUCT_LOWER)
PRODUCT_INSTALL = $(PROJECT_ROOT)/install

# config values
CXX ?= g++

# variables
LIBS += -Wl,-Bstatic -lframework -loglft -lglfw -L$(PRODUCT_INSTALL)/lib
LIBS += $(shell $(PRODUCT_INSTALL)/bin/freetype-config --libs)
LIBS += $(shell $(PRODUCT_INSTALL)/bin/xml2-config --libs)
LIBS += -lboinc_api -lboinc
LIBS += -lstdc++
LIBS += -L/usr/lib
LIBS += -Wl,-Bdynamic
LIBS += -lGL -lGLU
LIBS += -lpthread -lm -lc

LDFLAGS += -static-libgcc

CPPFLAGS += -I$(PRODUCT_INSTALL)/include
CPPFLAGS += $(shell $(PRODUCT_INSTALL)/bin/freetype-config --cflags)
CPPFLAGS += $(shell $(PRODUCT_INSTALL)/bin/xml2-config --cflags)
CPPFLAGS += -I$(PRODUCT_INSTALL)/include/boinc -I/usr/include
CPPFLAGS += -I$(FRAMEWORK_SRC) -I$(PRODUCT_SRC)/../framework
CPPFLAGS += -DGL_GLEXT_PROTOTYPES

DEPS = Makefile
OBJS = $(PRODUCT).o $(PRODUCT)$(GRAV).o $(PRODUCT)$(RADIO).o

OBJS += AcceleratedPlatform.o AutoPilot.o Buffer_OBJ.o CameraState.o Constellation.o
OBJS += Constellations.o Craft.o Curve.o
OBJS += EinsteinRadioAdapter.o EinsteinS5R3Adapter.o Globe.o GridGlobe.o
OBJS += HUDBorderLayout.o HUDContainer.o HUDContent.o
OBJS += HUDFlowLayout.o HUDFlowHorizontalLayout.o HUDFlowVerticalLayout.o
OBJS += HUDImage.o HUDItem.o HUDTextLine.o HUDTextLineScroll.o
OBJS += InertialPlatform.o LookOut.o OGL_ID.o
OBJS += OrdStar.o OrthoNormalPlatform.o Path.o Pulsar.o PulsarEAH.o Renderable.o
OBJS += Simulation.o
OBJS += SolarSystemGlobals.o Sphere.o SpinPlatform.o Star.o SunOrbit.o
OBJS += Supernova.o Texture_OBJ.o TranslatablePlatform.o Traverse.o Traversable.o
OBJS += UTC.o Vector3D.o
OBJS += VectorSP.o Vertex.o

OBJS += $(RESOURCESPEC).o
RESOURCESPEC = resources

# TODO: GraphicsEngineFactory obviously depends on the actual implementations! need to change the structure! what about plugins?
CPPFLAGS += -I$(PRODUCT_SRC) -I$(PRODUCT_SRC)/../framework

# primary role based targets
# Both GW and ABP versions will be built by default !!
default: release
debug: $(PRODUCT_LOWER)
release: clean $(PRODUCT_LOWER)
$(PRODUCT_LOWER): $(PRODUCT_LOWER)_Einstein_$(GRAV) $(PRODUCT_LOWER)_Einstein_$(RADIO)
memcheck: clean debug $(PRODUCT_LOWER)
callgrind: clean debug $(PRODUCT_LOWER)

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum -std=c++0x
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum -std=c++0x
$(PRODUCT_LOWER)_Einstein_$(GRAV): CPPFLAGS += -D SCIENCE_APP=Einstein$(GRAV)
$(PRODUCT_LOWER)_Einstein_$(RADIO): CPPFLAGS += -D SCIENCE_APP=Einstein$(RADIO)
memcheck: CPPFLAGS += $(DEBUGFLAGSCPP) -D DEBUG_VALGRIND
callgrind: CPPFLAGS += $(DEBUGFLAGSCPP) -D DEBUG_VALGRIND

# file based targets
$(PRODUCT_LOWER)_Einstein_$(GRAV): $(DEPS) $(PRODUCT_SRC)/main.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(PRODUCT_SRC)/main.cpp -o "graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu" $(OBJS) $(LIBS)

$(PRODUCT_LOWER)_Einstein_$(RADIO): $(DEPS) $(PRODUCT_SRC)/main.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(PRODUCT_SRC)/main.cpp -o "graphics_app=einsteinbinary_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu" $(OBJS) $(LIBS)

$(PRODUCT).o: $(DEPS) $(PRODUCT_SRC)/$(PRODUCT).cpp $(PRODUCT_SRC)/$(PRODUCT).h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/$(PRODUCT).cpp

$(PRODUCT)$(GRAV).o: $(DEPS) $(PRODUCT_SRC)/$(PRODUCT)$(GRAV).cpp $(PRODUCT_SRC)/$(PRODUCT)$(GRAV).h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/$(PRODUCT)$(GRAV).cpp

$(PRODUCT)$(RADIO).o: $(DEPS) $(PRODUCT_SRC)/$(PRODUCT)$(RADIO).cpp $(PRODUCT_SRC)/$(PRODUCT)$(RADIO).h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/$(PRODUCT)$(RADIO).cpp
	

AcceleratedPlatform.o: $(DEPS) $(PRODUCT_SRC)/AcceleratedPlatform.cpp $(PRODUCT_SRC)/AcceleratedPlatform.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/AcceleratedPlatform.cpp
	
AutoPilot.o: $(DEPS) $(PRODUCT_SRC)/AutoPilot.cpp $(PRODUCT_SRC)/AutoPilot.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/AutoPilot.cpp	

Axes.o: $(DEPS) $(PRODUCT_SRC)/Axes.cpp $(PRODUCT_SRC)/Axes.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Axes.cpp

Buffer_OBJ.o: $(DEPS) $(PRODUCT_SRC)/Buffer_OBJ.cpp $(PRODUCT_SRC)/Buffer_OBJ.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Buffer_OBJ.cpp
	
CameraState.o: $(DEPS) $(PRODUCT_SRC)/CameraState.cpp $(PRODUCT_SRC)/CameraState.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/CameraState.cpp

Constellation.o: $(DEPS) $(PRODUCT_SRC)/Constellation.cpp $(PRODUCT_SRC)/Constellation.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Constellation.cpp

Constellations.o: $(DEPS) $(PRODUCT_SRC)/Constellations.cpp $(PRODUCT_SRC)/Constellations.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Constellations.cpp

Craft.o: $(DEPS) $(PRODUCT_SRC)/Craft.cpp $(PRODUCT_SRC)/Craft.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Craft.cpp

Curve.o: $(DEPS) $(PRODUCT_SRC)/Curve.cpp $(PRODUCT_SRC)/Curve.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Curve.cpp	

DisplayList.o: $(DEPS) $(PRODUCT_SRC)/DisplayList.cpp $(PRODUCT_SRC)/DisplayList.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/DisplayList.cpp

Ecliptic.o: $(DEPS) $(PRODUCT_SRC)/Ecliptic.cpp $(PRODUCT_SRC)/Ecliptic.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Ecliptic.cpp

EinsteinS5R3Adapter.o: $(DEPS) $(PRODUCT_SRC)/EinsteinS5R3Adapter.cpp $(PRODUCT_SRC)/EinsteinS5R3Adapter.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/EinsteinS5R3Adapter.cpp

EinsteinRadioAdapter.o: $(PRODUCT_SRC)/EinsteinRadioAdapter.cpp $(PRODUCT_SRC)/EinsteinRadioAdapter.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/EinsteinRadioAdapter.cpp

Globe.o: $(DEPS) $(PRODUCT_SRC)/Globe.cpp $(PRODUCT_SRC)/Globe.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Globe.cpp

GridGlobe.o: $(DEPS) $(PRODUCT_SRC)/GridGlobe.cpp $(PRODUCT_SRC)/GridGlobe.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/GridGlobe.cpp

HUDBorderLayout.o: $(DEPS) $(PRODUCT_SRC)/HUDBorderLayout.cpp $(PRODUCT_SRC)/HUDBorderLayout.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDBorderLayout.cpp

HUDContainer.o: $(DEPS) $(PRODUCT_SRC)/HUDContainer.cpp $(PRODUCT_SRC)/HUDContainer.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDContainer.cpp

HUDContent.o: $(DEPS) $(PRODUCT_SRC)/HUDContent.cpp $(PRODUCT_SRC)/HUDContent.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDContent.cpp

HUDFlowLayout.o: $(DEPS) $(PRODUCT_SRC)/HUDFlowLayout.cpp $(PRODUCT_SRC)/HUDFlowLayout.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDFlowLayout.cpp

HUDFlowHorizontalLayout.o: $(DEPS) $(PRODUCT_SRC)/HUDFlowHorizontalLayout.cpp $(PRODUCT_SRC)/HUDFlowHorizontalLayout.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDFlowHorizontalLayout.cpp

HUDFlowVerticalLayout.o: $(DEPS) $(PRODUCT_SRC)/HUDFlowVerticalLayout.cpp $(PRODUCT_SRC)/HUDFlowVerticalLayout.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDFlowVerticalLayout.cpp

HUDImage.o: $(DEPS) $(PRODUCT_SRC)/HUDImage.cpp $(PRODUCT_SRC)/HUDImage.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDImage.cpp

HUDItem.o: $(DEPS) $(PRODUCT_SRC)/HUDItem.cpp $(PRODUCT_SRC)/HUDItem.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDItem.cpp

HUDTextLine.o: $(DEPS) $(PRODUCT_SRC)/HUDTextLine.cpp $(PRODUCT_SRC)/HUDTextLine.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDTextLine.cpp

HUDTextLineScroll.o: $(DEPS) $(PRODUCT_SRC)/HUDTextLineScroll.cpp $(PRODUCT_SRC)/HUDTextLineScroll.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/HUDTextLineScroll.cpp

InertialPlatform.o: $(DEPS) $(PRODUCT_SRC)/InertialPlatform.cpp $(PRODUCT_SRC)/InertialPlatform.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/InertialPlatform.cpp

LookOut.o: $(DEPS) $(PRODUCT_SRC)/LookOut.cpp $(PRODUCT_SRC)/LookOut.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/LookOut.cpp

OGL_ID.o: $(DEPS) $(PRODUCT_SRC)/OGL_ID.cpp $(PRODUCT_SRC)/OGL_ID.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/OGL_ID.cpp

OrdStar.o: $(DEPS) $(PRODUCT_SRC)/OrdStar.cpp $(PRODUCT_SRC)/OrdStar.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/OrdStar.cpp

OrthoNormalPlatform.o: $(DEPS) $(PRODUCT_SRC)/OrthoNormalPlatform.cpp $(PRODUCT_SRC)/OrthoNormalPlatform.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/OrthoNormalPlatform.cpp
	
Path.o: $(DEPS) $(PRODUCT_SRC)/Path.cpp $(PRODUCT_SRC)/Path.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Path.cpp	

Pulsar.o: $(DEPS) $(PRODUCT_SRC)/Pulsar.cpp $(PRODUCT_SRC)/Pulsar.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Pulsar.cpp

PulsarEAH.o: $(DEPS) $(PRODUCT_SRC)/PulsarEAH.cpp $(PRODUCT_SRC)/PulsarEAH.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/PulsarEAH.cpp

Renderable.o: $(DEPS) $(PRODUCT_SRC)/Renderable.cpp $(PRODUCT_SRC)/Renderable.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Renderable.cpp
	
Simulation.o: $(DEPS) $(PRODUCT_SRC)/Simulation.cpp $(PRODUCT_SRC)/Simulation.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Simulation.cpp

SolarSystemGlobals.o: $(DEPS) $(PRODUCT_SRC)/SolarSystemGlobals.cpp $(PRODUCT_SRC)/SolarSystemGlobals.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SolarSystemGlobals.cpp

Sphere.o: $(DEPS) $(PRODUCT_SRC)/Sphere.cpp $(PRODUCT_SRC)/Sphere.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Sphere.cpp

SphericalCoordSystem.o: $(DEPS) $(PRODUCT_SRC)/SphericalCoordSystem.cpp $(PRODUCT_SRC)/SphericalCoordSystem.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SphericalCoordSystem.cpp

SphericalCoordSystemAngles.o: $(DEPS) $(PRODUCT_SRC)/SphericalCoordSystemAngles.cpp $(PRODUCT_SRC)/SphericalCoordSystemAngles.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SphericalCoordSystemAngles.cpp

SphericalCoordSystemLabels.o: $(DEPS) $(PRODUCT_SRC)/SphericalCoordSystemLabels.cpp $(PRODUCT_SRC)/SphericalCoordSystemLabels.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SphericalCoordSystemLabels.cpp

SpinPlatform.o: $(DEPS) $(PRODUCT_SRC)/SpinPlatform.cpp $(PRODUCT_SRC)/SpinPlatform.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SpinPlatform.cpp

Star.o: $(DEPS) $(PRODUCT_SRC)/Star.cpp $(PRODUCT_SRC)/Star.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Star.cpp

SunOrbit.o: $(DEPS) $(PRODUCT_SRC)/SunOrbit.cpp $(PRODUCT_SRC)/SunOrbit.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/SunOrbit.cpp

Supernova.o: $(DEPS) $(PRODUCT_SRC)/Supernova.cpp $(PRODUCT_SRC)/Supernova.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Supernova.cpp

Texture_OBJ.o: $(DEPS) $(PRODUCT_SRC)/Texture_OBJ.cpp $(PRODUCT_SRC)/Texture_OBJ.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Texture_OBJ.cpp

TranslatablePlatform.o: $(DEPS) $(PRODUCT_SRC)/TranslatablePlatform.cpp $(PRODUCT_SRC)/TranslatablePlatform.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/TranslatablePlatform.cpp
	
Traverse.o: $(DEPS) $(PRODUCT_SRC)/Traverse.cpp $(PRODUCT_SRC)/Traverse.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Traverse.cpp	
	
Traversable.o: $(DEPS) $(PRODUCT_SRC)/Traversable.cpp $(PRODUCT_SRC)/Traversable.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Traversable.cpp		

UTC.o: $(DEPS) $(PRODUCT_SRC)/UTC.cpp $(PRODUCT_SRC)/UTC.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/UTC.cpp

Vector3D.o: $(DEPS) $(PRODUCT_SRC)/Vector3D.cpp $(PRODUCT_SRC)/Vector3D.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Vector3D.cpp

VectorSP.o: $(DEPS) $(PRODUCT_SRC)/VectorSP.cpp $(PRODUCT_SRC)/VectorSP.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/VectorSP.cpp

Vertex.o: $(DEPS) $(PRODUCT_SRC)/Vertex.cpp $(PRODUCT_SRC)/Vertex.h
	$(CXX) -g $(CPPFLAGS) -c $(PRODUCT_SRC)/Vertex.cpp

# resource compiler
$(RESOURCESPEC).o: $(PRODUCT_SRC)/$(RESOURCESPEC).orc
	$(PRODUCT_INSTALL)/bin/orc $(PRODUCT_SRC)/$(RESOURCESPEC).orc $(RESOURCESPEC).cpp
	$(CXX) -g $(CPPFLAGS) -c $(RESOURCESPEC).cpp -o $(RESOURCESPEC).o

# tools
memcheck:
	valgrind --tool=memcheck --track-fds=yes --time-stamp=yes --log-file=$(PWD)/memcheck.out.%p --leak-check=full $(PWD)/graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu

callgrind:
	valgrind --tool=callgrind --track-fds=yes --time-stamp=yes $(PWD)/graphics_app=einstein_RUNLABEL_VERSION_graphics_i686-pc-linux-gnu

install:
	mkdir -p $(PRODUCT_INSTALL)/../dist
	cp graphics_app* $(PRODUCT_INSTALL)/../dist

clean:
	rm -f $(RESOURCESPEC).cpp $(OBJS) graphics_app*
