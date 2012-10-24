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

#include "Simulation.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string Simulation::EARTH_NAME("Earth");
const std::string Simulation::EARTH_IMAGE_RESOURCE("EarthTGA");
const GLuint Simulation::EARTH_STACKS(37);
const GLuint Simulation::EARTH_SLICES(72);
const GLfloat Simulation::EARTH_TEXTURE_OFFSET(+0.5f);

const std::string Simulation::SUN_NAME("Sun");
const std::string Simulation::SUN_IMAGE_RESOURCE("SunTGA");
const GLuint Simulation::SUN_STACKS(37);
const GLuint Simulation::SUN_SLICES(72);
const GLfloat Simulation::SUN_TEXTURE_OFFSET(0.0f);

const GLfloat Simulation::AT_INFINITY(0.0f);

const GLuint Simulation::CONSTELLATIONS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS);

const GLuint Simulation::PULSARS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 25);
const GLfloat Simulation::PULSARS_MAG_SIZE(2.0f);
const GLfloat Simulation::PULSARS_RGB_RED(0.35f);
const GLfloat Simulation::PULSARS_RGB_GREEN(0.15f);
const GLfloat Simulation::PULSARS_RGB_BLUE(0.38f);

const GLuint Simulation::PULSARS_EAH_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 75);
const GLfloat Simulation::PULSARS_EAH_MAG_SIZE(4.0f);
const GLfloat Simulation::PULSARS_EAH_RGB_RED(1.00f);
const GLfloat Simulation::PULSARS_EAH_RGB_GREEN(0.60f);
const GLfloat Simulation::PULSARS_EAH_RGB_BLUE(0.20f);

const GLuint Simulation::SUPERNOVAE_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 50);
const GLfloat Simulation::SUPERNOVAE_MAG_SIZE(2.0f);
const GLfloat Simulation::SUPERNOVAE_RGB_RED(0.40f);
const GLfloat Simulation::SUPERNOVAE_RGB_GREEN(0.35f);
const GLfloat Simulation::SUPERNOVAE_RGB_BLUE(1.00f);

const GLuint Simulation::SKYGRID_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS + 50);
const GLuint Simulation::SKYGRID_STACKS(19);
const GLuint Simulation::SKYGRID_SLICES(24);
const GLfloat Simulation::SKYGRID_MAIN_WIDTH(0.5f);
const GLfloat Simulation::SKYGRID_MAIN_RED(0.12f);
const GLfloat Simulation::SKYGRID_MAIN_GREEN(0.17f);
const GLfloat Simulation::SKYGRID_MAIN_BLUE(0.12f);
const GLfloat Simulation::SKYGRID_CELESTIAL_EQUATOR_WIDTH(1.0f);
const GLfloat Simulation::SKYGRID_CELESTIAL_EQUATOR_RED(0.24f);
const GLfloat Simulation::SKYGRID_CELESTIAL_EQUATOR_GREEN(0.34f);
const GLfloat Simulation::SKYGRID_CELESTIAL_EQUATOR_BLUE(0.24f);
const GLfloat Simulation::SKYGRID_PRIME_MERIDIAN_WIDTH(1.0f);
const GLfloat Simulation::SKYGRID_PRIME_MERIDIAN_RED(0.36f);
const GLfloat Simulation::SKYGRID_PRIME_MERIDIAN_GREEN(0.51f);
const GLfloat Simulation::SKYGRID_PRIME_MERIDIAN_BLUE(0.36f);

const GLuint Simulation::EARTHGRID_RADIUS(SolarSystemGlobals::EARTH_RADIUS + 2);
const GLuint Simulation::EARTHGRID_STACKS(19);
const GLuint Simulation::EARTHGRID_SLICES(24);
const GLfloat Simulation::EARTHGRID_MAIN_WIDTH(0.5f);
const GLfloat Simulation::EARTHGRID_MAIN_RED(0.95f);
const GLfloat Simulation::EARTHGRID_MAIN_GREEN(0.74f);
const GLfloat Simulation::EARTHGRID_MAIN_BLUE(0.00f);
const GLfloat Simulation::EARTHGRID_CELESTIAL_EQUATOR_WIDTH(1.0f);
const GLfloat Simulation::EARTHGRID_CELESTIAL_EQUATOR_RED(0.77f);
const GLfloat Simulation::EARTHGRID_CELESTIAL_EQUATOR_GREEN(0.00f);
const GLfloat Simulation::EARTHGRID_CELESTIAL_EQUATOR_BLUE(0.28f);
const GLfloat Simulation::EARTHGRID_PRIME_MERIDIAN_WIDTH(1.0f);
const GLfloat Simulation::EARTHGRID_PRIME_MERIDIAN_RED(1.00f);
const GLfloat Simulation::EARTHGRID_PRIME_MERIDIAN_GREEN(0.20f);
const GLfloat Simulation::EARTHGRID_PRIME_MERIDIAN_BLUE(0.80f);

const GLint Simulation::HUD_LEFT_CLIP(0);
const GLint Simulation::HUD_BOTTOM_CLIP(0);
const GLint Simulation::HUD_NEAR_CLIP(-1);
const GLint Simulation::HUD_FAR_CLIP(+1);

const std::string Simulation::EAH_PULSAR_FILE("EatH_mastercat");
const std::string Simulation::EAH_PULSAR_FILE_EXT("txt");

Simulation::Simulation(void) : cs(CONSTELLATIONS_RADIUS),
                               ps(PULSARS_RADIUS, PULSARS_MAG_SIZE, PULSARS_RGB_RED, PULSARS_RGB_GREEN, PULSARS_RGB_BLUE),
                               ps_EAH(PULSARS_EAH_RADIUS, PULSARS_EAH_MAG_SIZE, PULSARS_EAH_RGB_RED, PULSARS_EAH_RGB_GREEN, PULSARS_EAH_RGB_BLUE),
                               sn(SUPERNOVAE_RADIUS, SUPERNOVAE_MAG_SIZE, SUPERNOVAE_RGB_RED, SUPERNOVAE_RGB_GREEN, SUPERNOVAE_RGB_BLUE),
                               c_sphere(SKYGRID_RADIUS, SKYGRID_SLICES, SKYGRID_STACKS, GridGlobe::INSIDE),
                               earth(EARTH_NAME,
                                     EARTH_IMAGE_RESOURCE,
                                     SolarSystemGlobals::EARTH_RADIUS,
                                     EARTH_STACKS,
                                     EARTH_SLICES,
                                     EARTH_TEXTURE_OFFSET),
                               e_sphere(EARTHGRID_RADIUS, EARTHGRID_SLICES, EARTHGRID_STACKS, GridGlobe::OUTSIDE),
                               sun(SUN_NAME,
                                   SUN_IMAGE_RESOURCE,
                                   SolarSystemGlobals::SUN_RADIUS,
                                   SUN_STACKS,
                                   SUN_SLICES,
                                   SUN_TEXTURE_OFFSET),
                               target(std::string("reticle"), screen_width, screen_height, 4),
                               overlay(NULL),
                               north_panel(&overlay, HUDFlowLayout::VERTICAL),
                               south_panel(&overlay, HUDFlowLayout::HORIZONTAL),
                               east_panel(&overlay, HUDFlowLayout::VERTICAL),
                               west_panel(&overlay, HUDFlowLayout::VERTICAL) {
    // Starting values of simulation parameters.
    min60 = 0;
    hour24 = 0;
    day366 = 0;
    earth_hour_angle = 0;
    sun_rot_angle = 0;

    // Image pointers.
    aei_image = NULL;
    aps_image = NULL;
    boinc_image = NULL;
    geo_image = NULL;
    ligo_image = NULL;
    opencl_image = NULL;
    virgo_image = NULL;
    wyp_image = NULL;

    // Pointer to scrolling marquee.
    version_text = NULL;

    // Line rendering detail for celestial coordinate grids.
    c_sphere.setLine(GridGlobe::MAIN, SKYGRID_MAIN_WIDTH,
                     SKYGRID_MAIN_RED, SKYGRID_MAIN_GREEN, SKYGRID_MAIN_BLUE);
    c_sphere.setLine(GridGlobe::EQUATOR, SKYGRID_CELESTIAL_EQUATOR_WIDTH,
                     SKYGRID_CELESTIAL_EQUATOR_RED, SKYGRID_CELESTIAL_EQUATOR_GREEN, SKYGRID_CELESTIAL_EQUATOR_BLUE);
    c_sphere.setLine(GridGlobe::PRIME_MERIDIAN, SKYGRID_PRIME_MERIDIAN_WIDTH,
                     SKYGRID_PRIME_MERIDIAN_RED, SKYGRID_PRIME_MERIDIAN_GREEN, SKYGRID_PRIME_MERIDIAN_BLUE);

    // Line rendering detail for terrestrial coordinate grids.
    e_sphere.setLine(GridGlobe::MAIN, EARTHGRID_MAIN_WIDTH,
                     EARTHGRID_MAIN_RED, EARTHGRID_MAIN_GREEN, EARTHGRID_MAIN_BLUE);
    e_sphere.setLine(GridGlobe::EQUATOR, EARTHGRID_CELESTIAL_EQUATOR_WIDTH,
                     EARTHGRID_CELESTIAL_EQUATOR_RED, EARTHGRID_CELESTIAL_EQUATOR_GREEN, EARTHGRID_CELESTIAL_EQUATOR_BLUE);
    e_sphere.setLine(GridGlobe::PRIME_MERIDIAN, EARTHGRID_PRIME_MERIDIAN_WIDTH,
                     EARTHGRID_PRIME_MERIDIAN_RED, EARTHGRID_PRIME_MERIDIAN_GREEN, EARTHGRID_PRIME_MERIDIAN_BLUE);

    // Get the pulsar and supernovae data.
    loadPulsars();
    loadPulsarsEAH();
    loadSupernovae();
    }

Simulation::~Simulation() {
    if(aei_image != NULL) {
        delete aei_image;
        }
    if(aps_image != NULL) {
        delete aps_image;
        }
    if(boinc_image != NULL) {
        delete boinc_image;
        }
    if(geo_image != NULL) {
        delete geo_image;
        }
    if(ligo_image != NULL) {
        delete ligo_image;
        }
    if(opencl_image != NULL) {
        delete opencl_image;
        }
    if(virgo_image != NULL) {
        delete virgo_image;
        }
    if(wyp_image != NULL) {
        delete wyp_image;
        }
    if(version_text != NULL) {
        delete version_text;
        }
    clearTextLines();
    clearImages();
    }

void Simulation::step(void) {
    // Is the autopilot inactive?
    if(!pilot.isActive()) {
        // No, then evolve the user's craft's mechanics
        // with knowledge of the day within the year.
        flyboy.step(day366);
        }
    else {
        // Yes, the autopilot is operating so check for any
        // content change of the tour's descriptive text.
        if(pilot.hasDescriptionChanged() == true) {
            target.inactivate();

            clearTextLines();
            clearImages();
            // Clean up any prior panel contents.
            north_panel.erase();
            west_panel.erase();
            north_panel.setLoad(HUDFlowLayout::FIRST);
            west_panel.setLoad(HUDFlowLayout::FIRST);

            // Then put new content lines, if any, into the panel.
            // Derived according to the current position in the tour.
            const std::vector<std::string>& messages = pilot.getDescription();
            if(messages.size() != 0) {
                target.activate();
                for(std::vector<std::string>::const_iterator message = messages.begin();
                    message != messages.end();
                    ++message) {
                    /// TODO - currently entering in reverse order ( see HUDFlowLayout::allocateItemBases() LAST case ).
                    HUDTextLine* line = new HUDTextLine(message->size(), overlay.getFont(), *message, 0, 2);
                    text_lines.push_back(line);
                    north_panel.addContent(line);
                    }
                }

            // Then put new images, if any, into the west panel.
            // Derived according to the current position in the tour
            // and are currently the pulse profile
            // Cast from base ( Renderable ) to derived ( HUDContent ) class.
            lookout_images.clear();
            const std::vector<std::string>& image_names = pilot.getImageResourceNames();
            for(std::vector<std::string>::const_iterator image_name = image_names.begin();
                image_name != image_names.end();
                ++image_name) {
                HUDImage* profile = new HUDImage(*image_name, 10, 10);
                lookout_images.push_back(profile);
                west_panel.addContent(profile);
                }

            west_panel.activate();
            north_panel.activate();
            }
        }

    // The time increments are minute granular.
    ++min60;
    // Have we rolled over to the next hour?
    if(min60 > 59) {
        // Yes, so zero minutes at the start of a fresh hour.
        min60 = 0;
        // Roll the hour forward.
        ++hour24;
        // Have we rolled over to another day?
        if(hour24 > 23) {
            // Yes, so zero hours at the start of a fresh day.
            hour24 = 0;
            }
        }
    // Set the Earth's hour angle based upon the above.
    earth_hour_angle = (hour24 + min60/60.0f) * 15.0f;

    /// TODO - demo code only, needs proper ephemeris model.
    // This is a fudge for demo purposes, and makes the
    // Sun orbit far quicker and out of sync with Earth
    // rotations. Time increment in 1/20th's of a day.
    day366 += 0.05f;
    // Have we rolled over to another year ?
    if(day366 >= SunOrbit::DAYS_PER_YEAR) {
        // Yup, reset the day count to zero
        // for a fresh year.
        day366 = 0.0f;
        }

    // Discover where the Sun should
    // be for that time of the year.
    sun_pos = SunOrbit::getPosition(day366);

    // Also set the Sun's rotation about
    // it's own axis accordingly.
    sun_rot_angle = SunOrbit::getRotation(day366);
    }

void Simulation::moveRequest(Craft::movements mv) {
    // Movement requests are only passed through
    // to the craft if the autopilot is off.
    if(!pilot.isActive()) {
        flyboy.manouevre(mv);
        }
    }

void Simulation::resize(GLuint width, GLuint height) {
    screen_width = width;
    screen_height = height;

    std::stringstream msg;
    msg << "Simulation::resize() - resize screen to "
        << "width = " << width
        << "\theight = " << height;
    ErrorHandler::record(msg.str(), ErrorHandler::INFORM);

    // Now tell the HUD of such settings.
    // TODO - if resize denied then inactivate HUD ?? Complex ....
    overlay.requestResize(width, height);

    target.resize(screen_width, screen_height);
    }

void Simulation::setFont(content element, OGLFT_ft* font) {
    fonts[element] = font;
    }

CameraState Simulation::viewPoint(void) {
    if(pilot.isActive()) {
        // The autopilot is flying the craft.
        return pilot.viewState();
        }
    else {
        // The user is flying the craft.
        return flyboy.viewState();;
        }
    }

void Simulation::prepare(SolarSystemGlobals::render_quality rq) {
    /// TODO - write cases per render_quality ... presently ignored
    /// at this level.

    // Activate 3D scene objects, while nominating any fonts
    // prior to activation of their respective objects..
    cs.setFont(fonts[CONSTELLATIONS]);
    cs.activate();
    ps.activate();
    ps_EAH.activate();
    sn.activate();
    c_sphere.setFont(fonts[SKY_GRID]);
    c_sphere.activate();
    earth.activate();
    e_sphere.setFont(fonts[EARTH_GRID]);
    e_sphere.activate();
    sun.activate();
    overlay.setFont(fonts[HUDOVER]);

    // Now to arrange the HUD components.

    // First empty the panels, as we may be recycling.
    north_panel.erase();
    south_panel.erase();
    east_panel.erase();
    west_panel.erase();

    // Set panel justifications.
    north_panel.setPrimaryJustification(HUDFlowLayout::START);
    north_panel.setSecondaryJustification(HUDFlowLayout::MIDDLE);
    south_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
    south_panel.setSecondaryJustification(HUDFlowLayout::MIDDLE);
    east_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
    east_panel.setSecondaryJustification(HUDFlowLayout::MIDDLE);
    west_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
    west_panel.setSecondaryJustification(HUDFlowLayout::MIDDLE);

    // Put the panels into the layout.
    overlay.setPanel(HUDBorderLayout::NORTH, &north_panel);
    overlay.setPanel(HUDBorderLayout::SOUTH, &south_panel);
    overlay.setPanel(HUDBorderLayout::EAST, &east_panel);
    overlay.setPanel(HUDBorderLayout::WEST, &west_panel);

    // Create content and include into panels.
    //loadImageToPanel(wyp_image, &west_panel, "wypTGA", 5, 5);
    //loadImageToPanel(aps_image, &west_panel, "apsTGA", 5, 5);
    //loadImageToPanel(aei_image, &east_panel, "aeiTGA", 5, 5);
    //loadImageToPanel(opencl_image, &east_panel, "openclTGA", 5, 5);
    // LoadImageToPanel(boinc_image, &south_panel, "boincTGA", 5, 5);
    version_text = new HUDTextLineScroll(70, overlay.getFont(),
                                         "                  http://einstein.phys.uwm.edu",
                                         35, 10, HUDTextLineScroll::LEFT, 10);
    if(version_text == NULL) {
        std::string msg = "Simulation::prepare() - failed creation of HUDTextLineScroll instance on heap";
        ErrorHandler::record(msg, ErrorHandler::FATAL);
        }

    // Put the content into the panel.
    south_panel.addContent(version_text);

    //loadImageToPanel(ligo_image, &west_panel, "ligoTGA", 5, 5);
    //loadImageToPanel(geo_image, &east_panel, "geoTGA", 5, 5);
    //loadImageToPanel(virgo_image, &south_panel, "virgoTGA", 5, 5);

    overlay.activate();
    }

void Simulation::release(void) {
    cs.inactivate();
    ps.inactivate();
    ps_EAH.inactivate();
    sn.inactivate();
    c_sphere.inactivate();
    earth.inactivate();
    e_sphere.inactivate();
    sun.inactivate();
    target.inactivate();

    // Must inactivate the layout first !!
    overlay.inactivate();

    if(aei_image != NULL) {
        delete aei_image;
        }
    if(aps_image != NULL) {
        delete aps_image;
        }
    if(aps_image != NULL) {
        delete aps_image;
        }
    if(geo_image != NULL) {
        delete geo_image;
        }
    if(ligo_image != NULL) {
        delete ligo_image;
        }
    if(opencl_image != NULL) {
        delete opencl_image;
        }
    if(virgo_image != NULL) {
        delete virgo_image;
        }
    if(wyp_image != NULL) {
        delete wyp_image;
        }
    if(version_text != NULL) {
        delete version_text;
        }
    clearTextLines();
    clearImages();
    }

void Simulation::render(void) {
    // Invoke the draw method for each scene element.
    cs.draw();
    ps.draw();
    ps_EAH.draw();
    sn.draw();
    c_sphere.draw();

    // For calculational efficiency/simplicity lights are of directional type
    // ie. shining from 'infinity' in the given direction.
    // glEnable(GL_LIGHTING);

    // The Earth requires rotation and lighting.
    //GLfloat sunlight_position[] = {sun_pos.x(),
                                  //sun_pos.y(),
                                  //sun_pos.z(),
                                  //AT_INFINITY};

    glPushMatrix();
        // The light is coming from the Sun's direction.
        // glLightfv(GL_LIGHT0, GL_POSITION, sunlight_position);

        // Turn the light on.
        // glEnable(GL_LIGHT0);

        // Render the Earth, suitably transformed.
        glRotatef(earth_hour_angle, 0, 0, 1);
        earth.draw();
        e_sphere.draw();

        // Turn the light off.
        // glDisable(GL_LIGHT0);
    glPopMatrix();

    // The Sun requires positioning and lighting.
    //GLfloat viewpoint_position[] = {flyboy.get_platform().position().x(),
                                   //flyboy.get_platform().position().y(),
                                   //flyboy.get_platform().position().z(),
                                   //AT_INFINITY};

    glPushMatrix();
        // The light is coming from our eye direction.
        // glLightfv(GL_LIGHT1, GL_POSITION, viewpoint_position);

        // Turn the light on.
        // glEnable(GL_LIGHT1);

        // Render the Sun, suitably transformed.
        glTranslatef(sun_pos.x(),
                     sun_pos.y(),
                     sun_pos.z());

        glRotatef(SunOrbit::ECLIPTIC_ANGLE_DEG, 1, 0, 0);

        glRotatef(sun_rot_angle, 0, 0, 1);

        sun.draw();

        // Turn the light off.
        // glDisable(GL_LIGHT1);
    glPopMatrix();

    // glDisable(GL_LIGHTING);

    // We only draw the HUD if the screen/window is of sufficient dimensions.
    // Constraint used is the current minimum size of the outermost HUD container,
    // which is in turn derived from an aggregation of all it's content.
    if((screen_width >= overlay.minWidth()) &&
       (screen_height >= overlay.minHeight())) {

        // Swap into 2D with back face culling, textures and no depth testing.
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // We'll store whatever was in the projection stack, set the
        // identity and choose the orthographic mode.
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        // Clip to screen/window size and nearby the z = 0 plane.
        glOrtho(0, screen_width, 0, screen_height, -1, 1);

        // Store what was in the modelview stack, and set the identity.
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Finally draw the HUD.
        overlay.draw();
        target.draw();

        // Restore the projection and modelview stacks.
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // Return to prior cull, texture and depth state.
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        }
    }

