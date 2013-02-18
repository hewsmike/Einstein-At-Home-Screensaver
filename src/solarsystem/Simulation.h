/***************************************************************************
 *   Copyright (C) 2013 by Mike Hewson                                     *
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
#include "Globe.h"
#include "GreatCircle.h"
#include "GridGlobe.h"
#include "HUDBorderLayout.h"
#include "HUDContainer.h"
#include "HUDFlowLayout.h"
#include "HUDImage.h"
#include "HUDTextLineScroll.h"
#include "Pulsar.h"
#include "PulsarEAH.h"
#include "PulsarsEAH.h"
#include "Renderable.h"
#include "SolarSystemGlobals.h"
#include "Stars.h"
#include "Supernova.h"
#include "TargetReticle.h"
#include "UTC.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Container/manager for scene renderable elements
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
 * \see TargetReticle
 * \see UTC
 * \see Vector3D
 *
 * \author Mike Hewson\n
 */

class Simulation : public Renderable {
    public:
        /**
         * \brief Constructor
         */
        Simulation(BOINCClientAdapter* boinc_adapter);

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
         * \brief Obtain the current viewpoints camera state
         *
         * \return the camera state
         */
        CameraState viewPoint(void);

        /**
         * \brief Cycle the activation state of a given scene element
         *
         * \param ct : the desired scene element
         */
        void cycle(SolarSystemGlobals::content ct);

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

        /**
         * \brief Render science run specific logo
         *
         * This abstract method is to be defined by derived classes implementing
         * the science run specific logo rendering.
         */
        virtual void includeLogo(HUDFlowLayout* container) = 0;

        /**
         * \brief Render science run specific search information
         *
         * This abstract method is to be defined by derived classes implementing
         * the science run specific search information handling and rendering.
         */
        virtual void includeSearchInformation(HUDFlowLayout* container) = 0;

        /**
         * \brief Render science run specific observatories
         *
         * This abstract method is to be defined by derived classes implementing
         * the science run specific observatories handling and rendering.
         */
        virtual void renderObservatories(void) = 0;

        /**
         * \brief Add an image as named by a Resource to a panel.
         *
         * \param hip : pointer to a HUDImage to store within.
         * \param hfl : pointer to a container to load into.
         * \param resource_name : the name of the resource to load image data from.
         * \param margin_width : the horizontal margin to apply around the image.
         * \param margin_height : the vertical margin to apply around the image.
         */
        void loadImageToPanel(HUDImage* hip, HUDFlowLayout* hfl,
                              std::string resource_name, GLuint margin_width,
                              GLuint margin_height);

    private:
        /// Enumerants for EAH pulsar file parsing
        enum line_content {NAME,
                           RIGHT_ASCENSION,
                           DECLINATION,
                           PERIOD,
                           DISPERSION_MODULUS,
                           DISTANCE,
                           VOLUNTEERS,
                           SEARCH_SOURCE,
                           LINE_TOKENS};

        /// Earth rendering parameters.
        static const std::string EARTH_NAME;
        static const std::string EARTH_IMAGE_RESOURCE;
        static const GLuint EARTH_STACKS;
        static const GLuint EARTH_SLICES;
        static const GLfloat EARTH_TEXTURE_OFFSET;

        /// Earth shadow rendering parameters.
        static const std::string EARTH_SHADOW_NAME;
        static const std::string EARTH_SHADOW_IMAGE_RESOURCE;
        static const GLfloat EARTH_SHADOW_RADIUS;
        static const GLuint EARTH_SHADOW_STACKS;
        static const GLuint EARTH_SHADOW_SLICES;
        static const GLfloat EARTH_SHADOW_TEXTURE_OFFSET;

        /// Sun rendering parameters.
        static const std::string SUN_NAME;
        static const std::string SUN_IMAGE_RESOURCE;
        static const GLuint SUN_STACKS;
        static const GLuint SUN_SLICES;
        static const GLfloat SUN_TEXTURE_OFFSET;

