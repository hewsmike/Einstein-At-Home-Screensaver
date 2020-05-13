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
    m_EinsteinAdapter(&m_BoincAdapter)
{
    m_WUDispersionMeasureValue = -1.0;
    m_PowerSpectrumCoordSystemList = 0;
    m_PowerSpectrumBinList = 0;
    m_PowerSpectrumFreqBins = 0;
}

StarsphereGamma::~StarsphereGamma()
{
}

void StarsphereGamma::initialize(const int width, const int height, const Resource* font) {
    Starsphere::initialize(width, height, font);

    // store quality setting
    m_QualitySetting = m_BoincAdapter.graphicsQualitySetting();

    /// TODO - retain this code ? Alternate method/flag ?
//    // check whether we initialize the first time or have to recycle (required for windoze)
//    if(!recycle) {
//
//        // adjust HUD config
//        m_YOffsetMedium = 15.0;
//        m_XStartPosRightWidthOffset = 125.0;
//        m_XStartPosRight = width - m_XStartPosRightWidthOffset;
//        m_YStartPosBottom = 100.0;
//        m_Y1StartPosBottom = m_YStartPosBottom  - m_YOffsetMedium;
//        m_Y2StartPosBottom = m_Y1StartPosBottom - m_YOffsetMedium;
//        m_Y3StartPosBottom = m_Y2StartPosBottom - m_YOffsetMedium;
//        m_Y4StartPosBottom = m_Y3StartPosBottom - m_YOffsetMedium;
//        m_Y5StartPosBottom = m_Y4StartPosBottom - m_YOffsetMedium;
//        m_Y6StartPosBottom = m_Y5StartPosBottom - m_YOffsetMedium;
//
//        // adjust Power Spectrum config
//        m_PowerSpectrumWidth = 200.0;
//        m_PowerSpectrumHeight = 50.0;
//        m_PowerSpectrumOriginWidthOffset = 210.0;
//        m_PowerSpectrumOriginHeightOffset = 60.0;
//        m_PowerSpectrumXPos = width - m_PowerSpectrumOriginWidthOffset;
//        m_PowerSpectrumYPos = height - m_PowerSpectrumOriginHeightOffset;
//        m_PowerSpectrumAxesWidth = 2.0;
//        m_PowerSpectrumBinWidth = 3.0;
//        m_PowerSpectrumBinDistance = 2.0;
//        m_PowerSpectrumLabelXOffset = (m_PowerSpectrumWidth - 150.0) / 2;
//        m_PowerSpectrumLabelYOffset = 15.0;
//        m_PowerSpectrumLabelXPos = m_PowerSpectrumXPos + m_PowerSpectrumLabelXOffset;
//        m_PowerSpectrumLabelYPos = m_PowerSpectrumYPos - m_PowerSpectrumLabelYOffset;
//    }

    // create large font instances using font resource (base address + size)
//    m_FontLogo1 = new OGLFT::Translucent(
//                                &m_FontResource->data()->at(0),
//                                m_FontResource->data()->size(),
//                                26, 78 );
//
//    if ( m_FontLogo1 == 0 || !m_FontLogo1->isValid() ) {
//         cerr << "Could not construct logo1 font face from in memory resource!" << endl;
//         return;
//    }
//
//    m_FontLogo1->setForegroundColor(1.0, 1.0, 0.0, 1.0);
//
//    // create medium font instances using font resource (base address + size)
//    m_FontLogo2 = new OGLFT::Translucent(
//                                &m_FontResource->data()->at(0),
//                                m_FontResource->data()->size(),
//                                12, 72 );
//
//    if ( m_FontLogo2 == 0 || !m_FontLogo2->isValid() ) {
//         cerr << "Could not construct logo2 font face from in memory resource!" << endl;
//         return;
//    }
//
//    m_FontLogo2->setForegroundColor(0.75, 0.75, 0.75, 1.0);

    // prepare power spectrum
    generatePowerSpectrumCoordSystem(m_PowerSpectrumXPos, m_PowerSpectrumYPos);

    // prepare base class observatories (dimmed to 33%)
    //generateObservatories(0.33);
}

void StarsphereGamma::resize(const int width, const int height)
{
    Starsphere::resize(width, height);

    // update HUD config
    m_XStartPosRight = width - m_XStartPosRightWidthOffset;


    // update power spectrum
    m_PowerSpectrumXPos = width - m_PowerSpectrumOriginWidthOffset;
    m_PowerSpectrumYPos = height - m_PowerSpectrumOriginHeightOffset;
    m_PowerSpectrumLabelXPos = m_PowerSpectrumXPos + m_PowerSpectrumLabelXOffset;
    m_PowerSpectrumLabelYPos = m_PowerSpectrumYPos - m_PowerSpectrumLabelYOffset;

    generatePowerSpectrumCoordSystem(m_PowerSpectrumXPos, m_PowerSpectrumYPos);
    generatePowerSpectrumBins(m_PowerSpectrumXPos, m_PowerSpectrumYPos);
}

void StarsphereGamma::renderAdditionalObservatories() {
//    glCallList(m_areciboObservatory);
}