void Simulation::loadPulsars(void) {
    ps.add(Pulsar(1.52000f, 18.58306f, "J0006+1834", Pulsar::ATNF));
    ps.add(Pulsar(1.75650f, 73.05228f, "J0007+7303", Pulsar::ATNF));
    ps.add(Pulsar(3.57396f, 47.77594f, "J0014+4746", Pulsar::ATNF));
    ps.add(Pulsar(5.95980f, -72.07541f, "J0024-7204C", Pulsar::ATNF));
    ps.add(Pulsar(6.05783f, -72.07884f, "J0024-7204D", Pulsar::ATNF));
    ps.add(Pulsar(6.04627f, -72.08893f, "J0024-7204E", Pulsar::ATNF));
    ps.add(Pulsar(6.01606f, -72.07856f, "J0024-7204F", Pulsar::ATNF));
    ps.add(Pulsar(6.03316f, -72.07769f, "J0024-7204G", Pulsar::ATNF));
    ps.add(Pulsar(6.02792f, -72.06855f, "J0024-7204H", Pulsar::ATNF));
    ps.add(Pulsar(6.03305f, -72.07769f, "J0024-7204I", Pulsar::ATNF));
    ps.add(Pulsar(5.99753f, -72.06633f, "J0024-7204J", Pulsar::ATNF));
    ps.add(Pulsar(6.01571f, -72.08248f, "J0024-7204L", Pulsar::ATNF));
    ps.add(Pulsar(5.97703f, -72.09187f, "J0024-7204M", Pulsar::ATNF));
    ps.add(Pulsar(6.03828f, -72.07469f, "J0024-7204N", Pulsar::ATNF));
    ps.add(Pulsar(6.01938f, -72.08160f, "J0024-7204O", Pulsar::ATNF));
    ps.add(Pulsar(6.02500f, -72.06667f, "J0024-7204P", Pulsar::ATNF));
    ps.add(Pulsar(6.06870f, -72.07365f, "J0024-7204Q", Pulsar::ATNF));
    ps.add(Pulsar(6.02083f, -72.06667f, "J0024-7204R", Pulsar::ATNF));
    ps.add(Pulsar(6.01657f, -72.07843f, "J0024-7204S", Pulsar::ATNF));
    ps.add(Pulsar(6.03562f, -72.07748f, "J0024-7204T", Pulsar::ATNF));
    ps.add(Pulsar(6.04098f, -72.06658f, "J0024-7204U", Pulsar::ATNF));
    ps.add(Pulsar(6.02083f, -72.06667f, "J0024-7204V", Pulsar::ATNF));
    ps.add(Pulsar(6.02083f, -72.06667f, "J0024-7204W", Pulsar::ATNF));
    ps.add(Pulsar(6.71067f, 63.33357f, "J0026+6320", Pulsar::ATNF));
    ps.add(Pulsar(7.61429f, 4.86104f, "J0030+0451", Pulsar::ATNF));
    ps.add(Pulsar(8.53696f, -7.36484f, "J0034-0721", Pulsar::ATNF));
    ps.add(Pulsar(8.59095f, -5.57684f, "J0034-0534", Pulsar::ATNF));
    ps.add(Pulsar(10.13484f, 57.27359f, "J0040+5716", Pulsar::ATNF));
    ps.add(Pulsar(11.35704f, -70.70197f, "J0045-7042", Pulsar::ATNF));
    ps.add(Pulsar(11.39650f, -73.31750f, "J0045-7319", Pulsar::ATNF));
    ps.add(Pulsar(12.14160f, 34.20223f, "J0048+3412", Pulsar::ATNF));
    ps.add(Pulsar(12.87542f, 4.38028f, "J0051+0423", Pulsar::ATNF));
    ps.add(Pulsar(13.93908f, 51.29027f, "J0055+5117", Pulsar::ATNF));
    ps.add(Pulsar(14.10630f, 47.93626f, "J0056+4756", Pulsar::ATNF));
    ps.add(Pulsar(14.43333f, -72.02194f, "J0057-7201", Pulsar::ATNF));
    ps.add(Pulsar(15.17929f, -72.19267f, "J0100-7211", Pulsar::ATNF));
    ps.add(Pulsar(15.63733f, 65.62039f, "J0102+6537", Pulsar::ATNF));
    ps.add(Pulsar(17.09436f, 66.14277f, "J0108+6608", Pulsar::ATNF));
    ps.add(Pulsar(17.12293f, 69.09795f, "J0108+6905", Pulsar::ATNF));
    ps.add(Pulsar(17.03478f, -14.53061f, "J0108-1431", Pulsar::ATNF));
    ps.add(Pulsar(17.86988f, -71.52967f, "J0111-7131", Pulsar::ATNF));
    ps.add(Pulsar(18.29621f, -72.34228f, "J0113-7220", Pulsar::ATNF));
    ps.add(Pulsar(19.41109f, 59.24400f, "J0117+5914", Pulsar::ATNF));
    ps.add(Pulsar(22.86879f, -73.16926f, "J0131-7310", Pulsar::ATNF));
    ps.add(Pulsar(23.38525f, -69.95824f, "J0133-6957", Pulsar::ATNF));
    ps.add(Pulsar(23.57779f, -29.62136f, "J0134-2937", Pulsar::ATNF));
    ps.add(Pulsar(24.34950f, 16.91169f, "J0137+1654", Pulsar::ATNF));
    ps.add(Pulsar(24.83225f, 58.24217f, "J0139+5814", Pulsar::ATNF));
    ps.add(Pulsar(25.41641f, 60.15897f, "J0141+6009", Pulsar::ATNF));
    ps.add(Pulsar(26.59342f, 61.75078f, "J0146+6145", Pulsar::ATNF));
    ps.add(Pulsar(26.93602f, 59.36758f, "J0147+5922", Pulsar::ATNF));
    ps.add(Pulsar(27.84459f, -6.58411f, "J0151-0635", Pulsar::ATNF));
    ps.add(Pulsar(28.04522f, -16.63139f, "J0152-1637", Pulsar::ATNF));
    ps.add(Pulsar(28.09887f, 9.80278f, "J0152+0948", Pulsar::ATNF));
    ps.add(Pulsar(29.23019f, 39.82459f, "J0156+3949", Pulsar::ATNF));
    ps.add(Pulsar(29.45806f, 62.20720f, "J0157+6212", Pulsar::ATNF));
    ps.add(Pulsar(31.40800f, 64.82856f, "J0205+6449", Pulsar::ATNF));
    ps.add(Pulsar(31.50528f, -40.46787f, "J0206-4028", Pulsar::ATNF));
    ps.add(Pulsar(32.99583f, -81.99339f, "J0211-8159", Pulsar::ATNF));
    ps.add(Pulsar(33.98595f, 62.30927f, "J0215+6218", Pulsar::ATNF));
    ps.add(Pulsar(34.52646f, 42.53818f, "J0218+4232", Pulsar::ATNF));
    ps.add(Pulsar(37.80832f, 70.44275f, "J0231+7026", Pulsar::ATNF));
    ps.add(Pulsar(42.00000f, 60.00000f, "J0248+60", Pulsar::ATNF));
    ps.add(Pulsar(43.98425f, -53.07260f, "J0255-5304", Pulsar::ATNF));
    ps.add(Pulsar(46.13798f, 19.54761f, "J0304+1932", Pulsar::ATNF));
    ps.add(Pulsar(50.86091f, 39.74803f, "J0323+3944", Pulsar::ATNF));
    ps.add(Pulsar(52.28563f, 16.90056f, "J0329+1654", Pulsar::ATNF));
    ps.add(Pulsar(53.24737f, 54.57877f, "J0332+5434", Pulsar::ATNF));
    ps.add(Pulsar(53.81936f, 45.93152f, "J0335+4555", Pulsar::ATNF));
    ps.add(Pulsar(55.80379f, 53.21484f, "J0343+5312", Pulsar::ATNF));
    ps.add(Pulsar(55.86642f, -30.00764f, "J0343-3000", Pulsar::ATNF));
    ps.add(Pulsar(59.43673f, 52.61603f, "J0357+5236", Pulsar::ATNF));
    ps.add(Pulsar(59.44500f, 32.10500f, "J0357+32", Pulsar::ATNF));
    ps.add(Pulsar(59.72382f, 54.22048f, "J0358+5413", Pulsar::ATNF));
    ps.add(Pulsar(60.46533f, -76.13717f, "J0401-7608", Pulsar::ATNF));
    ps.add(Pulsar(61.62534f, 61.64473f, "J0406+6138", Pulsar::ATNF));
    ps.add(Pulsar(61.97891f, 16.12122f, "J0407+1607", Pulsar::ATNF));
    ps.add(Pulsar(63.98189f, 69.90275f, "J0415+6954", Pulsar::ATNF));
    ps.add(Pulsar(64.42917f, 35.75000f, "J0417+35", Pulsar::ATNF));
    ps.add(Pulsar(65.00813f, -50.38003f, "J0420-5022", Pulsar::ATNF));
    ps.add(Pulsar(65.38982f, -3.75184f, "J0421-0345", Pulsar::ATNF));
    ps.add(Pulsar(68.89167f, 27.73333f, "J0435+27", Pulsar::ATNF));
    ps.add(Pulsar(69.31618f, -47.25251f, "J0437-4715", Pulsar::ATNF));
    ps.add(Pulsar(72.17320f, -27.82965f, "J0448-2749", Pulsar::ATNF));
    ps.add(Pulsar(72.27362f, -70.52547f, "J0449-7031", Pulsar::ATNF));
    ps.add(Pulsar(72.53659f, -12.80197f, "J0450-1248", Pulsar::ATNF));
    ps.add(Pulsar(72.95833f, -67.30000f, "J0451-67", Pulsar::ATNF));
    ps.add(Pulsar(73.14211f, -17.98983f, "J0452-1759", Pulsar::ATNF));
    ps.add(Pulsar(73.53229f, 55.72818f, "J0454+5543", Pulsar::ATNF));
    ps.add(Pulsar(73.94812f, -69.85953f, "J0455-6951", Pulsar::ATNF));
    ps.add(Pulsar(74.01042f, -70.51850f, "J0456-7031", Pulsar::ATNF));
    ps.add(Pulsar(74.28246f, -63.62511f, "J0457-6337", Pulsar::ATNF));
    ps.add(Pulsar(74.96642f, -2.16851f, "J0459-0210", Pulsar::ATNF));
    ps.add(Pulsar(75.51900f, 46.90169f, "J0502+4654", Pulsar::ATNF));
    ps.add(Pulsar(75.71054f, -66.29967f, "J0502-6617", Pulsar::ATNF));
    ps.add(Pulsar(77.98542f, -65.14347f, "J0511-6508", Pulsar::ATNF));
    ps.add(Pulsar(78.52789f, -40.04692f, "J0514-4002A", Pulsar::ATNF));
    ps.add(Pulsar(79.94549f, -69.53986f, "J0519-6932", Pulsar::ATNF));
    ps.add(Pulsar(80.15077f, -25.88675f, "J0520-2553", Pulsar::ATNF));
    ps.add(Pulsar(80.59608f, -68.78394f, "J0522-6847", Pulsar::ATNF));
    ps.add(Pulsar(81.48520f, 11.25530f, "J0525+1115", Pulsar::ATNF));
    ps.add(Pulsar(81.50375f, -66.07675f, "J0525-6607", Pulsar::ATNF));
    ps.add(Pulsar(82.21777f, 22.00111f, "J0528+2200", Pulsar::ATNF));
    ps.add(Pulsar(82.46217f, -66.87728f, "J0529-6652", Pulsar::ATNF));
    ps.add(Pulsar(83.24796f, -66.66036f, "J0532-6639", Pulsar::ATNF));
    ps.add(Pulsar(83.35758f, 4.03347f, "J0533+0402", Pulsar::ATNF));
    ps.add(Pulsar(83.63322f, 22.01446f, "J0534+2200", Pulsar::ATNF));
    ps.add(Pulsar(83.65071f, -67.06356f, "J0534-6703", Pulsar::ATNF));
    ps.add(Pulsar(83.75000f, -69.58333f, "J0535-6935", Pulsar::ATNF));
    ps.add(Pulsar(84.12829f, -75.73242f, "J0536-7543", Pulsar::ATNF));
    ps.add(Pulsar(84.44733f, -69.17233f, "J0537-6910", Pulsar::ATNF));
    ps.add(Pulsar(84.60440f, 28.28588f, "J0538+2817", Pulsar::ATNF));
    ps.add(Pulsar(85.04667f, -69.33171f, "J0540-6919", Pulsar::ATNF));
    ps.add(Pulsar(85.15833f, 32.03861f, "J0540+32", Pulsar::ATNF));
    ps.add(Pulsar(85.12833f, -71.42539f, "J0540-7125", Pulsar::ATNF));
    ps.add(Pulsar(85.79025f, 23.48472f, "J0543+2329", Pulsar::ATNF));
    ps.add(Pulsar(85.96963f, -68.85703f, "J0543-6851", Pulsar::ATNF));
    ps.add(Pulsar(86.61983f, 24.68917f, "J0546+2441", Pulsar::ATNF));
    ps.add(Pulsar(88.75771f, -70.94600f, "J0555-7056", Pulsar::ATNF));
    ps.add(Pulsar(90.19375f, -57.94833f, "J0600-5756", Pulsar::ATNF));
    ps.add(Pulsar(90.49576f, -5.46404f, "J0601-0527", Pulsar::ATNF));
    ps.add(Pulsar(92.49536f, 21.50079f, "J0609+2130", Pulsar::ATNF));
    ps.add(Pulsar(92.55663f, -21.00778f, "J0610-2100", Pulsar::ATNF));
    ps.add(Pulsar(92.81667f, 30.26667f, "J0611+30", Pulsar::ATNF));
    ps.add(Pulsar(93.20286f, 37.36038f, "J0612+3721", Pulsar::ATNF));
    ps.add(Pulsar(93.43323f, -2.01310f, "J0613-0200", Pulsar::ATNF));
    ps.add(Pulsar(93.57150f, 22.51000f, "J0614+2229", Pulsar::ATNF));
    ps.add(Pulsar(95.34213f, 10.04409f, "J0621+1002", Pulsar::ATNF));
    ps.add(Pulsar(96.08345f, -4.41401f, "J0624-0424", Pulsar::ATNF));
    ps.add(Pulsar(96.43750f, 10.26667f, "J0625+10", Pulsar::ATNF));
    ps.add(Pulsar(96.80417f, 16.20000f, "J0627+16", Pulsar::ATNF));
    ps.add(Pulsar(97.13750f, 9.15000f, "J0628+09", Pulsar::ATNF));
    ps.add(Pulsar(97.27387f, 24.26203f, "J0629+2415", Pulsar::ATNF));
    ps.add(Pulsar(97.70585f, -28.57855f, "J0630-2834", Pulsar::ATNF));
    ps.add(Pulsar(97.86468f, 10.61736f, "J0631+1036", Pulsar::ATNF));
    ps.add(Pulsar(98.43329f, 6.54031f, "J0633+0632", Pulsar::ATNF));
    ps.add(Pulsar(98.47564f, 17.77025f, "J0633+1746", Pulsar::ATNF));
    ps.add(Pulsar(98.28692f, -20.25197f, "J0633-2015", Pulsar::ATNF));
    ps.add(Pulsar(98.82500f, 5.55306f, "J0635+0533", Pulsar::ATNF));
    ps.add(Pulsar(99.14060f, -45.82573f, "J0636-4549", Pulsar::ATNF));
    ps.add(Pulsar(103.31287f, 80.86673f, "J0653+8051", Pulsar::ATNF));
    ps.add(Pulsar(104.12586f, -22.47376f, "J0656-2228", Pulsar::ATNF));
    ps.add(Pulsar(104.20413f, -54.82081f, "J0656-5449", Pulsar::ATNF));
    ps.add(Pulsar(104.95056f, 14.23931f, "J0659+1414", Pulsar::ATNF));
    ps.add(Pulsar(105.15751f, 64.30311f, "J0700+6418", Pulsar::ATNF));
    ps.add(Pulsar(107.38555f, -59.39878f, "J0709-5923", Pulsar::ATNF));
    ps.add(Pulsar(107.90075f, 9.52361f, "J0711+0931", Pulsar::ATNF));
    ps.add(Pulsar(107.97594f, -68.51322f, "J0711-6830", Pulsar::ATNF));
    ps.add(Pulsar(109.78075f, -25.75006f, "J0719-2545", Pulsar::ATNF));
    ps.add(Pulsar(110.10401f, -31.43058f, "J0720-3125", Pulsar::ATNF));
    ps.add(Pulsar(111.25168f, -16.59606f, "J0725-1635", Pulsar::ATNF));
    ps.add(Pulsar(111.53383f, -26.21058f, "J0726-2612", Pulsar::ATNF));
    ps.add(Pulsar(112.38480f, -18.61187f, "J0729-1836", Pulsar::ATNF));
    ps.add(Pulsar(112.31854f, -14.81022f, "J0729-1448", Pulsar::ATNF));
    ps.add(Pulsar(113.35250f, -23.76561f, "J0733-2345", Pulsar::ATNF));
    ps.add(Pulsar(113.84167f, -62.91667f, "J0735-62", Pulsar::ATNF));
    ps.add(Pulsar(114.43363f, -22.03481f, "J0737-2202", Pulsar::ATNF));
    ps.add(Pulsar(114.46354f, -30.66131f, "J0737-3039A", Pulsar::ATNF));
    ps.add(Pulsar(114.46354f, -30.66131f, "J0737-3039B", Pulsar::ATNF));
    ps.add(Pulsar(114.63470f, -40.71137f, "J0738-4042", Pulsar::ATNF));
    ps.add(Pulsar(115.70441f, -28.37882f, "J0742-2822", Pulsar::ATNF));
    ps.add(Pulsar(116.25958f, -53.85611f, "J0745-5353", Pulsar::ATNF));
    ps.add(Pulsar(116.74799f, -45.49074f, "J0746-4529", Pulsar::ATNF));
    ps.add(Pulsar(117.45698f, -42.79504f, "J0749-4247", Pulsar::ATNF));
    ps.add(Pulsar(117.78815f, 18.12739f, "J0751+1807", Pulsar::ATNF));
    ps.add(Pulsar(118.66953f, 32.53228f, "J0754+3231", Pulsar::ATNF));
    ps.add(Pulsar(119.62113f, -15.46909f, "J0758-1528", Pulsar::ATNF));
    ps.add(Pulsar(121.59750f, -41.37525f, "J0806-4123", Pulsar::ATNF));
    ps.add(Pulsar(122.43287f, -47.89865f, "J0809-4753", Pulsar::ATNF));
    ps.add(Pulsar(123.74792f, 74.48492f, "J0814+7429", Pulsar::ATNF));
    ps.add(Pulsar(123.78657f, 9.66408f, "J0815+0939", Pulsar::ATNF));
    ps.add(Pulsar(124.60806f, -32.54181f, "J0818-3232", Pulsar::ATNF));
    ps.add(Pulsar(125.10992f, -13.84885f, "J0820-1350", Pulsar::ATNF));
    ps.add(Pulsar(125.06437f, -41.24344f, "J0820-4114", Pulsar::ATNF));
    ps.add(Pulsar(125.20462f, -39.36547f, "J0820-3921", Pulsar::ATNF));
    ps.add(Pulsar(125.43598f, -42.35707f, "J0821-4221", Pulsar::ATNF));
    ps.add(Pulsar(125.48898f, -43.00477f, "J0821-4300", Pulsar::ATNF));
    ps.add(Pulsar(125.79070f, 1.98678f, "J0823+0159", Pulsar::ATNF));
    ps.add(Pulsar(126.71410f, 26.62327f, "J0826+2637", Pulsar::ATNF));
    ps.add(Pulsar(127.06927f, -34.28529f, "J0828-3417", Pulsar::ATNF));
    ps.add(Pulsar(127.88512f, -44.10331f, "J0831-4406", Pulsar::ATNF));
    ps.add(Pulsar(128.56792f, -41.99750f, "J0834-4159", Pulsar::ATNF));
    ps.add(Pulsar(128.70833f, -60.58333f, "J0834-60", Pulsar::ATNF));
    ps.add(Pulsar(128.83588f, -45.17635f, "J0835-4510", Pulsar::ATNF));
    ps.add(Pulsar(128.76283f, -37.13097f, "J0835-3707", Pulsar::ATNF));
    ps.add(Pulsar(129.27351f, 6.17071f, "J0837+0610", Pulsar::ATNF));
    ps.add(Pulsar(129.33826f, -41.58732f, "J0837-4135", Pulsar::ATNF));
    ps.add(Pulsar(129.57834f, -26.35821f, "J0838-2621", Pulsar::ATNF));
    ps.add(Pulsar(129.62833f, -39.78944f, "J0838-3947", Pulsar::ATNF));
    ps.add(Pulsar(130.14117f, -53.54319f, "J0840-5332", Pulsar::ATNF));
    ps.add(Pulsar(130.52237f, -48.85578f, "J0842-4851", Pulsar::ATNF));
    ps.add(Pulsar(130.89092f, 7.31328f, "J0843+0719", Pulsar::ATNF));
    ps.add(Pulsar(130.79118f, -50.37864f, "J0843-5022", Pulsar::ATNF));
    ps.add(Pulsar(131.52525f, -35.56129f, "J0846-3533", Pulsar::ATNF));
    ps.add(Pulsar(131.98887f, -43.28244f, "J0847-4316", Pulsar::ATNF));
    ps.add(Pulsar(132.25628f, 80.48309f, "J0849+8028", Pulsar::ATNF));
    ps.add(Pulsar(132.42746f, -63.37639f, "J0849-6322", Pulsar::ATNF));
    ps.add(Pulsar(133.91006f, -33.52758f, "J0855-3331", Pulsar::ATNF));
    ps.add(Pulsar(133.90075f, -46.73706f, "J0855-4644", Pulsar::ATNF));
    ps.add(Pulsar(133.83154f, -46.97294f, "J0855-4658", Pulsar::ATNF));
    ps.add(Pulsar(134.24712f, -61.63147f, "J0856-6137", Pulsar::ATNF));
    ps.add(Pulsar(134.47983f, -44.40287f, "J0857-4424", Pulsar::ATNF));
    ps.add(Pulsar(135.18314f, -31.74192f, "J0900-3144", Pulsar::ATNF));
    ps.add(Pulsar(135.41717f, -46.41347f, "J0901-4624", Pulsar::ATNF));
    ps.add(Pulsar(135.63500f, -63.42000f, "J0902-6325", Pulsar::ATNF));
    ps.add(Pulsar(136.24667f, -42.77056f, "J0904-4246", Pulsar::ATNF));
    ps.add(Pulsar(136.04500f, -74.99522f, "J0904-7459", Pulsar::ATNF));
    ps.add(Pulsar(136.34958f, -45.61472f, "J0905-4536", Pulsar::ATNF));
    ps.add(Pulsar(136.46634f, -51.46353f, "J0905-5127", Pulsar::ATNF));
    ps.add(Pulsar(136.81625f, -51.96646f, "J0907-5157", Pulsar::ATNF));
    ps.add(Pulsar(137.15909f, -17.66046f, "J0908-1739", Pulsar::ATNF));
    ps.add(Pulsar(137.14804f, -49.21847f, "J0908-4913", Pulsar::ATNF));
    ps.add(Pulsar(137.40000f, -72.20233f, "J0909-7212", Pulsar::ATNF));
    ps.add(Pulsar(140.30890f, 62.90386f, "J0921+6254", Pulsar::ATNF));
    ps.add(Pulsar(140.55842f, 6.63981f, "J0922+0638", Pulsar::ATNF));
    ps.add(Pulsar(140.56234f, -49.82002f, "J0922-4949", Pulsar::ATNF));
    ps.add(Pulsar(140.99167f, -31.95000f, "J0923-31", Pulsar::ATNF));
    ps.add(Pulsar(141.03621f, -53.04561f, "J0924-5302", Pulsar::ATNF));
    ps.add(Pulsar(141.12917f, -58.23511f, "J0924-5814", Pulsar::ATNF));
    ps.add(Pulsar(141.90417f, 23.78333f, "J0927+23", Pulsar::ATNF));
    ps.add(Pulsar(143.16312f, -32.28728f, "J0932-3217", Pulsar::ATNF));
    ps.add(Pulsar(143.09525f, -53.45306f, "J0932-5327", Pulsar::ATNF));
    ps.add(Pulsar(143.61792f, -52.82422f, "J0934-5249", Pulsar::ATNF));
    ps.add(Pulsar(143.74250f, -41.90542f, "J0934-4154", Pulsar::ATNF));
    ps.add(Pulsar(145.24258f, -54.47794f, "J0940-5428", Pulsar::ATNF));
    ps.add(Pulsar(145.40833f, -39.68333f, "J0941-39", Pulsar::ATNF));
    ps.add(Pulsar(145.42145f, -52.73458f, "J0941-5244", Pulsar::ATNF));
    ps.add(Pulsar(145.56596f, -55.88119f, "J0942-5552", Pulsar::ATNF));
    ps.add(Pulsar(145.72692f, -56.96207f, "J0942-5657", Pulsar::ATNF));
    ps.add(Pulsar(145.87542f, 16.52694f, "J0943+1631", Pulsar::ATNF));
    ps.add(Pulsar(145.85417f, 22.93333f, "J0943+22", Pulsar::ATNF));
    ps.add(Pulsar(146.12065f, -13.91156f, "J0944-1354", Pulsar::ATNF));
    ps.add(Pulsar(146.40939f, -48.55403f, "J0945-4833", Pulsar::ATNF));
    ps.add(Pulsar(146.53167f, 9.86528f, "J0946+0951", Pulsar::ATNF));
    ps.add(Pulsar(146.84167f, 27.70000f, "J0947+27", Pulsar::ATNF));
    ps.add(Pulsar(148.07167f, -38.65283f, "J0952-3839", Pulsar::ATNF));
    ps.add(Pulsar(148.28879f, 7.92660f, "J0953+0755", Pulsar::ATNF));
    ps.add(Pulsar(148.52517f, -54.51486f, "J0954-5430", Pulsar::ATNF));
    ps.add(Pulsar(148.87292f, -53.07131f, "J0955-5304", Pulsar::ATNF));
    ps.add(Pulsar(149.48337f, -54.53442f, "J0957-5432", Pulsar::ATNF));
    ps.add(Pulsar(149.85942f, -48.16319f, "J0959-4809", Pulsar::ATNF));
    ps.add(Pulsar(150.11725f, -51.83281f, "J1000-5149", Pulsar::ATNF));
    ps.add(Pulsar(150.40825f, -55.11853f, "J1001-5507", Pulsar::ATNF));
    ps.add(Pulsar(150.28583f, -55.98339f, "J1001-5559", Pulsar::ATNF));
    ps.add(Pulsar(150.38429f, -59.65494f, "J1001-5939", Pulsar::ATNF));
    ps.add(Pulsar(150.74125f, -55.99361f, "J1002-5559", Pulsar::ATNF));
    ps.add(Pulsar(150.83992f, -47.78389f, "J1003-4747", Pulsar::ATNF));
    ps.add(Pulsar(151.65250f, -63.19478f, "J1006-6311", Pulsar::ATNF));
    ps.add(Pulsar(153.14053f, -23.63956f, "J1012-2337", Pulsar::ATNF));
    ps.add(Pulsar(153.20208f, -58.96111f, "J1012-5857", Pulsar::ATNF));
    ps.add(Pulsar(153.13931f, 53.11739f, "J1012+5307", Pulsar::ATNF));
    ps.add(Pulsar(153.22875f, -58.50711f, "J1012-5830", Pulsar::ATNF));
    ps.add(Pulsar(153.38273f, -59.57408f, "J1013-5934", Pulsar::ATNF));
    ps.add(Pulsar(153.90817f, -57.32022f, "J1015-5719", Pulsar::ATNF));
    ps.add(Pulsar(154.13000f, -53.75383f, "J1016-5345", Pulsar::ATNF));
    ps.add(Pulsar(154.05042f, -58.31699f, "J1016-5819", Pulsar::ATNF));
    ps.add(Pulsar(154.08817f, -58.95336f, "J1016-5857", Pulsar::ATNF));
    ps.add(Pulsar(154.30358f, -56.35853f, "J1017-5621", Pulsar::ATNF));
    ps.add(Pulsar(154.66819f, -16.70282f, "J1018-1642", Pulsar::ATNF));
    ps.add(Pulsar(154.96725f, -57.81831f, "J1019-5749", Pulsar::ATNF));
    ps.add(Pulsar(155.05846f, -59.35944f, "J1020-5921", Pulsar::ATNF));
    ps.add(Pulsar(155.04750f, -60.43522f, "J1020-6026", Pulsar::ATNF));
    ps.add(Pulsar(155.35342f, -56.03081f, "J1021-5601", Pulsar::ATNF));
    ps.add(Pulsar(155.74168f, 10.03132f, "J1022+1001", Pulsar::ATNF));
    ps.add(Pulsar(155.61708f, -58.22500f, "J1022-5813", Pulsar::ATNF));
    ps.add(Pulsar(155.94870f, 0.64476f, "J1023+0038", Pulsar::ATNF));
    ps.add(Pulsar(156.16120f, -7.32199f, "J1024-0719", Pulsar::ATNF));
    ps.add(Pulsar(157.11667f, -58.31811f, "J1028-5819", Pulsar::ATNF));
    ps.add(Pulsar(157.75933f, -61.29739f, "J1031-6117", Pulsar::ATNF));
    ps.add(Pulsar(158.02917f, -59.16667f, "J1032-5911", Pulsar::ATNF));
    ps.add(Pulsar(158.11538f, -52.10236f, "J1032-5206", Pulsar::ATNF));
    ps.add(Pulsar(158.58130f, -32.40722f, "J1034-3224", Pulsar::ATNF));
    ps.add(Pulsar(158.76283f, -63.75511f, "J1035-6345", Pulsar::ATNF));
    ps.add(Pulsar(159.05447f, -49.43938f, "J1036-4926", Pulsar::ATNF));
    ps.add(Pulsar(159.58208f, -58.52422f, "J1038-5831", Pulsar::ATNF));
    ps.add(Pulsar(159.61222f, 0.54544f, "J1038+0032", Pulsar::ATNF));
    ps.add(Pulsar(160.40082f, -19.70378f, "J1041-1942", Pulsar::ATNF));
    ps.add(Pulsar(160.50254f, -55.35153f, "J1042-5521", Pulsar::ATNF));
    ps.add(Pulsar(160.98038f, -61.28078f, "J1043-6116", Pulsar::ATNF));
    ps.add(Pulsar(161.45912f, -45.16504f, "J1045-4509", Pulsar::ATNF));
    ps.add(Pulsar(161.57863f, -58.23106f, "J1046-5813", Pulsar::ATNF));
    ps.add(Pulsar(161.68013f, 3.06858f, "J1046+0304", Pulsar::ATNF));
    ps.add(Pulsar(161.75339f, -30.53833f, "J1047-3032", Pulsar::ATNF));
    ps.add(Pulsar(161.75000f, -58.68333f, "J1047-58", Pulsar::ATNF));
    ps.add(Pulsar(161.86784f, -67.16419f, "J1047-6709", Pulsar::ATNF));
    ps.add(Pulsar(162.05083f, -58.53494f, "J1048-5832", Pulsar::ATNF));
    ps.add(Pulsar(162.52967f, -59.88928f, "J1048-5937", Pulsar::ATNF));
    ps.add(Pulsar(162.45975f, -58.56239f, "J1049-5833", Pulsar::ATNF));
    ps.add(Pulsar(163.15879f, -59.91225f, "J1052-5954", Pulsar::ATNF));
    ps.add(Pulsar(163.22246f, -63.80461f, "J1052-6348", Pulsar::ATNF));
    ps.add(Pulsar(163.74062f, -59.72058f, "J1054-5943", Pulsar::ATNF));
    ps.add(Pulsar(163.53683f, -64.87708f, "J1054-6452", Pulsar::ATNF));
    ps.add(Pulsar(163.95208f, -60.38111f, "J1055-6022", Pulsar::ATNF));
    ps.add(Pulsar(163.91329f, -60.47708f, "J1055-6028", Pulsar::ATNF));
    ps.add(Pulsar(163.97754f, -62.61342f, "J1055-6236", Pulsar::ATNF));
    ps.add(Pulsar(163.93629f, -69.08650f, "J1055-6905", Pulsar::ATNF));
    ps.add(Pulsar(164.10646f, -62.97989f, "J1056-6258", Pulsar::ATNF));
    ps.add(Pulsar(164.18246f, -57.15958f, "J1056-5709", Pulsar::ATNF));
    ps.add(Pulsar(164.49517f, -52.44897f, "J1057-5226", Pulsar::ATNF));
    ps.add(Pulsar(164.36417f, -79.24006f, "J1057-7914", Pulsar::ATNF));
    ps.add(Pulsar(164.47141f, -47.91600f, "J1057-4754", Pulsar::ATNF));
    ps.add(Pulsar(164.64271f, -59.96011f, "J1058-5957", Pulsar::ATNF));
    ps.add(Pulsar(164.75375f, -57.70394f, "J1059-5742", Pulsar::ATNF));
    ps.add(Pulsar(165.88117f, -60.42677f, "J1103-6025", Pulsar::ATNF));
    ps.add(Pulsar(166.07192f, -61.05108f, "J1104-6103", Pulsar::ATNF));
    ps.add(Pulsar(166.35904f, -61.13094f, "J1105-6107", Pulsar::ATNF));
    ps.add(Pulsar(166.61850f, -64.65000f, "J1106-6438", Pulsar::ATNF));
    ps.add(Pulsar(166.99458f, -59.78444f, "J1107-5947", Pulsar::ATNF));
    ps.add(Pulsar(166.89358f, -59.12186f, "J1107-5907", Pulsar::ATNF));
    ps.add(Pulsar(166.80125f, -61.73306f, "J1107-6143", Pulsar::ATNF));
    ps.add(Pulsar(167.50167f, -56.62581f, "J1110-5637", Pulsar::ATNF));
    ps.add(Pulsar(168.16054f, -66.21800f, "J1112-6613", Pulsar::ATNF));
    ps.add(Pulsar(168.21250f, -69.44194f, "J1112-6926", Pulsar::ATNF));
    ps.add(Pulsar(168.06171f, -61.05864f, "J1112-6103", Pulsar::ATNF));
    ps.add(Pulsar(168.59383f, -61.00950f, "J1114-6100", Pulsar::ATNF));
    ps.add(Pulsar(168.91000f, 50.50341f, "J1115+5030", Pulsar::ATNF));
    ps.add(Pulsar(168.97367f, -60.87161f, "J1115-6052", Pulsar::ATNF));
    ps.add(Pulsar(169.17953f, -41.37888f, "J1116-4122", Pulsar::ATNF));
    ps.add(Pulsar(169.34921f, -61.90611f, "J1117-6154", Pulsar::ATNF));
    ps.add(Pulsar(169.43750f, -64.79944f, "J1117-6447", Pulsar::ATNF));
    ps.add(Pulsar(169.92000f, -79.60842f, "J1119-7936", Pulsar::ATNF));
    ps.add(Pulsar(169.80958f, -61.46375f, "J1119-6127", Pulsar::ATNF));
    ps.add(Pulsar(170.32995f, -54.73486f, "J1121-5444", Pulsar::ATNF));
    ps.add(Pulsar(170.81521f, -48.73965f, "J1123-4844", Pulsar::ATNF));
    ps.add(Pulsar(170.92375f, -61.03506f, "J1123-6102", Pulsar::ATNF));
    ps.add(Pulsar(170.98145f, -62.98632f, "J1123-6259", Pulsar::ATNF));
    ps.add(Pulsar(170.96754f, -66.86363f, "J1123-6651", Pulsar::ATNF));
    ps.add(Pulsar(171.23529f, -56.64436f, "J1124-5638", Pulsar::ATNF));
    ps.add(Pulsar(171.16292f, -59.27222f, "J1124-5916", Pulsar::ATNF));
    ps.add(Pulsar(171.24792f, -64.35472f, "J1124-6421", Pulsar::ATNF));
    ps.add(Pulsar(171.48007f, -60.23517f, "J1125-6014", Pulsar::ATNF));
    ps.add(Pulsar(171.71920f, -60.91110f, "J1126-6054", Pulsar::ATNF));
    ps.add(Pulsar(171.59029f, -69.70425f, "J1126-6942", Pulsar::ATNF));
    ps.add(Pulsar(172.19458f, -62.31917f, "J1128-6219", Pulsar::ATNF));
    ps.add(Pulsar(172.33750f, -53.51667f, "J1129-53", Pulsar::ATNF));
    ps.add(Pulsar(172.56804f, -58.43397f, "J1130-5826", Pulsar::ATNF));
    ps.add(Pulsar(172.54350f, -59.42614f, "J1130-5925", Pulsar::ATNF));
    ps.add(Pulsar(172.63297f, -68.12459f, "J1130-6807", Pulsar::ATNF));
    ps.add(Pulsar(173.06558f, -56.45803f, "J1132-5627", Pulsar::ATNF));
    ps.add(Pulsar(173.46437f, -62.84739f, "J1133-6250", Pulsar::ATNF));
    ps.add(Pulsar(174.01353f, 15.85124f, "J1136+1551", Pulsar::ATNF));
    ps.add(Pulsar(174.00904f, -55.41897f, "J1136-5525", Pulsar::ATNF));
    ps.add(Pulsar(174.38370f, -67.00917f, "J1137-6700", Pulsar::ATNF));
    ps.add(Pulsar(174.59008f, -62.13314f, "J1138-6207", Pulsar::ATNF));
    ps.add(Pulsar(175.35679f, -31.13128f, "J1141-3107", Pulsar::ATNF));
    ps.add(Pulsar(175.42817f, -33.37706f, "J1141-3322", Pulsar::ATNF));
    ps.add(Pulsar(175.27922f, -65.75531f, "J1141-6545", Pulsar::ATNF));
    ps.add(Pulsar(175.71875f, -62.50108f, "J1142-6230", Pulsar::ATNF));
    ps.add(Pulsar(175.81277f, -51.97775f, "J1143-5158", Pulsar::ATNF));
    ps.add(Pulsar(176.14500f, -61.78028f, "J1144-6146", Pulsar::ATNF));
    ps.add(Pulsar(176.00879f, -62.29175f, "J1144-6217", Pulsar::ATNF));
    ps.add(Pulsar(176.53225f, -60.51650f, "J1146-6030", Pulsar::ATNF));
    ps.add(Pulsar(177.11888f, -57.42017f, "J1148-5725", Pulsar::ATNF));
    ps.add(Pulsar(177.15750f, -64.25917f, "J1148-6415", Pulsar::ATNF));
    ps.add(Pulsar(178.04167f, -58.00944f, "J1152-5800", Pulsar::ATNF));
    ps.add(Pulsar(178.22417f, -60.20583f, "J1152-6012", Pulsar::ATNF));
    ps.add(Pulsar(178.58375f, -62.83408f, "J1154-6250", Pulsar::ATNF));
    ps.add(Pulsar(179.03104f, -57.11719f, "J1156-5707", Pulsar::ATNF));
    ps.add(Pulsar(179.08958f, -59.15342f, "J1156-5909", Pulsar::ATNF));
    ps.add(Pulsar(179.31350f, -62.41413f, "J1157-6224", Pulsar::ATNF));
    ps.add(Pulsar(179.28402f, -51.21559f, "J1157-5112", Pulsar::ATNF));
    ps.add(Pulsar(179.84042f, -64.16583f, "J1159-6409", Pulsar::ATNF));
    ps.add(Pulsar(179.89879f, -79.16822f, "J1159-7910", Pulsar::ATNF));
    ps.add(Pulsar(180.34588f, -63.11653f, "J1201-6306", Pulsar::ATNF));
    ps.add(Pulsar(180.61835f, -58.34261f, "J1202-5820", Pulsar::ATNF));
    ps.add(Pulsar(181.15300f, -68.72144f, "J1204-6843", Pulsar::ATNF));
    ps.add(Pulsar(182.50379f, -52.44122f, "J1210-5226", Pulsar::ATNF));
    ps.add(Pulsar(182.52495f, -55.98442f, "J1210-5559", Pulsar::ATNF));
    ps.add(Pulsar(182.67500f, -65.83461f, "J1210-6550", Pulsar::ATNF));
    ps.add(Pulsar(182.85075f, -63.41256f, "J1211-6324", Pulsar::ATNF));
    ps.add(Pulsar(183.53508f, -58.50719f, "J1214-5830", Pulsar::ATNF));
    ps.add(Pulsar(183.75258f, -53.47544f, "J1215-5328", Pulsar::ATNF));
    ps.add(Pulsar(184.17454f, -62.39939f, "J1216-6223", Pulsar::ATNF));
    ps.add(Pulsar(184.03058f, -64.16923f, "J1216-6410", Pulsar::ATNF));
    ps.add(Pulsar(185.07442f, -63.31292f, "J1220-6318", Pulsar::ATNF));
    ps.add(Pulsar(185.71792f, -57.63889f, "J1222-5738", Pulsar::ATNF));
    ps.add(Pulsar(186.09244f, -64.13164f, "J1224-6407", Pulsar::ATNF));
    ps.add(Pulsar(186.18438f, -62.14475f, "J1224-6208", Pulsar::ATNF));
    ps.add(Pulsar(186.42833f, -64.14528f, "J1225-6408", Pulsar::ATNF));
    ps.add(Pulsar(186.43261f, -55.94532f, "J1225-5556", Pulsar::ATNF));
    ps.add(Pulsar(186.36929f, -60.59378f, "J1225-6035", Pulsar::ATNF));
    ps.add(Pulsar(186.67083f, -32.46667f, "J1226-32", Pulsar::ATNF));
    ps.add(Pulsar(187.94067f, -46.16256f, "J1231-4609", Pulsar::ATNF));
    ps.add(Pulsar(187.80417f, -63.05500f, "J1231-6303", Pulsar::ATNF));
    ps.add(Pulsar(188.07958f, -47.71414f, "J1232-4742", Pulsar::ATNF));
    ps.add(Pulsar(188.07433f, -65.01759f, "J1232-6501", Pulsar::ATNF));
    ps.add(Pulsar(188.38125f, -63.20806f, "J1233-6312", Pulsar::ATNF));
    ps.add(Pulsar(188.41625f, -63.74861f, "J1233-6344", Pulsar::ATNF));
    ps.add(Pulsar(188.82917f, -55.26667f, "J1235-5516", Pulsar::ATNF));
    ps.add(Pulsar(188.99050f, -63.90844f, "J1235-6354", Pulsar::ATNF));
    ps.add(Pulsar(189.24646f, -50.56008f, "J1236-5033", Pulsar::ATNF));
    ps.add(Pulsar(189.35792f, -67.42608f, "J1237-6725", Pulsar::ATNF));
    ps.add(Pulsar(189.59657f, 21.86974f, "J1238+21", Pulsar::ATNF));
    ps.add(Pulsar(189.99583f, -68.54103f, "J1239-6832", Pulsar::ATNF));
    ps.add(Pulsar(189.91859f, 24.89702f, "J1239+2453", Pulsar::ATNF));
    ps.add(Pulsar(190.07338f, -41.41436f, "J1240-4124", Pulsar::ATNF));
    ps.add(Pulsar(190.82149f, -64.38996f, "J1243-6423", Pulsar::ATNF));
    ps.add(Pulsar(190.89742f, -57.59522f, "J1243-5735", Pulsar::ATNF));
    ps.add(Pulsar(191.04783f, -50.88906f, "J1244-5053", Pulsar::ATNF));
    ps.add(Pulsar(191.15958f, -65.52000f, "J1244-6531", Pulsar::ATNF));
    ps.add(Pulsar(191.33800f, -62.64886f, "J1245-6238", Pulsar::ATNF));
    ps.add(Pulsar(191.65833f, 22.88333f, "J1246+22", Pulsar::ATNF));
    ps.add(Pulsar(192.19317f, -63.73600f, "J1248-6344", Pulsar::ATNF));
    ps.add(Pulsar(192.47633f, -65.12217f, "J1249-6507", Pulsar::ATNF));
    ps.add(Pulsar(193.17758f, -63.24242f, "J1252-6314", Pulsar::ATNF));
    ps.add(Pulsar(193.36809f, -58.34469f, "J1253-5820", Pulsar::ATNF));
    ps.add(Pulsar(193.63533f, -61.84744f, "J1254-6150", Pulsar::ATNF));
    ps.add(Pulsar(193.97858f, -61.51947f, "J1255-6131", Pulsar::ATNF));
    ps.add(Pulsar(194.26987f, -10.45160f, "J1257-1027", Pulsar::ATNF));
    ps.add(Pulsar(194.84458f, -67.69444f, "J1259-6741", Pulsar::ATNF));
    ps.add(Pulsar(195.01490f, 12.68235f, "J1300+1240", Pulsar::ATNF));
    ps.add(Pulsar(195.44067f, -63.09275f, "J1301-6305", Pulsar::ATNF));
    ps.add(Pulsar(195.36792f, -63.17792f, "J1301-6310", Pulsar::ATNF));
    ps.add(Pulsar(195.69854f, -63.83575f, "J1302-6350", Pulsar::ATNF));
    ps.add(Pulsar(195.51667f, -63.74806f, "J1302-63", Pulsar::ATNF));
    ps.add(Pulsar(195.58000f, -63.22472f, "J1302-6313", Pulsar::ATNF));
    ps.add(Pulsar(195.75008f, -63.08356f, "J1303-6305", Pulsar::ATNF));
    ps.add(Pulsar(196.34875f, -64.92397f, "J1305-6455", Pulsar::ATNF));
    ps.add(Pulsar(196.33708f, -62.05600f, "J1305-6203", Pulsar::ATNF));
    ps.add(Pulsar(196.36667f, -62.94417f, "J1305-6256", Pulsar::ATNF));
    ps.add(Pulsar(196.65912f, -66.28939f, "J1306-6617", Pulsar::ATNF));
    ps.add(Pulsar(196.68583f, -62.70083f, "J1306-6242", Pulsar::ATNF));
    ps.add(Pulsar(196.97792f, -63.30972f, "J1307-6318", Pulsar::ATNF));
    ps.add(Pulsar(197.20112f, 21.45188f, "J1308+2127", Pulsar::ATNF));
    ps.add(Pulsar(199.68579f, -46.84158f, "J1308-4650", Pulsar::ATNF));
    ps.add(Pulsar(197.08783f, -58.73717f, "J1308-5844", Pulsar::ATNF));
    ps.add(Pulsar(197.31917f, -64.26639f, "J1309-6415", Pulsar::ATNF));
    ps.add(Pulsar(197.25121f, -65.43794f, "J1309-6526", Pulsar::ATNF));
    ps.add(Pulsar(197.96937f, -12.46712f, "J1311-1228", Pulsar::ATNF));
    ps.add(Pulsar(198.01917f, -54.04500f, "J1312-5402", Pulsar::ATNF));
    ps.add(Pulsar(198.22429f, -55.27975f, "J1312-5516", Pulsar::ATNF));
    ps.add(Pulsar(198.22917f, 18.16667f, "J1312+1810", Pulsar::ATNF));
    ps.add(Pulsar(198.02983f, -64.01544f, "J1312-6400", Pulsar::ATNF));
    ps.add(Pulsar(198.34583f, 9.53222f, "J1313+0931", Pulsar::ATNF));
    ps.add(Pulsar(198.59750f, -61.02111f, "J1314-6101", Pulsar::ATNF));
    ps.add(Pulsar(199.19292f, -62.53672f, "J1316-6232", Pulsar::ATNF));
    ps.add(Pulsar(199.44287f, -57.99181f, "J1317-5759", Pulsar::ATNF));
    ps.add(Pulsar(199.43621f, -63.04783f, "J1317-6302", Pulsar::ATNF));
    ps.add(Pulsar(199.83454f, -60.94630f, "J1319-6056", Pulsar::ATNF));
    ps.add(Pulsar(199.85967f, -61.09061f, "J1319-6105", Pulsar::ATNF));
    ps.add(Pulsar(200.22467f, -53.98511f, "J1320-5359", Pulsar::ATNF));
    ps.add(Pulsar(200.05283f, -35.20722f, "J1320-3512", Pulsar::ATNF));
    ps.add(Pulsar(200.44242f, 83.39414f, "J1321+8323", Pulsar::ATNF));
    ps.add(Pulsar(200.41483f, -59.38114f, "J1321-5922", Pulsar::ATNF));
    ps.add(Pulsar(200.63396f, -62.69819f, "J1322-6241", Pulsar::ATNF));
    ps.add(Pulsar(200.57500f, -63.49361f, "J1322-6329", Pulsar::ATNF));
    ps.add(Pulsar(201.18292f, -61.76667f, "J1324-6146", Pulsar::ATNF));
    ps.add(Pulsar(201.05687f, -63.03919f, "J1324-6302", Pulsar::ATNF));
    ps.add(Pulsar(201.51333f, -67.01417f, "J1326-6700", Pulsar::ATNF));
    ps.add(Pulsar(201.74279f, -58.99142f, "J1326-5859", Pulsar::ATNF));
    ps.add(Pulsar(201.63583f, -64.14556f, "J1326-6408", Pulsar::ATNF));
    ps.add(Pulsar(201.82250f, -62.37906f, "J1327-6222", Pulsar::ATNF));
    ps.add(Pulsar(201.78113f, -63.02094f, "J1327-6301", Pulsar::ATNF));
    ps.add(Pulsar(201.79279f, -64.00364f, "J1327-6400", Pulsar::ATNF));
    ps.add(Pulsar(202.02680f, -43.96226f, "J1328-4357", Pulsar::ATNF));
    ps.add(Pulsar(202.13992f, -49.35928f, "J1328-4921", Pulsar::ATNF));
    ps.add(Pulsar(202.26375f, -61.98306f, "J1329-6158", Pulsar::ATNF));
    ps.add(Pulsar(203.21869f, -30.53814f, "J1332-3032", Pulsar::ATNF));
    ps.add(Pulsar(203.43679f, -44.82395f, "J1333-4449", Pulsar::ATNF));
    ps.add(Pulsar(203.62545f, -58.65460f, "J1334-5839", Pulsar::ATNF));
    ps.add(Pulsar(203.90000f, -36.70000f, "J1335-3642", Pulsar::ATNF));
    ps.add(Pulsar(204.33479f, -63.10647f, "J1337-6306", Pulsar::ATNF));
    ps.add(Pulsar(204.53896f, -62.07186f, "J1338-6204", Pulsar::ATNF));
    ps.add(Pulsar(204.98579f, -47.20153f, "J1339-4712", Pulsar::ATNF));
    ps.add(Pulsar(204.98583f, -66.30217f, "J1339-6618", Pulsar::ATNF));
    ps.add(Pulsar(205.00133f, -64.94525f, "J1340-6456", Pulsar::ATNF));
    ps.add(Pulsar(205.42762f, -62.33908f, "J1341-6220", Pulsar::ATNF));
    ps.add(Pulsar(205.28071f, -60.39297f, "J1341-6023", Pulsar::ATNF));
    ps.add(Pulsar(205.54583f, 28.37556f, "J1342+2822A", Pulsar::ATNF));
    ps.add(Pulsar(205.54620f, 28.37782f, "J1342+2822B", Pulsar::ATNF));
    ps.add(Pulsar(205.54583f, 28.37556f, "J1342+2822C", Pulsar::ATNF));
    ps.add(Pulsar(205.54250f, 28.37667f, "J1342+2822D", Pulsar::ATNF));
    ps.add(Pulsar(206.16500f, -60.99194f, "J1344-6059", Pulsar::ATNF));
    ps.add(Pulsar(206.43512f, -61.25850f, "J1345-6115", Pulsar::ATNF));
    ps.add(Pulsar(206.83075f, -59.79439f, "J1347-5947", Pulsar::ATNF));
    ps.add(Pulsar(207.17667f, -63.11778f, "J1348-6307", Pulsar::ATNF));
    ps.add(Pulsar(207.40271f, -61.50475f, "J1349-6130", Pulsar::ATNF));
    ps.add(Pulsar(207.56716f, -51.25688f, "J1350-5115", Pulsar::ATNF));
    ps.add(Pulsar(208.14354f, -68.06031f, "J1352-6803", Pulsar::ATNF));
    ps.add(Pulsar(208.64583f, -62.82500f, "J1354-6249", Pulsar::ATNF));
    ps.add(Pulsar(208.99450f, -51.89828f, "J1355-5153", Pulsar::ATNF));
    ps.add(Pulsar(208.90396f, -57.78750f, "J1355-5747", Pulsar::ATNF));
    ps.add(Pulsar(208.99629f, -59.41692f, "J1355-5925", Pulsar::ATNF));
    ps.add(Pulsar(208.83892f, -62.10558f, "J1355-6206", Pulsar::ATNF));
    ps.add(Pulsar(209.21066f, -55.35360f, "J1356-5521", Pulsar::ATNF));
    ps.add(Pulsar(209.35417f, -62.46667f, "J1357-62", Pulsar::ATNF));
    ps.add(Pulsar(209.26012f, -64.49172f, "J1357-6429", Pulsar::ATNF));
    ps.add(Pulsar(209.67500f, -25.55000f, "J1358-2533", Pulsar::ATNF));
    ps.add(Pulsar(209.99258f, -60.63556f, "J1359-6038", Pulsar::ATNF));
    ps.add(Pulsar(210.19038f, -63.42850f, "J1400-6325", Pulsar::ATNF));
    ps.add(Pulsar(210.46867f, -63.96265f, "J1401-6357", Pulsar::ATNF));
    ps.add(Pulsar(210.73750f, -51.40000f, "J1402-5124", Pulsar::ATNF));
    ps.add(Pulsar(210.80833f, -63.17417f, "J1403-6310", Pulsar::ATNF));
    ps.add(Pulsar(210.76708f, -76.78264f, "J1403-7646", Pulsar::ATNF));
    ps.add(Pulsar(211.30171f, -56.69006f, "J1405-5641", Pulsar::ATNF));
    ps.add(Pulsar(211.50500f, -58.10889f, "J1406-5806", Pulsar::ATNF));
    ps.add(Pulsar(211.70850f, -61.35775f, "J1406-6121", Pulsar::ATNF));
    ps.add(Pulsar(211.99433f, -60.81633f, "J1407-6048", Pulsar::ATNF));
    ps.add(Pulsar(211.98542f, -61.89972f, "J1407-6153", Pulsar::ATNF));
    ps.add(Pulsar(212.60000f, -61.53333f, "J1410-6132", Pulsar::ATNF));
    ps.add(Pulsar(212.53071f, -74.08148f, "J1410-7404", Pulsar::ATNF));
    ps.add(Pulsar(213.24842f, -61.19181f, "J1412-6111", Pulsar::ATNF));
    ps.add(Pulsar(213.03204f, -61.75800f, "J1412-6145", Pulsar::ATNF));
    ps.add(Pulsar(213.38048f, -63.12651f, "J1413-6307", Pulsar::ATNF));
    ps.add(Pulsar(213.29113f, -61.68694f, "J1413-6141", Pulsar::ATNF));
    ps.add(Pulsar(213.27279f, -62.37439f, "J1413-6222", Pulsar::ATNF));
    ps.add(Pulsar(213.60708f, -68.04944f, "J1414-6802", Pulsar::ATNF));
    ps.add(Pulsar(213.88029f, -66.35339f, "J1415-6621", Pulsar::ATNF));
    ps.add(Pulsar(214.12746f, -60.63319f, "J1416-6037", Pulsar::ATNF));
    ps.add(Pulsar(214.70951f, -39.35514f, "J1418-3921", Pulsar::ATNF));
    ps.add(Pulsar(214.63458f, -59.75000f, "J1418-5945", Pulsar::ATNF));
    ps.add(Pulsar(214.67792f, -60.96750f, "J1418-6058", Pulsar::ATNF));
    ps.add(Pulsar(215.12125f, -54.27306f, "J1420-5416", Pulsar::ATNF));
    ps.add(Pulsar(215.01276f, -56.43194f, "J1420-5625", Pulsar::ATNF));
    ps.add(Pulsar(215.03432f, -60.80456f, "J1420-6048", Pulsar::ATNF));
    ps.add(Pulsar(215.75000f, -56.78333f, "J1423-56", Pulsar::ATNF));
    ps.add(Pulsar(215.86163f, -69.89523f, "J1423-6953", Pulsar::ATNF));
    ps.add(Pulsar(216.05317f, -55.93719f, "J1424-5556", Pulsar::ATNF));
    ps.add(Pulsar(216.13379f, -58.38222f, "J1424-5822", Pulsar::ATNF));
    ps.add(Pulsar(216.24667f, -64.63611f, "J1424-6438", Pulsar::ATNF));
    ps.add(Pulsar(216.40233f, -57.39189f, "J1425-5723", Pulsar::ATNF));
    ps.add(Pulsar(216.49629f, -57.98617f, "J1425-5759", Pulsar::ATNF));
    ps.add(Pulsar(216.28208f, -62.16803f, "J1425-6210", Pulsar::ATNF));
    ps.add(Pulsar(216.96154f, -41.98231f, "J1427-4158", Pulsar::ATNF));
    ps.add(Pulsar(217.10957f, -55.51394f, "J1428-5530", Pulsar::ATNF));
    ps.add(Pulsar(217.35792f, -59.59969f, "J1429-5935", Pulsar::ATNF));
    ps.add(Pulsar(217.67030f, -66.38473f, "J1430-6623", Pulsar::ATNF));
    ps.add(Pulsar(218.30492f, -60.64297f, "J1433-6038", Pulsar::ATNF));
    ps.add(Pulsar(218.52208f, -60.10806f, "J1434-6006", Pulsar::ATNF));
    ps.add(Pulsar(218.66292f, -60.49694f, "J1434-6029", Pulsar::ATNF));
    ps.add(Pulsar(218.75150f, -59.91367f, "J1435-5954", Pulsar::ATNF));
    ps.add(Pulsar(218.83449f, -61.01610f, "J1435-6100", Pulsar::ATNF));
    ps.add(Pulsar(219.25796f, -59.98372f, "J1437-5959", Pulsar::ATNF));
    ps.add(Pulsar(219.31379f, -61.76722f, "J1437-6146", Pulsar::ATNF));
    ps.add(Pulsar(219.91559f, -55.02323f, "J1439-5501", Pulsar::ATNF));
    ps.add(Pulsar(220.13045f, -63.74656f, "J1440-6344", Pulsar::ATNF));
    ps.add(Pulsar(220.43458f, -61.62333f, "J1441-6137", Pulsar::ATNF));
    ps.add(Pulsar(221.19375f, -59.68861f, "J1444-5941", Pulsar::ATNF));
    ps.add(Pulsar(221.02508f, -60.43594f, "J1444-6026", Pulsar::ATNF));
    ps.add(Pulsar(222.35596f, -58.77789f, "J1449-5846", Pulsar::ATNF));
    ps.add(Pulsar(223.21908f, -58.85369f, "J1452-5851", Pulsar::ATNF));
    ps.add(Pulsar(223.21624f, -60.60871f, "J1452-6036", Pulsar::ATNF));
    ps.add(Pulsar(223.38640f, -64.22100f, "J1453-6413", Pulsar::ATNF));
    ps.add(Pulsar(223.44049f, 19.03673f, "J1453+1902", Pulsar::ATNF));
    ps.add(Pulsar(223.54545f, -58.77632f, "J1454-5846", Pulsar::ATNF));
    ps.add(Pulsar(223.94984f, -33.51288f, "J1455-3330", Pulsar::ATNF));
    ps.add(Pulsar(224.00066f, -68.72757f, "J1456-6843", Pulsar::ATNF));
    ps.add(Pulsar(224.41746f, -51.38139f, "J1457-5122", Pulsar::ATNF));
    ps.add(Pulsar(224.41250f, -59.01417f, "J1457-5900", Pulsar::ATNF));
    ps.add(Pulsar(224.38292f, -59.03444f, "J1457-5902", Pulsar::ATNF));
    ps.add(Pulsar(224.87417f, -60.87778f, "J1459-60", Pulsar::ATNF));
    ps.add(Pulsar(225.46250f, -56.63000f, "J1501-5637", Pulsar::ATNF));
    ps.add(Pulsar(225.73912f, -56.89423f, "J1502-5653", Pulsar::ATNF));
    ps.add(Pulsar(225.68250f, -58.47833f, "J1502-5828", Pulsar::ATNF));
    ps.add(Pulsar(225.62433f, -61.48064f, "J1502-6128", Pulsar::ATNF));
    ps.add(Pulsar(225.97750f, 21.18592f, "J1503+2111", Pulsar::ATNF));
    ps.add(Pulsar(226.20475f, -56.35889f, "J1504-5621", Pulsar::ATNF));
    ps.add(Pulsar(226.66458f, -51.96944f, "J1506-5158", Pulsar::ATNF));
    ps.add(Pulsar(226.95275f, -66.68253f, "J1507-6640", Pulsar::ATNF));
    ps.add(Pulsar(226.89242f, -43.86756f, "J1507-4352", Pulsar::ATNF));
    ps.add(Pulsar(227.35679f, 55.52567f, "J1509+5531", Pulsar::ATNF));
    ps.add(Pulsar(227.36304f, -58.84892f, "J1509-5850", Pulsar::ATNF));
    ps.add(Pulsar(227.28125f, -60.25517f, "J1509-6015", Pulsar::ATNF));
    ps.add(Pulsar(227.70375f, -44.36861f, "J1510-4422", Pulsar::ATNF));
    ps.add(Pulsar(227.96378f, -54.24453f, "J1511-5414", Pulsar::ATNF));
    ps.add(Pulsar(227.77917f, -58.59111f, "J1511-5835", Pulsar::ATNF));
    ps.add(Pulsar(228.17927f, -57.99994f, "J1512-5759", Pulsar::ATNF));
    ps.add(Pulsar(228.02375f, -54.52194f, "J1512-5431", Pulsar::ATNF));
    ps.add(Pulsar(228.48175f, -59.13583f, "J1513-5908", Pulsar::ATNF));
    ps.add(Pulsar(228.49579f, -57.65039f, "J1513-5739", Pulsar::ATNF));
    ps.add(Pulsar(228.56067f, -48.57208f, "J1514-4834", Pulsar::ATNF));
    ps.add(Pulsar(228.50000f, -59.86667f, "J1514-59", Pulsar::ATNF));
    ps.add(Pulsar(228.74625f, -59.42869f, "J1514-5925", Pulsar::ATNF));
    ps.add(Pulsar(228.78875f, -57.34694f, "J1515-5720", Pulsar::ATNF));
    ps.add(Pulsar(229.36392f, -43.93831f, "J1517-4356", Pulsar::ATNF));
    ps.add(Pulsar(229.63882f, 2.09099f, "J1518+0204A", Pulsar::ATNF));
    ps.add(Pulsar(229.63107f, 2.08763f, "J1518+0204B", Pulsar::ATNF));
    ps.add(Pulsar(229.64167f, 2.08278f, "J1518+0204C", Pulsar::ATNF));
    ps.add(Pulsar(229.64167f, 2.08278f, "J1518+0204D", Pulsar::ATNF));
    ps.add(Pulsar(229.64167f, 2.08278f, "J1518+0204E", Pulsar::ATNF));
    ps.add(Pulsar(229.57000f, 49.07618f, "J1518+4904", Pulsar::ATNF));
    ps.add(Pulsar(229.55625f, -54.26250f, "J1518-5415", Pulsar::ATNF));
    ps.add(Pulsar(229.94829f, -57.57044f, "J1519-5734", Pulsar::ATNF));
    ps.add(Pulsar(229.89921f, -61.11517f, "J1519-6106", Pulsar::ATNF));
    ps.add(Pulsar(229.78983f, -63.13875f, "J1519-6308", Pulsar::ATNF));
    ps.add(Pulsar(230.67612f, -58.48408f, "J1522-5829", Pulsar::ATNF));
    ps.add(Pulsar(230.52792f, -55.42153f, "J1522-5525", Pulsar::ATNF));
    ps.add(Pulsar(231.20775f, -56.42317f, "J1524-5625", Pulsar::ATNF));
    ps.add(Pulsar(231.08929f, -57.10972f, "J1524-5706", Pulsar::ATNF));
    ps.add(Pulsar(231.10271f, -58.32058f, "J1524-5819", Pulsar::ATNF));
    ps.add(Pulsar(231.36813f, -54.28889f, "J1525-5417", Pulsar::ATNF));
    ps.add(Pulsar(231.42271f, -56.08694f, "J1525-5605", Pulsar::ATNF));
    ps.add(Pulsar(231.67167f, -56.56194f, "J1526-5633", Pulsar::ATNF));
    ps.add(Pulsar(231.92042f, -55.86861f, "J1527-5552", Pulsar::ATNF));
    ps.add(Pulsar(231.99554f, -39.52639f, "J1527-3931", Pulsar::ATNF));
    ps.add(Pulsar(232.14564f, -31.76857f, "J1528-3146", Pulsar::ATNF));
    ps.add(Pulsar(232.03347f, -41.15800f, "J1528-4109", Pulsar::ATNF));
    ps.add(Pulsar(232.49000f, -53.92667f, "J1529-5355", Pulsar::ATNF));
    ps.add(Pulsar(232.39917f, -56.19139f, "J1529-5611", Pulsar::ATNF));
    ps.add(Pulsar(232.61196f, -53.46564f, "J1530-5327", Pulsar::ATNF));
    ps.add(Pulsar(232.78354f, -40.20858f, "J1531-4012", Pulsar::ATNF));
    ps.add(Pulsar(232.86629f, -56.18194f, "J1531-5610", Pulsar::ATNF));
    ps.add(Pulsar(233.04318f, 27.76372f, "J1532+2745", Pulsar::ATNF));
    ps.add(Pulsar(233.14792f, -53.13500f, "J1532-5308", Pulsar::ATNF));
    ps.add(Pulsar(233.53487f, -53.57203f, "J1534-5334", Pulsar::ATNF));
    ps.add(Pulsar(233.63996f, -54.09436f, "J1534-5405", Pulsar::ATNF));
    ps.add(Pulsar(233.53333f, -46.23333f, "J1534-46", Pulsar::ATNF));
    ps.add(Pulsar(233.82113f, -41.23419f, "J1535-4114", Pulsar::ATNF));
    ps.add(Pulsar(233.98300f, -44.25233f, "J1535-4415", Pulsar::ATNF));
    ps.add(Pulsar(233.99275f, -54.84056f, "J1535-5450", Pulsar::ATNF));
    ps.add(Pulsar(233.81983f, -58.80769f, "J1535-5848", Pulsar::ATNF));
    ps.add(Pulsar(234.07242f, -36.04967f, "J1536-3602", Pulsar::ATNF));
    ps.add(Pulsar(234.02008f, -54.55417f, "J1536-5433", Pulsar::ATNF));
    ps.add(Pulsar(234.07383f, -59.11767f, "J1536-5907", Pulsar::ATNF));
    ps.add(Pulsar(234.29150f, 11.93210f, "J1537+1155", Pulsar::ATNF));
    ps.add(Pulsar(234.36750f, -49.20083f, "J1537-4912", Pulsar::ATNF));
    ps.add(Pulsar(234.31554f, -51.88500f, "J1537-5153", Pulsar::ATNF));
    ps.add(Pulsar(234.46250f, -56.75111f, "J1537-5645", Pulsar::ATNF));
    ps.add(Pulsar(234.70396f, -54.63806f, "J1538-5438", Pulsar::ATNF));
    ps.add(Pulsar(234.67012f, -55.32944f, "J1538-5519", Pulsar::ATNF));
    ps.add(Pulsar(234.68762f, -55.86025f, "J1538-5551", Pulsar::ATNF));
    ps.add(Pulsar(234.52375f, -56.63667f, "J1538-5638", Pulsar::ATNF));
    ps.add(Pulsar(234.57575f, -57.54147f, "J1538-5732", Pulsar::ATNF));
    ps.add(Pulsar(234.53504f, -57.83808f, "J1538-5750", Pulsar::ATNF));
    ps.add(Pulsar(234.80817f, -56.44039f, "J1539-5626", Pulsar::ATNF));
    ps.add(Pulsar(234.78321f, -55.35311f, "J1539-5521", Pulsar::ATNF));
    ps.add(Pulsar(234.85292f, -63.38144f, "J1539-6322", Pulsar::ATNF));
    ps.add(Pulsar(235.24592f, -57.61583f, "J1540-5736", Pulsar::ATNF));
    ps.add(Pulsar(235.45667f, -55.58361f, "J1541-5535", Pulsar::ATNF));
    ps.add(Pulsar(235.68946f, -50.56706f, "J1542-5034", Pulsar::ATNF));
    ps.add(Pulsar(235.58304f, -51.55972f, "J1542-5133", Pulsar::ATNF));
    ps.add(Pulsar(235.72713f, -53.06139f, "J1542-5303", Pulsar::ATNF));
    ps.add(Pulsar(235.87566f, -6.34590f, "J1543-0620", Pulsar::ATNF));
    ps.add(Pulsar(235.91177f, 9.48787f, "J1543+0929", Pulsar::ATNF));
    ps.add(Pulsar(235.99271f, -50.23278f, "J1543-5013", Pulsar::ATNF));
    ps.add(Pulsar(235.98438f, -54.98714f, "J1543-5459", Pulsar::ATNF));
    ps.add(Pulsar(236.24935f, -53.14625f, "J1544-5308", Pulsar::ATNF));
    ps.add(Pulsar(236.53083f, -53.03972f, "J1546-5302", Pulsar::ATNF));
    ps.add(Pulsar(236.87750f, -57.84150f, "J1547-5750", Pulsar::ATNF));
    ps.add(Pulsar(236.89579f, -58.65272f, "J1547-5839", Pulsar::ATNF));
    ps.add(Pulsar(237.09692f, -48.36381f, "J1548-4821", Pulsar::ATNF));
    ps.add(Pulsar(237.08112f, -49.46122f, "J1548-4927", Pulsar::ATNF));
    ps.add(Pulsar(237.18346f, -56.12608f, "J1548-5607", Pulsar::ATNF));
    ps.add(Pulsar(237.42059f, 21.22414f, "J1549+2113", Pulsar::ATNF));
    ps.add(Pulsar(237.33814f, -48.81040f, "J1549-4848", Pulsar::ATNF));
    ps.add(Pulsar(237.44958f, -57.36722f, "J1549-5722", Pulsar::ATNF));
    ps.add(Pulsar(237.51229f, -52.70194f, "J1550-5242", Pulsar::ATNF));
    ps.add(Pulsar(237.52000f, -53.28917f, "J1550-5317", Pulsar::ATNF));
    ps.add(Pulsar(237.72546f, -54.30658f, "J1550-5418", Pulsar::ATNF));
    ps.add(Pulsar(237.92188f, -53.18322f, "J1551-5310", Pulsar::ATNF));
    ps.add(Pulsar(238.49833f, -54.93500f, "J1553-5456", Pulsar::ATNF));
    ps.add(Pulsar(238.50000f, -52.16667f, "J1554-52", Pulsar::ATNF));
    ps.add(Pulsar(238.66875f, -55.20917f, "J1554-5512", Pulsar::ATNF));
    ps.add(Pulsar(238.88827f, -23.68608f, "J1555-2341", Pulsar::ATNF));
    ps.add(Pulsar(238.82480f, -31.57225f, "J1555-3134", Pulsar::ATNF));
    ps.add(Pulsar(239.21458f, -53.98194f, "J1556-5358", Pulsar::ATNF));
    ps.add(Pulsar(239.25108f, -42.97018f, "J1557-4258", Pulsar::ATNF));
    ps.add(Pulsar(239.67271f, -54.32389f, "J1558-5419", Pulsar::ATNF));
    ps.add(Pulsar(239.62721f, -57.94058f, "J1558-5756", Pulsar::ATNF));
    ps.add(Pulsar(239.83875f, -55.76111f, "J1559-5545", Pulsar::ATNF));
    ps.add(Pulsar(239.92303f, -44.64608f, "J1559-4438", Pulsar::ATNF));
    ps.add(Pulsar(240.08304f, -57.85408f, "J1600-5751", Pulsar::ATNF));
    ps.add(Pulsar(240.22096f, -50.73916f, "J1600-5044", Pulsar::ATNF));
    ps.add(Pulsar(240.21627f, -30.89704f, "J1600-3053", Pulsar::ATNF));
    ps.add(Pulsar(240.14583f, -59.28306f, "J1600-5916", Pulsar::ATNF));
    ps.add(Pulsar(240.36375f, -52.73583f, "J1601-5244", Pulsar::ATNF));
    ps.add(Pulsar(240.47879f, -53.59533f, "J1601-5335", Pulsar::ATNF));
    ps.add(Pulsar(240.57833f, -51.00056f, "J1602-5100", Pulsar::ATNF));
    ps.add(Pulsar(240.57583f, -49.95889f, "J1602-4957", Pulsar::ATNF));
    ps.add(Pulsar(240.78360f, -27.22434f, "J1603-2712", Pulsar::ATNF));
    ps.add(Pulsar(240.77039f, -25.52982f, "J1603-2531", Pulsar::ATNF));
    ps.add(Pulsar(240.97374f, -35.66586f, "J1603-3539", Pulsar::ATNF));
    ps.add(Pulsar(240.88108f, -56.96227f, "J1603-5657", Pulsar::ATNF));
    ps.add(Pulsar(240.89867f, -72.04242f, "J1603-7202", Pulsar::ATNF));
    ps.add(Pulsar(241.09583f, -49.16621f, "J1604-4909", Pulsar::ATNF));
    ps.add(Pulsar(241.12254f, -47.31331f, "J1604-4718", Pulsar::ATNF));
    ps.add(Pulsar(241.12157f, -72.05846f, "J1604-7203", Pulsar::ATNF));
    ps.add(Pulsar(241.31792f, -52.95944f, "J1605-5257", Pulsar::ATNF));
    ps.add(Pulsar(241.32717f, 32.82167f, "J1605+3249", Pulsar::ATNF));
    ps.add(Pulsar(241.32917f, -52.26333f, "J1605-5215", Pulsar::ATNF));
    ps.add(Pulsar(241.80043f, -0.54467f, "J1607-0032", Pulsar::ATNF));
    ps.add(Pulsar(241.95542f, -51.67111f, "J1607-5140", Pulsar::ATNF));
    ps.add(Pulsar(242.27229f, -19.50222f, "J1609-1930", Pulsar::ATNF));
    ps.add(Pulsar(242.42137f, -46.27292f, "J1609-4616", Pulsar::ATNF));
    ps.add(Pulsar(242.36125f, -51.97167f, "J1609-5158", Pulsar::ATNF));
    ps.add(Pulsar(242.67823f, -13.37286f, "J1610-1322", Pulsar::ATNF));
    ps.add(Pulsar(242.54583f, -17.83333f, "J1610-17", Pulsar::ATNF));
    ps.add(Pulsar(242.68458f, -50.11172f, "J1610-5006", Pulsar::ATNF));
    ps.add(Pulsar(242.55342f, -53.06361f, "J1610-5303", Pulsar::ATNF));
    ps.add(Pulsar(242.76333f, -52.15656f, "J1611-5209", Pulsar::ATNF));
    ps.add(Pulsar(242.76125f, -48.19417f, "J1611-4811", Pulsar::ATNF));
    ps.add(Pulsar(242.94429f, -49.83256f, "J1611-4949", Pulsar::ATNF));
    ps.add(Pulsar(242.96380f, -58.79509f, "J1611-5847", Pulsar::ATNF));
    ps.add(Pulsar(243.10858f, -24.13444f, "J1612-2408", Pulsar::ATNF));
    ps.add(Pulsar(243.00250f, -51.61500f, "J1612-5136", Pulsar::ATNF));
    ps.add(Pulsar(243.37096f, -47.24050f, "J1613-4714", Pulsar::ATNF));
    ps.add(Pulsar(243.42692f, -52.18917f, "J1613-5211", Pulsar::ATNF));
    ps.add(Pulsar(243.48954f, -52.57139f, "J1613-5234", Pulsar::ATNF));
    ps.add(Pulsar(243.54704f, -50.80097f, "J1614-5048", Pulsar::ATNF));
    ps.add(Pulsar(243.67044f, 7.62528f, "J1614+0737", Pulsar::ATNF));
    ps.add(Pulsar(243.50000f, -22.50000f, "J1614-22", Pulsar::ATNF));
    ps.add(Pulsar(243.50000f, -23.25000f, "J1614-23", Pulsar::ATNF));
    ps.add(Pulsar(243.63011f, -39.62467f, "J1614-3937", Pulsar::ATNF));
    ps.add(Pulsar(243.69042f, -51.74694f, "J1614-5144", Pulsar::ATNF));
    ps.add(Pulsar(243.70992f, -54.04639f, "J1614-5402", Pulsar::ATNF));
    ps.add(Pulsar(243.91846f, -55.61681f, "J1615-5537", Pulsar::ATNF));
    ps.add(Pulsar(243.97015f, -29.67104f, "J1615-2940", Pulsar::ATNF));
    ps.add(Pulsar(243.75496f, -54.74233f, "J1615-5444", Pulsar::ATNF));
    ps.add(Pulsar(244.12875f, -51.15472f, "J1616-5109", Pulsar::ATNF));
    ps.add(Pulsar(244.09750f, -52.14667f, "J1616-5208", Pulsar::ATNF));
    ps.add(Pulsar(244.34742f, -42.28306f, "J1617-4216", Pulsar::ATNF));
    ps.add(Pulsar(244.39627f, -46.14347f, "J1617-4608", Pulsar::ATNF));
    ps.add(Pulsar(244.37208f, -50.92033f, "J1617-5055", Pulsar::ATNF));
    ps.add(Pulsar(244.62500f, -39.31667f, "J1618-39", Pulsar::ATNF));
    ps.add(Pulsar(244.52833f, -47.38864f, "J1618-4723", Pulsar::ATNF));
    ps.add(Pulsar(245.06017f, -54.24769f, "J1620-5414", Pulsar::ATNF));
    ps.add(Pulsar(245.26962f, -50.66350f, "J1621-5039", Pulsar::ATNF));
    ps.add(Pulsar(245.48250f, -52.72889f, "J1621-5243", Pulsar::ATNF));
    ps.add(Pulsar(245.68945f, -43.54117f, "J1622-4332", Pulsar::ATNF));
    ps.add(Pulsar(245.62525f, -43.78928f, "J1622-4347", Pulsar::ATNF));
    ps.add(Pulsar(245.69662f, -48.03697f, "J1622-4802", Pulsar::ATNF));
    ps.add(Pulsar(245.65625f, -49.74167f, "J1622-4944", Pulsar::ATNF));
    ps.add(Pulsar(245.82365f, -9.14700f, "J1623-0908", Pulsar::ATNF));
    ps.add(Pulsar(245.90926f, -26.53160f, "J1623-2631", Pulsar::ATNF));
    ps.add(Pulsar(245.95142f, -42.94778f, "J1623-4256", Pulsar::ATNF));
    ps.add(Pulsar(245.97833f, -49.81778f, "J1623-4949", Pulsar::ATNF));
    ps.add(Pulsar(246.08900f, -44.19272f, "J1624-4411", Pulsar::ATNF));
    ps.add(Pulsar(246.07792f, -46.21722f, "J1624-4613", Pulsar::ATNF));
    ps.add(Pulsar(246.22871f, -47.35750f, "J1624-4721", Pulsar::ATNF));
    ps.add(Pulsar(246.29304f, -40.80597f, "J1625-4048", Pulsar::ATNF));
    ps.add(Pulsar(246.32521f, -49.07600f, "J1625-4904", Pulsar::ATNF));
    ps.add(Pulsar(246.70408f, -45.62378f, "J1626-4537", Pulsar::ATNF));
    ps.add(Pulsar(246.67708f, -48.13167f, "J1626-4807", Pulsar::ATNF));
    ps.add(Pulsar(246.82816f, 14.32242f, "J1627+1419", Pulsar::ATNF));
    ps.add(Pulsar(246.86979f, -47.11394f, "J1627-4706", Pulsar::ATNF));
    ps.add(Pulsar(246.79308f, -48.75222f, "J1627-4845", Pulsar::ATNF));
    ps.add(Pulsar(246.83783f, -55.79789f, "J1627-5547", Pulsar::ATNF));
    ps.add(Pulsar(247.11179f, -48.08306f, "J1628-4804", Pulsar::ATNF));
    ps.add(Pulsar(247.12875f, -48.47833f, "J1628-4828", Pulsar::ATNF));
    ps.add(Pulsar(247.28654f, -69.04592f, "J1629-6902", Pulsar::ATNF));
    ps.add(Pulsar(247.65613f, -47.55139f, "J1630-4733", Pulsar::ATNF));
    ps.add(Pulsar(247.51029f, -47.32833f, "J1630-4719", Pulsar::ATNF));
    ps.add(Pulsar(247.82637f, -41.91769f, "J1631-4155", Pulsar::ATNF));
    ps.add(Pulsar(248.22583f, -10.22167f, "J1632-1013", Pulsar::ATNF));
    ps.add(Pulsar(248.05833f, -45.15250f, "J1632-4509", Pulsar::ATNF));
    ps.add(Pulsar(248.20755f, -46.36350f, "J1632-4621", Pulsar::ATNF));
    ps.add(Pulsar(248.06967f, -47.95953f, "J1632-4757", Pulsar::ATNF));
    ps.add(Pulsar(248.16558f, -48.31461f, "J1632-4818", Pulsar::ATNF));
    ps.add(Pulsar(248.25043f, -50.25225f, "J1633-5015", Pulsar::ATNF));
    ps.add(Pulsar(248.44603f, -44.88550f, "J1633-4453", Pulsar::ATNF));
    ps.add(Pulsar(248.27250f, -48.09333f, "J1633-4805", Pulsar::ATNF));
    ps.add(Pulsar(248.52046f, -51.12911f, "J1634-5107", Pulsar::ATNF));
    ps.add(Pulsar(248.78783f, -59.91139f, "J1635-5954", Pulsar::ATNF));
    ps.add(Pulsar(248.85742f, 24.31314f, "J1635+2418", Pulsar::ATNF));
    ps.add(Pulsar(248.94733f, -15.19778f, "J1635-1511", Pulsar::ATNF));
    ps.add(Pulsar(248.98083f, -45.22417f, "J1635-4513", Pulsar::ATNF));
    ps.add(Pulsar(248.98083f, -49.74333f, "J1635-4944", Pulsar::ATNF));
    ps.add(Pulsar(249.06887f, -44.67361f, "J1636-4440", Pulsar::ATNF));
    ps.add(Pulsar(249.13333f, -48.06528f, "J1636-4803", Pulsar::ATNF));
    ps.add(Pulsar(249.22979f, -49.55278f, "J1636-4933", Pulsar::ATNF));
    ps.add(Pulsar(249.49462f, -45.89092f, "J1637-4553", Pulsar::ATNF));
    ps.add(Pulsar(249.48667f, -43.59500f, "J1637-4335", Pulsar::ATNF));
    ps.add(Pulsar(249.47146f, -44.84064f, "J1637-4450", Pulsar::ATNF));
    ps.add(Pulsar(249.30737f, -46.70417f, "J1637-4642", Pulsar::ATNF));
    ps.add(Pulsar(249.29750f, -47.35083f, "J1637-4721", Pulsar::ATNF));
    ps.add(Pulsar(249.49450f, -48.27000f, "J1637-4816", Pulsar::ATNF));
    ps.add(Pulsar(249.65562f, -38.25094f, "J1638-3815", Pulsar::ATNF));
    ps.add(Pulsar(249.72033f, -43.73444f, "J1638-4344", Pulsar::ATNF));
    ps.add(Pulsar(249.69259f, -44.28433f, "J1638-4417", Pulsar::ATNF));
    ps.add(Pulsar(249.59575f, -46.13656f, "J1638-4608", Pulsar::ATNF));
    ps.add(Pulsar(249.55392f, -47.42556f, "J1638-4725", Pulsar::ATNF));
    ps.add(Pulsar(249.74983f, -52.44928f, "J1638-5226", Pulsar::ATNF));
    ps.add(Pulsar(249.83834f, -46.07586f, "J1639-4604", Pulsar::ATNF));
    ps.add(Pulsar(249.77854f, -43.99778f, "J1639-4359", Pulsar::ATNF));
    ps.add(Pulsar(250.05496f, -47.25981f, "J1640-4715", Pulsar::ATNF));
    ps.add(Pulsar(250.06976f, 22.40248f, "J1640+2224", Pulsar::ATNF));
    ps.add(Pulsar(250.19842f, -46.81250f, "J1640-4648", Pulsar::ATNF));
    ps.add(Pulsar(250.18150f, -49.85058f, "J1640-4951", Pulsar::ATNF));
    ps.add(Pulsar(250.42033f, 36.45429f, "J1641+3627A", Pulsar::ATNF));
    ps.add(Pulsar(250.42083f, 36.46028f, "J1641+3627B", Pulsar::ATNF));
    ps.add(Pulsar(250.32517f, -23.79333f, "J1641-2347", Pulsar::ATNF));
    ps.add(Pulsar(250.42083f, 36.46028f, "J1641+3627C", Pulsar::ATNF));
    ps.add(Pulsar(250.42083f, 36.46028f, "J1641+3627D", Pulsar::ATNF));
    ps.add(Pulsar(250.42083f, 36.46028f, "J1641+3627E", Pulsar::ATNF));
    ps.add(Pulsar(250.90898f, -12.41632f, "J1643-1224", Pulsar::ATNF));
    ps.add(Pulsar(250.90404f, -45.09611f, "J1643-4505", Pulsar::ATNF));
    ps.add(Pulsar(250.83500f, -45.36694f, "J1643-4522", Pulsar::ATNF));
    ps.add(Pulsar(250.80633f, -45.84847f, "J1643-4550", Pulsar::ATNF));
    ps.add(Pulsar(251.20534f, -45.98597f, "J1644-4559", Pulsar::ATNF));
    ps.add(Pulsar(251.25851f, -3.29953f, "J1645-0317", Pulsar::ATNF));
    ps.add(Pulsar(251.39340f, 10.20446f, "J1645+1012", Pulsar::ATNF));
    ps.add(Pulsar(251.72833f, -68.53083f, "J1646-6831", Pulsar::ATNF));
    ps.add(Pulsar(251.71375f, -43.77722f, "J1646-4346", Pulsar::ATNF));
    ps.add(Pulsar(251.73042f, -43.13528f, "J1646-4308", Pulsar::ATNF));
    ps.add(Pulsar(251.65146f, -51.38739f, "J1646-5123", Pulsar::ATNF));
    ps.add(Pulsar(251.92917f, -36.08333f, "J1647-36", Pulsar::ATNF));
    ps.add(Pulsar(251.79250f, -45.87133f, "J1647-4552", Pulsar::ATNF));
    ps.add(Pulsar(252.02529f, -32.94467f, "J1648-3256", Pulsar::ATNF));
    ps.add(Pulsar(252.05417f, -44.97389f, "J1648-4458", Pulsar::ATNF));
    ps.add(Pulsar(252.09175f, -46.18778f, "J1648-4611", Pulsar::ATNF));
    ps.add(Pulsar(252.43429f, 25.55194f, "J1649+2533", Pulsar::ATNF));
    ps.add(Pulsar(252.45092f, -38.09975f, "J1649-3805", Pulsar::ATNF));
    ps.add(Pulsar(252.33508f, -43.82283f, "J1649-4349", Pulsar::ATNF));
    ps.add(Pulsar(252.35208f, -46.88583f, "J1649-4653", Pulsar::ATNF));
    ps.add(Pulsar(252.32612f, -47.49806f, "J1649-4729", Pulsar::ATNF));
    ps.add(Pulsar(252.37958f, -55.89444f, "J1649-5553", Pulsar::ATNF));
    ps.add(Pulsar(252.61335f, -16.91114f, "J1650-1654", Pulsar::ATNF));
    ps.add(Pulsar(252.55489f, -41.44272f, "J1650-4126", Pulsar::ATNF));
    ps.add(Pulsar(252.68617f, -43.69189f, "J1650-4341", Pulsar::ATNF));
    ps.add(Pulsar(252.63458f, -45.04369f, "J1650-4502", Pulsar::ATNF));
    ps.add(Pulsar(252.64625f, -49.35108f, "J1650-4921", Pulsar::ATNF));
    ps.add(Pulsar(252.92904f, -52.38283f, "J1651-5222", Pulsar::ATNF));
    ps.add(Pulsar(252.92167f, -52.93000f, "J1651-5255", Pulsar::ATNF));
    ps.add(Pulsar(252.88230f, -17.15614f, "J1651-1709", Pulsar::ATNF));
    ps.add(Pulsar(252.95329f, -42.76972f, "J1651-4246", Pulsar::ATNF));
    ps.add(Pulsar(252.98833f, -45.31972f, "J1651-4519", Pulsar::ATNF));
    ps.add(Pulsar(252.78279f, -76.71097f, "J1651-7642", Pulsar::ATNF));
    ps.add(Pulsar(253.24387f, -24.06413f, "J1652-2404", Pulsar::ATNF));
    ps.add(Pulsar(253.06949f, -14.00761f, "J1652-1400", Pulsar::ATNF));
    ps.add(Pulsar(253.01279f, 26.86122f, "J1652+2651", Pulsar::ATNF));
    ps.add(Pulsar(253.41582f, -38.63911f, "J1653-3838", Pulsar::ATNF));
    ps.add(Pulsar(253.39262f, -40.50039f, "J1653-4030", Pulsar::ATNF));
    ps.add(Pulsar(253.41758f, -42.81758f, "J1653-4249", Pulsar::ATNF));
    ps.add(Pulsar(253.37375f, -43.25028f, "J1653-4315", Pulsar::ATNF));
    ps.add(Pulsar(253.48625f, -48.91417f, "J1653-4854", Pulsar::ATNF));
    ps.add(Pulsar(253.49583f, -23.51667f, "J1654-23", Pulsar::ATNF));
    ps.add(Pulsar(253.59845f, -27.21710f, "J1654-2713", Pulsar::ATNF));
    ps.add(Pulsar(253.68511f, -37.18253f, "J1654-3710", Pulsar::ATNF));
    ps.add(Pulsar(253.59792f, -41.67333f, "J1654-4140", Pulsar::ATNF));
    ps.add(Pulsar(253.59200f, -42.76083f, "J1654-4245", Pulsar::ATNF));
    ps.add(Pulsar(253.85221f, -30.81167f, "J1655-3048", Pulsar::ATNF));
    ps.add(Pulsar(254.13731f, -36.36658f, "J1656-3621", Pulsar::ATNF));
    ps.add(Pulsar(254.40304f, -44.53889f, "J1657-4432", Pulsar::ATNF));
    ps.add(Pulsar(254.56917f, -43.11389f, "J1658-4306", Pulsar::ATNF));
    ps.add(Pulsar(254.72883f, -49.98289f, "J1658-4958", Pulsar::ATNF));
    ps.add(Pulsar(254.97120f, -13.08583f, "J1659-1305", Pulsar::ATNF));
    ps.add(Pulsar(254.98492f, -43.26833f, "J1659-4316", Pulsar::ATNF));
    ps.add(Pulsar(254.91433f, -44.65028f, "J1659-4439", Pulsar::ATNF));
    ps.add(Pulsar(255.22076f, -33.21292f, "J1700-3312", Pulsar::ATNF));
    ps.add(Pulsar(255.20550f, -36.19806f, "J1700-3611", Pulsar::ATNF));
    ps.add(Pulsar(255.09279f, -39.31667f, "J1700-3919", Pulsar::ATNF));
    ps.add(Pulsar(255.16063f, -40.21072f, "J1700-4012", Pulsar::ATNF));
    ps.add(Pulsar(255.09400f, -49.65417f, "J1700-4939", Pulsar::ATNF));
    ps.add(Pulsar(255.37148f, -45.56353f, "J1701-4533", Pulsar::ATNF));
    ps.add(Pulsar(255.32725f, -37.44028f, "J1701-3726", Pulsar::ATNF));
    ps.add(Pulsar(255.30214f, -30.10837f, "J1701-3006A", Pulsar::ATNF));
    ps.add(Pulsar(255.30279f, -30.11362f, "J1701-3006B", Pulsar::ATNF));
    ps.add(Pulsar(255.30361f, -30.11651f, "J1701-3006C", Pulsar::ATNF));
    ps.add(Pulsar(255.30000f, -30.10000f, "J1701-3006D", Pulsar::ATNF));
    ps.add(Pulsar(255.30000f, -30.10000f, "J1701-3006E", Pulsar::ATNF));
    ps.add(Pulsar(255.30000f, -30.10000f, "J1701-3006F", Pulsar::ATNF));
    ps.add(Pulsar(255.43130f, -31.51019f, "J1701-3130", Pulsar::ATNF));
    ps.add(Pulsar(255.55983f, -39.54444f, "J1702-3932", Pulsar::ATNF));
    ps.add(Pulsar(255.71883f, -41.48006f, "J1702-4128", Pulsar::ATNF));
    ps.add(Pulsar(255.65183f, -42.28367f, "J1702-4217", Pulsar::ATNF));
    ps.add(Pulsar(255.61371f, -43.11222f, "J1702-4306", Pulsar::ATNF));
    ps.add(Pulsar(255.61225f, -43.17778f, "J1702-4310", Pulsar::ATNF));
    ps.add(Pulsar(255.71917f, -44.46750f, "J1702-4428", Pulsar::ATNF));
    ps.add(Pulsar(255.96287f, -18.77105f, "J1703-1846", Pulsar::ATNF));
    ps.add(Pulsar(255.84392f, -32.69668f, "J1703-3241", Pulsar::ATNF));
    ps.add(Pulsar(255.75000f, -38.20000f, "J1703-38", Pulsar::ATNF));
    ps.add(Pulsar(255.83579f, -44.71167f, "J1703-4442", Pulsar::ATNF));
    ps.add(Pulsar(255.97667f, -48.86550f, "J1703-4851", Pulsar::ATNF));
    ps.add(Pulsar(256.06167f, -60.28167f, "J1704-6016", Pulsar::ATNF));
    ps.add(Pulsar(256.40042f, -19.11073f, "J1705-1906", Pulsar::ATNF));
    ps.add(Pulsar(256.42651f, -34.39588f, "J1705-3423", Pulsar::ATNF));
    ps.add(Pulsar(256.40458f, -39.60806f, "J1705-3936", Pulsar::ATNF));
    ps.add(Pulsar(256.37433f, -39.84972f, "J1705-3950", Pulsar::ATNF));
    ps.add(Pulsar(256.33500f, -41.14556f, "J1705-4108", Pulsar::ATNF));
    ps.add(Pulsar(256.58875f, -38.66417f, "J1706-3839", Pulsar::ATNF));
    ps.add(Pulsar(256.51879f, -43.17250f, "J1706-4310", Pulsar::ATNF));
    ps.add(Pulsar(256.54081f, -61.30325f, "J1706-6118", Pulsar::ATNF));
    ps.add(Pulsar(256.84053f, -40.89892f, "J1707-4053", Pulsar::ATNF));
    ps.add(Pulsar(256.91712f, -43.68667f, "J1707-4341", Pulsar::ATNF));
    ps.add(Pulsar(256.75000f, -44.20000f, "J1707-44", Pulsar::ATNF));
    ps.add(Pulsar(256.81478f, -47.49292f, "J1707-4729", Pulsar::ATNF));
    ps.add(Pulsar(257.01042f, -75.65639f, "J1708-7539", Pulsar::ATNF));
    ps.add(Pulsar(257.24078f, -34.44559f, "J1708-3426", Pulsar::ATNF));
    ps.add(Pulsar(257.06875f, -38.46000f, "J1708-3827", Pulsar::ATNF));
    ps.add(Pulsar(257.19583f, -40.15750f, "J1708-4009", Pulsar::ATNF));
    ps.add(Pulsar(257.05383f, -45.38083f, "J1708-4522", Pulsar::ATNF));
    ps.add(Pulsar(257.36017f, -16.68270f, "J1709-1640", Pulsar::ATNF));
    ps.add(Pulsar(257.42803f, -44.48562f, "J1709-4429", Pulsar::ATNF));
    ps.add(Pulsar(257.27413f, 23.22440f, "J1709+2313", Pulsar::ATNF));
    ps.add(Pulsar(257.43813f, -36.43433f, "J1709-3626", Pulsar::ATNF));
    ps.add(Pulsar(257.31662f, -38.68806f, "J1709-3841", Pulsar::ATNF));
    ps.add(Pulsar(257.37875f, -43.70389f, "J1709-4342", Pulsar::ATNF));
    ps.add(Pulsar(257.59896f, -41.80528f, "J1710-4148", Pulsar::ATNF));
    ps.add(Pulsar(257.97133f, -53.83778f, "J1711-5350", Pulsar::ATNF));
    ps.add(Pulsar(257.97942f, -15.16104f, "J1711-1509", Pulsar::ATNF));
    ps.add(Pulsar(257.93521f, -38.43722f, "J1711-3826", Pulsar::ATNF));
    ps.add(Pulsar(257.79400f, -43.38142f, "J1711-4322", Pulsar::ATNF));
    ps.add(Pulsar(258.04879f, -27.26472f, "J1712-2715", Pulsar::ATNF));
    ps.add(Pulsar(258.14583,  -39.71667, "J1712-3943-1", Pulsar::ATNF));
    ps.add(Pulsar(258.14583,  -39.71667, "J1712-3943-2", Pulsar::ATNF));
    ps.add(Pulsar(258.45638f, 7.79376f, "J1713+0747", Pulsar::ATNF));
    ps.add(Pulsar(258.25958f, -38.74139f, "J1713-3844", Pulsar::ATNF));
    ps.add(Pulsar(258.36667f, -39.81667f, "J1713-3949", Pulsar::ATNF));
    ps.add(Pulsar(258.66717f, -10.90303f, "J1714-1054", Pulsar::ATNF));
    ps.add(Pulsar(258.84775f, -32.79167f, "J1715-3247", Pulsar::ATNF));
    ps.add(Pulsar(258.79021f, -37.00111f, "J1715-3700", Pulsar::ATNF));
    ps.add(Pulsar(258.90787f, -38.99031f, "J1715-3859", Pulsar::ATNF));
    ps.add(Pulsar(258.80958f, -39.05056f, "J1715-3903", Pulsar::ATNF));
    ps.add(Pulsar(258.92079f, -40.57278f, "J1715-4034", Pulsar::ATNF));
    ps.add(Pulsar(258.79392f, -42.91500f, "J1715-4254", Pulsar::ATNF));
    ps.add(Pulsar(259.04733f, -37.34556f, "J1716-3720", Pulsar::ATNF));
    ps.add(Pulsar(259.18461f, -41.18597f, "J1716-4111", Pulsar::ATNF));
    ps.add(Pulsar(259.33750f, -40.91056f, "J1717-4054", Pulsar::ATNF));
    ps.add(Pulsar(259.33435f, -34.41655f, "J1717-3425", Pulsar::ATNF));
    ps.add(Pulsar(259.31650f, -37.62667f, "J1717-3737", Pulsar::ATNF));
    ps.add(Pulsar(259.32667f, -38.78417f, "J1717-3847", Pulsar::ATNF));
    ps.add(Pulsar(259.35646f, -39.89861f, "J1717-3953", Pulsar::ATNF));
    ps.add(Pulsar(259.44963f, -40.73056f, "J1717-4043", Pulsar::ATNF));
    ps.add(Pulsar(259.25833f, -40.72528f, "J1717-40433", Pulsar::ATNF));
    ps.add(Pulsar(259.39854f, -58.00150f, "J1717-5800", Pulsar::ATNF));
    ps.add(Pulsar(259.57746f, -37.23778f, "J1718-3714", Pulsar::ATNF));
    ps.add(Pulsar(259.54167f, -37.31472f, "J1718-3718", Pulsar::ATNF));
    ps.add(Pulsar(259.55652f, -38.42168f, "J1718-3825", Pulsar::ATNF));
    ps.add(Pulsar(259.54958f, -45.65442f, "J1718-4539", Pulsar::ATNF));
    ps.add(Pulsar(259.75718f, -40.11653f, "J1719-4006", Pulsar::ATNF));
    ps.add(Pulsar(259.95321f, -43.03650f, "J1719-4302", Pulsar::ATNF));
    ps.add(Pulsar(260.10503f, -16.55937f, "J1720-1633", Pulsar::ATNF));
    ps.add(Pulsar(260.14216f, -29.55451f, "J1720-2933", Pulsar::ATNF));
    ps.add(Pulsar(260.23862f, -2.20664f, "J1720-0212", Pulsar::ATNF));
    ps.add(Pulsar(260.00542f, 21.83689f, "J1720+2150", Pulsar::ATNF));
    ps.add(Pulsar(260.00823f, -36.98514f, "J1720-3659", Pulsar::ATNF));
    ps.add(Pulsar(260.25617f, -19.61405f, "J1721-1936", Pulsar::ATNF));
    ps.add(Pulsar(260.38658f, -35.54711f, "J1721-3532", Pulsar::ATNF));
    ps.add(Pulsar(260.44421f, -19.66361f, "J1721-1939", Pulsar::ATNF));
    ps.add(Pulsar(260.27290f, -24.95177f, "J1721-2457", Pulsar::ATNF));
    ps.add(Pulsar(260.51231f, -32.12926f, "J1722-3207", Pulsar::ATNF));
    ps.add(Pulsar(260.54090f, -36.54814f, "J1722-3632", Pulsar::ATNF));
    ps.add(Pulsar(260.74654f, -37.20103f, "J1722-3712", Pulsar::ATNF));
    ps.add(Pulsar(260.69382f, -44.00922f, "J1722-4400", Pulsar::ATNF));
    ps.add(Pulsar(260.78158f, -36.98719f, "J1723-3659", Pulsar::ATNF));
    ps.add(Pulsar(261.18696f, -31.81778f, "J1724-3149", Pulsar::ATNF));
    ps.add(Pulsar(261.19954f, -35.09333f, "J1724-3505", Pulsar::ATNF));
    ps.add(Pulsar(261.00000f, -35.81667f, "J1724-35", Pulsar::ATNF));
    ps.add(Pulsar(261.10750f, -45.00417f, "J1724-4500", Pulsar::ATNF));
    ps.add(Pulsar(261.30117f, -7.54978f, "J1725-0732", Pulsar::ATNF));
    ps.add(Pulsar(261.29125f, -28.87278f, "J1725-2852", Pulsar::ATNF));
    ps.add(Pulsar(261.42583f, -35.77111f, "J1725-3546", Pulsar::ATNF));
    ps.add(Pulsar(261.25042f, -38.81000f, "J1725-3848", Pulsar::ATNF));
    ps.add(Pulsar(261.42258f, -40.71972f, "J1725-4043", Pulsar::ATNF));
    ps.add(Pulsar(261.53129f, -35.49944f, "J1726-3530", Pulsar::ATNF));
    ps.add(Pulsar(261.70671f, -36.59600f, "J1726-3635", Pulsar::ATNF));
    ps.add(Pulsar(261.63904f, -40.10056f, "J1726-4006", Pulsar::ATNF));
    ps.add(Pulsar(261.87913f, -27.65014f, "J1727-2739", Pulsar::ATNF));
    ps.add(Pulsar(261.75000f, -29.98333f, "J1727-29", Pulsar::ATNF));
    ps.add(Pulsar(262.14508f, -0.12917f, "J1728-0007", Pulsar::ATNF));
    ps.add(Pulsar(262.19271f, -37.55222f, "J1728-3733", Pulsar::ATNF));
    ps.add(Pulsar(262.11533f, -40.46944f, "J1728-4028", Pulsar::ATNF));
    ps.add(Pulsar(262.63566f, -33.84429f, "J1730-3350", Pulsar::ATNF));
    ps.add(Pulsar(262.59025f, -23.07536f, "J1730-2304", Pulsar::ATNF));
    ps.add(Pulsar(262.53450f, -29.01278f, "J1730-2900", Pulsar::ATNF));
    ps.add(Pulsar(262.73158f, -33.89389f, "J1730-3353", Pulsar::ATNF));
    ps.add(Pulsar(262.92543f, -47.74293f, "J1731-4744", Pulsar::ATNF));
    ps.add(Pulsar(262.75221f, -31.39528f, "J1731-3123", Pulsar::ATNF));
    ps.add(Pulsar(263.20833f, -41.48000f, "J1732-4128", Pulsar::ATNF));
    ps.add(Pulsar(263.08344f, -19.50238f, "J1732-1930", Pulsar::ATNF));
    ps.add(Pulsar(263.16917f, -31.61000f, "J1732-31", Pulsar::ATNF));
    ps.add(Pulsar(263.02992f, -34.43472f, "J1732-3426", Pulsar::ATNF));
    ps.add(Pulsar(263.08671f, -37.48472f, "J1732-3729", Pulsar::ATNF));
    ps.add(Pulsar(263.20358f, -41.94156f, "J1732-4156", Pulsar::ATNF));
    ps.add(Pulsar(263.19903f, -50.81671f, "J1732-5049", Pulsar::ATNF));
    ps.add(Pulsar(263.36016f, -22.47701f, "J1733-2228", Pulsar::ATNF));
    ps.add(Pulsar(263.36147f, -37.28183f, "J1733-3716", Pulsar::ATNF));
    ps.add(Pulsar(263.35775f, -25.55306f, "J1733-2533", Pulsar::ATNF));
    ps.add(Pulsar(263.39125f, -28.62583f, "J1733-2837", Pulsar::ATNF));
    ps.add(Pulsar(263.49538f, -30.51361f, "J1733-3030", Pulsar::ATNF));
    ps.add(Pulsar(263.48004f, -33.36750f, "J1733-3322", Pulsar::ATNF));
    ps.add(Pulsar(263.49433f, -40.09436f, "J1733-4005", Pulsar::ATNF));
    ps.add(Pulsar(263.69025f, -2.21085f, "J1734-0212", Pulsar::ATNF));
    ps.add(Pulsar(263.67333f, -24.25556f, "J1734-2415", Pulsar::ATNF));
    ps.add(Pulsar(263.61083f, -33.55611f, "J1734-3333", Pulsar::ATNF));
    ps.add(Pulsar(263.77072f, -7.41458f, "J1735-0724", Pulsar::ATNF));
    ps.add(Pulsar(263.98708f, -32.97194f, "J1735-3258", Pulsar::ATNF));
    ps.add(Pulsar(264.22500f, 5.80000f, "J1736+05", Pulsar::ATNF));
    ps.add(Pulsar(264.18933f, -24.96389f, "J1736-2457", Pulsar::ATNF));
    ps.add(Pulsar(264.10304f, -28.32833f, "J1736-2819", Pulsar::ATNF));
    ps.add(Pulsar(264.17746f, -28.73083f, "J1736-2843", Pulsar::ATNF));
    ps.add(Pulsar(264.01125f, -35.19889f, "J1736-3511", Pulsar::ATNF));
    ps.add(Pulsar(264.41683f, -35.92883f, "J1737-3555", Pulsar::ATNF));
    ps.add(Pulsar(264.39054f, -31.03347f, "J1737-3102", Pulsar::ATNF));
    ps.add(Pulsar(264.26788f, -31.62250f, "J1737-3137", Pulsar::ATNF));
    ps.add(Pulsar(264.29379f, -33.33889f, "J1737-3320", Pulsar::ATNF));
    ps.add(Pulsar(264.72577f, -32.19823f, "J1738-3211", Pulsar::ATNF));
    ps.add(Pulsar(264.72485f, 3.55301f, "J1738+0333", Pulsar::ATNF));
    ps.add(Pulsar(264.53667f, -23.51306f, "J1738-2330", Pulsar::ATNF));
    ps.add(Pulsar(264.52096f, -26.79611f, "J1738-2647", Pulsar::ATNF));
    ps.add(Pulsar(264.71767f, -29.93083f, "J1738-2955", Pulsar::ATNF));
    ps.add(Pulsar(264.69750f, -31.12889f, "J1738-3107", Pulsar::ATNF));
    ps.add(Pulsar(264.64354f, -33.26711f, "J1738-3316", Pulsar::ATNF));
    ps.add(Pulsar(264.89283f, -29.05097f, "J1739-2903", Pulsar::ATNF));
    ps.add(Pulsar(264.85127f, -31.52092f, "J1739-3131", Pulsar::ATNF));
    ps.add(Pulsar(264.82486f, 6.20789f, "J1739+0612", Pulsar::ATNF));
    ps.add(Pulsar(264.99092f, -13.22183f, "J1739-1313", Pulsar::ATNF));
    ps.add(Pulsar(264.91583f, -30.38667f, "J1739-3023", Pulsar::ATNF));
    ps.add(Pulsar(264.84675f, -30.82778f, "J1739-3049", Pulsar::ATNF));
    ps.add(Pulsar(264.95283f, -31.99694f, "J1739-3159", Pulsar::ATNF));
    ps.add(Pulsar(264.90938f, -39.86675f, "J1739-3951", Pulsar::ATNF));
    ps.add(Pulsar(265.03061f, 13.19908f, "J1740+1311", Pulsar::ATNF));
    ps.add(Pulsar(265.14092f, -30.26208f, "J1740-3015", Pulsar::ATNF));
    ps.add(Pulsar(265.10813f, 10.00175f, "J1740+1000", Pulsar::ATNF));
    ps.add(Pulsar(265.18883f, -25.67194f, "J1740-2540", Pulsar::ATNF));
    ps.add(Pulsar(265.20846f, -30.86781f, "J1740-3052", Pulsar::ATNF));
    ps.add(Pulsar(265.10717f, -33.46486f, "J1740-3327", Pulsar::ATNF));
    ps.add(Pulsar(265.18579f, -53.67803f, "J1740-5340A", Pulsar::ATNF));
    ps.add(Pulsar(265.32534f, -39.46056f, "J1741-3927", Pulsar::ATNF));
    ps.add(Pulsar(265.34398f, -8.67551f, "J1741-0840", Pulsar::ATNF));
    ps.add(Pulsar(265.40417f, 13.91139f, "J1741+1354", Pulsar::ATNF));
    ps.add(Pulsar(265.27863f, -20.32333f, "J1741-2019", Pulsar::ATNF));
    ps.add(Pulsar(265.48863f, -20.88694f, "J1741-2054", Pulsar::ATNF));
    ps.add(Pulsar(265.39608f, -27.32306f, "J1741-2719", Pulsar::ATNF));
    ps.add(Pulsar(265.25558f, -27.56417f, "J1741-2733", Pulsar::ATNF));
    ps.add(Pulsar(265.47296f, 27.96917f, "J1741+2758", Pulsar::ATNF));
    ps.add(Pulsar(265.31029f, -29.75972f, "J1741-2945", Pulsar::ATNF));
    ps.add(Pulsar(265.27933f, -30.27528f, "J1741-3016", Pulsar::ATNF));
    ps.add(Pulsar(265.51846f, -39.95611f, "J1742-3957", Pulsar::ATNF));
    ps.add(Pulsar(265.60875f, -46.28153f, "J1742-4616", Pulsar::ATNF));
    ps.add(Pulsar(265.78410f, -3.65319f, "J1743-0339", Pulsar::ATNF));
    ps.add(Pulsar(265.90676f, -13.86055f, "J1743-1351", Pulsar::ATNF));
    ps.add(Pulsar(265.90286f, -31.83936f, "J1743-3150", Pulsar::ATNF));
    ps.add(Pulsar(265.83383f, -24.71528f, "J1743-2442", Pulsar::ATNF));
    ps.add(Pulsar(265.81485f, -31.88481f, "J1743-3153", Pulsar::ATNF));
    ps.add(Pulsar(265.77176f, -42.20067f, "J1743-4212", Pulsar::ATNF));
    ps.add(Pulsar(266.12251f, -11.58185f, "J1744-1134", Pulsar::ATNF));
    ps.add(Pulsar(266.06889f, -16.17661f, "J1744-1610", Pulsar::ATNF));
    ps.add(Pulsar(266.20196f, -23.59861f, "J1744-2335", Pulsar::ATNF));
    ps.add(Pulsar(266.02368f, -31.50111f, "J1744-3130", Pulsar::ATNF));
    ps.add(Pulsar(266.01115f, -39.37253f, "J1744-3922", Pulsar::ATNF));
    ps.add(Pulsar(266.48460f, -30.67319f, "J1745-3040", Pulsar::ATNF));
    ps.add(Pulsar(266.25858f, -1.48836f, "J1745-0129", Pulsar::ATNF));
    ps.add(Pulsar(266.28806f, -9.87769f, "J1745-0952", Pulsar::ATNF));
    ps.add(Pulsar(266.31963f, -22.48722f, "J1745-2229", Pulsar::ATNF));
    ps.add(Pulsar(266.31667f, -29.16667f, "J1745-2910", Pulsar::ATNF));
    ps.add(Pulsar(266.45833f, -29.20000f, "J1745-2912", Pulsar::ATNF));
    ps.add(Pulsar(266.50350f, 22.75803f, "J1746+2245", Pulsar::ATNF));
    ps.add(Pulsar(266.52863f, 25.67708f, "J1746+2540", Pulsar::ATNF));
    ps.add(Pulsar(266.51542f, -28.82194f, "J1746-2849", Pulsar::ATNF));
    ps.add(Pulsar(266.52750f, -28.84500f, "J1746-2850", Pulsar::ATNF));
    ps.add(Pulsar(266.70625f, -28.94194f, "J1746-2856", Pulsar::ATNF));
    ps.add(Pulsar(266.87873f, -26.78722f, "J1747-2647", Pulsar::ATNF));
    ps.add(Pulsar(266.86063f, -28.04361f, "J1747-2802", Pulsar::ATNF));
    ps.add(Pulsar(266.84500f, -28.15417f, "J1747-2809", Pulsar::ATNF));
    ps.add(Pulsar(266.81618f, -29.96694f, "J1747-2958", Pulsar::ATNF));
    ps.add(Pulsar(267.00940f, -24.77692f, "J1748-2446A", Pulsar::ATNF));
    ps.add(Pulsar(267.07253f, -13.01447f, "J1748-1300", Pulsar::ATNF));
    ps.add(Pulsar(267.21954f, -20.36103f, "J1748-2021A", Pulsar::ATNF));
    ps.add(Pulsar(267.22064f, -20.36079f, "J1748-2021B", Pulsar::ATNF));
    ps.add(Pulsar(267.21322f, -20.36495f, "J1748-2021C", Pulsar::ATNF));
    ps.add(Pulsar(267.21519f, -20.35206f, "J1748-2021D", Pulsar::ATNF));
    ps.add(Pulsar(267.22000f, -20.35816f, "J1748-2021E", Pulsar::ATNF));
    ps.add(Pulsar(267.21806f, -20.36092f, "J1748-2021F", Pulsar::ATNF));
    ps.add(Pulsar(267.20213f, -24.74360f, "J1748-2444", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446ad", Pulsar::ATNF));
    ps.add(Pulsar(267.01892f, -24.77667f, "J1748-2446C", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446D", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446E", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446F", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446G", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446H", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446I", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446J", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446K", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446L", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446M", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446N", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446O", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446P", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446Q", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446R", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446S", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446T", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446U", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446V", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446W", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446X", Pulsar::ATNF));
    ps.add(Pulsar(267.02042f, -24.77917f, "J1748-2446Y", Pulsar::ATNF));
    ps.add(Pulsar(267.43542f, -56.08861f, "J1749-5605", Pulsar::ATNF));
    ps.add(Pulsar(267.30622f, -30.04303f, "J1749-3002", Pulsar::ATNF));
    ps.add(Pulsar(267.31504f, -23.78806f, "J1749-2347", Pulsar::ATNF));
    ps.add(Pulsar(267.29700f, -26.48611f, "J1749-2629", Pulsar::ATNF));
    ps.add(Pulsar(267.69716f, -31.96226f, "J1750-3157", Pulsar::ATNF));
    ps.add(Pulsar(267.57667f, -20.71889f, "J1750-2043", Pulsar::ATNF));
    ps.add(Pulsar(267.74911f, -24.64944f, "J1750-2438", Pulsar::ATNF));
    ps.add(Pulsar(267.59567f, -24.74639f, "J1750-2444", Pulsar::ATNF));
    ps.add(Pulsar(267.68542f, -35.05247f, "J1750-3503", Pulsar::ATNF));
    ps.add(Pulsar(267.55751f, -37.05304f, "J1750-3703A", Pulsar::ATNF));
    ps.add(Pulsar(267.55074f, -37.05637f, "J1750-3703B", Pulsar::ATNF));
    ps.add(Pulsar(267.55606f, -37.05155f, "J1750-3703C", Pulsar::ATNF));
    ps.add(Pulsar(267.55457f, -37.05177f, "J1750-3703D", Pulsar::ATNF));
    ps.add(Pulsar(267.92597f, -46.95689f, "J1751-4657", Pulsar::ATNF));
    ps.add(Pulsar(267.96929f, -25.27861f, "J1751-2516", Pulsar::ATNF));
    ps.add(Pulsar(267.88624f, -28.96292f, "J1751-2857", Pulsar::ATNF));
    ps.add(Pulsar(267.88635f, -33.39433f, "J1751-3323", Pulsar::ATNF));
    ps.add(Pulsar(268.24454f, -28.11036f, "J1752-2806", Pulsar::ATNF));
    ps.add(Pulsar(268.14758f, 23.99672f, "J1752+2359", Pulsar::ATNF));
    ps.add(Pulsar(268.24476f, -24.17389f, "J1752-2410", Pulsar::ATNF));
    ps.add(Pulsar(268.10229f, -28.35278f, "J1752-2821", Pulsar::ATNF));
    ps.add(Pulsar(268.37758f, -25.00700f, "J1753-2501", Pulsar::ATNF));
    ps.add(Pulsar(268.22083f, -12.98333f, "J1753-12", Pulsar::ATNF));
    ps.add(Pulsar(268.39655f, -19.24944f, "J1753-1914", Pulsar::ATNF));
    ps.add(Pulsar(268.41596f, -22.68111f, "J1753-2240", Pulsar::ATNF));
    ps.add(Pulsar(268.41250f, -38.85000f, "J1753-38", Pulsar::ATNF));
    ps.add(Pulsar(268.59554f, 52.02007f, "J1754+5201", Pulsar::ATNF));
    ps.add(Pulsar(268.50000f, -30.18333f, "J1754-30", Pulsar::ATNF));
    ps.add(Pulsar(268.65572f, -34.73164f, "J1754-3443", Pulsar::ATNF));
    ps.add(Pulsar(268.72757f, -35.17861f, "J1754-3510", Pulsar::ATNF));
    ps.add(Pulsar(268.79853f, -16.84472f, "J1755-1650", Pulsar::ATNF));
    ps.add(Pulsar(268.89917f, -20.41667f, "J1755-2025", Pulsar::ATNF));
    ps.add(Pulsar(268.83046f, -25.35250f, "J1755-25211", Pulsar::ATNF));
    ps.add(Pulsar(268.99879f, -25.35750f, "J1755-2521", Pulsar::ATNF));
    ps.add(Pulsar(268.95758f, -25.57750f, "J1755-2534", Pulsar::ATNF));
    ps.add(Pulsar(268.92450f, -27.42917f, "J1755-2725", Pulsar::ATNF));
    ps.add(Pulsar(269.24131f, -24.59286f, "J1756-2435", Pulsar::ATNF));
    ps.add(Pulsar(269.00000f, 18.31667f, "J1756+18", Pulsar::ATNF));
    ps.add(Pulsar(269.10650f, -22.43000f, "J1756-2225", Pulsar::ATNF));
    ps.add(Pulsar(269.19430f, -22.86650f, "J1756-2251", Pulsar::ATNF));
    ps.add(Pulsar(269.33079f, -26.31889f, "J1756-2619", Pulsar::ATNF));
    ps.add(Pulsar(269.37223f, -24.36873f, "J1757-2421", Pulsar::ATNF));
    ps.add(Pulsar(269.46155f, -22.39694f, "J1757-2223", Pulsar::ATNF));
    ps.add(Pulsar(269.31317f, -53.37400f, "J1757-5322", Pulsar::ATNF));
    ps.add(Pulsar(269.52333f, -19.52806f, "J1758-1931", Pulsar::ATNF));
    ps.add(Pulsar(269.68521f, -22.11250f, "J1758-2206", Pulsar::ATNF));
    ps.add(Pulsar(269.63308f, -25.68028f, "J1758-2540", Pulsar::ATNF));
    ps.add(Pulsar(269.64279f, -26.50278f, "J1758-2630", Pulsar::ATNF));
    ps.add(Pulsar(269.56421f, -28.76722f, "J1758-2846", Pulsar::ATNF));
    ps.add(Pulsar(269.60783f, 30.50667f, "J1758+3030", Pulsar::ATNF));
    ps.add(Pulsar(269.85065f, -22.09251f, "J1759-2205", Pulsar::ATNF));
    ps.add(Pulsar(269.86733f, -17.60278f, "J1759-1736", Pulsar::ATNF));
    ps.add(Pulsar(269.92400f, -19.05528f, "J1759-1903", Pulsar::ATNF));
    ps.add(Pulsar(269.98767f, -19.67472f, "J1759-1940", Pulsar::ATNF));
    ps.add(Pulsar(269.89758f, -19.93556f, "J1759-1956", Pulsar::ATNF));
    ps.add(Pulsar(269.95513f, -23.03556f, "J1759-2302", Pulsar::ATNF));
    ps.add(Pulsar(269.87894f, -23.12139f, "J1759-2307", Pulsar::ATNF));
    ps.add(Pulsar(269.89633f, -25.81861f, "J1759-2549", Pulsar::ATNF));
    ps.add(Pulsar(269.95103f, -29.36867f, "J1759-2922", Pulsar::ATNF));
    ps.add(Pulsar(269.84190f, -31.12264f, "J1759-3107", Pulsar::ATNF));
    ps.add(Pulsar(270.00000f, -23.71667f, "J1800-2343", Pulsar::ATNF));
    ps.add(Pulsar(270.09200f, -1.42517f, "J1800-0125", Pulsar::ATNF));
    ps.add(Pulsar(270.05125f, -21.23861f, "J1800-2114", Pulsar::ATNF));
    ps.add(Pulsar(270.25007f, -24.85764f, "J1801-2451", Pulsar::ATNF));
    ps.add(Pulsar(270.34442f, -3.96529f, "J1801-0357", Pulsar::ATNF));
    ps.add(Pulsar(270.33262f, -23.07894f, "J1801-2304", Pulsar::ATNF));
    ps.add(Pulsar(270.44515f, -29.34405f, "J1801-2920", Pulsar::ATNF));
    ps.add(Pulsar(270.46282f, -14.29292f, "J1801-1417", Pulsar::ATNF));
    ps.add(Pulsar(270.34292f, -18.93028f, "J1801-1855", Pulsar::ATNF));
    ps.add(Pulsar(270.44454f, -19.16000f, "J1801-1909", Pulsar::ATNF));
    ps.add(Pulsar(270.38538f, -21.25500f, "J1801-2115", Pulsar::ATNF));
    ps.add(Pulsar(270.28471f, -21.90889f, "J1801-2154", Pulsar::ATNF));
    ps.add(Pulsar(270.46942f, -34.97694f, "J1801-3458", Pulsar::ATNF));
    ps.add(Pulsar(270.61437f, 1.47325f, "J1802+0128", Pulsar::ATNF));
    ps.add(Pulsar(270.56187f, -17.75472f, "J1802-1745", Pulsar::ATNF));
    ps.add(Pulsar(270.52223f, -21.40101f, "J1802-2124", Pulsar::ATNF));
    ps.add(Pulsar(270.51292f, -24.44528f, "J1802-2426", Pulsar::ATNF));
    ps.add(Pulsar(270.96421f, -21.61871f, "J1803-2137", Pulsar::ATNF));
    ps.add(Pulsar(270.88205f, -27.20177f, "J1803-2712", Pulsar::ATNF));
    ps.add(Pulsar(270.89450f, -16.27500f, "J1803-1616", Pulsar::ATNF));
    ps.add(Pulsar(270.99608f, -18.95528f, "J1803-1857", Pulsar::ATNF));
    ps.add(Pulsar(270.87267f, -19.34472f, "J1803-1920", Pulsar::ATNF));
    ps.add(Pulsar(270.89208f, -30.03389f, "J1803-30", Pulsar::ATNF));
    ps.add(Pulsar(271.20790f, -7.59019f, "J1804-0735", Pulsar::ATNF));
    ps.add(Pulsar(271.11746f, -22.47167f, "J1804-2228", Pulsar::ATNF));
    ps.add(Pulsar(271.08805f, -27.29200f, "J1804-2717", Pulsar::ATNF));
    ps.add(Pulsar(271.29231f, 3.10841f, "J1805+0306", Pulsar::ATNF));
    ps.add(Pulsar(271.38098f, -6.32928f, "J1805-0619", Pulsar::ATNF));
    ps.add(Pulsar(271.27542f, -15.07667f, "J1805-1504", Pulsar::ATNF));
    ps.add(Pulsar(271.40454f, -20.54750f, "J1805-2032", Pulsar::ATNF));
    ps.add(Pulsar(271.36746f, -20.62111f, "J1805-2037", Pulsar::ATNF));
    ps.add(Pulsar(271.35804f, -24.79167f, "J1805-2447", Pulsar::ATNF));
    ps.add(Pulsar(271.52830f, -11.90798f, "J1806-1154", Pulsar::ATNF));
    ps.add(Pulsar(271.71714f, 10.38842f, "J1806+1023", Pulsar::ATNF));
    ps.add(Pulsar(271.60742f, -16.31056f, "J1806-1618", Pulsar::ATNF));
    ps.add(Pulsar(271.52750f, -19.33972f, "J1806-1920", Pulsar::ATNF));
    ps.add(Pulsar(271.58162f, -21.42778f, "J1806-2125", Pulsar::ATNF));
    ps.add(Pulsar(271.90845f, -8.79535f, "J1807-0847", Pulsar::ATNF));
    ps.add(Pulsar(271.78540f, -27.25082f, "J1807-2715", Pulsar::ATNF));
    ps.add(Pulsar(271.96348f, 7.94539f, "J1807+0756", Pulsar::ATNF));
    ps.add(Pulsar(271.83483f, -24.99794f, "J1807-2459A", Pulsar::ATNF));
    ps.add(Pulsar(271.83583f, -24.99750f, "J1807-2459B", Pulsar::ATNF));
    ps.add(Pulsar(271.75000f, -25.91667f, "J1807-25", Pulsar::ATNF));
    ps.add(Pulsar(272.02665f, -20.96876f, "J1808-2057", Pulsar::ATNF));
    ps.add(Pulsar(272.07917f, 0.56667f, "J1808+00", Pulsar::ATNF));
    ps.add(Pulsar(272.03930f, -8.21717f, "J1808-0813", Pulsar::ATNF));
    ps.add(Pulsar(272.19014f, -10.34675f, "J1808-1020", Pulsar::ATNF));
    ps.add(Pulsar(272.17625f, -17.44083f, "J1808-1726", Pulsar::ATNF));
    ps.add(Pulsar(272.16383f, -20.41097f, "J1808-2024", Pulsar::ATNF));
    ps.add(Pulsar(272.05512f, -27.02250f, "J1808-2701", Pulsar::ATNF));
    ps.add(Pulsar(272.01867f, -32.82611f, "J1808-3249", Pulsar::ATNF));
    ps.add(Pulsar(272.30971f, -21.15080f, "J1809-2109", Pulsar::ATNF));
    ps.add(Pulsar(272.39967f, -7.71706f, "J1809-0743", Pulsar::ATNF));
    ps.add(Pulsar(272.43962f, -14.49028f, "J1809-1429", Pulsar::ATNF));
    ps.add(Pulsar(272.40504f, -18.84861f, "J1809-1850", Pulsar::ATNF));
    ps.add(Pulsar(272.42978f, -19.29392f, "J1809-1917", Pulsar::ATNF));
    ps.add(Pulsar(272.46286f, -19.73109f, "J1809-1943", Pulsar::ATNF));
    ps.add(Pulsar(272.31612f, -20.07000f, "J1809-2004", Pulsar::ATNF));
    ps.add(Pulsar(272.45921f, -23.53969f, "J1809-2332", Pulsar::ATNF));
    ps.add(Pulsar(272.36242f, -35.78833f, "J1809-3547", Pulsar::ATNF));
    ps.add(Pulsar(272.68512f, -53.63547f, "J1810-5338", Pulsar::ATNF));
    ps.add(Pulsar(272.74651f, -14.69267f, "J1810-1441", Pulsar::ATNF));
    ps.add(Pulsar(272.73133f, -18.34417f, "J1810-1820", Pulsar::ATNF));
    ps.add(Pulsar(272.74580f, -20.08563f, "J1810-2005", Pulsar::ATNF));
    ps.add(Pulsar(272.83283f, -1.90858f, "J1811-0154", Pulsar::ATNF));
    ps.add(Pulsar(272.83508f, 7.04158f, "J1811+0702", Pulsar::ATNF));
    ps.add(Pulsar(272.97931f, -17.61047f, "J1811-1736", Pulsar::ATNF));
    ps.add(Pulsar(272.87383f, -18.59556f, "J1811-1835", Pulsar::ATNF));
    ps.add(Pulsar(272.87167f, -19.42444f, "J1811-1925", Pulsar::ATNF));
    ps.add(Pulsar(272.98138f, -24.66472f, "J1811-2439", Pulsar::ATNF));
    ps.add(Pulsar(273.03004f, -17.30819f, "J1812-1718", Pulsar::ATNF));
    ps.add(Pulsar(273.06622f, -17.56041f, "J1812-1733", Pulsar::ATNF));
    ps.add(Pulsar(273.22162f, 2.44920f, "J1812+0226", Pulsar::ATNF));
    ps.add(Pulsar(273.14538f, -19.17750f, "J1812-1910", Pulsar::ATNF));
    ps.add(Pulsar(273.08721f, -21.04333f, "J1812-2102", Pulsar::ATNF));
    ps.add(Pulsar(273.13458f, -25.44389f, "J1812-2526", Pulsar::ATNF));
    ps.add(Pulsar(273.30534f, 40.22751f, "J1813+4013", Pulsar::ATNF));
    ps.add(Pulsar(273.34750f, -12.76708f, "J1813-1246", Pulsar::ATNF));
    ps.add(Pulsar(273.39652f, -17.83263f, "J1813-1749", Pulsar::ATNF));
    ps.add(Pulsar(273.41150f, 18.37083f, "J1813+1822", Pulsar::ATNF));
    ps.add(Pulsar(273.41612f, -21.21678f, "J1813-2113", Pulsar::ATNF));
    ps.add(Pulsar(273.37150f, -22.70167f, "J1813-2242", Pulsar::ATNF));
    ps.add(Pulsar(273.67192f, -6.30047f, "J1814-0618", Pulsar::ATNF));
    ps.add(Pulsar(273.67809f, 11.51221f, "J1814+1130", Pulsar::ATNF));
    ps.add(Pulsar(273.65562f, -16.82444f, "J1814-1649", Pulsar::ATNF));
    ps.add(Pulsar(273.67958f, -17.74667f, "J1814-1744", Pulsar::ATNF));
    ps.add(Pulsar(273.81113f, -17.63417f, "J1815-1738", Pulsar::ATNF));
    ps.add(Pulsar(273.76283f, -19.16678f, "J1815-1910", Pulsar::ATNF));
    ps.add(Pulsar(274.07776f, -17.48408f, "J1816-1729", Pulsar::ATNF));
    ps.add(Pulsar(274.14751f, -26.83153f, "J1816-2650", Pulsar::ATNF));
    ps.add(Pulsar(274.10226f, -7.92292f, "J1816-0755", Pulsar::ATNF));
    ps.add(Pulsar(274.12162f, -14.77500f, "J1816-1446", Pulsar::ATNF));
    ps.add(Pulsar(274.15193f, -56.72836f, "J1816-5643", Pulsar::ATNF));
    ps.add(Pulsar(274.27407f, -36.30111f, "J1817-3618", Pulsar::ATNF));
    ps.add(Pulsar(274.25000f, -23.18333f, "J1817-2311", Pulsar::ATNF));
    ps.add(Pulsar(274.45746f, -7.72192f, "J1817-0743", Pulsar::ATNF));
    ps.add(Pulsar(274.40083f, -15.19417f, "J1817-1511", Pulsar::ATNF));
    ps.add(Pulsar(274.25113f, -38.63325f, "J1817-3837", Pulsar::ATNF));
    ps.add(Pulsar(274.59902f, -14.37686f, "J1818-1422", Pulsar::ATNF));
    ps.add(Pulsar(274.61021f, -11.27472f, "J1818-1116", Pulsar::ATNF));
    ps.add(Pulsar(274.56083f, -15.32861f, "J1818-1519", Pulsar::ATNF));
    ps.add(Pulsar(274.65633f, -15.69583f, "J1818-1541", Pulsar::ATNF));
    ps.add(Pulsar(274.71646f, -15.93444f, "J1818-1556", Pulsar::ATNF));
    ps.add(Pulsar(274.96059f, -9.43053f, "J1819-0925", Pulsar::ATNF));
    ps.add(Pulsar(274.91661f, -10.14111f, "J1819-1008", Pulsar::ATNF));
    ps.add(Pulsar(274.99229f, -11.52472f, "J1819-1131", Pulsar::ATNF));
    ps.add(Pulsar(274.98427f, 13.08757f, "J1819+1305", Pulsar::ATNF));
    ps.add(Pulsar(274.93192f, -13.31167f, "J1819-1318", Pulsar::ATNF));
    ps.add(Pulsar(274.98667f, -14.13378f, "J1819-1408", Pulsar::ATNF));
    ps.add(Pulsar(274.89239f, -14.96766f, "J1819-1458", Pulsar::ATNF));
    ps.add(Pulsar(274.97371f, -15.17239f, "J1819-1510", Pulsar::ATNF));
    ps.add(Pulsar(275.08233f, -13.77091f, "J1820-1346", Pulsar::ATNF));
    ps.add(Pulsar(275.16287f, -18.30091f, "J1820-1818", Pulsar::ATNF));
    ps.add(Pulsar(275.21920f, -4.46059f, "J1820-0427", Pulsar::ATNF));
    ps.add(Pulsar(275.09470f, -5.16069f, "J1820-0509", Pulsar::ATNF));
    ps.add(Pulsar(275.17008f, -15.49722f, "J1820-1529", Pulsar::ATNF));
    ps.add(Pulsar(275.29296f, -2.94406f, "J1821-0256", Pulsar::ATNF));
    ps.add(Pulsar(275.39292f, -14.32389f, "J1821-1419", Pulsar::ATNF));
    ps.add(Pulsar(275.41574f, -14.54811f, "J1821-1432", Pulsar::ATNF));
    ps.add(Pulsar(275.30626f, 17.26307f, "J1821+1715", Pulsar::ATNF));
    ps.add(Pulsar(275.74564f, -22.94184f, "J1822-2256", Pulsar::ATNF));
    ps.add(Pulsar(275.72518f, -14.00067f, "J1822-1400", Pulsar::ATNF));
    ps.add(Pulsar(275.57684f, 7.08862f, "J1822+0705", Pulsar::ATNF));
    ps.add(Pulsar(275.71608f, -8.81639f, "J1822-0848", Pulsar::ATNF));
    ps.add(Pulsar(275.66583f, -9.12667f, "J1822-0907", Pulsar::ATNF));
    ps.add(Pulsar(275.56083f, 11.34894f, "J1822+1120", Pulsar::ATNF));
    ps.add(Pulsar(275.67375f, -12.88028f, "J1822-1252", Pulsar::ATNF));
    ps.add(Pulsar(275.65250f, -16.29306f, "J1822-1617", Pulsar::ATNF));
    ps.add(Pulsar(275.54732f, -42.15305f, "J1822-4209", Pulsar::ATNF));
    ps.add(Pulsar(275.91794f, -11.25314f, "J1823-1115", Pulsar::ATNF));
    ps.add(Pulsar(275.91868f, -30.36109f, "J1823-3021A", Pulsar::ATNF));
    ps.add(Pulsar(275.92308f, -30.36158f, "J1823-3021B", Pulsar::ATNF));
    ps.add(Pulsar(275.94495f, -31.11382f, "J1823-3106", Pulsar::ATNF));
    ps.add(Pulsar(275.87905f, 5.84009f, "J1823+0550", Pulsar::ATNF));
    ps.add(Pulsar(275.96728f, -1.90127f, "J1823-0154", Pulsar::ATNF));
    ps.add(Pulsar(275.83275f, -11.43444f, "J1823-1126", Pulsar::ATNF));
    ps.add(Pulsar(275.85104f, -13.79833f, "J1823-1347", Pulsar::ATNF));
    ps.add(Pulsar(275.83925f, -15.43944f, "J1823-1526", Pulsar::ATNF));
    ps.add(Pulsar(275.79033f, -18.12583f, "J1823-1807", Pulsar::ATNF));
    ps.add(Pulsar(275.91875f, -24.99750f, "J1823-3021C", Pulsar::ATNF));
    ps.add(Pulsar(276.12308f, -11.31184f, "J1824-1118", Pulsar::ATNF));
    ps.add(Pulsar(276.00190f, -19.76437f, "J1824-1945", Pulsar::ATNF));
    ps.add(Pulsar(276.13337f, -24.86967f, "J1824-2452", Pulsar::ATNF));
    ps.add(Pulsar(276.22263f, -1.46428f, "J1824-0127", Pulsar::ATNF));
    ps.add(Pulsar(276.23396f, -11.99817f, "J1824-1159", Pulsar::ATNF));
    ps.add(Pulsar(276.23911f, -14.38481f, "J1824-1423", Pulsar::ATNF));
    ps.add(Pulsar(276.05875f, -15.00917f, "J1824-1500", Pulsar::ATNF));
    ps.add(Pulsar(276.04300f, -22.55306f, "J1824-2233", Pulsar::ATNF));
    ps.add(Pulsar(276.11933f, -23.47139f, "J1824-2328", Pulsar::ATNF));
    ps.add(Pulsar(276.12745f, -25.62194f, "J1824-2537", Pulsar::ATNF));
    ps.add(Pulsar(276.31385f, 0.07214f, "J1825+0004", Pulsar::ATNF));
    ps.add(Pulsar(276.37762f, -9.58953f, "J1825-0935", Pulsar::ATNF));
    ps.add(Pulsar(276.26220f, -14.78128f, "J1825-1446", Pulsar::ATNF));
    ps.add(Pulsar(276.50833f, -33.35000f, "J1825-33", Pulsar::ATNF));
    ps.add(Pulsar(276.52280f, -11.52878f, "J1826-1131", Pulsar::ATNF));
    ps.add(Pulsar(276.55490f, -13.57967f, "J1826-1334", Pulsar::ATNF));
    ps.add(Pulsar(276.53562f, -12.94289f, "J1826-1256", Pulsar::ATNF));
    ps.add(Pulsar(276.67663f, -14.32267f, "J1826-1419", Pulsar::ATNF));
    ps.add(Pulsar(276.55250f, -15.43417f, "J1826-1526", Pulsar::ATNF));
    ps.add(Pulsar(276.77283f, -9.97864f, "J1827-0958", Pulsar::ATNF));
    ps.add(Pulsar(276.76140f, -7.83867f, "J1827-0750", Pulsar::ATNF));
    ps.add(Pulsar(276.94118f, -9.57122f, "J1827-0934", Pulsar::ATNF));
    ps.add(Pulsar(277.08631f, -6.19764f, "J1828-0611", Pulsar::ATNF));
    ps.add(Pulsar(277.12648f, -10.11947f, "J1828-1007", Pulsar::ATNF));
    ps.add(Pulsar(277.13837f, -10.95722f, "J1828-1057", Pulsar::ATNF));
    ps.add(Pulsar(277.07854f, -11.03081f, "J1828-1101", Pulsar::ATNF));
    ps.add(Pulsar(277.17854f, -13.61250f, "J1828-1336", Pulsar::ATNF));
    ps.add(Pulsar(277.22224f, 13.99316f, "J1828+1359", Pulsar::ATNF));
    ps.add(Pulsar(277.08983f, -21.33222f, "J1828-2119", Pulsar::ATNF));
    ps.add(Pulsar(277.42974f, -17.85108f, "J1829-1751", Pulsar::ATNF));
    ps.add(Pulsar(277.44642f, 0.00236f, "J1829+0000", Pulsar::ATNF));
    ps.add(Pulsar(277.27238f, -7.57278f, "J1829-0734", Pulsar::ATNF));
    ps.add(Pulsar(277.39445f, 24.93839f, "J1829+2456", Pulsar::ATNF));
    ps.add(Pulsar(277.69820f, -10.99109f, "J1830-1059", Pulsar::ATNF));
    ps.add(Pulsar(277.62792f, -0.88167f, "J1830-0052", Pulsar::ATNF));
    ps.add(Pulsar(277.58172f, -1.53003f, "J1830-0131", Pulsar::ATNF));
    ps.add(Pulsar(277.50717f, -11.59222f, "J1830-1135", Pulsar::ATNF));
    ps.add(Pulsar(277.67492f, -13.22117f, "J1830-1313", Pulsar::ATNF));
    ps.add(Pulsar(277.50146f, -14.24361f, "J1830-1414", Pulsar::ATNF));
    ps.add(Pulsar(277.90139f, -8.38997f, "J1831-0823", Pulsar::ATNF));
    ps.add(Pulsar(277.89293f, -9.86714f, "J1831-0952", Pulsar::ATNF));
    ps.add(Pulsar(277.80346f, -12.39194f, "J1831-1223", Pulsar::ATNF));
    ps.add(Pulsar(277.98317f, -13.49889f, "J1831-1329", Pulsar::ATNF));
    ps.add(Pulsar(277.87125f, -14.39611f, "J1831-1423", Pulsar::ATNF));
    ps.add(Pulsar(278.15425f, -8.45101f, "J1832-0827", Pulsar::ATNF));
    ps.add(Pulsar(278.17028f, -10.35910f, "J1832-1021", Pulsar::ATNF));
    ps.add(Pulsar(278.21125f, 0.49083f, "J1832+0029", Pulsar::ATNF));
    ps.add(Pulsar(278.17779f, -6.73383f, "J1832-0644", Pulsar::ATNF));
    ps.add(Pulsar(278.12500f, -28.71667f, "J1832-28", Pulsar::ATNF));
    ps.add(Pulsar(278.31167f, -60.38444f, "J1833-6023", Pulsar::ATNF));
    ps.add(Pulsar(278.41792f, -8.45868f, "J1833-0827", Pulsar::ATNF));
    ps.add(Pulsar(278.42464f, -3.65120f, "J1833-0338", Pulsar::ATNF));
    ps.add(Pulsar(278.41200f, -5.93472f, "J1833-0556", Pulsar::ATNF));
    ps.add(Pulsar(278.28167f, -5.99028f, "J1833-0559", Pulsar::ATNF));
    ps.add(Pulsar(278.38988f, -10.56875f, "J1833-1034", Pulsar::ATNF));
    ps.add(Pulsar(278.49333f, -10.92528f, "J1833-1055", Pulsar::ATNF));
    ps.add(Pulsar(278.57190f, -0.18147f, "J1834-0010", Pulsar::ATNF));
    ps.add(Pulsar(278.60668f, -4.43773f, "J1834-0426", Pulsar::ATNF));
    ps.add(Pulsar(278.71289f, -0.52619f, "J1834-0031", Pulsar::ATNF));
    ps.add(Pulsar(278.65821f, -6.04306f, "J1834-0602", Pulsar::ATNF));
    ps.add(Pulsar(278.62188f, -6.55031f, "J1834-0633", Pulsar::ATNF));
    ps.add(Pulsar(278.56667f, -7.51861f, "J1834-0731", Pulsar::ATNF));
    ps.add(Pulsar(278.63050f, -7.70572f, "J1834-0742", Pulsar::ATNF));
    ps.add(Pulsar(278.61250f, 10.73333f, "J1834+10", Pulsar::ATNF));
    ps.add(Pulsar(278.59633f, -12.04067f, "J1834-1202", Pulsar::ATNF));
    ps.add(Pulsar(278.72250f, -17.18056f, "J1834-1710", Pulsar::ATNF));
    ps.add(Pulsar(278.69142f, -18.93306f, "J1834-1855", Pulsar::ATNF));
    ps.add(Pulsar(278.77317f, -6.71843f, "J1835-0643", Pulsar::ATNF));
    ps.add(Pulsar(278.80394f, -3.81942f, "J1835-0349", Pulsar::ATNF));
    ps.add(Pulsar(278.78383f, -5.36889f, "J1835-0522", Pulsar::ATNF));
    ps.add(Pulsar(278.90842f, -9.40750f, "J1835-0924", Pulsar::ATNF));
    ps.add(Pulsar(278.94442f, -9.74139f, "J1835-0944", Pulsar::ATNF));
    ps.add(Pulsar(278.93692f, -9.77722f, "J1835-0946", Pulsar::ATNF));
    ps.add(Pulsar(278.98980f, -10.33467f, "J1835-1020", Pulsar::ATNF));
    ps.add(Pulsar(278.82620f, -11.10419f, "J1835-1106", Pulsar::ATNF));
    ps.add(Pulsar(278.95971f, -15.81056f, "J1835-1548", Pulsar::ATNF));
    ps.add(Pulsar(279.21579f, -4.61047f, "J1836-0436", Pulsar::ATNF));
    ps.add(Pulsar(279.22469f, -10.13565f, "J1836-1008", Pulsar::ATNF));
    ps.add(Pulsar(279.10500f, -5.29306f, "J1836-0517", Pulsar::ATNF));
    ps.add(Pulsar(279.21786f, -13.40933f, "J1836-1324", Pulsar::ATNF));
    ps.add(Pulsar(279.05729f, 59.42508f, "J1836+5925", Pulsar::ATNF));
    ps.add(Pulsar(279.31104f, -6.88392f, "J1837-0653", Pulsar::ATNF));
    ps.add(Pulsar(279.38398f, -0.75296f, "J1837-0045", Pulsar::ATNF));
    ps.add(Pulsar(279.36854f, 0.88694f, "J1837+0053", Pulsar::ATNF));
    ps.add(Pulsar(279.34860f, -5.99119f, "J1837-0559", Pulsar::ATNF));
    ps.add(Pulsar(279.43146f, -6.08028f, "J1837-0604", Pulsar::ATNF));
    ps.add(Pulsar(279.27967f, 12.36500f, "J1837+1221", Pulsar::ATNF));
    ps.add(Pulsar(279.28921f, -12.73222f, "J1837-1243", Pulsar::ATNF));
    ps.add(Pulsar(279.47604f, -18.61889f, "J1837-1837", Pulsar::ATNF));
    ps.add(Pulsar(279.54663f, -4.88972f, "J1838-0453", Pulsar::ATNF));
    ps.add(Pulsar(279.65871f, -5.82000f, "J1838-0549", Pulsar::ATNF));
    ps.add(Pulsar(279.71575f, -6.41500f, "J1838-0624", Pulsar::ATNF));
    ps.add(Pulsar(279.51304f, -6.92594f, "J1838-0655", Pulsar::ATNF));
    ps.add(Pulsar(279.61033f, -10.78250f, "J1838-1046", Pulsar::ATNF));
    ps.add(Pulsar(279.67943f, 16.83779f, "J1838+1650", Pulsar::ATNF));
    ps.add(Pulsar(279.75000f, -1.60000f, "J1839-01", Pulsar::ATNF));
    ps.add(Pulsar(279.90631f, -3.35292f, "J1839-0321", Pulsar::ATNF));
    ps.add(Pulsar(279.96275f, -4.04025f, "J1839-0402", Pulsar::ATNF));
    ps.add(Pulsar(279.75027f, -4.61597f, "J1839-0436", Pulsar::ATNF));
    ps.add(Pulsar(279.92758f, -4.99967f, "J1839-0459", Pulsar::ATNF));
    ps.add(Pulsar(279.83525f, -6.45944f, "J1839-0627", Pulsar::ATNF));
    ps.add(Pulsar(279.79085f, -6.72922f, "J1839-0643", Pulsar::ATNF));
    ps.add(Pulsar(279.97274f, -9.08744f, "J1839-0905", Pulsar::ATNF));
    ps.add(Pulsar(279.93212f, -12.64464f, "J1839-1238", Pulsar::ATNF));
    ps.add(Pulsar(280.18587f, 56.68208f, "J1840+5640", Pulsar::ATNF));
    ps.add(Pulsar(280.14203f, 2.24853f, "J1840+0214", Pulsar::ATNF));
    ps.add(Pulsar(280.09658f, -5.98783f, "J1840-0559", Pulsar::ATNF));
    ps.add(Pulsar(280.06796f, -6.43761f, "J1840-0626", Pulsar::ATNF));
    ps.add(Pulsar(280.13902f, -8.15092f, "J1840-0809", Pulsar::ATNF));
    ps.add(Pulsar(280.05740f, -8.25294f, "J1840-0815", Pulsar::ATNF));
    ps.add(Pulsar(280.21625f, -8.67472f, "J1840-0840", Pulsar::ATNF));
    ps.add(Pulsar(280.10028f, -11.36964f, "J1840-1122", Pulsar::ATNF));
    ps.add(Pulsar(280.22409f, -12.12572f, "J1840-1207", Pulsar::ATNF));
    ps.add(Pulsar(280.27360f, -4.42208f, "J1841-0425", Pulsar::ATNF));
    ps.add(Pulsar(280.48317f, 9.20204f, "J1841+0912", Pulsar::ATNF));
    ps.add(Pulsar(280.32400f, 1.50469f, "J1841+0130", Pulsar::ATNF));
    ps.add(Pulsar(280.48420f, -1.96517f, "J1841-0157", Pulsar::ATNF));
    ps.add(Pulsar(280.35787f, -3.17250f, "J1841-0310", Pulsar::ATNF));
    ps.add(Pulsar(280.41117f, -3.81183f, "J1841-0345", Pulsar::ATNF));
    ps.add(Pulsar(280.33000f, -4.93694f, "J1841-0456", Pulsar::ATNF));
    ps.add(Pulsar(280.45550f, -5.40819f, "J1841-0524", Pulsar::ATNF));
    ps.add(Pulsar(280.39292f, -14.07194f, "J1841-1404", Pulsar::ATNF));
    ps.add(Pulsar(280.25000f, -14.30000f, "J1841-14", Pulsar::ATNF));
    ps.add(Pulsar(280.35792f, -78.75417f, "J1841-7845", Pulsar::ATNF));
    ps.add(Pulsar(280.61032f, -3.99995f, "J1842-0359", Pulsar::ATNF));
    ps.add(Pulsar(280.74113f, -1.89075f, "J1842-0153", Pulsar::ATNF));
    ps.add(Pulsar(280.62887f, 2.96633f, "J1842+0257", Pulsar::ATNF));
    ps.add(Pulsar(280.57925f, -3.16278f, "J1842-0309", Pulsar::ATNF));
    ps.add(Pulsar(280.57093f, 3.97647f, "J1842+0358", Pulsar::ATNF));
    ps.add(Pulsar(280.54704f, -4.26061f, "J1842-0415", Pulsar::ATNF));
    ps.add(Pulsar(280.67937f, -6.21000f, "J1842-0612", Pulsar::ATNF));
    ps.add(Pulsar(280.54167f, 6.63750f, "J1842+0638", Pulsar::ATNF));
    ps.add(Pulsar(280.59224f, -9.09017f, "J1842-0905", Pulsar::ATNF));
    ps.add(Pulsar(280.62483f, 13.53375f, "J1842+1332", Pulsar::ATNF));
    ps.add(Pulsar(280.86651f, -0.01136f, "J1843-0000", Pulsar::ATNF));
    ps.add(Pulsar(280.90300f, -0.83611f, "J1843-0050", Pulsar::ATNF));
    ps.add(Pulsar(280.80263f, -1.62953f, "J1843-0137", Pulsar::ATNF));
    ps.add(Pulsar(280.87637f, -2.18411f, "J1843-0211", Pulsar::ATNF));
    ps.add(Pulsar(280.77779f, -3.93222f, "J1843-0355", Pulsar::ATNF));
    ps.add(Pulsar(280.93100f, -4.13444f, "J1843-0408", Pulsar::ATNF));
    ps.add(Pulsar(280.86518f, -4.99178f, "J1843-0459", Pulsar::ATNF));
    ps.add(Pulsar(280.84350f, -7.04850f, "J1843-0702", Pulsar::ATNF));
    ps.add(Pulsar(280.77286f, -7.74169f, "J1843-0744", Pulsar::ATNF));
    ps.add(Pulsar(280.86965f, -8.11247f, "J1843-0806", Pulsar::ATNF));
    ps.add(Pulsar(280.92193f, -11.22529f, "J1843-1113", Pulsar::ATNF));
    ps.add(Pulsar(280.89003f, -15.11750f, "J1843-1507", Pulsar::ATNF));
    ps.add(Pulsar(280.85908f, 20.41517f, "J1843+2024", Pulsar::ATNF));
    ps.add(Pulsar(281.13938f, -4.55345f, "J1844-0433", Pulsar::ATNF));
    ps.add(Pulsar(281.02129f, -5.64283f, "J1844-0538", Pulsar::ATNF));
    ps.add(Pulsar(281.18740f, -2.74470f, "J1844-0244", Pulsar::ATNF));
    ps.add(Pulsar(281.22873f, 14.90392f, "J1844+1454", Pulsar::ATNF));
    ps.add(Pulsar(281.04583f, 0.58333f, "J1844+00", Pulsar::ATNF));
    ps.add(Pulsar(281.17125f, -0.50717f, "J1844-0030", Pulsar::ATNF));
    ps.add(Pulsar(281.22083f, -2.93333f, "J1844-0256", Pulsar::ATNF));
    ps.add(Pulsar(281.18950f, -3.17697f, "J1844-0310", Pulsar::ATNF));
    ps.add(Pulsar(281.00642f, -4.87247f, "J1844-0452", Pulsar::ATNF));
    ps.add(Pulsar(281.14150f, -5.03347f, "J1844-0502", Pulsar::ATNF));
    ps.add(Pulsar(281.39462f, -4.57495f, "J1845-0434", Pulsar::ATNF));
    ps.add(Pulsar(281.23750f, -3.00000f, "J1845-0256", Pulsar::ATNF));
    ps.add(Pulsar(281.47042f, -3.26806f, "J1845-0316", Pulsar::ATNF));
    ps.add(Pulsar(281.41037f, -5.75506f, "J1845-0545", Pulsar::ATNF));
    ps.add(Pulsar(281.28579f, 6.39933f, "J1845+0623", Pulsar::ATNF));
    ps.add(Pulsar(281.48825f, -7.72733f, "J1845-0743", Pulsar::ATNF));
    ps.add(Pulsar(281.27372f, -8.44436f, "J1845-0826", Pulsar::ATNF));
    ps.add(Pulsar(281.44074f, -11.23639f, "J1845-1114", Pulsar::ATNF));
    ps.add(Pulsar(281.29792f, -13.86514f, "J1845-1351", Pulsar::ATNF));
    ps.add(Pulsar(281.68259f, 0.86083f, "J1846+0051", Pulsar::ATNF));
    ps.add(Pulsar(281.56454f, -2.96000f, "J1846-0257", Pulsar::ATNF));
    ps.add(Pulsar(281.60392f, -2.97503f, "J1846-0258", Pulsar::ATNF));
    ps.add(Pulsar(281.58227f, -7.82261f, "J1846-0749", Pulsar::ATNF));
    ps.add(Pulsar(281.53333f, -7.82033f, "J1846-07492", Pulsar::ATNF));
    ps.add(Pulsar(281.55742f, -74.05111f, "J1846-7403", Pulsar::ATNF));
    ps.add(Pulsar(281.84516f, -4.03728f, "J1847-0402", Pulsar::ATNF));
    ps.add(Pulsar(281.89671f, -1.51278f, "J1847-0130", Pulsar::ATNF));
    ps.add(Pulsar(281.90802f, -4.63758f, "J1847-0438", Pulsar::ATNF));
    ps.add(Pulsar(281.96604f, -4.72672f, "J1847-0443", Pulsar::ATNF));
    ps.add(Pulsar(281.83780f, -6.08725f, "J1847-0605", Pulsar::ATNF));
    ps.add(Pulsar(282.09830f, -1.39951f, "J1848-0123", Pulsar::ATNF));
    ps.add(Pulsar(282.07514f, -19.87525f, "J1848-1952", Pulsar::ATNF));
    ps.add(Pulsar(282.15787f, -0.38806f, "J1848-0023", Pulsar::ATNF));
    ps.add(Pulsar(282.18958f, -0.93139f, "J1848-0055", Pulsar::ATNF));
    ps.add(Pulsar(282.06254f, -5.19389f, "J1848-0511", Pulsar::ATNF));
    ps.add(Pulsar(282.08479f, -6.01881f, "J1848-0601", Pulsar::ATNF));
    ps.add(Pulsar(282.22759f, 6.07967f, "J1848+0604", Pulsar::ATNF));
    ps.add(Pulsar(282.23337f, 6.79214f, "J1848+0647", Pulsar::ATNF));
    ps.add(Pulsar(282.18237f, 8.44331f, "J1848+0826", Pulsar::ATNF));
    ps.add(Pulsar(282.04921f, -11.83589f, "J1848-1150", Pulsar::ATNF));
    ps.add(Pulsar(282.00000f, -12.78333f, "J1848-12", Pulsar::ATNF));
    ps.add(Pulsar(282.12500f, 12.83333f, "J1848+12", Pulsar::ATNF));
    ps.add(Pulsar(282.16315f, -14.23816f, "J1848-1414", Pulsar::ATNF));
    ps.add(Pulsar(282.27682f, -6.61859f, "J1849-0636", Pulsar::ATNF));
    ps.add(Pulsar(282.29271f, -0.67222f, "J1849-0040", Pulsar::ATNF));
    ps.add(Pulsar(282.43408f, 1.45639f, "J1849+0127", Pulsar::ATNF));
    ps.add(Pulsar(282.49104f, -3.29194f, "J1849-0317", Pulsar::ATNF));
    ps.add(Pulsar(282.26446f, 4.16175f, "J1849+0409", Pulsar::ATNF));
    ps.add(Pulsar(282.43815f, -6.24208f, "J1849-0614", Pulsar::ATNF));
    ps.add(Pulsar(282.39448f, 24.39610f, "J1849+2423", Pulsar::ATNF));
    ps.add(Pulsar(282.64781f, 13.59955f, "J1850+1335", Pulsar::ATNF));
    ps.add(Pulsar(282.69971f, -0.10725f, "J1850-0006", Pulsar::ATNF));
    ps.add(Pulsar(282.56131f, -0.43656f, "J1850-0026", Pulsar::ATNF));
    ps.add(Pulsar(282.68810f, 0.44044f, "J1850+0026", Pulsar::ATNF));
    ps.add(Pulsar(282.63912f, -0.51917f, "J1850-0031", Pulsar::ATNF));
    ps.add(Pulsar(282.53750f, 15.53333f, "J1850+15", Pulsar::ATNF));
    ps.add(Pulsar(282.76375f, 4.30334f, "J1851+0418", Pulsar::ATNF));
    ps.add(Pulsar(282.80506f, 12.99313f, "J1851+1259", Pulsar::ATNF));
    ps.add(Pulsar(282.97955f, -0.49947f, "J1851-0029", Pulsar::ATNF));
    ps.add(Pulsar(282.76321f, -0.88536f, "J1851-0053", Pulsar::ATNF));
    ps.add(Pulsar(282.81838f, -1.23706f, "J1851-0114", Pulsar::ATNF));
    ps.add(Pulsar(282.96742f, 1.31639f, "J1851+0118", Pulsar::ATNF));
    ps.add(Pulsar(282.81358f, -2.69194f, "J1851-0241", Pulsar::ATNF));
    ps.add(Pulsar(283.11426f, 0.53388f, "J1852+0031", Pulsar::ATNF));
    ps.add(Pulsar(283.17825f, 0.13600f, "J1852+0008", Pulsar::ATNF));
    ps.add(Pulsar(283.17408f, 0.23253f, "J1852+0013", Pulsar::ATNF));
    ps.add(Pulsar(283.16071f, 0.67217f, "J1852+0040", Pulsar::ATNF));
    ps.add(Pulsar(283.07146f, -1.30411f, "J1852-0118", Pulsar::ATNF));
    ps.add(Pulsar(283.01500f, -1.45650f, "J1852-0127", Pulsar::ATNF));
    ps.add(Pulsar(283.13583f, 3.08472f, "J1852+0305", Pulsar::ATNF));
    ps.add(Pulsar(283.23908f, -6.59917f, "J1852-0635", Pulsar::ATNF));
    ps.add(Pulsar(283.24778f, -26.17020f, "J1852-2610", Pulsar::ATNF));
    ps.add(Pulsar(283.34591f, -0.07564f, "J1853-0004", Pulsar::ATNF));
    ps.add(Pulsar(283.37487f, 0.19158f, "J1853+0011", Pulsar::ATNF));
    ps.add(Pulsar(283.38608f, 0.94972f, "J1853+0056", Pulsar::ATNF));
    ps.add(Pulsar(283.26817f, 5.09058f, "J1853+0505", Pulsar::ATNF));
    ps.add(Pulsar(283.49338f, 5.76535f, "J1853+0545", Pulsar::ATNF));
    ps.add(Pulsar(283.34196f, 8.88806f, "J1853+0853", Pulsar::ATNF));
    ps.add(Pulsar(283.48883f, 13.06225f, "J1853+1303", Pulsar::ATNF));
    ps.add(Pulsar(283.68455f, -14.35734f, "J1854-1421", Pulsar::ATNF));
    ps.add(Pulsar(283.62223f, 10.77837f, "J1854+1050", Pulsar::ATNF));
    ps.add(Pulsar(283.53750f, 3.06667f, "J1854+03", Pulsar::ATNF));
    ps.add(Pulsar(283.86096f, 3.12228f, "J1855+0307", Pulsar::ATNF));
    ps.add(Pulsar(283.92237f, 4.37972f, "J1855+0422", Pulsar::ATNF));
    ps.add(Pulsar(283.81280f, 5.46131f, "J1855+0527", Pulsar::ATNF));
    ps.add(Pulsar(283.82383f, 7.01031f, "J1855+0700", Pulsar::ATNF));
    ps.add(Pulsar(283.81533f, -9.68389f, "J1855-0941", Pulsar::ATNF));
    ps.add(Pulsar(284.04438f, 1.22258f, "J1856+0113", Pulsar::ATNF));
    ps.add(Pulsar(284.11876f, 1.03628f, "J1856+0102", Pulsar::ATNF));
    ps.add(Pulsar(284.21167f, 2.76394f, "J1856+0245", Pulsar::ATNF));
    ps.add(Pulsar(284.11083f, 4.07389f, "J1856+0404", Pulsar::ATNF));
    ps.add(Pulsar(284.09157f, -5.44911f, "J1856-0526", Pulsar::ATNF));
    ps.add(Pulsar(284.14754f, -37.90222f, "J1856-3754", Pulsar::ATNF));
    ps.add(Pulsar(284.25337f, 0.95466f, "J1857+0057", Pulsar::ATNF));
    ps.add(Pulsar(284.43184f, 2.21142f, "J1857+0212", Pulsar::ATNF));
    ps.add(Pulsar(284.40164f, 9.72147f, "J1857+0943", Pulsar::ATNF));
    ps.add(Pulsar(284.38753f, 1.72972f, "J1857+0143", Pulsar::ATNF));
    ps.add(Pulsar(284.42042f, 2.18278f, "J1857+0210", Pulsar::ATNF));
    ps.add(Pulsar(284.31607f, 5.44131f, "J1857+0526", Pulsar::ATNF));
    ps.add(Pulsar(284.28879f, 8.15119f, "J1857+0809", Pulsar::ATNF));
    ps.add(Pulsar(284.36021f, -10.45028f, "J1857-1027", Pulsar::ATNF));
    ps.add(Pulsar(284.57262f, 2.26056f, "J1858+0215", Pulsar::ATNF));
    ps.add(Pulsar(284.72421f, 2.69389f, "J1858+0241", Pulsar::ATNF));
    ps.add(Pulsar(284.94167f, 0.58333f, "J1859+00", Pulsar::ATNF));
    ps.add(Pulsar(284.94067f, 6.02947f, "J1859+0601", Pulsar::ATNF));
    ps.add(Pulsar(284.93495f, 15.43639f, "J1859+1526", Pulsar::ATNF));
    ps.add(Pulsar(285.10167f, -79.86339f, "J1900-7951", Pulsar::ATNF));
    ps.add(Pulsar(285.19826f, -26.01217f, "J1900-2600", Pulsar::ATNF));
    ps.add(Pulsar(285.19435f, -0.85233f, "J1900-0051", Pulsar::ATNF));
    ps.add(Pulsar(285.16083f, 2.45889f, "J1900+0227", Pulsar::ATNF));
    ps.add(Pulsar(285.11681f, 6.57247f, "J1900+0634", Pulsar::ATNF));
    ps.add(Pulsar(285.07500f, 30.88333f, "J1900+30", Pulsar::ATNF));
    ps.add(Pulsar(285.39287f, 1.94395f, "J1901+0156", Pulsar::ATNF));
    ps.add(Pulsar(285.38242f, 3.51831f, "J1901+0331", Pulsar::ATNF));
    ps.add(Pulsar(285.41224f, 7.27633f, "J1901+0716", Pulsar::ATNF));
    ps.add(Pulsar(285.38333f, 0.43333f, "J1901+00", Pulsar::ATNF));
    ps.add(Pulsar(285.46894f, 1.41369f, "J1901+0124", Pulsar::ATNF));
    ps.add(Pulsar(285.31529f, 2.91139f, "J1901+0254", Pulsar::ATNF));
    ps.add(Pulsar(285.31531f, -3.20819f, "J1901-0312", Pulsar::ATNF));
    ps.add(Pulsar(285.31804f, -3.25397f, "J1901-0315", Pulsar::ATNF));
    ps.add(Pulsar(285.26254f, 3.33833f, "J1901+0320", Pulsar::ATNF));
    ps.add(Pulsar(285.37838f, 3.93303f, "J1901+0355", Pulsar::ATNF));
    ps.add(Pulsar(285.29292f, 4.23083f, "J1901+0413", Pulsar::ATNF));
    ps.add(Pulsar(285.38417f, 4.58972f, "J1901+0435", Pulsar::ATNF));
    ps.add(Pulsar(285.49104f, 5.17611f, "J1901+0510", Pulsar::ATNF));
    ps.add(Pulsar(285.40000f, 6.16000f, "J1901+06", Pulsar::ATNF));
    ps.add(Pulsar(285.27483f, 6.35528f, "J1901+0621", Pulsar::ATNF));
    ps.add(Pulsar(285.47090f, -9.10300f, "J1901-0906", Pulsar::ATNF));
    ps.add(Pulsar(285.45321f, 13.11342f, "J1901+1306", Pulsar::ATNF));
    ps.add(Pulsar(285.32513f, -17.66667f, "J1901-1740", Pulsar::ATNF));
    ps.add(Pulsar(285.67759f, 5.94053f, "J1902+0556", Pulsar::ATNF));
    ps.add(Pulsar(285.70949f, 6.27595f, "J1902+0615", Pulsar::ATNF));
    ps.add(Pulsar(285.70942f, 2.81556f, "J1902+0248", Pulsar::ATNF));
    ps.add(Pulsar(285.71125f, -3.67167f, "J1902-0340", Pulsar::ATNF));
    ps.add(Pulsar(285.55642f, 7.39750f, "J1902+0723", Pulsar::ATNF));
    ps.add(Pulsar(285.87492f, 1.59398f, "J1903+0135", Pulsar::ATNF));
    ps.add(Pulsar(285.90808f, -6.53943f, "J1903-0632", Pulsar::ATNF));
    ps.add(Pulsar(285.87643f, -2.97100f, "J1903-0258", Pulsar::ATNF));
    ps.add(Pulsar(285.77414f, 3.45534f, "J1903+0327", Pulsar::ATNF));
    ps.add(Pulsar(285.83697f, 6.02611f, "J1903+0601", Pulsar::ATNF));
    ps.add(Pulsar(285.97992f, 9.43194f, "J1903+0925", Pulsar::ATNF));
    ps.add(Pulsar(285.97062f, 22.42008f, "J1903+2225", Pulsar::ATNF));
    ps.add(Pulsar(286.01037f, 10.19294f, "J1904+1011", Pulsar::ATNF));
    ps.add(Pulsar(286.05300f, 0.06813f, "J1904+0004", Pulsar::ATNF));
    ps.add(Pulsar(286.23148f, -1.84236f, "J1904-0150", Pulsar::ATNF));
    ps.add(Pulsar(286.13076f, 4.20164f, "J1904+0412", Pulsar::ATNF));
    ps.add(Pulsar(286.03750f, 7.66139f, "J1904+07", Pulsar::ATNF));
    ps.add(Pulsar(286.01458f, 8.01461f, "J1904+0800", Pulsar::ATNF));
    ps.add(Pulsar(286.13867f, -12.40037f, "J1904-1224", Pulsar::ATNF));
    ps.add(Pulsar(286.36556f, -0.94471f, "J1905-0056", Pulsar::ATNF));
    ps.add(Pulsar(286.47343f, 7.15538f, "J1905+0709", Pulsar::ATNF));
    ps.add(Pulsar(286.31417f, 1.90917f, "J1905+0154A", Pulsar::ATNF));
    ps.add(Pulsar(286.36781f, 4.00303f, "J1905+0400", Pulsar::ATNF));
    ps.add(Pulsar(286.26812f, 6.01664f, "J1905+0600", Pulsar::ATNF));
    ps.add(Pulsar(286.27854f, 6.27131f, "J1905+0616", Pulsar::ATNF));
    ps.add(Pulsar(286.31667f, 9.02278f, "J1905+09", Pulsar::ATNF));
    ps.add(Pulsar(286.64685f, 6.68414f, "J1906+0641", Pulsar::ATNF));
    ps.add(Pulsar(286.74080f, 4.24151f, "J1906+0414", Pulsar::ATNF));
    ps.add(Pulsar(286.54987f, 6.83003f, "J1906+0649", Pulsar::ATNF));
    ps.add(Pulsar(286.70280f, 7.77461f, "J1906+0746", Pulsar::ATNF));
    ps.add(Pulsar(286.61858f, 9.21572f, "J1906+0912", Pulsar::ATNF));
    ps.add(Pulsar(286.68629f, 18.90236f, "J1906+1854", Pulsar::ATNF));
    ps.add(Pulsar(286.79458f, 12.79331f, "J1907+1247", Pulsar::ATNF));
    ps.add(Pulsar(286.89440f, 40.03492f, "J1907+4002", Pulsar::ATNF));
    ps.add(Pulsar(286.92512f, 2.82806f, "J1907+0249", Pulsar::ATNF));
    ps.add(Pulsar(286.81060f, 3.75294f, "J1907+0345", Pulsar::ATNF));
    ps.add(Pulsar(286.84725f, 5.58139f, "J1907+0534", Pulsar::ATNF));
    ps.add(Pulsar(286.97796f, 6.03781f, "J1907+0602", Pulsar::ATNF));
    ps.add(Pulsar(286.97829f, 7.52275f, "J1907+0731", Pulsar::ATNF));
    ps.add(Pulsar(286.93382f, 7.67294f, "J1907+0740", Pulsar::ATNF));
    ps.add(Pulsar(286.84350f, 9.30854f, "J1907+0918", Pulsar::ATNF));
    ps.add(Pulsar(286.80971f, 9.32225f, "J1907+0919", Pulsar::ATNF));
    ps.add(Pulsar(286.90742f, 11.82697f, "J1907+1149", Pulsar::ATNF));
    ps.add(Pulsar(287.24483f, 9.27056f, "J1908+0916", Pulsar::ATNF));
    ps.add(Pulsar(287.06729f, 4.96139f, "J1908+0457", Pulsar::ATNF));
    ps.add(Pulsar(287.02151f, 5.01512f, "J1908+0500", Pulsar::ATNF));
    ps.add(Pulsar(287.07088f, 7.57066f, "J1908+0734", Pulsar::ATNF));
    ps.add(Pulsar(287.07713f, 8.66644f, "J1908+0839", Pulsar::ATNF));
    ps.add(Pulsar(287.03099f, 9.15344f, "J1908+0909", Pulsar::ATNF));
    ps.add(Pulsar(287.13308f, 23.86164f, "J1908+2351", Pulsar::ATNF));
    ps.add(Pulsar(287.39691f, 0.13271f, "J1909+0007", Pulsar::ATNF));
    ps.add(Pulsar(287.40963f, 2.91408f, "J1909+0254", Pulsar::ATNF));
    ps.add(Pulsar(287.45289f, 11.03426f, "J1909+1102", Pulsar::ATNF));
    ps.add(Pulsar(287.46337f, 6.28106f, "J1909+0616", Pulsar::ATNF));
    ps.add(Pulsar(287.35000f, 6.66667f, "J1909+06", Pulsar::ATNF));
    ps.add(Pulsar(287.33296f, 9.21492f, "J1909+0912", Pulsar::ATNF));
    ps.add(Pulsar(287.36233f, 14.84944f, "J1909+1450", Pulsar::ATNF));
    ps.add(Pulsar(287.32752f, 18.98632f, "J1909+1859", Pulsar::ATNF));
    ps.add(Pulsar(287.44765f, -37.73733f, "J1909-3744", Pulsar::ATNF));
    ps.add(Pulsar(287.62366f, -3.16502f, "J1910-0309", Pulsar::ATNF));
    ps.add(Pulsar(287.53777f, 3.97445f, "J1910+0358", Pulsar::ATNF));
    ps.add(Pulsar(287.55641f, 12.52783f, "J1910+1231", Pulsar::ATNF));
    ps.add(Pulsar(287.56542f, -1.20167f, "J1910-0112", Pulsar::ATNF));
    ps.add(Pulsar(287.54316f, 2.42322f, "J1910+0225", Pulsar::ATNF));
    ps.add(Pulsar(287.61046f, 5.56917f, "J1910+0534", Pulsar::ATNF));
    ps.add(Pulsar(287.57749f, 7.23661f, "J1910+0714", Pulsar::ATNF));
    ps.add(Pulsar(287.59200f, 7.47697f, "J1910+0728", Pulsar::ATNF));
    ps.add(Pulsar(287.54042f, 12.94042f, "J1910+1256", Pulsar::ATNF));
    ps.add(Pulsar(287.92815f, -59.97414f, "J1910-5959A", Pulsar::ATNF));
    ps.add(Pulsar(287.71690f, -59.98357f, "J1910-5959B", Pulsar::ATNF));
    ps.add(Pulsar(287.77315f, -60.01658f, "J1910-5959C", Pulsar::ATNF));
    ps.add(Pulsar(287.71840f, -59.98486f, "J1910-5959D", Pulsar::ATNF));
    ps.add(Pulsar(287.71732f, -59.98391f, "J1910-5959E", Pulsar::ATNF));
    ps.add(Pulsar(287.79621f, 1.03604f, "J1911+0101A", Pulsar::ATNF));
    ps.add(Pulsar(287.75000f, 0.61667f, "J1911+00", Pulsar::ATNF));
    ps.add(Pulsar(287.80239f, 1.03068f, "J1911+0101B", Pulsar::ATNF));
    ps.add(Pulsar(287.95539f, -11.23954f, "J1911-1114", Pulsar::ATNF));
    ps.add(Pulsar(287.98002f, 13.79289f, "J1911+1347", Pulsar::ATNF));
    ps.add(Pulsar(287.98079f, 17.97939f, "J1911+1758", Pulsar::ATNF));
    ps.add(Pulsar(288.19304f, 10.61156f, "J1912+1036", Pulsar::ATNF));
    ps.add(Pulsar(288.18064f, 21.07605f, "J1912+2104", Pulsar::ATNF));
    ps.add(Pulsar(288.07761f, 25.41726f, "J1912+2525", Pulsar::ATNF));
    ps.add(Pulsar(288.47572f, -4.67991f, "J1913-0440", Pulsar::ATNF));
    ps.add(Pulsar(288.46958f, 9.61161f, "J1913+0936", Pulsar::ATNF));
    ps.add(Pulsar(288.35149f, 14.01464f, "J1913+1400", Pulsar::ATNF));
    ps.add(Pulsar(288.46175f, 4.76833f, "J1913+0446", Pulsar::ATNF));
    ps.add(Pulsar(288.25208f, 8.53475f, "J1913+0832", Pulsar::ATNF));
    ps.add(Pulsar(288.33775f, 9.07928f, "J1913+0904", Pulsar::ATNF));
    ps.add(Pulsar(288.26496f, 10.00067f, "J1913+1000", Pulsar::ATNF));
    ps.add(Pulsar(288.33475f, 10.18971f, "J1913+1011", Pulsar::ATNF));
    ps.add(Pulsar(288.43283f, 11.75919f, "J1913+1145", Pulsar::ATNF));
    ps.add(Pulsar(288.32490f, 13.50911f, "J1913+1330", Pulsar::ATNF));
    ps.add(Pulsar(288.54217f, 11.36769f, "J1914+1122", Pulsar::ATNF));
    ps.add(Pulsar(288.59914f, 2.32994f, "J1914+0219", Pulsar::ATNF));
    ps.add(Pulsar(288.57183f, 6.53231f, "J1914+0631", Pulsar::ATNF));
    ps.add(Pulsar(288.87493f, 10.16213f, "J1915+1009", Pulsar::ATNF));
    ps.add(Pulsar(288.86667f, 16.10761f, "J1915+1606", Pulsar::ATNF));
    ps.add(Pulsar(288.82962f, 16.78569f, "J1915+1647", Pulsar::ATNF));
    ps.add(Pulsar(288.75884f, 2.46327f, "J1915+0227", Pulsar::ATNF));
    ps.add(Pulsar(288.85333f, 7.64206f, "J1915+0738", Pulsar::ATNF));
    ps.add(Pulsar(288.75808f, 7.86922f, "J1915+0752", Pulsar::ATNF));
    ps.add(Pulsar(288.80779f, 8.64992f, "J1915+0838", Pulsar::ATNF));
    ps.add(Pulsar(288.89757f, 14.18097f, "J1915+1410", Pulsar::ATNF));
    ps.add(Pulsar(289.04883f, 10.51483f, "J1916+1030", Pulsar::ATNF));
    ps.add(Pulsar(289.13477f, 9.85722f, "J1916+0951", Pulsar::ATNF));
    ps.add(Pulsar(289.24446f, 13.21389f, "J1916+1312", Pulsar::ATNF));
    ps.add(Pulsar(289.21458f, 7.80000f, "J1916+0748", Pulsar::ATNF));
    ps.add(Pulsar(289.07950f, 8.73528f, "J1916+0844", Pulsar::ATNF));
    ps.add(Pulsar(289.10250f, 8.87667f, "J1916+0852", Pulsar::ATNF));
    ps.add(Pulsar(289.15379f, 10.38417f, "J1916+1023", Pulsar::ATNF));
    ps.add(Pulsar(289.41579f, 13.89915f, "J1917+1353", Pulsar::ATNF));
    ps.add(Pulsar(289.43428f, 22.38021f, "J1917+2224", Pulsar::ATNF));
    ps.add(Pulsar(289.45355f, 8.58184f, "J1917+0834", Pulsar::ATNF));
    ps.add(Pulsar(289.59849f, 14.75167f, "J1918+1444", Pulsar::ATNF));
    ps.add(Pulsar(289.70015f, -6.70968f, "J1918-0642", Pulsar::ATNF));
    ps.add(Pulsar(289.53208f, 15.68756f, "J1918+1541", Pulsar::ATNF));
    ps.add(Pulsar(289.96110f, 0.36106f, "J1919+0021", Pulsar::ATNF));
    ps.add(Pulsar(289.93175f, 1.58236f, "J1919+0134", Pulsar::ATNF));
    ps.add(Pulsar(289.94583f, 17.73333f, "J1919+17", Pulsar::ATNF));
    ps.add(Pulsar(290.15989f, 26.84400f, "J1920+2650", Pulsar::ATNF));
    ps.add(Pulsar(290.23075f, 10.67528f, "J1920+1040", Pulsar::ATNF));
    ps.add(Pulsar(290.05546f, 11.18306f, "J1920+1110", Pulsar::ATNF));
    ps.add(Pulsar(290.26583f, 19.81241f, "J1921+1948", Pulsar::ATNF));
    ps.add(Pulsar(290.35067f, 14.32141f, "J1921+1419", Pulsar::ATNF));
    ps.add(Pulsar(290.46499f, 20.05578f, "J1921+2003", Pulsar::ATNF));
    ps.add(Pulsar(290.43673f, 21.88396f, "J1921+2153", Pulsar::ATNF));
    ps.add(Pulsar(290.44877f, 8.21441f, "J1921+0812", Pulsar::ATNF));
    ps.add(Pulsar(290.53336f, 20.29926f, "J1922+2018", Pulsar::ATNF));
    ps.add(Pulsar(290.72305f, 21.17832f, "J1922+2110", Pulsar::ATNF));
    ps.add(Pulsar(290.78271f, 17.10261f, "J1923+1706", Pulsar::ATNF));
    ps.add(Pulsar(291.16667f, 20.66750f, "J1924+2040", Pulsar::ATNF));
    ps.add(Pulsar(291.60196f, 4.52546f, "J1926+0431", Pulsar::ATNF));
    ps.add(Pulsar(291.73844f, 14.58204f, "J1926+1434", Pulsar::ATNF));
    ps.add(Pulsar(291.68885f, 16.80911f, "J1926+1648", Pulsar::ATNF));
    ps.add(Pulsar(291.59508f, 19.46992f, "J1926+1928", Pulsar::ATNF));
    ps.add(Pulsar(291.79343f, 18.86903f, "J1927+1852", Pulsar::ATNF));
    ps.add(Pulsar(291.85404f, 18.94356f, "J1927+1856", Pulsar::ATNF));
    ps.add(Pulsar(291.78367f, 22.58258f, "J1927+2234", Pulsar::ATNF));
    ps.add(Pulsar(291.85678f, 9.18492f, "J1927+0911", Pulsar::ATNF));
    ps.add(Pulsar(292.08333f, 15.21667f, "J1928+15", Pulsar::ATNF));
    ps.add(Pulsar(292.17700f, 17.77417f, "J1928+1746", Pulsar::ATNF));
    ps.add(Pulsar(292.31992f, 18.74986f, "J1929+1844", Pulsar::ATNF));
    ps.add(Pulsar(292.36667f, 0.43333f, "J1929+00", Pulsar::ATNF));
    ps.add(Pulsar(292.50395f, 13.26975f, "J1930+1316", Pulsar::ATNF));
    ps.add(Pulsar(292.62554f, 18.87058f, "J1930+1852", Pulsar::ATNF));
    ps.add(Pulsar(292.98212f, 15.61597f, "J1931+1536", Pulsar::ATNF));
    ps.add(Pulsar(292.86667f, 30.58333f, "J1931+30", Pulsar::ATNF));
    ps.add(Pulsar(293.05812f, 10.99234f, "J1932+1059", Pulsar::ATNF));
    ps.add(Pulsar(293.03343f, 20.34623f, "J1932+2020", Pulsar::ATNF));
    ps.add(Pulsar(293.09476f, 22.34764f, "J1932+2220", Pulsar::ATNF));
    ps.add(Pulsar(293.02562f, -36.91725f, "J1932-3655", Pulsar::ATNF));
    ps.add(Pulsar(293.34387f, 13.08055f, "J1933+1304", Pulsar::ATNF));
    ps.add(Pulsar(293.40764f, 24.61101f, "J1933+2421", Pulsar::ATNF));
    ps.add(Pulsar(293.33086f, 7.96861f, "J1933+0758", Pulsar::ATNF));
    ps.add(Pulsar(293.38511f, -62.19636f, "J1933-6211", Pulsar::ATNF));
    ps.add(Pulsar(293.94927f, 16.27777f, "J1935+1616", Pulsar::ATNF));
    ps.add(Pulsar(293.87487f, 17.75339f, "J1935+1745", Pulsar::ATNF));
    ps.add(Pulsar(294.00633f, 15.61000f, "J1936+1536", Pulsar::ATNF));
    ps.add(Pulsar(294.25527f, 25.73713f, "J1937+2544", Pulsar::ATNF));
    ps.add(Pulsar(294.31796f, 15.08861f, "J1937+1505", Pulsar::ATNF));
    ps.add(Pulsar(294.44835f, 29.83383f, "J1937+2950", Pulsar::ATNF));
    ps.add(Pulsar(294.47275f, 6.83500f, "J1938+0650", Pulsar::ATNF));
    ps.add(Pulsar(294.91066f, 21.58309f, "J1939+2134", Pulsar::ATNF));
    ps.add(Pulsar(294.77332f, 24.71545f, "J1939+2449", Pulsar::ATNF));
    ps.add(Pulsar(294.79583f, 10.75000f, "J1939+10", Pulsar::ATNF));
    ps.add(Pulsar(295.12500f, -24.05000f, "J1940-2403", Pulsar::ATNF));
    ps.add(Pulsar(295.25170f, -26.03493f, "J1941-2602", Pulsar::ATNF));
    ps.add(Pulsar(295.38473f, 10.44044f, "J1941+1026", Pulsar::ATNF));
    ps.add(Pulsar(295.27028f, 13.69178f, "J1941+1341", Pulsar::ATNF));
    ps.add(Pulsar(295.50429f, 17.72453f, "J1942+1743", Pulsar::ATNF));
    ps.add(Pulsar(295.85617f, -12.62845f, "J1943-1237", Pulsar::ATNF));
    ps.add(Pulsar(295.87138f, 6.16600f, "J1943+0609", Pulsar::ATNF));
    ps.add(Pulsar(296.02183f, -17.83641f, "J1944-1750", Pulsar::ATNF));
    ps.add(Pulsar(296.13250f, 17.92844f, "J1944+1755", Pulsar::ATNF));
    ps.add(Pulsar(296.03884f, 9.12312f, "J1944+0907", Pulsar::ATNF));
    ps.add(Pulsar(296.36832f, -0.68280f, "J1945-0040", Pulsar::ATNF));
    ps.add(Pulsar(296.40042f, 18.57225f, "J1945+1834", Pulsar::ATNF));
    ps.add(Pulsar(296.71556f, -29.22975f, "J1946-2913", Pulsar::ATNF));
    ps.add(Pulsar(296.72102f, 18.09479f, "J1946+1805", Pulsar::ATNF));
    ps.add(Pulsar(296.60371f, 22.74983f, "J1946+2244", Pulsar::ATNF));
    ps.add(Pulsar(296.74095f, -13.21011f, "J1946-1312", Pulsar::ATNF));
    ps.add(Pulsar(296.50000f, 23.96667f, "J1946+24", Pulsar::ATNF));
    ps.add(Pulsar(296.69087f, 26.19700f, "J1946+2611", Pulsar::ATNF));
    ps.add(Pulsar(296.94258f, 9.25222f, "J1947+0915", Pulsar::ATNF));
    ps.add(Pulsar(296.90000f, 10.73333f, "J1947+10", Pulsar::ATNF));
    ps.add(Pulsar(296.77500f, -42.25000f, "J1947-4215", Pulsar::ATNF));
    ps.add(Pulsar(297.10419f, 35.66974f, "J1948+3540", Pulsar::ATNF));
    ps.add(Pulsar(297.35589f, -25.40027f, "J1949-2524", Pulsar::ATNF));
    ps.add(Pulsar(297.74167f, 5.58333f, "J1950+05", Pulsar::ATNF));
    ps.add(Pulsar(297.78437f, 11.39033f, "J1951+1123", Pulsar::ATNF));
    ps.add(Pulsar(298.02565f, 14.12483f, "J1952+1410", Pulsar::ATNF));
    ps.add(Pulsar(298.24252f, 32.87792f, "J1952+3252", Pulsar::ATNF));
    ps.add(Pulsar(298.44779f, 11.82850f, "J1953+1149", Pulsar::ATNF));
    ps.add(Pulsar(298.44167f, 18.77833f, "J1953+1846A", Pulsar::ATNF));
    ps.add(Pulsar(298.59397f, 29.38814f, "J1954+2923", Pulsar::ATNF));
    ps.add(Pulsar(298.86615f, 29.14542f, "J1955+2908", Pulsar::ATNF));
    ps.add(Pulsar(298.82818f, 50.99869f, "J1955+5059", Pulsar::ATNF));
    ps.add(Pulsar(299.21775f, 8.63800f, "J1956+0838", Pulsar::ATNF));
    ps.add(Pulsar(299.33082f, 28.52885f, "J1957+2831", Pulsar::ATNF));
    ps.add(Pulsar(299.69208f, 28.76739f, "J1958+2846", Pulsar::ATNF));
    ps.add(Pulsar(299.90321f, 20.80420f, "J1959+2048", Pulsar::ATNF));
    ps.add(Pulsar(300.51844f, 32.28842f, "J2002+3217", Pulsar::ATNF));
    ps.add(Pulsar(300.68346f, 40.84831f, "J2002+4050", Pulsar::ATNF));
    ps.add(Pulsar(300.69904f, 16.62131f, "J2002+1637", Pulsar::ATNF));
    ps.add(Pulsar(300.74167f, 30.58333f, "J2002+30", Pulsar::ATNF));
    ps.add(Pulsar(301.21786f, 31.61945f, "J2004+3137", Pulsar::ATNF));
    ps.add(Pulsar(301.43229f, -0.33943f, "J2005-0020", Pulsar::ATNF));
    ps.add(Pulsar(301.56820f, -8.11720f, "J2006-0807", Pulsar::ATNF));
    ps.add(Pulsar(301.80625f, 8.15917f, "J2007+0809", Pulsar::ATNF));
    ps.add(Pulsar(301.99210f, 9.17026f, "J2007+0910", Pulsar::ATNF));
    ps.add(Pulsar(302.14784f, 25.22510f, "J2008+2513", Pulsar::ATNF));
    ps.add(Pulsar(302.41250f, 33.43278f, "J2009+33", Pulsar::ATNF));
    ps.add(Pulsar(302.69133f, -13.39890f, "J2010-1323", Pulsar::ATNF));
    ps.add(Pulsar(302.58750f, 32.50611f, "J2010+32", Pulsar::ATNF));
    ps.add(Pulsar(302.94583f, 33.36361f, "J2011+33", Pulsar::ATNF));
    ps.add(Pulsar(303.29320f, 38.76203f, "J2013+3845", Pulsar::ATNF));
    ps.add(Pulsar(303.80292f, 25.40869f, "J2015+2524", Pulsar::ATNF));
    ps.add(Pulsar(304.23625f, 19.80083f, "J2016+1948", Pulsar::ATNF));
    ps.add(Pulsar(304.37057f, 20.72553f, "J2017+2043", Pulsar::ATNF));
    ps.add(Pulsar(304.51597f, 28.66506f, "J2018+2839", Pulsar::ATNF));
    ps.add(Pulsar(304.72500f, 34.54556f, "J2018+34", Pulsar::ATNF));
    ps.add(Pulsar(304.88312f, 24.42092f, "J2019+2425", Pulsar::ATNF));
    ps.add(Pulsar(305.27275f, 36.85133f, "J2021+3651", Pulsar::ATNF));
    ps.add(Pulsar(305.37771f, 40.44619f, "J2021+4026", Pulsar::ATNF));
    ps.add(Pulsar(305.65445f, 28.90642f, "J2022+2854", Pulsar::ATNF));
    ps.add(Pulsar(305.70780f, 51.91395f, "J2022+5154", Pulsar::ATNF));
    ps.add(Pulsar(305.92476f, 50.62635f, "J2023+5037", Pulsar::ATNF));
    ps.add(Pulsar(306.81954f, 21.76790f, "J2027+2146", Pulsar::ATNF));
    ps.add(Pulsar(306.81764f, 45.96586f, "J2027+4557", Pulsar::ATNF));
    ps.add(Pulsar(307.34947f, 37.73560f, "J2029+3744", Pulsar::ATNF));
    ps.add(Pulsar(307.66853f, 22.47273f, "J2030+2228", Pulsar::ATNF));
    ps.add(Pulsar(308.05446f, 41.45650f, "J2032+4127", Pulsar::ATNF));
    ps.add(Pulsar(308.44167f, 0.80000f, "J2033+00", Pulsar::ATNF));
    ps.add(Pulsar(308.33750f, 17.60000f, "J2033+17", Pulsar::ATNF));
    ps.add(Pulsar(309.31058f, 19.71508f, "J2037+1942", Pulsar::ATNF));
    ps.add(Pulsar(309.36434f, 36.35669f, "J2037+3621", Pulsar::ATNF));
    ps.add(Pulsar(309.51320f, 53.32023f, "J2038+5319", Pulsar::ATNF));
    ps.add(Pulsar(309.72579f, -38.26997f, "J2038-3816", Pulsar::ATNF));
    ps.add(Pulsar(310.07444f, 16.95846f, "J2040+1657", Pulsar::ATNF));
    ps.add(Pulsar(310.93125f, 27.68222f, "J2043+2740", Pulsar::ATNF));
    ps.add(Pulsar(311.24500f, 46.24833f, "J2044+4614", Pulsar::ATNF));
    ps.add(Pulsar(311.44727f, 9.20810f, "J2045+0912", Pulsar::ATNF));
    ps.add(Pulsar(311.50065f, -4.35722f, "J2046-0421", Pulsar::ATNF));
    ps.add(Pulsar(311.66395f, 15.67600f, "J2046+1540", Pulsar::ATNF));
    ps.add(Pulsar(311.69420f, 57.14364f, "J2046+5708", Pulsar::ATNF));
    ps.add(Pulsar(311.97767f, 50.49394f, "J2047+5029", Pulsar::ATNF));
    ps.add(Pulsar(312.14850f, -16.27904f, "J2048-1616", Pulsar::ATNF));
    ps.add(Pulsar(312.19112f, 22.91814f, "J2048+2255", Pulsar::ATNF));
    ps.add(Pulsar(312.50000f, 13.01667f, "J2050+13", Pulsar::ATNF));
    ps.add(Pulsar(312.78131f, -8.46050f, "J2051-0827", Pulsar::ATNF));
    ps.add(Pulsar(313.44642f, -72.01172f, "J2053-7200", Pulsar::ATNF));
    ps.add(Pulsar(313.91313f, 22.15756f, "J2055+2209", Pulsar::ATNF));
    ps.add(Pulsar(313.88063f, 36.50596f, "J2055+3630", Pulsar::ATNF));
    ps.add(Pulsar(317.08534f, 44.69691f, "J2108+4441", Pulsar::ATNF));
    ps.add(Pulsar(317.12704f, -34.49386f, "J2108-3429", Pulsar::ATNF));
    ps.add(Pulsar(318.26829f, 27.90064f, "J2113+2754", Pulsar::ATNF));
    ps.add(Pulsar(318.35128f, 46.73575f, "J2113+4644", Pulsar::ATNF));
    ps.add(Pulsar(319.05730f, 14.23918f, "J2116+1414", Pulsar::ATNF));
    ps.add(Pulsar(321.19406f, 14.12204f, "J2124+1407", Pulsar::ATNF));
    ps.add(Pulsar(321.18272f, -33.97907f, "J2124-3358", Pulsar::ATNF));
    ps.add(Pulsar(321.88033f, -66.80781f, "J2127-6648", Pulsar::ATNF));
    ps.add(Pulsar(322.49270f, 12.16702f, "J2129+1210A", Pulsar::ATNF));
    ps.add(Pulsar(322.49430f, 12.16675f, "J2129+1210B", Pulsar::ATNF));
    ps.add(Pulsar(322.50502f, 12.17728f, "J2129+1210C", Pulsar::ATNF));
    ps.add(Pulsar(322.49281f, 12.16659f, "J2129+1210D", Pulsar::ATNF));
    ps.add(Pulsar(322.49245f, 12.16906f, "J2129+1210E", Pulsar::ATNF));
    ps.add(Pulsar(322.48824f, 12.16747f, "J2129+1210F", Pulsar::ATNF));
    ps.add(Pulsar(322.49146f, 12.16592f, "J2129+1210G", Pulsar::ATNF));
    ps.add(Pulsar(322.49243f, 12.16651f, "J2129+1210H", Pulsar::ATNF));
    ps.add(Pulsar(322.34486f, -57.35394f, "J2129-5721", Pulsar::ATNF));
    ps.add(Pulsar(324.92500f, 0.60000f, "J2139+00", Pulsar::ATNF));
    ps.add(Pulsar(324.86208f, 22.71167f, "J2139+2242", Pulsar::ATNF));
    ps.add(Pulsar(325.09336f, -23.18022f, "J2140-2310A", Pulsar::ATNF));
    ps.add(Pulsar(325.09167f, -23.17972f, "J2140-2310B", Pulsar::ATNF));
    ps.add(Pulsar(325.76375f, 6.90472f, "J2143+0654", Pulsar::ATNF));
    ps.add(Pulsar(326.05025f, -39.56580f, "J2144-3933", Pulsar::ATNF));
    ps.add(Pulsar(326.46027f, -7.83846f, "J2145-0750", Pulsar::ATNF));
    ps.add(Pulsar(327.49412f, 63.49542f, "J2149+6329", Pulsar::ATNF));
    ps.add(Pulsar(327.65722f, 52.79712f, "J2150+5247", Pulsar::ATNF));
    ps.add(Pulsar(327.87042f, 23.25356f, "J2151+2315", Pulsar::ATNF));
    ps.add(Pulsar(328.75583f, -56.69917f, "J2155-5641", Pulsar::ATNF));
    ps.add(Pulsar(328.80671f, -31.31519f, "J2155-3118", Pulsar::ATNF));
    ps.add(Pulsar(328.81592f, 28.22003f, "J2155+2813", Pulsar::ATNF));
    ps.add(Pulsar(329.09877f, 26.30846f, "J2156+2618", Pulsar::ATNF));
    ps.add(Pulsar(329.25771f, 40.29611f, "J2157+4017", Pulsar::ATNF));
    ps.add(Pulsar(331.32186f, 14.74203f, "J2205+1444", Pulsar::ATNF));
    ps.add(Pulsar(332.09883f, 55.00234f, "J2208+5500", Pulsar::ATNF));
    ps.add(Pulsar(333.09729f, 29.55158f, "J2212+2933", Pulsar::ATNF));
    ps.add(Pulsar(333.91522f, 15.64302f, "J2215+1538", Pulsar::ATNF));
    ps.add(Pulsar(334.47929f, 57.55111f, "J2217+5733", Pulsar::ATNF));
    ps.add(Pulsar(334.95058f, 47.91498f, "J2219+4754", Pulsar::ATNF));
    ps.add(Pulsar(335.76343f, 29.39962f, "J2222+2923", Pulsar::ATNF));
    ps.add(Pulsar(336.46967f, 65.59322f, "J2225+6535", Pulsar::ATNF));
    ps.add(Pulsar(336.87500f, 30.60000f, "J2227+30", Pulsar::ATNF));
    ps.add(Pulsar(337.42431f, 62.09334f, "J2229+6205", Pulsar::ATNF));
    ps.add(Pulsar(337.46203f, 26.73272f, "J2229+2643", Pulsar::ATNF));
    ps.add(Pulsar(337.27200f, 61.23592f, "J2229+6114", Pulsar::ATNF));
    ps.add(Pulsar(338.73600f, 21.23856f, "J2234+2114", Pulsar::ATNF));
    ps.add(Pulsar(338.93208f, 15.11363f, "J2235+1506", Pulsar::ATNF));
    ps.add(Pulsar(339.56083f, 59.08000f, "J2238+59", Pulsar::ATNF));
    ps.add(Pulsar(340.00000f, 58.00000f, "J2240+58", Pulsar::ATNF));
    ps.add(Pulsar(340.73503f, 69.84782f, "J2242+6950", Pulsar::ATNF));
    ps.add(Pulsar(340.79070f, 15.30697f, "J2243+1518", Pulsar::ATNF));
    ps.add(Pulsar(342.11210f, -1.03002f, "J2248-0101", Pulsar::ATNF));
    ps.add(Pulsar(343.31055f, 15.27717f, "J2253+1516", Pulsar::ATNF));
    ps.add(Pulsar(344.49060f, 59.15412f, "J2257+5909", Pulsar::ATNF));
    ps.add(Pulsar(345.28625f, 58.87944f, "J2301+5852", Pulsar::ATNF));
    ps.add(Pulsar(345.54667f, 60.46673f, "J2302+6028", Pulsar::ATNF));
    ps.add(Pulsar(346.49302f, 31.00049f, "J2305+3100", Pulsar::ATNF));
    ps.add(Pulsar(346.48268f, 47.12926f, "J2305+4707", Pulsar::ATNF));
    ps.add(Pulsar(346.92203f, 22.43059f, "J2307+2225", Pulsar::ATNF));
    ps.add(Pulsar(347.05759f, 55.79334f, "J2308+5547", Pulsar::ATNF));
    ps.add(Pulsar(348.28592f, 42.88696f, "J2313+4253", Pulsar::ATNF));
    ps.add(Pulsar(349.49095f, 21.83001f, "J2317+2149", Pulsar::ATNF));
    ps.add(Pulsar(349.28849f, 14.65867f, "J2317+1439", Pulsar::ATNF));
    ps.add(Pulsar(350.48006f, 60.40853f, "J2321+6024", Pulsar::ATNF));
    ps.add(Pulsar(350.59315f, 20.95082f, "J2322+2057", Pulsar::ATNF));
    ps.add(Pulsar(351.11242f, -60.90169f, "J2324-6054", Pulsar::ATNF));
    ps.add(Pulsar(351.30553f, 63.28119f, "J2325+6316", Pulsar::ATNF));
    ps.add(Pulsar(351.74457f, 61.22680f, "J2326+6113", Pulsar::ATNF));
    ps.add(Pulsar(352.61202f, -20.09156f, "J2330-2005", Pulsar::ATNF));
    ps.add(Pulsar(354.27408f, 61.85047f, "J2337+6151", Pulsar::ATNF));
    ps.add(Pulsar(356.71023f, -6.16653f, "J2346-0609", Pulsar::ATNF));
    ps.add(Pulsar(358.51968f, 61.92966f, "J2354+6155", Pulsar::ATNF));
    }

bool Simulation::loadPulsarsEAH(void) {
    // Assume failure.
    bool ret_val = false;

    std::string data_file_name = getEAHPulsarDataFileName();
    if(data_file_name.size() != 0) {
        std::cout << "Simulation::loadPulsarsEAH() - filename is '"
                  << data_file_name
                  << "'"
                  << std::endl;

        // Load the data file.
        ifstream data_file(data_file_name.c_str());
        if(data_file.good() == true) {
            std::string line;
            // Toss the first line ( header ).
            getline(data_file, line);
            do {
                line.clear();
                getline(data_file, line);
                if(line.size() != 0) {

                    std::vector<std::string> tokens(parseLine(line));

                    if(tokens.size() == LINE_TOKENS) {
                        // Get the pulsar's name.
                        std::string pulsar_name(tokens.at(NAME));

                        // Get the pulsar's right ascension.
                        std::string right_ascension(tokens.at(RIGHT_ASCENSION));
                        float pulsar_ra = 0.0f;
                        if(!str2ra(right_ascension, &pulsar_ra)) {
                            ErrorHandler::record("Simulation::loadPulsarsEAH() - bad right ascension value", ErrorHandler::WARN);
                            }

                        // Get the pulsar's declination.
                        std::string declination(tokens.at(DECLINATION));
                        float pulsar_dec = 0.0f;
                        if(!str2dec(declination, &pulsar_dec)){
                            ErrorHandler::record("Simulation::loadPulsarsEAH() - bad declination value", ErrorHandler::WARN);
                            }

                        // Get the pulsar's period.
                        float pp;
                        std::stringstream period(tokens.at(PERIOD));
                        period >> pp;

                        // Get the pulsar's dispersion modulus.
                        float dm;
                        std::stringstream dispersion_modulus(tokens.at(DISPERSION_MODULUS));
                        dispersion_modulus >> dm;

                        // Get the pulsar's distance.
                        float ds;
                        std::stringstream distance(tokens.at(DISTANCE));
                        distance >> ds;

                        std::string discoverers(tokens.at(VOLUNTEERS));

                        std::string source(tokens.at(SEARCH_SOURCE));
                        Pulsar::pulsar_source src;

                        if(source == "ATNF") {
                            src = Pulsar::ATNF;
                            }
                        else {
                            if(source == "PALFA") {
                                src = Pulsar::PALFA;
                                }
                            else {
                                if(source == "PMPS") {
                                    src = Pulsar::PMPS;
                                    }
                                else {
                                    src = Pulsar::DUNNO;
                                    }
                                }
                            }

                        ps_EAH.add(PulsarEAH(pulsar_ra,
                                             pulsar_dec,
                                             pulsar_name,
                                             src,
                                             pp,
                                             dm,
                                             ds,
                                             discoverers));
                        }
                    else {
                        ErrorHandler::record("Simulation::loadPulsarsEAH() - wrong number of tokens on line",
                                             ErrorHandler::WARN);
                        }
                    }
                }while(line.size() != 0);
            data_file.close();
            }
        else {
            // Failed to load data file.
            ErrorHandler::record("Simulation::loadPulsarsEAH() - could not open pulsar data file",
                                 ErrorHandler::WARN);
            }
        }

    return ret_val;
    }

void Simulation::loadSupernovae(void) {
    sn.add(Supernova(266.433333f , -29.00000f ));
    sn.add(Supernova(266.562500f , -28.63333f ));
    sn.add(Supernova(266.837500f , -28.15000f ));
    sn.add(Supernova(267.125000f , -28.15000f ));
    sn.add(Supernova(267.412500f , -27.76667f ));
    sn.add(Supernova(267.187500f , -27.16667f ));
    sn.add(Supernova(268.858333f , -25.83333f ));
    sn.add(Supernova(268.229167f , -25.46667f ));
    sn.add(Supernova(272.229167f , -27.05000f ));
    sn.add(Supernova(262.675000f , -21.48333f ));
    sn.add(Supernova(263.354167f , -21.56667f ));
    sn.add(Supernova(271.875000f , -25.75000f ));
    sn.add(Supernova(270.541667f , -24.90000f ));
    sn.add(Supernova(266.833333f , -22.26667f ));
    sn.add(Supernova(268.729167f , -23.08333f ));
    sn.add(Supernova(270.125000f , -23.43333f ));
    sn.add(Supernova(266.291667f , -21.36667f ));
    sn.add(Supernova(270.458333f , -22.90000f ));
    sn.add(Supernova(274.354167f , -24.06667f ));
    sn.add(Supernova(276.041667f , -23.80000f ));
    sn.add(Supernova(271.375000f , -21.43333f ));
    sn.add(Supernova(271.283333f , -20.23333f ));
    sn.add(Supernova(272.862500f , -19.41667f ));
    sn.add(Supernova(272.695833f , -19.08333f ));
    sn.add(Supernova(273.045833f , -18.61667f ));
    sn.add(Supernova(274.833333f , -18.00000f ));
    sn.add(Supernova(273.558333f , -17.20000f ));
    sn.add(Supernova(276.000000f , -16.56667f ));
    sn.add(Supernova(274.716667f , -15.03333f ));
    sn.add(Supernova(277.208333f , -16.18333f ));
    sn.add(Supernova(275.233333f , -14.33333f ));
    sn.add(Supernova(276.333333f , -14.76667f ));
    sn.add(Supernova(277.729167f , -14.86667f ));
    sn.add(Supernova(278.208333f , -14.65000f ));
    sn.add(Supernova(275.991667f , -12.38333f ));
    sn.add(Supernova(277.458333f , -12.96667f ));
    sn.add(Supernova(277.029167f , -11.58333f ));
    sn.add(Supernova(278.387500f , -10.58333f ));
    sn.add(Supernova(278.187500f , -10.13333f ));
    sn.add(Supernova(278.312500f , -9.21667f ));
    sn.add(Supernova(278.687500f , -8.80000f ));
    sn.add(Supernova(278.262500f , -8.21667f ));
    sn.add(Supernova(279.679167f , -7.53333f ));
    sn.add(Supernova(278.541667f , -7.08333f ));
    sn.add(Supernova(280.329167f , -4.93333f ));
    sn.add(Supernova(279.958333f , -4.40000f ));
    sn.add(Supernova(280.979167f , -3.88333f ));
    sn.add(Supernova(279.750000f , -2.91667f ));
    sn.add(Supernova(281.216667f , -2.95000f ));
    sn.add(Supernova(281.604167f , -2.98333f ));
    sn.add(Supernova(283.604167f , -2.90000f ));
    sn.add(Supernova(281.000000f , -1.53333f ));
    sn.add(Supernova(282.791667f , -1.51667f ));
    sn.add(Supernova(282.354167f , 0.00000f ));
    sn.add(Supernova(286.500000f , -3.00000f ));
    sn.add(Supernova(283.291667f , -1.13333f ));
    sn.add(Supernova(282.854167f , 0.00000f ));
    sn.add(Supernova(283.458333f , 0.00000f ));
    sn.add(Supernova(283.200000f , 0.00000f ));
    sn.add(Supernova(284.000000f , 1.36667f ));
    sn.add(Supernova(285.145833f , 2.93333f ));
    sn.add(Supernova(282.204167f , 4.43333f ));
    sn.add(Supernova(286.033333f , 5.46667f ));
    sn.add(Supernova(288.083333f , 4.91667f ));
    sn.add(Supernova(286.791667f , 6.51667f ));
    sn.add(Supernova(286.891667f , 7.13333f ));
    sn.add(Supernova(286.833333f , 9.08333f ));
    sn.add(Supernova(287.783333f , 9.10000f ));
    sn.add(Supernova(286.458333f , 10.50000f ));
    sn.add(Supernova(289.104167f , 11.15000f ));
    sn.add(Supernova(289.541667f , 12.15000f ));
    sn.add(Supernova(290.958333f , 14.10000f ));
    sn.add(Supernova(294.708333f , 17.23333f ));
    sn.add(Supernova(292.629167f , 18.86667f ));
    sn.add(Supernova(293.333333f , 18.93333f ));
    sn.add(Supernova(293.000000f , 19.83333f ));
    sn.add(Supernova(290.333333f , 21.73333f ));
    sn.add(Supernova(293.745833f , 21.95000f ));
    sn.add(Supernova(295.637500f , 23.58333f ));
    sn.add(Supernova(294.729167f , 24.31667f ));
    sn.add(Supernova(296.966667f , 27.75000f ));
    sn.add(Supernova(298.666667f , 28.58333f ));
    sn.add(Supernova(293.250000f , 31.16667f ));
    sn.add(Supernova(298.041667f , 29.43333f ));
    sn.add(Supernova(298.633333f , 31.48333f ));
    sn.add(Supernova(302.166667f , 30.61667f ));
    sn.add(Supernova(298.333333f , 32.91667f ));
    sn.add(Supernova(300.666667f , 32.71667f ));
    sn.add(Supernova(303.562500f , 36.20000f ));
    sn.add(Supernova(312.750000f , 30.66667f ));
    sn.add(Supernova(304.008333f , 37.20000f ));
    sn.add(Supernova(305.583333f , 38.71667f ));
    sn.add(Supernova(305.208333f , 40.43333f ));
    sn.add(Supernova(304.750000f , 45.50000f ));
    sn.add(Supernova(313.333333f , 43.45000f ));
    sn.add(Supernova(312.625000f , 44.88333f ));
    sn.add(Supernova(312.666667f , 45.36667f ));
    sn.add(Supernova(314.666667f , 44.88333f ));
    sn.add(Supernova(311.250000f , 50.58333f ));
    sn.add(Supernova(313.104167f , 55.35000f ));
    sn.add(Supernova(322.333333f , 50.83333f ));
    sn.add(Supernova(321.208333f , 51.88333f ));
    sn.add(Supernova(336.875000f , 60.83333f ));
    sn.add(Supernova(345.395833f , 58.88333f ));
    sn.add(Supernova(350.858333f , 58.80000f ));
    sn.add(Supernova(354.250000f , 61.91667f ));
    sn.add(Supernova(358.416667f , 63.25000f ));
    sn.add(Supernova(359.791667f , 62.43333f ));
    sn.add(Supernova(  1.666667f , 72.75000f ));
    sn.add(Supernova(  6.325000f , 64.15000f ));
    sn.add(Supernova( 20.500000f , 64.25000f ));
    sn.add(Supernova( 22.083333f , 63.16667f ));
    sn.add(Supernova( 31.420833f , 64.81667f ));
    sn.add(Supernova( 34.416667f , 62.75000f ));
    sn.add(Supernova( 74.666667f , 51.83333f ));
    sn.add(Supernova( 75.250000f , 46.66667f ));
    sn.add(Supernova( 81.625000f , 42.93333f ));
    sn.add(Supernova( 79.750000f , 41.91667f ));
    sn.add(Supernova( 88.416667f , 31.08333f ));
    sn.add(Supernova( 84.750000f , 27.83333f ));
    sn.add(Supernova( 92.041667f , 29.00000f ));
    sn.add(Supernova( 83.629167f , 22.01667f ));
    sn.add(Supernova( 94.250000f , 22.56667f ));
    sn.add(Supernova( 92.333333f , 17.33333f ));
    sn.add(Supernova( 99.750000f , 6.50000f ));
    sn.add(Supernova(102.166667f , 6.43333f ));
    sn.add(Supernova(125.541667f , -43.00000f ));
    sn.add(Supernova(136.083333f , -38.70000f ));
    sn.add(Supernova(128.500000f , -45.83333f ));
    sn.add(Supernova(133.000000f , -46.33333f ));
    sn.add(Supernova(136.708333f , -52.11667f ));
    sn.add(Supernova(149.416667f , -53.25000f ));
    sn.add(Supernova(154.562500f , -59.00000f ));
    sn.add(Supernova(158.916667f , -59.70000f ));
    sn.add(Supernova(165.312500f , -60.30000f ));
    sn.add(Supernova(165.770833f , -60.93333f ));
    sn.add(Supernova(167.975000f , -60.63333f ));
    sn.add(Supernova(171.150000f , -59.26667f ));
    sn.add(Supernova(169.833333f , -61.46667f ));
    sn.add(Supernova(173.750000f , -60.90000f ));
    sn.add(Supernova(174.041667f , -61.63333f ));
    sn.add(Supernova(177.791667f , -62.56667f ));
    sn.add(Supernova(182.416667f , -52.41667f ));
    sn.add(Supernova(179.625000f , -62.58333f ));
    sn.add(Supernova(183.166667f , -62.86667f ));
    sn.add(Supernova(183.420833f , -62.61667f ));
    sn.add(Supernova(183.804167f , -65.50000f ));
    sn.add(Supernova(185.437500f , -63.15000f ));
    sn.add(Supernova(189.479167f , -63.81667f ));
    sn.add(Supernova(191.479167f , -62.13333f ));
    sn.add(Supernova(196.495833f , -62.70000f ));
    sn.add(Supernova(204.404167f , -63.06667f ));
    sn.add(Supernova(205.625000f , -62.38333f ));
    sn.add(Supernova(206.629167f , -62.90000f ));
    sn.add(Supernova(207.625000f , -62.08333f ));
    sn.add(Supernova(209.500000f , -62.15000f ));
    sn.add(Supernova(210.000000f , -62.28333f ));
    sn.add(Supernova(211.408333f , -61.96667f ));
    sn.add(Supernova(213.250000f , -61.73333f ));
    sn.add(Supernova(215.250000f , -64.20000f ));
    sn.add(Supernova(220.750000f , -62.50000f ));
    sn.add(Supernova(218.979167f , -60.60000f ));
    sn.add(Supernova(219.604167f , -60.18333f ));
    sn.add(Supernova(220.375000f , -60.00000f ));
    sn.add(Supernova(222.416667f , -59.76667f ));
    sn.add(Supernova(223.708333f , -59.06667f ));
    sn.add(Supernova(224.625000f , -58.48333f ));
    sn.add(Supernova(228.625000f , -59.13333f ));
    sn.add(Supernova(229.458333f , -59.26667f ));
    sn.add(Supernova(230.937500f , -58.21667f ));
    sn.add(Supernova(230.166667f , -57.56667f ));
    sn.add(Supernova(230.845833f , -57.10000f ));
    sn.add(Supernova(232.175000f , -56.35000f ));
    sn.add(Supernova(238.250000f , -56.16667f ));
    sn.add(Supernova(238.604167f , -55.15000f ));
    sn.add(Supernova(237.083333f , -53.81667f ));
    sn.add(Supernova(236.700000f , -53.33333f ));
    sn.add(Supernova(225.708333f , -41.93333f ));
    sn.add(Supernova(238.875000f , -53.28333f ));
    sn.add(Supernova(240.333333f , -52.30000f ));
    sn.add(Supernova(227.500000f , -40.00000f ));
    sn.add(Supernova(240.275000f , -51.56667f ));
    sn.add(Supernova(243.320833f , -50.88333f ));
    sn.add(Supernova(244.387500f , -51.03333f ));
    sn.add(Supernova(243.833333f , -50.70000f ));
    sn.add(Supernova(246.937500f , -48.78333f ));
    sn.add(Supernova(248.045833f , -47.31667f ));
    sn.add(Supernova(248.987500f , -47.60000f ));
    sn.add(Supernova(249.866667f , -47.85000f ));
    sn.add(Supernova(248.162500f , -46.60000f ));
    sn.add(Supernova(249.754167f , -46.98333f ));
    sn.add(Supernova(249.495833f , -46.40000f ));
    sn.add(Supernova(250.250000f , -46.56667f ));
    sn.add(Supernova(250.287500f , -46.31667f ));
    sn.add(Supernova(251.629167f , -44.65000f ));
    sn.add(Supernova(251.920833f , -44.56667f ));
    sn.add(Supernova(251.895833f , -43.78333f ));
    sn.add(Supernova(253.754167f , -44.01667f ));
    sn.add(Supernova(253.708333f , -43.88333f ));
    sn.add(Supernova(252.679167f , -43.06667f ));
    sn.add(Supernova(261.250000f , -46.50000f ));
    sn.add(Supernova(257.000000f , -44.26667f ));
    sn.add(Supernova(255.104167f , -43.23333f ));
    sn.add(Supernova(255.962500f , -41.70000f ));
    sn.add(Supernova(256.833333f , -40.88333f ));
    sn.add(Supernova(257.579167f , -40.18333f ));
    sn.add(Supernova(258.458333f , -39.75000f ));
    sn.add(Supernova(258.858333f , -38.46667f ));
    sn.add(Supernova(258.525000f , -38.53333f ));
    sn.add(Supernova(258.479167f , -38.18333f ));
    sn.add(Supernova(259.312500f , -38.06667f ));
    sn.add(Supernova(259.495833f , -37.43333f ));
    sn.add(Supernova(261.958333f , -38.53333f ));
    sn.add(Supernova(260.612500f , -36.18333f ));
    sn.add(Supernova(260.250000f , -35.45000f ));
    sn.add(Supernova(262.216667f , -36.26667f ));
    sn.add(Supernova(261.916667f , -35.11667f ));
    sn.add(Supernova(264.729167f , -35.18333f ));
    sn.add(Supernova(262.616667f , -33.76667f ));
    sn.add(Supernova(264.000000f , -33.70000f ));
    sn.add(Supernova(263.816667f , -32.63333f ));
    sn.add(Supernova(266.470833f , -33.71667f ));
    sn.add(Supernova(259.750000f , -29.66667f ));
    sn.add(Supernova(264.483333f , -32.26667f ));
    sn.add(Supernova(265.645833f , -32.86667f ));
    sn.add(Supernova(265.120833f , -30.96667f ));
    sn.add(Supernova(264.645833f , -30.73333f ));
    sn.add(Supernova(261.500000f , -28.60000f ));
    sn.add(Supernova(266.708333f , -30.26667f ));
    sn.add(Supernova(266.375000f , -29.95000f ));
    sn.add(Supernova(264.900000f , -29.18333f ));
    }

void Simulation::cycle(Simulation::content ct) {
    // Which scene element to toggle?
    switch(ct) {
        case Simulation::AXES:
         // We don't cycle the axes ... yet
            break;
        case Simulation::CONSTELLATIONS:
            cs.cycleActivation();
            break;
        case Simulation::EARTH:
            // We don't cycle the Earth ... yet
            break;
        case Simulation::EARTH_GRID:
            e_sphere.cycleActivation();
            break;
        case Simulation::SUN:
            // We don't cycle the Sun ... yet
            break;
        case Simulation::SKY_GRID:
            c_sphere.cycleActivation();
            break;
        case Simulation::PULSARS:
            ps.cycleActivation();
            break;
        case Simulation::SUPERNOVAE:
            sn.cycleActivation();
            break;
        case Simulation::HUDOVER:
            // TODO : still buggy .....
            // overlay.cycleActivation();
            break;
        case Simulation::AUTOPILOT:
            if(pilot.isActive() == true) {
                // When returning to user control ...
                flyboy.manouevre(Craft::STOP_ROTATION);
                flyboy.manouevre(Craft::STOP_TRANSLATION);
                flyboy.setViewState(pilot.viewState());
                pilot.inactivate();

                // Eliminate any remaining informative
                // text from the HUD.
                north_panel.erase();
                }
            else {
                // When enabling autopilot ....
                // flyboy.manouevre(Craft::STOP_ROTATION);
                // flyboy.manouevre(Craft::STOP_TRANSLATION);
                /// TODO Choice between Traversable objects ....
                CameraState current = flyboy.viewState();

                // When obtained from our craft the focus vector is unit vector
                // and also craft based. Thus prefer a distant focus point to
                // reduce possible high initial slew rate.
                //current.setFocus(current.position() + SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS*current.focus());
                pilot.activate(ps_EAH, current);
                }
            break;
        case Simulation::TARGET_RETICLE:
            target.cycleActivation();
            break;
        default:
            // Ought not get here !!
            std::string msg = "Simulation::cycle() - bad switch case reached (default)";
            ErrorHandler::record(msg, ErrorHandler::FATAL);
            break;
        }
    }

void Simulation::loadImageToPanel(HUDImage* hip, HUDFlowLayout* hfl,
                                  std::string resource_name,
                                  GLuint margin_width, GLuint margin_height) {
    // Put an image into the content.
    hip = new HUDImage(resource_name, margin_width, margin_height);
    if(hip == NULL) {
        std::string msg = "Simulation::LoadImageToPanel() - failed creation of HUDImage instance on heap : ";
        msg += resource_name;
        ErrorHandler::record(msg, ErrorHandler::FATAL);
        }

    // Put the content into the panel.
    hfl->addContent(hip);
    }

std::string Simulation::getEAHPulsarDataFileName(void) {
    std::string ret_val;

    // Read the soft link file to get the latest
    // catalog filename.
    std::string soft_link_file(EAH_PULSAR_FILE + "." + EAH_PULSAR_FILE_EXT);
    ifstream soft_link(soft_link_file.c_str());

    std::string soft_link_contents;
    if(soft_link.good() == true) {
        soft_link >> soft_link_contents;
        std::string::size_type data_file_name_idx_start = soft_link_contents.find(EAH_PULSAR_FILE.c_str(),
                                                                                  0, EAH_PULSAR_FILE.size());
        if(data_file_name_idx_start != string::npos) {
            std::string::size_type data_file_name_idx_end = soft_link_contents.find(".", data_file_name_idx_start);
            if(data_file_name_idx_start != string::npos) {
                std::string data_file(soft_link_contents,
                                      data_file_name_idx_start,
                                      data_file_name_idx_end - data_file_name_idx_start);
                std::cout << "Simulation::getEAHPulsarDataFileName() - filename ( stub ) is '"
                          << data_file
                          << "'"
                          << std::endl;
                data_file += ".";
                data_file += EAH_PULSAR_FILE_EXT;
                return data_file;
                }
            else {
                ErrorHandler::record("Simulation::getEAHPulsarDataFileName() - could not parse(2) pulsar soft link file",
                                 ErrorHandler::WARN);
                }
            }
        else {
            ErrorHandler::record("Simulation::getEAHPulsarDataFileName() - could not parse(1) pulsar soft link file",
                                 ErrorHandler::WARN);
            }
        soft_link.close();
        }
    else {
        ErrorHandler::record("Simulation::getEAHPulsarDataFileName() - could not open pulsar soft link file",
                             ErrorHandler::WARN);
        }

    return ret_val;
    }

bool Simulation::str2ra(std::string right_ascension, float* result) const {
    static const char DELIM = ':';
    static const unsigned int RA_TOKENS = 3;

    // Assume failure.
    bool ret_val = false;

    // Split the right ascension string into tokens.
    std::vector<std::string> tokens = tokenise(right_ascension, DELIM);

    // Assuming we have the correct number of tokens.
    if(tokens.size() == RA_TOKENS) {
        /// TODO bounds checking on values?
        float hours = 0.0f;
        std::stringstream ra_hours(tokens.at(0));
        ra_hours >> hours;

        float minutes = 0.0f;
        std::stringstream ra_minutes(tokens.at(1));
        ra_minutes >> minutes;

        float seconds = 0.0f;
        std::stringstream ra_seconds(tokens.at(2));
        ra_seconds >> seconds;

        *result = (hours +
                  (minutes +
                   seconds/SolarSystemGlobals::SECONDS_PER_MINUTE)/
                   SolarSystemGlobals::MINUTES_PER_HOUR)*
                   SolarSystemGlobals::DEGREES_PER_HOUR;

        ret_val = true;
        }

    return ret_val;
    }

bool Simulation::str2dec(std::string declination, float* result) const {
    static const char DELIM = ':';
    static const unsigned int DEC_TOKENS = 3;

    // Assume failure.
    bool ret_val = false;

    // Split the declination string into tokens.
    std::vector<std::string> tokens = tokenise(declination, DELIM);

    // Assuming we have the correct number of tokens.
    if(tokens.size() == DEC_TOKENS) {
        /// TODO bounds checking on values?
        float degrees = 0.0f;
        std::stringstream dec_degrees(tokens.at(0));
        dec_degrees >> degrees;

        float signum = 1;
        if(degrees < 0.0f) {
            signum = -1;
            degrees = signum*degrees;
            }

        float minutes = 0.0f;
        std::stringstream dec_minutes(tokens.at(1));
        dec_minutes >> minutes;

        float seconds = 0.0f;
        std::stringstream dec_seconds(tokens.at(2));
        dec_seconds >> seconds;

        *result = signum*(degrees + (minutes + seconds/SolarSystemGlobals::SECONDS_PER_MINUTE)/SolarSystemGlobals::MINUTES_PER_DEGREE);
        ret_val = true;
        }

    return ret_val;
    }

std::vector<std::string> Simulation::tokenise(std::string input, char delimiter) const {
    // The array of tokens to return.
    std::vector<std::string> ret_val;

    // Assuming there is any ( remaining ) input string to examine.
    while(!input.empty()) {
        // Find the next instance of the delimiter.
        size_t found = input.find(delimiter);

        // Did we find a delimiter?
        if(found != std::string::npos) {
            // Yes then add this token to the array.
            ret_val.push_back(input.substr(0, found));
            // Remove the found token plus the delimiter
            // from the input string.
            input = input.substr(found + 1);
            }
        else {
            // No delimiter found with end of input string reached,
            // so store ( the remainder of ) the string as a token.
            ret_val.push_back(input);
            // then clear the input string.
            input.clear();
            }
        }

    // Return the array of tokens.
    return ret_val;
    }

std::vector<std::string> Simulation::parseLine(std::string input) const {
    static const char LINE_DELIMITER = ' ';
    std::vector<std::string> ret_val;

    std::vector<std::string> splits(tokenise(input, LINE_DELIMITER));

    unsigned int token;
    for(token = NAME; token <= DISTANCE; ++token) {
        ret_val.push_back(splits.at(token));
        }

    std::string discs;
    for(token = VOLUNTEERS; token < splits.size() - 1; ++token) {
        std::string temp = splits.at(token);
        if(token == VOLUNTEERS) {
            discs += temp.substr(1);
            discs += LINE_DELIMITER;
            }
        else {
            if(token == (splits.size() - 2)) {
                discs += temp.substr(0, temp.size() - 1);
                }
            else {
                discs += temp;
                discs += LINE_DELIMITER;
                }
            }
        }

    ret_val.push_back(discs);

    ++token;
    ret_val.push_back(splits.at(splits.size() - 1));

    return ret_val;
    }

void Simulation::clearTextLines(void) {
    for(std::vector<HUDTextLine*>::iterator line = text_lines.begin();
        line != text_lines.end();
        ++line) {
        if(*line != NULL) {
            (*line)->inactivate();
            }
        }
    text_lines.clear();
    }

void Simulation::clearImages(void) {
    for(std::vector<HUDImage*>::iterator image = lookout_images.begin();
        image != lookout_images.end();
        ++image) {
        if(*image != NULL) {
            (*image)->inactivate();
            }
        }
    lookout_images.clear();
    }
