/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
 *   hewsmike@iinet.net.au                                                 *
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

const unsigned int Simulation::COUNT_START(5);
const unsigned int Simulation::COUNT_END(0);

const GLuint Simulation::CONSTELLATIONS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS);
const GLuint Simulation::PULSARS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 25);
const GLuint Simulation::SUPERNOVAE_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 50);

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

Simulation::Simulation(void): autopilotActive(false),
										min60(0),
										hour24(0),
										day366(0),
										earth_hour_angle(0),
										sun_rot_angle(0),
										cs(CONSTELLATIONS_RADIUS),
                              ps(PULSARS_RADIUS),
                              sn(SUPERNOVAE_RADIUS),
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
                              aei_image(NULL),
                              aps_image(NULL),
                              boinc_image(NULL),
                              geo_image(NULL),
                              ligo_image(NULL),
                              opencl_image(NULL),
                              virgo_image(NULL),
                              wyp_image(NULL),
                              version_text(NULL),
                              overlay(NULL),
                              north_panel(&overlay),
                              south_panel(&overlay),
                              east_panel(&overlay),
                              west_panel(&overlay) {
   c_sphere.setLine(GridGlobe::MAIN, SKYGRID_MAIN_WIDTH, SKYGRID_MAIN_RED, SKYGRID_MAIN_GREEN, SKYGRID_MAIN_BLUE);
   c_sphere.setLine(GridGlobe::EQUATOR, SKYGRID_CELESTIAL_EQUATOR_WIDTH, SKYGRID_CELESTIAL_EQUATOR_RED, SKYGRID_CELESTIAL_EQUATOR_GREEN, SKYGRID_CELESTIAL_EQUATOR_BLUE);
   c_sphere.setLine(GridGlobe::PRIME_MERIDIAN, SKYGRID_PRIME_MERIDIAN_WIDTH, SKYGRID_PRIME_MERIDIAN_RED, SKYGRID_PRIME_MERIDIAN_GREEN, SKYGRID_PRIME_MERIDIAN_BLUE);
   e_sphere.setLine(GridGlobe::MAIN, EARTHGRID_MAIN_WIDTH, EARTHGRID_MAIN_RED, EARTHGRID_MAIN_GREEN, EARTHGRID_MAIN_BLUE);
   e_sphere.setLine(GridGlobe::EQUATOR, EARTHGRID_CELESTIAL_EQUATOR_WIDTH, EARTHGRID_CELESTIAL_EQUATOR_RED, EARTHGRID_CELESTIAL_EQUATOR_GREEN, EARTHGRID_CELESTIAL_EQUATOR_BLUE);
   e_sphere.setLine(GridGlobe::PRIME_MERIDIAN, EARTHGRID_PRIME_MERIDIAN_WIDTH, EARTHGRID_PRIME_MERIDIAN_RED, EARTHGRID_PRIME_MERIDIAN_GREEN, EARTHGRID_PRIME_MERIDIAN_BLUE);
   }

Simulation::~Simulation() {
   }

void Simulation::step(void) {
	/// TODO - demo code only, needs proper ephemeris model.
   // Evolve the craft's mechanics with
   // knowledge of the day within the year.
   flyboy.step(day366);

   ++min60;
   if(min60 > 59) {
      min60 = 0;
      ++hour24;
      if(hour24 > 23) {
         hour24 = 0;
         }
      }
   earth_hour_angle = (hour24 + min60/60.0f)* 15.0f;

   day366 += 0.05f;
   if(day366 >= SunOrbit::DAYS_PER_YEAR) {
      day366 = 0.0f;
      }
   sun_pos = SunOrbit::getPosition(day366);

   sun_rot_angle = SunOrbit::getRotation(day366);
   }

void Simulation::moveRequest(Craft::movements mv) {
	if(!autopilotActive) {
		flyboy.manouevre(mv);
		}
   }

void Simulation::resize(GLuint width, GLuint height) {
   screen_width = width;
   screen_height = height;
   std::cout << "Simulation::resize() : resize screen to "
             << "width = " << screen_width
             << "\theight = " << screen_height << std::endl;

   // Now tell the HUD of such settings.
   // TODO - if resize denied then inactivate HUD ?? Complex ....
   overlay.requestResize(width, height);
   }

void Simulation::setFont(content element, OGLFT_ft* font) {
   fonts[element] = font;
   }