        /// Constellation rendering parameters.
        static const GLuint CONSTELLATIONS_RADIUS;

        /// Pulsars rendering parameters.
        static const GLuint PULSARS_RADIUS;
        static const GLfloat PULSARS_MAG_SIZE;
        static const GLfloat PULSARS_RGB_RED;
        static const GLfloat PULSARS_RGB_GREEN;
        static const GLfloat PULSARS_RGB_BLUE;

        /// E@H discovered pulsars rendering parameters.
        static const GLuint PULSARS_EAH_RADIUS;
        static const GLfloat PULSARS_EAH_MAG_SIZE;
        static const GLfloat PULSARS_EAH_RGB_RED;
        static const GLfloat PULSARS_EAH_RGB_GREEN;
        static const GLfloat PULSARS_EAH_RGB_BLUE;

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
        static const GLfloat SKYGRID_MAIN_ALPHA;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_WIDTH;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_RED;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_GREEN;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_BLUE;
        static const GLfloat SKYGRID_CELESTIAL_EQUATOR_ALPHA;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_WIDTH;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_RED;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_GREEN;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_BLUE;
        static const GLfloat SKYGRID_PRIME_MERIDIAN_ALPHA;

        /// Earth coordinate grid rendering parameters.
        static const GLuint EARTHGRID_RADIUS;
        static const GLuint EARTHGRID_STACKS;
        static const GLuint EARTHGRID_SLICES;
        static const GLfloat EARTHGRID_MAIN_WIDTH;
        static const GLfloat EARTHGRID_MAIN_RED;
        static const GLfloat EARTHGRID_MAIN_GREEN;
        static const GLfloat EARTHGRID_MAIN_BLUE;
        static const GLfloat EARTHGRID_MAIN_ALPHA;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_WIDTH;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_RED;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_GREEN;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_BLUE;
        static const GLfloat EARTHGRID_CELESTIAL_EQUATOR_ALPHA;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_WIDTH;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_RED;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_GREEN;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_BLUE;
        static const GLfloat EARTHGRID_PRIME_MERIDIAN_ALPHA;

        /// HUD orthographic clipping bounds.
        static const GLint HUD_LEFT_CLIP;
        static const GLint HUD_BOTTOM_CLIP;
        static const GLint HUD_NEAR_CLIP;
        static const GLint HUD_FAR_CLIP;

        /// Filename to obtain EAH assisted pulsar data.
        static const std::string EAH_PULSAR_FILE;
        static const std::string EAH_PULSAR_FILE_EXT;

        /// The dimensions in pixels of the target reticle images
        /// used in animation, assumed square.
        static const GLuint TARGET_RETICLE_SIZE;

        /// The number of image frames used to
        /// animate the target reticle.
        static const GLuint TARGET_RETICLE_FRAMES;

        /// The base name of the image resources used for reticle animation.
        static const std::string TARGET_RETICLE_RESOURCE_NAME_BASE;

        static const VectorSP ECLIPTIC_NORTH_POLE;
        static const VectorSP ECLIPTIC_LONG_ZERO;
        static const GLfloat ECLIPTIC_LINE_WIDTH;
        static const GLfloat ECLIPTIC_LINE_RED;
        static const GLfloat ECLIPTIC_LINE_GREEN;
        static const GLfloat ECLIPTIC_LINE_BLUE;
        static const GLfloat ECLIPTIC_LINE_ALPHA;

        static const VectorSP GALACTIC_NORTH_POLE;
        static const VectorSP GALACTIC_LONG_ZERO;
        static const GLfloat GALACTIC_LINE_WIDTH;
        static const GLfloat GALACTIC_LINE_RED;
        static const GLfloat GALACTIC_LINE_GREEN;
        static const GLfloat GALACTIC_LINE_BLUE;
        static const GLfloat GALACTIC_LINE_ALPHA;

