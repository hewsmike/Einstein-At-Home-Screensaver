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

StarsphereGravity::StarsphereGravity() :
        Starsphere(EinsteinGravityAdapter::SharedMemoryIdentifier),
        m_EinsteinAdapter(&m_BoincAdapter) {
    m_logo = NULL;

    m_text_surface = NULL;

    m_right_ascension_info = NULL;
    m_declination_info = NULL;
    m_percent_done_info = NULL;
    m_cpu_time_info = NULL;
    }

StarsphereGravity::~StarsphereGravity() {
    if(m_logo) delete m_logo;
    if(m_text_surface) SDL_FreeSurface(m_text_surface);
    if(m_right_ascension_info) delete m_right_ascension_info;
    if(m_declination_info) delete m_declination_info;
    if(m_percent_done_info) delete m_percent_done_info;
    if(m_cpu_time_info) delete m_cpu_time_info;
    }

void StarsphereGravity::initialize(const int width, const int height, const Resource* font) {
    Starsphere::initialize(width, height, font);

    prepareLogo();
    prepareSearchInformation();

    }

void StarsphereGravity::resize(const int width, const int height) {
    Starsphere::resize(width, height);
    }

void StarsphereGravity::render(const double timeOfDay) {
    Starsphere::render(timeOfDay);
    const GLuint SEARCH_INFO_REFRESH_INTERVAL(500);

    // Regularly update search progress data.
    if((m_framecount % SEARCH_INFO_REFRESH_INTERVAL) == 0) {
        refreshBOINCInformation();
        prepareSearchInformation();
        }

    // Display the Open CL logo.
    if(isFeature(LOGO)) {
        m_logo->utilise();
        }

    // Show the search data, if any.
    m_right_ascension_info->utilise();
    m_declination_info->utilise();
    m_percent_done_info->utilise();
    m_cpu_time_info->utilise();
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
    if(m_QualitySetting == BOINCClientAdapter::HighGraphicsQualitySetting) {
        //glDisable(GL_POINT_SMOOTH);
        //        glDisable(GL_LINE_SMOOTH);
        }
    GLfloat wu_top_line = 100.0f;

    this->refreshBOINCInformation();
    SDL_Color color = {255, 255, 255, 255};

    if(!(m_text_surface = TTF_RenderText_Blended(m_FontText, m_WUSkyPosRightAscension.c_str(), color))){
        ErrorHandler::record("StarsphereGravity::prepareSearchInformation() : can't make SDL_Surface for right asccension!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group RA_info_texture = {(const GLvoid*)m_text_surface->pixels,
                                                        m_text_surface->w * m_text_surface->h * BYTES_PER_TEXEL,
                                                        m_text_surface->w,
                                                        m_text_surface->h,
                                                        GL_RGBA,
                                                        GL_UNSIGNED_BYTE,
                                                        GL_CLAMP_TO_EDGE,
                                                        GL_CLAMP_TO_EDGE,
                                                        false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    if(m_right_ascension_info) delete m_right_ascension_info;
    m_right_ascension_info = new TexturedParallelogram(glm::vec2(m_HUD_XRight - m_text_surface->w, m_HUD_YBottom + wu_top_line),
                                                       glm::vec2(m_text_surface->w, 0.0f),
                                                       glm::vec2(0.0f, -m_text_surface->h),
                                                       RA_info_texture);
    wu_top_line -= m_text_surface->h;

    if(!(m_text_surface = TTF_RenderText_Blended(m_FontText, m_WUSkyPosDeclination.c_str(), color))){
        ErrorHandler::record("StarsphereGravity::prepareSearchInformation() : can't make SDL_Surface for declination!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group DEC_info_texture = {(const GLvoid*)m_text_surface->pixels,
                                                         m_text_surface->w * m_text_surface->h * BYTES_PER_TEXEL,
                                                         m_text_surface->w,
                                                         m_text_surface->h,
                                                         GL_RGBA,
                                                         GL_UNSIGNED_BYTE,
                                                         GL_CLAMP_TO_EDGE,
                                                         GL_CLAMP_TO_EDGE,
                                                         false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    if(m_declination_info) delete m_declination_info;
    m_declination_info = new TexturedParallelogram(glm::vec2(m_HUD_XRight - m_text_surface->w, m_HUD_YBottom + wu_top_line),
                                                   glm::vec2(m_text_surface->w, 0.0f),
                                                   glm::vec2(0.0f, -m_text_surface->h),
                                                   DEC_info_texture);
    wu_top_line -= m_text_surface->h;

    if(!(m_text_surface = TTF_RenderText_Blended(m_FontText, m_WUPercentDone.c_str(), color))){
        ErrorHandler::record("StarsphereGravity::prepareSearchInformation() : can't make SDL_Surface for percent done!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group percent_info_texture = {(const GLvoid*)m_text_surface->pixels,
                                                             m_text_surface->w * m_text_surface->h * BYTES_PER_TEXEL,
                                                             m_text_surface->w,
                                                             m_text_surface->h,
                                                             GL_RGBA,
                                                             GL_UNSIGNED_BYTE,
                                                             GL_CLAMP_TO_EDGE,
                                                             GL_CLAMP_TO_EDGE,
                                                             false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    if(m_percent_done_info) delete m_percent_done_info;
    m_percent_done_info = new TexturedParallelogram(glm::vec2(m_HUD_XRight - m_text_surface->w, m_HUD_YBottom + wu_top_line),
                                                     glm::vec2(m_text_surface->w, 0.0f),
                                                     glm::vec2(0.0f, -m_text_surface->h),
                                                     percent_info_texture);
    wu_top_line -= m_text_surface->h;

    if(!(m_text_surface = TTF_RenderText_Blended(m_FontText, m_WUCPUTime.c_str(), color))){
        ErrorHandler::record("StarsphereGravity::prepareSearchInformation() : can't make SDL_Surface for CPU time!", ErrorHandler::FATAL);
        }

    RenderTask::texture_buffer_group CPU_time_info_texture = {(const GLvoid*)m_text_surface->pixels,
                                                              m_text_surface->w * m_text_surface->h * BYTES_PER_TEXEL,
                                                              m_text_surface->w,
                                                              m_text_surface->h,
                                                              GL_RGBA,
                                                              GL_UNSIGNED_BYTE,
                                                              GL_CLAMP_TO_EDGE,
                                                              GL_CLAMP_TO_EDGE,
                                                              false};

    // The negative Y-offset vector here is in order to invert the SDL image.
    if(m_cpu_time_info) delete m_cpu_time_info;
    m_cpu_time_info = new TexturedParallelogram(glm::vec2(m_HUD_XRight - m_text_surface->w, m_HUD_YBottom + wu_top_line),
                                                glm::vec2(m_text_surface->w, 0.0f),
                                                glm::vec2(0.0f, -m_text_surface->h),
                                                CPU_time_info_texture);
    }

void StarsphereGravity::prepareLogo() {
    // Constants for this rendering.
    const GLuint TEXTURE_WIDTH = 307;
    const GLuint TEXTURE_HEIGHT = 148;
    const GLuint LOGO_SCREEN_WIDTH = 160;
    const GLuint LOGO_SCREEN_HEIGHT = 110;

    // Create factory instance to then access the texture/bitmap.
    ResourceFactory factory;

    // Create HUD logo features.
    RenderTask::texture_buffer_group logo_texture = {(const GLvoid*)factory.createInstance("Logo_OCL")->data()->data(),
                                                      TEXTURE_WIDTH * TEXTURE_HEIGHT * BYTES_PER_TEXEL,
                                                      TEXTURE_WIDTH,
                                                      TEXTURE_HEIGHT,
                                                      GL_BGRA,
                                                      GL_UNSIGNED_BYTE,
                                                      GL_CLAMP_TO_EDGE,
                                                      GL_CLAMP_TO_EDGE,
                                                      false};

   // Decorate and prepare to place a parallelogram on the HUD.
    m_logo = new TexturedParallelogram(glm::vec2(m_HUD_XLeft, (m_HUD_YTop - m_HUD_YBottom)/2.0f + m_HUD_YBottom - LOGO_SCREEN_HEIGHT/2.0f),
                                       glm::vec2(LOGO_SCREEN_WIDTH, 0.0f),
                                       glm::vec2(0.0f, LOGO_SCREEN_HEIGHT),
                                       logo_texture);
    }
