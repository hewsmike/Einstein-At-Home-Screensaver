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

#include "EinsteinGammaAdapter.h"

#include <sstream>

const string EinsteinGammaAdapter::SharedMemoryIdentifier = "EinsteinHS";

EinsteinGammaAdapter::EinsteinGammaAdapter(BOINCClientAdapter *boincClient) {
    this->boincClient = boincClient;


    m_WUSkyPosRightAscension = 0.0;
    m_WUSkyPosDeclination = 0.0;
    m_WUFractionDone = 0.0;
    m_WUCPUTime = 0.0;
    }

EinsteinGammaAdapter::~EinsteinGammaAdapter() {
    }

void EinsteinGammaAdapter::refresh() {
    boincClient->refresh();
    parseApplicationInformation();
    }

void EinsteinGammaAdapter::parseApplicationInformation() {
    // get updated application information
    string info = boincClient->applicationInformation();

    // do we have any data?
    if(info.length() > 0) {

        // parse data into members
        if(4 != sscanf(info.c_str(),
                "<graphics_info>\n"
                "  <skypos_rac>%lf</skypos_rac>\n"
                "  <skypos_dec>%lf</skypos_dec>\n"
                "  <fraction_done>%lf</fraction_done>\n"
                "  <cpu_time>%lf</cpu_time>\n",
                &m_WUSkyPosRightAscension,
                &m_WUSkyPosDeclination,
                &m_WUFractionDone,
                &m_WUCPUTime)) {
            cerr << "Incompatible shared memory data encountered!" << endl;
            }
        else {
            // convert radians to degrees
            m_WUSkyPosRightAscension *= 180/PI;
            m_WUSkyPosDeclination *= 180/PI;
            }
        }
    }

double EinsteinGammaAdapter::wuSkyPosRightAscension() const {
    return m_WUSkyPosRightAscension;
    }

double EinsteinGammaAdapter::wuSkyPosDeclination() const {
    return m_WUSkyPosDeclination;
    }

double EinsteinGammaAdapter::wuFractionDone() const {
    return m_WUFractionDone;
    }

double EinsteinGammaAdapter::wuCPUTime() const {
    return m_WUCPUTime;
    }