CameraState Simulation::viewPoint(void) const {
	CameraState ret_val;
	if(autopilotActive) {
		// The autopilot is flying the craft.

		ret_val = flyboy.getViewState();
		}
	else {
		// The user is flying the craft.
		ret_val = flyboy.getViewState();
		}

	return ret_val;
	}

void Simulation::prepare(SolarSystemGlobals::render_quality rq) {
   // TODO - write cases per render_quality ... presently ignored
   // at this level.

   // Activate 3D scene objects, while nominating any fonts
   // prior to activation of their respective objects..
   cs.setFont(fonts[CONSTELLATIONS]);
   cs.activate();
   ps.activate();
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
   north_panel.emptyContainer();
   south_panel.emptyContainer();
   east_panel.emptyContainer();
   west_panel.emptyContainer();

   // Set panel justifications.
   north_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
   // north_panel.setSecondaryJustification(HUDFlowLayout::END);
   south_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
   // south_panel.setSecondaryJustification(HUDFlowLayout::END);
   east_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
   // east_panel.setSecondaryJustification(HUDFlowLayout::END);
   west_panel.setPrimaryJustification(HUDFlowLayout::CENTRE);
   // west_panel.setSecondaryJustification(HUDFlowLayout::CENTRE);

   // Put the panels into the layout.
   overlay.setPanel(HUDBorderLayout::NORTH, &north_panel);
   overlay.setPanel(HUDBorderLayout::SOUTH, &south_panel);
   overlay.setPanel(HUDBorderLayout::EAST, &east_panel);
   overlay.setPanel(HUDBorderLayout::WEST, &west_panel);

   // Create content and include into panels.
   LoadImageToPanel(wyp_image, &north_panel, "wypTGA", 5, 5);
   LoadImageToPanel(aps_image, &north_panel, "apsTGA", 5, 5);
   LoadImageToPanel(aei_image, &north_panel, "aeiTGA", 5, 5);
   LoadImageToPanel(opencl_image, &north_panel, "openclTGA", 5, 5);

   LoadImageToPanel(boinc_image, &south_panel, "boincTGA", 5, 5);
   version_text = new HUDTextLineScroll(50, overlay.getFont(), 35, 10, HUDTextLineScroll::LEFT, 10);
   if(version_text == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDTextLineScroll instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }
   version_text->setText("    https://github.com/hewsmike/Einstein-At-Home-Screensaver/tree/solarsystem");

   // Put the content into the panel.
   south_panel.addContent(version_text);

   LoadImageToPanel(ligo_image, &south_panel, "ligoTGA", 5, 5);
   LoadImageToPanel(geo_image, &south_panel, "geoTGA", 5, 5);
   // LoadImageToPanel(virgo_image, &south_panel, "virgoTGA", 5, 5);

   overlay.activate();
   }

void Simulation::release(void) {
   cs.inactivate();
   ps.inactivate();
   sn.inactivate();
   c_sphere.inactivate();
   earth.inactivate();
   e_sphere.inactivate();
   sun.inactivate();

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
   }

void Simulation::render(void) {
	if(autopilotActive) {
		flyboy.setViewState(autopilot);
		}
	else {
		autopilot = flyboy.getViewState();
		}

   // Invoke the draw method for each scene element.
   cs.draw();
   ps.draw();
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
      	autopilotActive = !autopilotActive;
      	if(!autopilotActive) {
      		// When returning to user control ...
      		flyboy.manouevre(Craft::STOP_ROTATION);
      		flyboy.manouevre(Craft::STOP_TRANSLATION);
      		flyboy.setViewState(autopilot);
				}
      	break;
      default:
         // Ought not get here !!
         std::string msg = "Simulation::cycle() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void Simulation::LoadImageToPanel(HUDImage* hip, HUDFlowLayout* hfl, std::string resource_name,
											 GLuint margin_x, GLuint margin_y) {
	// Put an image into the content.
	hip = new HUDImage(resource_name, margin_x, margin_y);
	if(hip == NULL) {
	  std::string msg = "Simulation::LoadImageToPanel() - failed creation of HUDImage instance on heap : ";
	  msg += resource_name;
	  ErrorHandler::record(msg, ErrorHandler::FATAL);
	  }

	// Put the content into the panel.
	hfl->addContent(hip);
	}
