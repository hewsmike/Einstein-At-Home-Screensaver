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

#include "StarsphereGamma.h"

StarsphereGamma::StarsphereGamma() :
    Starsphere(EinsteinGammaAdapter::SharedMemoryIdentifier),
    m_EinsteinAdapter(&m_BoincAdapter) {
    m_right_ascension_info = NULL;
    m_declination_info = NULL;
    m_percent_done_info = NULL;
    m_cpu_time_info = NULL;
    }

StarsphereGamma::~StarsphereGamma() {
    if(m_logo) delete m_logo;
    }

void StarsphereGamma::initialize(const int width, const int height, const Resource* font) {
    Starsphere::initialize(width, height, font);

    prepareLogo();

    prepareSearchInformation();

    // store quality setting
    m_QualitySetting = m_BoincAdapter.graphicsQualitySetting();
    }

void StarsphereGamma::resize(const int width, const int height) {
    Starsphere::resize(width, height);
    }

void StarsphereGamma::render(const double timeOfDay) {
    Starsphere::render(timeOfDay);

    if(isFeature(LOGO)) {
        m_logo->utilise();
        }

    m_right_ascension_info->utilise();
    m_declination_info->utilise();
    m_percent_done_info->utilise();
    m_cpu_time_info->utilise();
    }

void StarsphereGamma::refreshBOINCInformation() {
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

void StarsphereGamma::prepareSearchInformation() {
    if(m_QualitySetting == BOINCClientAdapter::HighGraphicsQualitySetting) {
        //glDisable(GL_POINT_SMOOTH);
        //        glDisable(GL_LINE_SMOOTH);
        }
    GLfloat wu_top_line = 100.0f;

    this->refreshBOINCInformation();
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* text_surface;
    if(!(text_surface=TTF_RenderText_Blended(m_FontText, m_WUSkyPosRightAscension.c_str(), color))){
        ErrorHandler::record("StarsphereGamma::renderSearchInformation() : can't make SDL_Surface for right asccension!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group RA_info_texture = {(const GLvoid*)text_surface->pixels,
                                                        text_surface->w * text_surface->h * 4,
                                                        text_surface->w,
                                                        text_surface->h,
                                                        GL_RGBA,
                                                        GL_UNSIGNED_BYTE,
                                                        GL_CLAMP_TO_EDGE,
                                                        GL_CLAMP_TO_EDGE,
                                                        false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    m_right_ascension_info = new TexturedParallelogram(glm::vec2(m_XStartPosRight - text_surface->w * 2, 10.0f + wu_top_line),
                                                       glm::vec2(text_surface->w * 2, 0.0f),
                                                       glm::vec2(0.0f, -text_surface->h * 2),
                                                       RA_info_texture);
    wu_top_line -= text_surface->h *2;

    if(!(text_surface=TTF_RenderText_Blended(m_FontText, m_WUSkyPosDeclination.c_str(), color))){
        ErrorHandler::record("StarsphereGamma::renderSearchInformation() : can't make SDL_Surface for declination!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group DEC_info_texture = {(const GLvoid*)text_surface->pixels,
                                                         text_surface->w * text_surface->h * 4,
                                                         text_surface->w,
                                                         text_surface->h,
                                                         GL_RGBA,
                                                         GL_UNSIGNED_BYTE,
                                                         GL_CLAMP_TO_EDGE,
                                                         GL_CLAMP_TO_EDGE,
                                                         false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    m_declination_info = new TexturedParallelogram(glm::vec2(m_XStartPosRight - text_surface->w * 2, 10.0f + wu_top_line),
                                                   glm::vec2(text_surface->w * 2, 0.0f),
                                                   glm::vec2(0.0f, -text_surface->h * 2),
                                                   DEC_info_texture);

    wu_top_line -= text_surface->h *2;

    if(!(text_surface=TTF_RenderText_Blended(m_FontText, m_WUPercentDone.c_str(), color))){
        ErrorHandler::record("StarsphereGamma::renderSearchInformation() : can't make SDL_Surface for percent done!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group percent_info_texture = {(const GLvoid*)text_surface->pixels,
                                                             text_surface->w * text_surface->h * 4,
                                                             text_surface->w,
                                                             text_surface->h,
                                                             GL_RGBA,
                                                             GL_UNSIGNED_BYTE,
                                                             GL_CLAMP_TO_EDGE,
                                                             GL_CLAMP_TO_EDGE,
                                                             false};

    // The negative Y-offset vector here is in order to invert the SDL image.
     m_percent_done_info = new TexturedParallelogram(glm::vec2(m_XStartPosRight - text_surface->w * 2, 10.0f + wu_top_line),
                                                     glm::vec2(text_surface->w * 2, 0.0f),
                                                     glm::vec2(0.0f, -text_surface->h * 2),
                                                     percent_info_texture);
    wu_top_line -= text_surface->h *2;

     if(!(text_surface=TTF_RenderText_Blended(m_FontText, m_WUCPUTime.c_str(), color))){
        ErrorHandler::record("StarsphereGamma::renderSearchInformation() : can't make SDL_Surface for CPU time!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group CPU_time_info_texture = {(const GLvoid*)text_surface->pixels,
                                                              text_surface->w * text_surface->h * 4,
                                                              text_surface->w,
                                                              text_surface->h,
                                                              GL_RGBA,
                                                              GL_UNSIGNED_BYTE,
                                                              GL_CLAMP_TO_EDGE,
                                                              GL_CLAMP_TO_EDGE,
                                                              false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    m_cpu_time_info = new TexturedParallelogram(glm::vec2(m_XStartPosRight - text_surface->w * 2, 10.0f + wu_top_line),
                                                glm::vec2(text_surface->w * 2, 0.0f),
                                                glm::vec2(0.0f, -text_surface->h * 2),
                                                CPU_time_info_texture);

    delete text_surface;
    }

void StarsphereGamma::prepareLogo() {
 // Create factory instance to then access the texture/bitmap.
    ResourceFactory factory;

    // Create HUD logo features.
    RenderTask::texture_buffer_group logo_texture = {(const GLvoid*)factory.createInstance("Logo_FERMI")->data()->data(),
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