void StarsphereGamma::refreshBOINCInformation()
{
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

    if(m_WUDispersionMeasureValue != m_EinsteinAdapter.wuDispersionMeasure()) {
        // we've got a new dispersion measure, update HUD
        m_WUDispersionMeasureValue = m_EinsteinAdapter.wuDispersionMeasure();
        buffer << "DM: " << fixed << m_WUDispersionMeasureValue << " pc/cm3" << ends;
        m_WUDispersionMeasure = buffer.str();
        buffer.str("");
        }

    // update the following information every time (no need to check first)

    buffer.precision(3);
    buffer << "Orb. Radius: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalRadius() << " ls" << ends;
    m_WUTemplateOrbitalRadius = buffer.str();
    buffer.str("");

    buffer.precision(0);
    buffer << "Orb. Period: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalPeriod() << " s" << ends;
    m_WUTemplateOrbitalPeriod = buffer.str();
    buffer.str("");

    buffer.precision(2);
    buffer << "Orb. Phase: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalPhase() << " rad" << ends;
    m_WUTemplateOrbitalPhase = buffer.str();
    buffer.str("");

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

    // update power spectrum bin data
    generatePowerSpectrumBins(m_PowerSpectrumXPos, m_PowerSpectrumYPos);
}

void StarsphereGamma::renderSearchInformation() {
    // disable opt-in quality feature for power spectrum
    if(m_QualitySetting == BOINCClientAdapter::HighGraphicsQualitySetting) {
//        glDisable(GL_POINT_SMOOTH);
//        glDisable(GL_LINE_SMOOTH);
    }

    }

void StarsphereGamma::generatePowerSpectrumCoordSystem(const int originX, const int originY)
{
    GLfloat offsetX = (GLfloat)originX;
    GLfloat offsetY = (GLfloat)originY;

    // delete existing, create new (required for windoze)
//    if(m_PowerSpectrumCoordSystemList) glDeleteLists(m_PowerSpectrumCoordSystemList, 1);
//    m_PowerSpectrumCoordSystemList = glGenLists(1);
//    glNewList(m_PowerSpectrumCoordSystemList, GL_COMPILE);
//
//        glLineWidth(m_PowerSpectrumAxesWidth);
//
//        // draw coordinate system axes
//        glBegin(GL_LINE_STRIP);
//            glColor4f(1.0, 1.0, 0.0, 1.0);
//            glVertex2f(offsetX, offsetY + m_PowerSpectrumHeight);
//            glVertex2f(offsetX, offsetY);
//            glVertex2f(offsetX + m_PowerSpectrumWidth + 1, offsetY);
//        glEnd();
//
//        glPointSize(m_PowerSpectrumAxesWidth);
//
//        // draw origin (axes joint)
//        glBegin(GL_POINTS);
//            glColor4f(1.0, 1.0, 0.0, 1.0);
//            glVertex2f(offsetX, offsetY);
//        glEnd();
//
//        //TODO: for high quality mode: draw coord. system backdrop with alpha = ~0.3 (attn: alpha blend. deactivated!)
//
//    glEndList();
}

void StarsphereGamma::generatePowerSpectrumBins(const int originX, const int originY)
{
    GLfloat offsetX = (GLfloat)originX;
    GLfloat offsetY = (GLfloat)originY;
    GLfloat axesXOffset = m_PowerSpectrumAxesWidth + 2;
    GLfloat axesYOffset = m_PowerSpectrumAxesWidth / 2.0 + 1;
    GLfloat binXOffset = m_PowerSpectrumBinWidth + m_PowerSpectrumBinDistance;

    // set pixel normalization factor for maximum bin height
    GLfloat normalizationFactor = 255.0 / (m_PowerSpectrumHeight - axesYOffset);

    // fetch pointer to power spectrum data
    m_PowerSpectrumFreqBins = m_EinsteinAdapter.wuTemplatePowerSpectrum();
    if(!m_PowerSpectrumFreqBins) {
        cerr << "Power spectrum data currently unavailable!" << endl;
        return;
    }

    // delete existing, create new (required for windoze)
//    if(m_PowerSpectrumBinList) glDeleteLists(m_PowerSpectrumBinList, 1);
//    m_PowerSpectrumBinList = glGenLists(1);
//    glNewList(m_PowerSpectrumBinList, GL_COMPILE);
//
//        glLineWidth(m_PowerSpectrumBinWidth);
//
//        // draw frequency bins
//        glBegin(GL_LINES);
//            // iterate over all bins
//            for(int i = 0; i < POWERSPECTRUM_BINS; ++i) {
//                // show potential candidates (power >= 100)...
//                if(m_PowerSpectrumFreqBins->at(i) >= 100) {
//                     // ...in bright white
//                    glColor4f(1.0, 1.0, 1.0, 1.0);
//                }
//                else {
//                    // ...in light grey
//                    glColor4f(0.66, 0.66, 0.66, 1.0);
//                }
//                // lower vertex
//                glVertex2f(offsetX + axesXOffset + i*binXOffset,
//                           offsetY + axesYOffset);
//                // upper vertex
//                glVertex2f(offsetX + axesXOffset + i*binXOffset,
//                           offsetY + axesYOffset + m_PowerSpectrumFreqBins->at(i) / normalizationFactor);
//            }
//        glEnd();
//
//    glEndList();
}

void StarsphereGamma::renderLogo()
{
//    m_FontLogo1->draw(m_XStartPosLeft, m_YStartPosTop, "Einstein@Home");
//    m_FontLogo2->draw(m_XStartPosLeft, m_YStartPosTop - m_YOffsetLarge, "International Year of Astronomy 2009");
}
