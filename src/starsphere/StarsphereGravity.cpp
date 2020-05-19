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

#include "StarsphereGravity.h"

#include <time.h>

#include "ErrorHandler.h"
#include "ResourceFactory.h"
#include "TransformGlobals.h"

StarsphereGravity::StarsphereGravity() :
        Starsphere(EinsteinGravityAdapter::SharedMemoryIdentifier),
        m_EinsteinAdapter(&m_BoincAdapter) {
    m_logo = NULL;
    }

StarsphereGravity::~StarsphereGravity() {
    if(m_logo) delete m_logo;
    }

void StarsphereGravity::initialize(const int width, const int height, const Resource* font) {
    Starsphere::initialize(width, height, font);

    refreshBOINCInformation();

    prepareLogo();
    prepareSearchInformation();
    }

void StarsphereGravity::resize(const int width, const int height) {
    Starsphere::resize(width, height);
    }

void StarsphereGravity::render(const double timeOfDay) {
    Starsphere::render(timeOfDay);

    if(isFeature(LOGO)) {
        m_logo->utilise();
        }
    }

void StarsphereGravity::refreshBOINCInformation() {
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
        buffer.str("");
        buffer << "Ascension: " << fixed << m_CurrentRightAscension << " deg" << ends;
        m_WUSkyPosRightAscension = buffer.str();
        }

    if(m_CurrentDeclination != m_EinsteinAdapter.wuSkyPosDeclination()) {
        // we've got a new position, update search marker and HUD
        m_CurrentDeclination = m_EinsteinAdapter.wuSkyPosDeclination();
        m_RefreshSearchMarker = true;
        buffer.str("");
        buffer << "Declination: " << fixed << m_CurrentDeclination << " deg" << ends;
        m_WUSkyPosDeclination = buffer.str();
        }

    buffer.str("");
    buffer << "Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
    m_WUPercentDone = buffer.str();

    // show WU's total CPU time (previously accumulated + current session)
    double timeCPU = m_BoincAdapter.wuCPUTimeSpent() + m_EinsteinAdapter.wuCPUTime();
    int hrs = timeCPU / 3600;
    int min = fmod(timeCPU, 3600) / 60;
    int sec = fmod(timeCPU, 60);

    buffer.str("");
    buffer << "CPU Time: "  << right << setw(2) << hrs << ":"
                            << right << setw(2) << min << ":"
                            << right << setw(2) << sec << ends;

    m_WUCPUTime = buffer.str();
    }

void StarsphereGravity::prepareSearchInformation() {
//        // clock
//        m_FontLogo1->draw(m_XStartPosClock, m_YStartPosTop, m_CurrentTime.c_str());
//
//        // left info block
//        m_FontHeader->draw(m_XStartPosLeft, m_YStartPosBottom, "BOINC Statistics");
//        m_FontText->draw(m_XStartPosLeft, m_Y1StartPosBottom, m_UserName.c_str());
//        m_FontText->draw(m_XStartPosLeft, m_Y2StartPosBottom, m_TeamName.c_str());
//        m_FontText->draw(m_XStartPosLeft, m_Y3StartPosBottom, m_UserCredit.c_str());
//        m_FontText->draw(m_XStartPosLeft, m_Y4StartPosBottom, m_UserRACredit.c_str());
//
//        // right info block
//        m_FontHeader->draw(m_XStartPosRight, m_YStartPosBottom, "Search Information");
//        m_FontText->draw(m_XStartPosRight, m_Y1StartPosBottom, m_WUSkyPosRightAscension.c_str());
//        m_FontText->draw(m_XStartPosRight, m_Y2StartPosBottom, m_WUSkyPosDeclination.c_str());
//        m_FontText->draw(m_XStartPosRight, m_Y3StartPosBottom, m_WUPercentDone.c_str());
//        m_FontText->draw(m_XStartPosRight, m_Y4StartPosBottom, m_WUCPUTime.c_str());
    }

void StarsphereGravity::prepareLogo() {
    // Create factory instance to then access the texture/bitmap.
    ResourceFactory factory;

    // Create HUD logo features.
    RenderTask::texture_buffer_group logo_texture = {(const GLvoid*)factory.createInstance("Logo_OCL")->data()->data(),
                                                      330*150*4,
                                                      330,
                                                      150,
                                                      GL_BGRA,
                                                      GL_UNSIGNED_BYTE,
                                                      GL_CLAMP_TO_EDGE,
                                                      GL_CLAMP_TO_EDGE,
                                                      false};

    m_logo = new TexturedParallelogram(glm::vec2(10.0f, 10.0f + (m_YStartPosTop - 10 - 50)/2),
                                       glm::vec2(220.0f, 0.0f),
                                       glm::vec2(0.0f, 100.0f),
                                       logo_texture);
    }