        /// How many frames between refreshes of WU data.
        static const GLuint WU_DETAILS_REFRESH_INTERVAL;

        /// How many frames between refreshes of user data.
        static const GLuint USER_DETAILS_REFRESH_INTERVAL;

        /**
         * \brief Load a list of general pulsar data
         */
        void loadPulsars(void);

        /**
         * \brief Load a list of E@H assisted pulsar data
         *
         * \return boolean indicating the success of the load
         *                 false : load succeeded
         *                 true : load failed
         */
        bool loadPulsarsEAH(void);

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
        PulsarsEAH<PulsarEAH> ps_EAH;
        Stars<Supernova> sn;
        GridGlobe c_sphere;
        Globe earth;
        Globe earth_shadow;
        GridGlobe earth_grid;
        Globe sun;
        TargetReticle target;
        GreatCircle ecliptic;
        GreatCircle galactic;

        /// The craft instance.
        Craft flyboy;

        /// The realtime clock instance.
        UTC clock;

        /// Pointers to logo type images used on the
        /// generic portion of the HUD.
        HUDImage* aei_image;
        HUDImage* aps_image;
        HUDImage* boinc_image;
        HUDImage* wyp_image;

        /// Associative array of names to image pointers
        /// for pulse profiles to display on the HUD.
        std::map<std::string, HUDItem*> pulse_profiles;

        /// Pointer to the scrolling marquee text.
        HUDTextLineScroll* version_text;

        /// The overall HUD layout and it's component panels.
        HUDBorderLayout overlay;
        HUDFlowLayout north_panel;
        HUDFlowLayout south_panel;
        HUDTempFlowLayout east_panel;
        HUDTempFlowLayout west_panel;

        HUDTempFlowLayout north_west_panel;
        HUDTempFlowLayout north_east_panel;

        HUDFlowLayout south_west_panel;
        HUDFlowLayout south_centre_panel;
        HUDFlowLayout south_east_panel;

        /// The index of the current rendered frame ( since program start ).
        GLuint frame_number;

        BOINCClientAdapter* BC_adapter;

        /**
         * \brief Obtain the file name containing pulsar data
         *
         * \return the file's name
         */
        std::string getEAHPulsarDataFileName(void);

        /**
         * \brief Convert a string representing right ascensions to a number
         *
         * \param right_ascension : string in an expected format for right ascension,
         *                          currently determined by the 'EatH_mastercat_****.txt'
         *                          file.
         * \param result : pointer to write the result to
         * \return boolean indicating success of conversion
         *                 false - no conversion ( string has bad format )
         *                 true - conversion succeeded
         */
        bool str2ra(std::string right_ascension, float* result) const;

        /**
         * \brief Convert a string representing declination to a number
         *
         * \param declination : string in an expected format for declination,
         *                      currently determined by the 'EatH_mastercat_****.txt'
         *                      file.
         * \param result : pointer to write the result to
         * \return boolean indicating success of conversion
         *                 false - no conversion ( string has bad format )
         *                 true - conversion succeeded
         */
        bool str2dec(std::string declination, float* result) const;

        /**
         * \brief Convert a string to a sequence of strings as per delimiter
         *
         * \param input : the string to be tokenised
         * \param delimiter : the character to use to separate the tokens
         * \return an array of tokens/strings
         */
        std::vector<std::string> tokenise(std::string input, char delimiter) const;

        /**
         * \brief Take a line of input from the 'EatH_mastercat_****.txt' and
         *        separate into data fields
         *
         * \param input : the line to be broken up
         * \return an array of data fields
         */
        std::vector<std::string> parseLine(std::string input) const;

        /**
         * \brief Insert the data from the current Lookout and place on HUD panels
         */
        void loadLookoutDataToPanels(void);

        /**
         * \brief Render general user and host information
         */
        void includeUserInformation(HUDFlowLayout* container);
    };

/**
 * @}
 */

#endif /* SIMULATION_H_*/
