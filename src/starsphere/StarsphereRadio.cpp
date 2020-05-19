/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   Copyright (C) 2020 by Mike Hewson                                     *
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

#include "StarsphereRadio.h"

StarsphereRadio::StarsphereRadio() :
    Starsphere(EinsteinRadioAdapter::SharedMemoryIdentifier),
    m_EinsteinAdapter(&m_BoincAdapter) {
    }

StarsphereRadio::~StarsphereRadio() {
    }

void StarsphereRadio::initialize(const int width, const int height, const Resource* font) {
    Starsphere::initialize(width, height, font);

    // store quality setting
    m_QualitySetting = m_BoincAdapter.graphicsQualitySetting();
    }

void StarsphereRadio::resize(const int width, const int height) {
    Starsphere::resize(width, height);
    }

void StarsphereRadio::refreshBOINCInformation() {
    // call base class implementation
    Starsphere::refreshLocalBOINCInformation();

    // update local/specific content
    m_EinsteinAdapter.refresh();

    // prepare conversion buffer
    stringstream buffer;
    buffer.precision(2);
    buffer.setf(ios::fixed, ios::floatfield);
    buffer.fill('0');
    buffer.setf(ios::right, ios::adjustfield);

    // store content required for our HUD (search info)
    if(m_CurrentRightAscension != m_EinsteinAdapter.wuSkyPosRightAscension()) {
        // we've got a new position, update search marker and HUD
        m_CurrentRightAscension = m_EinsteinAdapter.wuSkyPosRightAscension();
        m_RefreshSearchMarker = true;
        buffer << "Ascension: " << fixed << m_CurrentRightAscension << " deg" << ends;
        m_WUSkyPosRightAscension = buffer.str();
        buffer.str("");
        }

    if(m_CurrentDeclination != m_EinsteinAdapter.wuSkyPosDeclination()) {
        // we've got a new position, update search marker and HUD
        m_CurrentDeclination = m_EinsteinAdapter.wuSkyPosDeclination();
        m_RefreshSearchMarker = true;
        buffer << "Declination: " << fixed << m_CurrentDeclination << " deg" << ends;
        m_WUSkyPosDeclination = buffer.str();
        buffer.str("");
        }

    buffer << "WU Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
    m_WUPercentDone = buffer.str();
    buffer.str("");

    // show WU's total CPU time (previously accumulated + current session)
    const double timeCPU = m_BoincAdapter.wuCPUTimeSpent() + m_EinsteinAdapter.wuCPUTime();
    const int hrs = timeCPU / 3600;
    const int min = fmod(timeCPU, 3600) / 60;
    const int sec = fmod(timeCPU, 60);

    buffer << "WU CPU Time: " << right << setw(2) << hrs << ":"
                              << right << setw(2) << min << ":"
                              << right << setw(2) << sec << ends;

    m_WUCPUTime = buffer.str();
    }

void StarsphereRadio::prepareSearchInformation() {
    // disable opt-in quality feature for power spectrum
    if(m_QualitySetting == BOINCClientAdapter::HighGraphicsQualitySetting) {
        }
    }

void StarsphereRadio::prepareLogo() {
    }
