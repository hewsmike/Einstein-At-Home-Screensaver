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
const std::string Simulation::EARTH_IMAGE_FILE("earthmap.bmp");
const GLuint Simulation::EARTH_STACKS(37);
const GLuint Simulation::EARTH_SLICES(72);
const GLfloat Simulation::EARTH_TEXTURE_OFFSET(+0.5f);

const std::string Simulation::SUN_NAME("Sun");
const std::string Simulation::SUN_IMAGE_FILE("sunmap.bmp");
const GLuint Simulation::SUN_STACKS(37);
const GLuint Simulation::SUN_SLICES(72);
const GLfloat Simulation::SUN_TEXTURE_OFFSET(0.0f);

const GLfloat Simulation::AT_INFINITY(0.0f);

const unsigned int Simulation::COUNT_START(5);
const unsigned int Simulation::COUNT_END(0);

const GLuint Simulation::CONSTELLATIONS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS);
const GLuint Simulation::PULSARS_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 25);
const GLuint Simulation::SUPERNOVAE_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS - 50);
const GLuint Simulation::GRID_RADIUS(SolarSystemGlobals::CELESTIAL_SPHERE_RADIUS + 50);
const GLuint Simulation::GRID_STACKS(19);
const GLuint Simulation::GRID_SLICES(24);

const GLint Simulation::HUD_LEFT_CLIP(0);
const GLint Simulation::HUD_BOTTOM_CLIP(0);
const GLint Simulation::HUD_NEAR_CLIP(-1);
const GLint Simulation::HUD_FAR_CLIP(+1);

Simulation::Simulation(void): cs(CONSTELLATIONS_RADIUS),
                              ps(PULSARS_RADIUS),
                              sn(SUPERNOVAE_RADIUS),
                              gg(GRID_RADIUS, GRID_SLICES, GRID_STACKS),
                              earth(EARTH_NAME,
                                    EARTH_IMAGE_FILE,
                                    SolarSystemGlobals::EARTH_RADIUS,
                                    EARTH_STACKS,
                                    EARTH_SLICES,
                                    EARTH_TEXTURE_OFFSET),
                              sun(SUN_NAME,
                                  SUN_IMAGE_FILE,
                                  SolarSystemGlobals::SUN_RADIUS,
                                  SUN_STACKS,
                                  SUN_SLICES,
                                  SUN_TEXTURE_OFFSET),
                              geo_image(NULL),
                              ligo_image(NULL),
                              aps_image(NULL),
                              wyp_image(NULL),
                              lsc_image(NULL),
                              welcome_text(NULL),
                              overlay(NULL),
                              north_panel(&overlay),
                              south_panel(&overlay),
                              east_panel(&overlay),
                              west_panel(&overlay) {
   }

Simulation::~Simulation() {
   }

Vector3D Simulation::getViewPosition(void) const {
   return flyboy.get_platform().position();
   }

Vector3D Simulation::getViewDirection(void) const {
   return flyboy.get_platform().look();
   }

Vector3D Simulation::getViewUp(void) const {
   return flyboy.get_platform().up();
   }

void Simulation::step(void) {
   flyboy.step();
   //if(count_down == COUNT_END) {
      // trigger something
      // clock.getTime(&min60, &hour24, &day366);
      // min60+;
      if(min60 > 59) {
         min60 = 0;
         hour24++;
         if(hour24 > 23) {
            hour24 = 0;
            }
         }
      earth_hour_angle = (hour24 + min60/60.0f)* 15.0f;
      // reset the counter
      count_down = COUNT_START;

      day366 += 0.05;
      if(day366 >= SunOrbit::DAYS_PER_YEAR) {
         day366 = 0;
         }
      sun_pos = SunOrbit::getPosition(day366);
   //   }
   //else {
     // --count_down;
   //   }
   }

