/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
 *   hewsmike[AT]iinet.net.au                                              *
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
#include "SolarSystemGravity.h"

SolarSystemGravity::SolarSystemGravity() :
	SolarSystem(EinsteinS5R3Adapter::SharedMemoryIdentifier),
	m_EinsteinAdapter(&m_BoincAdapter) {
   }

SolarSystemGravity::~SolarSystemGravity() {
   }

void SolarSystemGravity::initialize(const int width, const int height, const Resource *font, const bool recycle) {
	SolarSystem::initialize(width, height, font, recycle);
   }

void SolarSystemGravity::resize(const int width, const int height) {
	SolarSystem::resize(width, height);
   }

void SolarSystemGravity::refreshBOINCInformation(void) {
	// call base class implementation
	SolarSystem::refreshLocalBOINCInformation();

	// update local/specific content
	m_EinsteinAdapter.refresh();
   }

void SolarSystemGravity::renderSearchInformation(void) {
	}

void SolarSystemGravity::generateObservatories(void) {
	}

void SolarSystemGravity::renderLogo(void) {
	}

Simulation& SolarSystemGravity::simulationInstance(void) {
    return sim;
    }
