/***************************************************************************
 *   Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen              *
 *   Copyright (C) 2008 Bernd Machenschalk                                 *
 *                                                                         *
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

#ifndef STARSPHERE_H_
#define STARSPHERE_H_

#include "framework.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <util.h>

#include "AbstractGraphicsEngine.h"
#include "EinsteinGravityAdapter.h"
#include "ogl_utility.h"
#include "RenderTask.h"
#include "SDL_ttf.h"
#include "TexturedParallelogram.h"
#include "WindowManager.h"

// SIN and COS take arguments in DEGREES
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0f )
#define SIN(X)   sin( (X) * PI2/360.0f )

// search marker status
#define MARKER_NONE 0
#define MARKER_SHOW 1
#define MARKER_NEW  2

using namespace std;

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief %Starsphere rendering engine for \b Einstein\@Home
 *
 * This class comprises the generic parts of the %Starsphere rendering engine.
 * %Starsphere displays the celestial sphere indicating a fixed set of stars with
 * their constellations as well as known pulsars and supernova remnants. Apart from
 * that the four main gravitational wave observatory locations are shown at their actual
 * real-time position. Furthermore a head-up display (HUD) shows relevant BOINC
 * statistics as well as information on the current workunit (WU) being processed.
 *
 * Note: all science run specific parts are implemented in specialized subclasses
 * of this engine.
 *
 * \todo The code of this implementation is based on the former version of %Starsphere
 * and there's still some refactoring, code cleanup and documenting left to be done.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class Starsphere : public AbstractGraphicsEngine {
    public:
        /// Destructor
        virtual ~Starsphere();

        /**
         * \brief This method is called to initialize the engine
         *
         * \param width The current width of the display surface
         * \param height The current height of the display surface
         * \param font A pointer to a Resource object containing TTF font faces for text rendering
         */
        virtual void initialize(const int width, const int height, const Resource *font);

        /**
         * \brief This method is called when the windowing system encounters a window resize event
         *
         * \param width The new width of the display surface
         * \param height The new height of the display surface
         */
        virtual void resize(const int width, const int height);

        /**
         * \brief This method renders one frame of the animation
         *
         * \param timeOfDay The current time (e.g. BOINC's dtime(), used for time-based rendering evolution)
         */
        virtual void render(const double timeOfDay);

        /**
         * \brief Event handler for mouse button events
         *
         * \param positionX The mouse position's x-coordinate
         * \param positionY The mouse position's y-coordinate
         * \param buttonPressed The mouse button pressed
         */
        void mouseButtonEvent(const int positionX, const int positionY,
                        const AbstractGraphicsEngine::MouseButton buttonPressed);

        /**
         * \brief Event handler for mouse move events
         *
         * \param deltaX The relative mouse movement in the x-direction
         * \param deltaY The relative mouse movement in the y-direction
         * \param buttonPressed The mouse button pressed
         */
        void mouseMoveEvent(const int deltaX, const int deltaY,
                      const AbstractGraphicsEngine::MouseButton buttonPressed);

        /**
         * \brief Event handler for mouse wheel events
         *
         * \param pos The new mouse wheel position
         */
        void mouseWheelEvent(const int pos);

        /**
         * \brief Event handler for key press events
         *
         * \param keyPressed The key pressed
         */
        void keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed);

    protected:
        /**
         * \brief Constructor
         *
         * The constructor is protected since this is an abstract class. It takes
         * as an argument the name of the shared memory area which is propagated
         * to the BOINC client adapter instance (during construction).
         *
         * \param sharedMemoryIdentifier The identifier of the shared memory area
         *
         * \see AbstractGraphicsEngine::AbstractGraphicsEngine()
         * \see BOINCClientAdapter::BOINCClientAdapter()
         */
        Starsphere(string sharedMemoryIdentifier);

        /**
         * \brief Render science run specific logo
         *
         * This abtract method is to be defined by derived classes implementing
         * the science run specific logo rendering.
         */
        virtual void prepareLogo(void) = 0;

        /**
         * \brief Render science run specific search information
         *
         * This abtract method is to be defined by derived classes implementing
         * the science run specific search information handling and rendering.
         *
         * Note: for this engine this also includes the "BOINC Statistics"
         * as it is top-aligned to the "Search Information".
         */
        virtual void prepareSearchInformation(void) = 0;

        /**
         * \brief This method has to be called in order to update the BOINC client information
         *
         * This is the local/generic implementation which calls
         * AbstractGraphicsEngine::refreshLocalBOINCInformation() first and
         * refreshes the "BOINC Statistics" afterwards.
         *
         * \see AbstractGraphicsEngine::refreshLocalBOINCInformation()
         */
        virtual void refreshLocalBOINCInformation(void);

        /**
         * \brief Generates the OpenGL call lists for the displayed observatories
         *
         * \param dimFactor A dim factor (range: 0 <= x <= 1) that will, well, dim the color
         * of the observatories.
         */
        // virtual void generateObservatories(const float dimFactor);

        /**
         * \brief Available feature IDs
         *
         * \see Starsphere::setFeature()
         * \see Starsphere::isFeature()
         */
        enum Features {
            AXES = 1,
            CONSTELLATIONS = 2,
            GAMMAS = 4,
            GRID = 8,
            LOGO = 16,
            MARKER = 32,
            OBSERVATORIES = 64,
            PULSARS = 128,
            SEARCHINFO = 256,
            SNRS = 512,
            STARS = 1024,
            XRAYS = 2048,
            EARTH = 4096,
            AUTO_ROLL = 8192,
            HELP = 16384
            };

        /**
         * \brief Set the display state of a certain feature
         *
         * \param feature The feature to enable/disable
         * \param enable The state to set for the feature
         *
         * \see Starsphere::Features
         * \see Starsphere::isFeature()
         */
        void setFeature(const Features feature, const bool enable);

        /**
         * \brief Query the display state of a certain feature
         *
         * \param feature The feature to query
         *
         * \return The current state of the feature
         *
         * \see Starsphere::Features
         * \see Starsphere::setFeature()
         */
        bool isFeature(const Features feature);

        /**
         * \brief Computes the Right Ascension of the zenith at a given time (from
         * the Unix epoch, in seconds).
         *
         * From 'The Cambridge Handbook of Physics Formulas', Graham Woan, 2003
         * edition, CUP. (NOT the first edition), p177.
         *
         * \param T Current time in seconds since the epoch
         *
         * \return The right ascension of the zenith
         */
        GLfloat RAofZenithGreenwich(double T);

        /**
         * \brief By accessing the system clock computes the number of days
         * since the first of January.
         *
         * \return The day number for the current year.
         */
        GLuint dayOfYear(void);

        /**
         * \brief Creates a GL vertex in 3D sky sphere coordinates
         *
         * Use like glVertex()
         *
         * \param RAdeg The right ascension of the new vertex
         * \param DEdeg The declination of the new vertex
         */
        glm::vec3 sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius);

        glm::vec3 sphVertex(GLfloat RAdeg, GLfloat DEdeg);

        /// How many bytes for an RGBA pixel/texel ?
        static const GLuint BYTES_PER_TEXEL;

        /// Cumulative frame count.
        GLuint m_framecount;

        // resource handling

        /// Font resource instance
        const Resource* m_FontResource;

        /// Shader resource instance
        const Resource* m_vertex_shader_resource;
        const Resource* m_fragment_shader_resource;

        /// Font texture instance for info box content rendering
        TTF_Font* m_FontText;

        // Graphics state info:

        /// Current window width (x-resolution)
        int m_CurrentWidth;

        /// Current window height (y-resolution)
        int m_CurrentHeight;

        // HUD text rendering config (maybe overridden in subclasses)

        /// Margin from edge of screen to place HUD ( in pixels ).
        GLfloat m_HUD_Margin;

        /// X-coordinate position for head up display (HUD) positioning
        GLfloat m_HUD_XLeft;

        /// X-coordinate position for head up display (HUD) positioning
        GLfloat m_HUD_XRight;

        /// Y-coordinate position for head up display (HUD) positioning
        GLfloat m_HUD_YTop;

        /// Y-coordinate line offset for head up display (HUD) positioning
        GLfloat m_HUD_YBottom;

        // local HUD contents

        /// User name to be displayed in "BOINC Information" panel
        string m_UserName;

        /// Team name to be displayed in "BOINC Information" panel
        string m_TeamName;

        /// User total credit to be displayed in "BOINC Information" panel
        string m_UserCredit;

        /// User recent average credit to be displayed in "BOINC Information" panel
        string m_UserRACredit;

        // search marker info

        /// Current right ascension of the search marker (gunsight)
        double m_CurrentRightAscension;

        /// Current declination of the search marker (gunsight)
        double m_CurrentDeclination;

        /// Refresh indicator when the search marker (gunsight) coordinates changed
        bool m_RefreshSearchMarker;

    private:
        // Class constants.
        static const GLfloat DEFAULT_CLEAR_DEPTH;
        static const GLfloat DEFAULT_LINE_WIDTH;
        static const GLfloat DEFAULT_POINT_SIZE;
        static const GLuint  PIXEL_UNPACK_BOUNDARY;
        static const GLfloat SPHERE_RADIUS;
        static const GLfloat EARTH_RADIUS;
        static const GLfloat EARTH_MAX_OFFSET;

        // Fonts.
        static const GLboolean TTF_FREE_SOURCE;
        static const GLuint TTF_FONT_LOAD_TEXT_POINT_SIZE;

        // Transform parameters.
        static const GLfloat PERSPECTIVE_NEAR_FRUSTUM_DISTANCE;
        static const GLfloat PERSPECTIVE_FAR_FRUSTUM_DISTANCE;

        // Limit of viewpoint.
        static const GLfloat FARTHEST_VIEWPOINT;

        // Field of view angle.
        static const GLfloat PERSPECTIVE_FOV_DEFAULT;

        // Globe feature parameters.
        static const GLuint GLOBE_LATITUDE_LAYERS;
        static const GLuint GLOBE_LONGITUDE_SLICES;
        static const GLfloat GLOBE_TEXTURE_OFFSET;
        static const GLuint GRID_LATITUDE_LAYERS;
        static const GLuint GRID_LONGITUDE_SLICES;
        static const GLuint VERTICES_PER_TRIANGLE;
        static const GLuint AXES_LINE_LENGTH;
        static const GLuint NUMBER_OF_AXES;

        static const GLuint OBSERVATORY_COUNT;
        static const GLuint GEODE_AXES_PER_OBSERVATORY;
        static const GLuint ARMS_PER_OBSERVATORY;
        static const GLuint COORDS_PER_VERTEX_BARE;
        static const GLuint COORDS_PER_VERTEX_COLOR;
        static const GLuint COORDS_PER_VERTEX_TEXTURE;
        static const GLuint VERTICES_PER_ARM;
        static const GLuint VERTICES_PER_LINE;

        /// HUD elements.
        TexturedParallelogram* m_logo1;
        TexturedParallelogram* m_logo2;
        TexturedParallelogram* m_user_info;
        TexturedParallelogram* m_team_info;
        TexturedParallelogram* m_total_info;
        TexturedParallelogram* m_RAC_info;
        TexturedParallelogram* m_help_info;

        /// Pointers to rendering tasks.
        RenderTask* m_render_task_cons;
        RenderTask* m_render_task_gammas;
        RenderTask* m_render_task_globe;
        RenderTask* m_render_task_earth;
        RenderTask* m_render_task_psr;
        RenderTask* m_render_task_snr;
        RenderTask* m_render_task_star;
        RenderTask* m_render_task_axes;
        RenderTask* m_render_task_arms;

        /// Colors.
        glm::vec3 m_axes_color;
        glm::vec3 m_geode_axes_color;
        glm::vec3 m_arms_color;
        glm::vec3 m_gamma_color;
        glm::vec3 m_grid_color;
        glm::vec3 m_earth_color;
        glm::vec3 m_pulsar_color;
        glm::vec3 m_star_color;
        glm::vec3 m_supernova_color;
        glm::vec3 m_constellation_line_color;

        /// Point & line sizes.
        GLfloat m_gamma_point_size;
        GLfloat m_globe_point_size;
        GLfloat m_pulsar_point_size;
        GLfloat m_star_point_size;
        GLfloat m_supernova_point_size;
        GLfloat m_constellation_line_width;
        GLfloat m_axes_line_width;
        GLfloat m_geode_line_width;
        GLfloat m_arms_line_width;

        // Number of distinct stars.
        GLuint m_distinct_stars;

        // Number of constellation lines.
        GLuint m_constellation_lines;

        // Number of observatory arms.
        GLuint m_arms_lines;
        GLuint m_geode_lines;

        // Number of line segments for globe.
        GLuint m_globe_vertices;
        GLuint m_globe_lines;
        GLuint m_earth_triangles;

        glm::mat4 m_orthographic_projection;
        glm::mat4 m_perspective_projection;

        glm::mat4 m_view;
        glm::mat4 m_rotation;
        glm::mat4 m_camera;

        glm::mat4 m_view_earth;
        glm::mat4 m_rotation_earth;
        glm::mat4 m_camera_earth;

        glm::vec3 m_vector_sun;

        GLfloat m_sun_RA;
        GLfloat m_sun_DEC;

        GLuint m_current_help_entry;
        GLuint m_num_help_entries;
        std::vector<std::string> m_HUD_help_texts;
        SDL_Surface* m_help_text_surface;

        SDL_Surface* m_info_text_surface;

        void make_local_arms(GLfloat latitude, GLfloat longitude,
                             GLfloat x_arm_azimuth, GLfloat y_arm_azimuth,
                             glm::vec3 color, GLfloat* vertex_data, GLuint array_offset);

        void make_local_geode_axes(GLfloat latitude,
                                   GLfloat longitude,
                                   glm::vec3 color,
                                   GLfloat* vertex_data,
                                   GLuint array_offset);

        /**
         * \brief Calculate and store the right ascension and declination of
         * the Sun on the celestial sphere, also a vector from the origin
         * to the Sun on the celestial sphere.
         *
         */
        void setSunPosition(void);

        /// Generate TexturedParallelograms for logos.
        void make_logos(void);

        void make_user_info(void);

        /// Generate rendering task for stars.
        void make_stars(void);

        /// Generate rendering task for pulsars.
        void make_pulsars(void);

        /// Generate rendering task for supernovae.
        void make_snrs(void);

        /// Generate rendering task for constellations.
        void make_constellations(void);

        /// Generate rendering task for gamma ray pulsars.
        void make_gammas(void);

        /// Generate OpenGL display list for the axes (debug)
        void make_axes(void);

        /// Generate a latitude/longitude mesh globe.
        void make_globe_mesh_lat_long(void);

        /// Generate a globe with a texture.
        void make_globe_mesh_texture(void);

        void make_observatories(void);

        void make_HUD_help_entries(void);

        void make_HUD_help_entry(void);

        /**
         * \brief Generate OpenGL display list for search marker (gunsight)
         *
         * \param RAdeg Right ascension in degrees
         * \param DEdeg Declination in degrees
         * \param size Base size (radius/length) of the marker
         */
        void make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size);

        /**
         * \brief Current state of all features (bitmask)
         * \see Starsphere::Features()
         * \see Starsphere::setFeature()
         * \see Starsphere::isFeature()
         */
        int featureFlags;

        // Viewpoint (can be changed with mouse)

        /// Angle of perspective field of view.
        GLfloat m_viewpt_fov;

        /// Aspect ratio of screen.
        GLfloat m_aspect;

        /// Viewpoint azimuth in degrees
        GLfloat m_viewpt_azimuth;

        /// Viewpoint elevation angle in degrees
        GLfloat m_viewpt_elev;

        /// Viewpoint distance out
        GLfloat m_viewpt_radius;

        /// Viewpoint rotation offset
        GLfloat m_rotation_offset;

        // view control

        /**
         * \brief Rotates the sphere by changing the viewpoint rotation/elevation relatively
         *
         * \param relativeRotation Relative rotation factor (e.g. relative mouse movement)
         * \param relativeElevation Relative elevation factor (e.g. relative mouse movement)
         *
         * \see Starsphere::mouseMoveEvent()
         */
        void rotateSphere(const int relativeRotation, const int relativeElevation);

        /**
         * \brief Zooms the sphere by changing the viewpoint radius relatively
         *
         * \param relativeZoom Relative zoom factor (e.g. relative mouse movement)
         *
         * \see Starsphere::mouseMoveEvent()
         */
        void zoomSphere(const int relativeZoom);

        /**
         * \brief Configures the perspective and orthographic transform matrices.
         *
         */
        void configTransformMatrices(void);
    };

/// Constellation & star coordinates (starlist.c)
extern float star_info[][2];

/// Total number of stars
extern int Nstars;

/// Gamma pulsar coordinates (gamma_list.c)
extern float gamma_info[][2];

/// Total number of pulsars
extern int Ngammas;

/// Pulsar coordinates (pulsar_list.c)
extern float pulsar_info[][2];

/// Total number of pulsars
extern int Npulsars;

/// SNR coordinates (snr_list.C)
extern float SNR_info[][2];

/// Total number of SNRs
extern int NSNRs;

/**
 * @}
 */

#endif /*STARSPHERE_H_*/