void Simulation::moveRequest(SolarSystemGlobals::movements mv) {
   flyboy.manouevre(mv);
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

void Simulation::prepare(SolarSystemGlobals::render_quality rq) {
   // TODO - write cases per render_quality ... presently ignored
   // at this level.

   // Activate 3D scene objects, while nominating any fonts
   // prior to activation of their respective objects..
   cs.setFont(fonts[CONSTELLATIONS]);
   cs.activate();
   ps.activate();
   sn.activate();
   gg.setFont(fonts[GRID]);
   gg.activate();
   earth.activate();
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
   west_panel.setPrimaryJustification(HUDFlowLayout::END);
   // west_panel.setSecondaryJustification(HUDFlowLayout::CENTRE);

   // Put the panels into the layout.
   overlay.setPanel(HUDBorderLayout::NORTH, &north_panel);
   overlay.setPanel(HUDBorderLayout::SOUTH, &south_panel);
   overlay.setPanel(HUDBorderLayout::EAST, &east_panel);
   overlay.setPanel(HUDBorderLayout::WEST, &west_panel);

   // Create content for the panels.

   // Put an image into the content.
   wyp_image = new HUDImage("wyplogo.bmp", 5, 5);
   if(wyp_image == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDImage instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Put the content into the panel.
   north_panel.addContent(wyp_image);

   // Put text into the content.
   welcome_text = new HUDTextLine(40, overlay.getFont(), 15, 15);
   if(welcome_text == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDTextLine instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   welcome_text->setText("Hi! Welcome to Einstein @ Home!");

    // Put the content into the panel.
   north_panel.addContent(welcome_text);

   // Put an image into the content.
   lsc_image = new HUDImage("lsclogo.bmp", 5, 5);
   if(lsc_image == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDImage instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Put the content into the panel.
   north_panel.addContent(lsc_image);

   // Put an image into the content.
   geo_image = new HUDImage("geo_small.bmp", 5, 5);
   if(geo_image == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDImage instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Put the content into the panel.
   south_panel.addContent(geo_image);

   // Put an image into the content.
   aps_image = new HUDImage("aps_header.bmp", 5, 5);
   if(aps_image == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDImage instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Put the content into the panel.
   south_panel.addContent(aps_image);

   // Put an image into the content.
   ligo_image = new HUDImage("ligo_small.bmp", 5, 5);
   if(ligo_image == NULL) {
      std::string msg = "Simulation::prepare() - failed creation of HUDImage instance on heap";
      ErrorHandler::record(msg, ErrorHandler::FATAL);
      }

   // Put the content into the panel.
   south_panel.addContent(ligo_image);

   overlay.activate();
   }

void Simulation::release(void) {
   cs.inactivate();
   ps.inactivate();
   sn.inactivate();
   gg.inactivate();
   earth.inactivate();
   sun.inactivate();

   // Must inactivate the layout first !!
   overlay.inactivate();

   if(geo_image != NULL) {
      delete geo_image;
      }
   if(ligo_image != NULL) {
      delete ligo_image;
      }
   if(aps_image != NULL) {
      delete aps_image;
      }
   if(wyp_image != NULL) {
      delete wyp_image;
      }
   if(lsc_image != NULL) {
      delete lsc_image;
      }
   if(welcome_text != NULL) {
      delete welcome_text;
      }
   }

void Simulation::render(void) {
   // Invoke the draw method for each scene element.
   cs.draw();
   ps.draw();
   sn.draw();
   gg.draw();

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
      case Simulation::SUN:
         // We don't cycle the Sun ... yet
         break;
      case Simulation::GRID:
         gg.cycleActivation();
         break;
      case Simulation::PULSARS:
         ps.cycleActivation();
         break;
      case Simulation::SUPERNOVAE:
         sn.cycleActivation();
         break;
      case Simulation::HUDOVER:
         // We don't cycle the HUD ... yet
         break;
      default:
         // Ought not get here !!
         std::string msg = "Simulation::cycle() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }
