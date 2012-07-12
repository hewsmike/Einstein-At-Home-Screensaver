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

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "framework.h"

#include <iostream>
#include <map>
#include <string>

#include <oglft/OGLFT.h>

#include "AutoPilot.h"
#include "CameraState.h"
#include "Constellations.h"
#include "Craft.h"
#include "ErrorHandler.h"
#include "Globe.h"
#include "GridGlobe.h"
#include "HUDBorderLayout.h"
#include "HUDContent.h"
#include "HUDFlowLayout.h"
#include "HUDImage.h"
#include "HUDTextLine.h"
#include "HUDTextLineScroll.h"
#include "Pulsar.h"
#include "PulsarEAH.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Sphere.h"
#include "Stars.h"
#include "SunOrbit.h"
#include "Supernova.h"
#include "UTC.h"
#include "Vector3D.h"
#include "VectorSP.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Container for scene renderable elements
 *
 *      This class aggregates all the scene elements ( 3D + HUD )
 * that one wants in a given rendering frame. Preparation, rendering
 * and release are done under the Renderable interface.
 *
 * \see AutoPilot
 * \see CameraState
 * \see Constellations
 * \see Craft
 * \see Globe
 * \see GridGlobe
 * \see HUDBorderLayout
 * \see HUDFlowLayout
 * \see HUDImage
 * \see HUDTextLineScroll
 * \see Pulsars
 * \see PulsarsEAH
 * \see Renderable
 * \see Stars
 * \see Supernovae
 * \see UTC
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class Simulation : public Renderable {
    public:
        /// Enumerants for the scene elements
        enum content {AXES, CONSTELLATIONS, EARTH, EARTH_GRID, SUN, SKY_GRID, PULSARS, SUPERNOVAE, HUDOVER, AUTOPILOT};

        /**
         * \brief Constructor
         */
        Simulation(void);

        /**
         * \brief Destructor
         */
        virtual ~Simulation();

        /**
         * \brief Evolve the simulation by one notional time step.
         */
        void step(void);

        /**
         * \brief Operate the craft within.
         *
         * \param mv : one of Craft::movements enumerants
         */
        void moveRequest(Craft::movements mv);

        /**
         * \brief Called when this window/client area is resized.
         *        This is of special relevance to the HUD.
         *
         * \param width : the new client area width.
         * \param height : the new client area height.
         */
        void resize(GLuint width, GLuint height);

        /**
         * \brief Set the font for a given scene element.
         *
         * \param element : one of the scene elements
         * \param font : a pointer to the desired OGLFT font
         */
        void setFont(content element, OGLFT_ft* font);

        /**
         * \brief Obtain the current viewpoints camera state
         *
         * \return the camera state
         */
        const CameraState viewPoint(void);

        /**
         * \brief Cycle the activation state of a given scene element
         *
         * \param ct : the desired scene element
         */
        void cycle(Simulation::content ct);

        protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// Earth rendering parameters.
        static const std::string EARTH_NAME;
        static const std::string EARTH_IMAGE_RESOURCE;
        static const GLuint EARTH_STACKS;
        static const GLuint EARTH_SLICES;
        static const GLfloat EARTH_TEXTURE_OFFSET;

        /// Sun rendering parameters.
        static const std::string SUN_NAME;
        static const std::string SUN_IMAGE_RESOURCE;
        static const GLuint SUN_STACKS;
        static const GLuint SUN_SLICES;
        static const GLfloat SUN_TEXTURE_OFFSET;

        /// Lighting constant.
        static const GLfloat AT_INFINITY;

        /// Constellation rendering parameters.
        static const GLuint CONSTELLATIONS_RADIUS;

        /// Pulsars rendering parameters.
        static const GLuint PULSARS_RADIUS;
        static const GLfloat PULSARS_MAG_SIZE;
        static const GLfloat PULSARS_RGB_RED;
        static const GLfloat PULSARS_RGB_GREEN;
        static const GLfloat PULSARS_RGB_BLUE;

        /// Supernovae rendering parameters.
        static const GLuint SUPERNOVAE_RADIUS;
        static const GLfloat SUPERNOVAE_MAG_SIZE;
        static const GLfloat SUPERNOVAE_RGB_RED;
        static const GLfloat SUPERNOVAE_RGB_GREEN;
        static const GLfloat SUPERNOVAE_RGB_BLUE;

        /// Celestial coordinate grid rendering parameters.
        static const GLuint SKYGRID_RADIUS;
        static const GLuint SKYGRID_STACKS;
        static const GLuint SKYGRID_SLICES;
        static const GLfloat SKYGRID_MAIN_WIDTH;
        static const GLfloat SKYGRID_MAIN_RED;
        static const GLfloat SKYGRID_MAIN_GREEN;
        static const GLfloat SKYGRID_MAIN_BLUE;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_WIDTH;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_RED;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_GREEN;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_BLUE;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_WIDTH;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_RED;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_GREEN;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_BLUE;

        /// Earth coordinate grid rendering parameters.
        static const GLuint EARTHGRID_RADIUS;
        static const GLuint EARTHGRID_STACKS;
        static const GLuint EARTHGRID_SLICES;
        static const GLfloat EARTHGRID_MAIN_WIDTH;
        static const GLfloat EARTHGRID_MAIN_RED;
        static const GLfloat EARTHGRID_MAIN_GREEN;
        static const GLfloat EARTHGRID_MAIN_BLUE;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_WIDTH;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_RED;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_GREEN;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_BLUE;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_WIDTH;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_RED;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_GREEN;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_BLUE;

        /// HUD orthographic clipping bounds.
        static const GLint HUD_LEFT_CLIP;
        static const GLint HUD_BOTTOM_CLIP;
        static const GLint HUD_NEAR_CLIP;
        static const GLint HUD_FAR_CLIP;

        /**
         * \brief Load a list of general pulsar data
         */
        void loadPulsars(void);

        /**
         * \brief Load a list of E@H assisted pulsar data
         */
        void loadPulsarsEAH(void);

        /**
         * \brief Load a list of supernovae data
         */
        void loadSupernovae(void);

        /// The current screen/window dimensions
        GLuint screen_width;
        GLuint screen_height;

        /// The autopilot instance.
        AutoPilot pilot;

        /// Last UTC query values.
        GLfloat day366;
        int hour24;
        int min60;

        /// Variable Earth factors.
        GLfloat earth_hour_angle;

        /// Variable Sun factors
        GLfloat sun_rot_angle;
        Vector3D sun_pos;

        /// The renderable scene elements
        Constellations cs;
        Stars<Pulsar> ps;
        Stars<PulsarEAH> ps_EAH;
        Stars<Supernova> sn;
        GridGlobe c_sphere;
        Globe earth;
        GridGlobe e_sphere;
        Globe sun;

        /// The craft instance.
        Craft flyboy;

        /// The current viewpoint for rendering.
        CameraState autopilot_view;

        /// The realtime clock instance.
        UTC clock;

        /// An associative array of font pointers to
        /// store choices for each scene element.
        std::map<content, OGLFT_ft*> fonts;

        /// Pointers to the images used on the HUD.
        HUDImage* aei_image;
        HUDImage* aps_image;
        HUDImage* boinc_image;
        HUDImage* geo_image;
        HUDImage* ligo_image;
        HUDImage* opencl_image;
        HUDImage* virgo_image;
        HUDImage* wyp_image;

        /// Pointer to the scrolling marquee.
        HUDTextLineScroll* version_text;

        /// The overall HUD layout and it's component panels.
        HUDBorderLayout overlay;
        HUDFlowLayout north_panel;
        HUDFlowLayout south_panel;
        HUDFlowLayout east_panel;
        HUDFlowLayout west_panel;

        /**
         * \brief Add an image as named by a Resource to a panel.
         *
         * \param hip : pointer to a HUDImage to store within.
         * \param hfl : pointer to a layout to load into.
         * \param resource_name : the name of the resource to load image data from.
         * \param margin_width : the horizontal margin to apply around the image.
         * \param margin_height : the vertical margin to apply around the image.
         */
        void LoadImageToPanel(HUDImage* hip, HUDFlowLayout* hfl,
                              std::string resource_name, GLuint margin_width, GLuint margin_height);
    };

/**
 * @}
 */

#endif /* SIMULATION_H_*/
