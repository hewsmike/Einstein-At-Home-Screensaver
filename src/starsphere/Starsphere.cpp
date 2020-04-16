/***************************************************************************
 *   Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen              *
 *   Copyright (C) 2008 Bernd Machenschalk                                 *
 *                                                                         *
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Starsphere.h"

#include "ogl_utility.h"

#include "AttributeInputAdapter.h"
#include "ErrorHandler.h"
#include "FragmentShader.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Program.h"
#include "ResourceFactory.h"
#include "TransformGlobals.h"
#include "VertexBuffer.h"
#include "VertexFetch.h"
#include "VertexShader.h"

// Class static constants
const GLfloat Starsphere::DEFAULT_CLEAR_DEPTH(1.0f);
const GLfloat Starsphere::DEFAULT_LINE_WIDTH(2.0f);
const GLfloat Starsphere::DEFAULT_POINT_SIZE(1.5f);
const GLuint Starsphere::PIXEL_UNPACK_BOUNDARY(1);
const GLfloat Starsphere::SPHERE_RADIUS(5.0f);
const GLboolean Starsphere::TTF_FREE_SOURCE(false);
const GLuint Starsphere::TTF_FONT_LOAD_HEADER_POINT_SIZE(13);
const GLuint Starsphere::TTF_FONT_LOAD_TEXT_POINT_SIZE(11);
const GLfloat Starsphere::VIEWPOINT_MAX_ZOOM(20.0f);
const GLfloat Starsphere::VIEWPOINT_MIN_ZOOM(0.5f);
const GLfloat Starsphere::VIEWPOINT_ZOOM_RATE(10.0f);
const GLfloat Starsphere::PERSPECTIVE_NEAR_FRUSTUM_DISTANCE(0.1f);
const GLfloat Starsphere::PERSPECTIVE_FAR_FRUSTUM_DISTANCE(100.0f);
const GLfloat Starsphere::PERSPECTIVE_FOV_DEFAULT(45.0f);
const GLfloat Starsphere::PERSPECTIVE_FOV_MIN(20.0f);
const GLfloat Starsphere::PERSPECTIVE_FOV_MAX(70.0f);
const GLfloat Starsphere::VIEWPOINT_MOUSEWHEEL_FOV_RATE(0.1f);
const GLuint Starsphere::GLOBE_LATITUDE_LAYERS(19);                     // Every ten degrees in latitude/declination, pole to pole.
const GLuint Starsphere::GLOBE_LONGITUDE_SLICES(36);                    // Every ten degrees in longitude/right-ascension, from equinox.
const GLuint Starsphere::VERTICES_PER_TRIANGLE(3);

Starsphere::Starsphere(string sharedMemoryAreaIdentifier) :
    AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
    m_framecount = 0;

    m_distinct_stars = 0;
    m_constellation_lines = 0;
    m_globe_lines = 0;

    m_perspective_projection = glm::mat4(0.0f);
    m_orthographic_projection = glm::mat4(0.0f);

    m_view = glm::mat4(0);
    m_rotation  = glm::mat4(0.0f);
    m_axis = glm::vec3(0.0f);
    m_camera = glm::mat4(0.0f);

    m_render_task_cons = NULL;
    m_render_task_gammas = NULL;
    m_render_task_globe = NULL;
    m_render_task_psr = NULL;
    m_render_task_snr = NULL;
    m_render_task_star = NULL;

    m_rotation = glm::mat4(1.0f);
    m_axis = glm::vec3(1.0f, 0.0f, 0.0f);

    m_ObservatoryDrawTimeLocal = 0;

    m_FontResource = NULL;

    m_vertex_shader_resource = NULL;
    m_fragment_shader_resource = NULL;

    m_FontLogo1 = NULL;
    m_FontLogo2 = NULL;
    m_FontHeader = NULL;
    m_FontText = NULL;

    m_CurrentWidth = 0;
    m_CurrentHeight = 0;
    m_XStartPosLeft = 0;
    m_YStartPosTop = 0;
    m_YOffsetLarge = 0;
    m_CurrentRightAscension = 0;
    m_CurrentDeclination = 0;
    m_RefreshSearchMarker = false;

    /**
     * Parameters and State info
     */
    featureFlags = 0;

    /**
     * Viewpoint (can be changed with mouse)
     */
    viewpt_fov = PERSPECTIVE_FOV_DEFAULT;
    m_aspect = 0.0f;
    viewpt_azimuth = 30.0f;
    viewpt_elev = 23.6f;
    viewpt_radius = (SPHERE_RADIUS + VIEWPOINT_MAX_ZOOM)/ 2.0f;

    wobble_amp = 37.0f;
    wobble_period = 17.0f;
    zoom_amp = 0.00f;
    zoom_period = 29.0f;

    rotation_offset = 0.0f;
    rotation_speed = 180.0f;

    m_CurrentRightAscension = -1.0f;
    m_CurrentDeclination = -1.0f;
    m_RefreshSearchMarker = true;

    m_gamma_color = glm::vec3(0.0f, 1.0f, 0.0f);              // Gammas are Green.
    m_globe_color = glm::vec3(0.15f, 0.15f, 0.15f);           // Globe is Grey.
    m_earth_color = glm::vec3(1.0f, 1.0f, 0.5f);
    m_pulsar_color = glm::vec3(0.80f, 0.0f, 0.85f);           // Pulsars are Purple.
    m_star_color = glm::vec3(1.0f, 1.0f, 1.0f);               // Stars are Silver.
    m_supernova_color = glm::vec3(1.0f, 0.0f, 0.0f);          // Supernovae are Sienna.
    m_constellation_line_color = glm::vec3(0.7f, 0.7f, 0.0f); // Lines are Light yellow.

    m_gamma_point_size = 3.0f;
    m_globe_point_size = 0.5f;
    m_pulsar_point_size = 3.0f;
    m_star_point_size = 4.0f;
    m_supernova_point_size = 3.0f;
    m_constellation_line_width = 1.0f;
    }

Starsphere::~Starsphere() {
    if(m_vertex_shader_resource) delete m_vertex_shader_resource;
    if(m_fragment_shader_resource) delete m_fragment_shader_resource;

//
//    /// TODO - longstanding unhappiness ( but compiles )
//    /// "warning: invalid use of incomplete type ‘struct _TTF_Font’"
//    /// "warning: forward declaration of ‘struct _TTF_Font’"
    if(m_FontLogo1) delete m_FontLogo1;
    if(m_FontLogo2) delete m_FontLogo2;
    if(m_FontHeader) delete m_FontHeader;
    if(m_FontText) delete m_FontText;
//

    if(m_render_task_cons) delete m_render_task_cons;
    if(m_render_task_gammas) delete m_render_task_gammas;
    if(m_render_task_psr) delete m_render_task_psr;
    if(m_render_task_snr) delete m_render_task_snr;
    if(m_render_task_star) delete m_render_task_star;

    }

glm::vec3 Starsphere::sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius) {
    GLfloat x = radius * COS(DEdeg) * COS(RAdeg);
    GLfloat z = -radius * COS(DEdeg) * SIN(RAdeg);
    GLfloat y = radius * SIN(DEdeg);

    return glm::vec3(x,y,z);
    }

glm::vec3 Starsphere::sphVertex(GLfloat RAdeg, GLfloat DEdeg) {
    return sphVertex3D(RAdeg, DEdeg, SPHERE_RADIUS);
    }

/**
 *  Create Stars: markers for each star
 */
void Starsphere::make_stars(void) {
    m_distinct_stars = 0;

    // Temporary array for vertex data, to populate a vertex buffer object.
    GLfloat star_vertex_data[Nstars * 3];

    bool is_dupe = false;

    /**
     * At some point in the future star_info[][] will also contain
     * star magnitude and the marker size will vary with this.
     * MAH [08/09/2015]- this can be achieved via code in vertex shader
     * using gl_PointSize coupled with a per-vertex attribute.
     */

    for(int i = 0; i < Nstars; ++i) {
        // As same stars appear more than once in constellations
        // then ignore duplicates.
        is_dupe = false;
        for(int j = 0; j< i; ++j) {
            if(star_info[j][0] == star_info[i][0] &&
               star_info[j][0] == star_info[i][0]) {
                is_dupe = true;
                break;
                }
            }
        if (!is_dupe) {
            // Vector for a single position.
            glm::vec3 temp = sphVertex3D(star_info[i][0], star_info[i][1], SPHERE_RADIUS);

            // Each array position is at successive locations as x, y then z
            star_vertex_data[m_distinct_stars*3] = temp.x;
            star_vertex_data[m_distinct_stars*3 + 1] = temp.y;
            star_vertex_data[m_distinct_stars*3 + 2] = temp.z;
            ++m_distinct_stars;
            }
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Stars")->std_string(),
                                         factory.createInstance("FragmentShader_Pass")->std_string()};

      // Populate data structure for vertices.
    RenderTask::vertex_buffer_group v_group1 = {star_vertex_data,
                                                GLuint(m_distinct_stars*3*sizeof(GLfloat)),
                                                m_distinct_stars,
                                                GL_STATIC_DRAW,
                                                VertexBuffer::BY_VERTEX};

    // Instantiate a rendering task with the provided information.
    m_render_task_star = new RenderTask(s_group1, v_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_star->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_star->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());

    m_render_task_star->setUniform("color", &m_star_color);

    m_render_task_star->setUniform("point_size", &m_star_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_star->acquire();

    return;
    }

/**
 *  Pulsar Markers:
 */
void Starsphere::make_pulsars(void) {
    // Temporary array for vertex data, to populate a vertex buffer object.
    GLfloat pulsar_vertex_data[Npulsars * 3];

    // Go through the pulsar position data and convert to cartesian
    // coordinates at the radius of the sphere.
    for(int i = 0; i < Npulsars; ++i) {
        // Vector for a single position.
        glm::vec3 temp = sphVertex3D(pulsar_info[i][0], pulsar_info[i][1], SPHERE_RADIUS);

        // Each array position is at successive locations as x, y then z
        pulsar_vertex_data[i*3] = temp.x;
        pulsar_vertex_data[i*3 + 1] = temp.y;
        pulsar_vertex_data[i*3 + 2] = temp.z;
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Pulsars")->std_string(),
                                         factory.createInstance("FragmentShader_Pass")->std_string()};

    // Populate data structure for vertices.
    RenderTask::vertex_buffer_group v_group1 = {pulsar_vertex_data,
                                                GLuint(sizeof(pulsar_vertex_data)),
                                                GLuint(Npulsars),
                                                GL_STATIC_DRAW,
                                                VertexBuffer::BY_VERTEX};

    // Instantiate a rendering task with the provided information.
    m_render_task_psr = new RenderTask(s_group1, v_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_psr->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_psr->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());

    m_render_task_psr->setUniform("color", &m_pulsar_color);

    m_render_task_psr->setUniform("point_size", &m_pulsar_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_psr->acquire();

    return;
    }

/**
 * Super Novae Remnants (SNRs):
 */
void Starsphere::make_snrs(void) {
    // Temporary array for vertex data, to populate a vertex buffer object.
    GLfloat snr_vertex_data[NSNRs * 3];

    // Go through the supernovae position data and convert to cartesian
    // coordinates at the radius of the sphere.
    for(int i = 0; i < NSNRs; ++i) {
        // Vector for a single position.
        glm::vec3 temp = sphVertex3D(SNR_info[i][0], SNR_info[i][1], SPHERE_RADIUS);

        // Each array position is at successive locations as x, y then z
        snr_vertex_data[i*3] = temp.x;
        snr_vertex_data[i*3 + 1] = temp.y;
        snr_vertex_data[i*3 + 2] = temp.z;
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Supernovae")->std_string(),
                                         factory.createInstance("FragmentShader_Pass")->std_string()};

    // Instantiate a rendering task with the provided information.
    RenderTask::vertex_buffer_group v_group1 = {snr_vertex_data,
                                                GLuint(sizeof(snr_vertex_data)),
                                                GLuint(NSNRs),
                                                GL_STATIC_DRAW,
                                                VertexBuffer::BY_VERTEX};

    // Instantiate a rendering task with the provided information.
    m_render_task_snr = new RenderTask(s_group1, v_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_snr->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_snr->setUniform("CameraMatrix", &m_camera);

    m_render_task_snr->setUniform("color", &m_supernova_color);

    m_render_task_snr->setUniform("point_size", &m_supernova_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_snr->acquire();

    return;
    }

/**
 * Create Constellations:
 * draws line links between pairs of stars in the list.
 */
void Starsphere::make_constellations(void) {
    // Ensure that we only deal in pairs, so singles will be ignored.
    m_constellation_lines = Nstars/2;

    GLfloat offset = 0.10f;

    // Temporary array for vertex data, to populate a vertex buffer object.
    GLfloat star_vertex_data[m_constellation_lines * 2 * 3];

    // Note assess stars in pairs.
    for(GLuint line = 0; line < m_constellation_lines; ++line) {
        // Vectors for each point at the ends of a constellation line.
        glm::vec3 temp1 = sphVertex3D(star_info[line*2][0], star_info[line*2][1], SPHERE_RADIUS);
        glm::vec3 temp2 = sphVertex3D(star_info[line*2+1][0], star_info[line*2+1][1], SPHERE_RADIUS);

        // The offset from either star.
        glm::vec3 diff = (temp2 - temp1) * offset;

        // Each array position is at successive locations as x, y then z
        star_vertex_data[line*6] = (temp1 + diff).x;
        star_vertex_data[line*6 + 1] = (temp1 + diff).y;
        star_vertex_data[line*6 + 2] = (temp1 + diff).z;

        // Offset from second star to end the line.
        // Each array position is at successive locations as x, y then z
        star_vertex_data[line*6 + 3] = (temp2 - diff).x;
        star_vertex_data[line*6 + 4] = (temp2 - diff).y;
        star_vertex_data[line*6 + 5] = (temp2 - diff).z;
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Stars")->std_string(),
                                         factory.createInstance("FragmentShader_Pass")->std_string()};

    // Populate data structure for vertices.
    RenderTask::vertex_buffer_group v_group1 = {star_vertex_data,
                                                GLuint(m_constellation_lines*2*3*sizeof(GLfloat)),
                                                GLuint(m_constellation_lines*2),
                                                GL_STATIC_DRAW,
                                                VertexBuffer::BY_VERTEX};

    // Instantiate a rendering task with the provided information.
    m_render_task_cons = new RenderTask(s_group1, v_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_cons->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_cons->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());;

    m_render_task_cons->setUniform("color", &m_constellation_line_color);

    m_render_task_cons->setUniform("point_size", &m_constellation_line_width);

    // Claim all required state machine resources for this rendering task.
    m_render_task_cons->acquire();

    return;
    }

/**
 * Gamma ray pulsars:
 */
void Starsphere::make_gammas(void) {
    // Temporary array for vertex data, to populate a vertex buffer object.
    GLfloat gamma_vertex_data[Ngammas * 3];

    // Go through the gamma pulsar position data and convert to cartesian
    // coordinates at the radius of the sphere.
    for(int i = 0; i < Ngammas; ++i) {
        // Vector for a single position.
        glm::vec3 temp = sphVertex3D(gamma_info[i][0], gamma_info[i][1], SPHERE_RADIUS);

        // Each array position is at successive locations as x, y then z
        gamma_vertex_data[i*3] = temp.x;
        gamma_vertex_data[i*3 + 1] = temp.y;
        gamma_vertex_data[i*3 + 2] = temp.z;
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Gammas")->std_string(),
                                         factory.createInstance("FragmentShader_Pass")->std_string()};

    // Instantiate a rendering task with the provided information.
    RenderTask::vertex_buffer_group v_group1 = {gamma_vertex_data,
                                                GLuint(sizeof(gamma_vertex_data)),
                                                GLuint(Ngammas),
                                                GL_STATIC_DRAW,
                                                VertexBuffer::BY_VERTEX};

    // Instantiate a rendering task with the provided information.
    m_render_task_gammas = new RenderTask(s_group1, v_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_gammas->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_gammas->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());

    m_render_task_gammas->setUniform("color", &m_gamma_color);

    m_render_task_gammas->setUniform("point_size", &m_gamma_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_gammas->acquire();

    return;
    }

/**
 * Create markers on sky sphere for LLO, LHO, GEO and VIRGO
 * IFO corner positions are from Myers' personal GPS and are +/- 100m
 */

GLfloat Starsphere::RAofZenith(double T, GLfloat LONdeg) {

//    // unix epoch at 12h  1/1/2000
//    const double T_0 = 946728000.0;
//
//    //  UT seconds of the day
//    double T_s = fmod(T, 24.0*3600.0);
//
//    // Julian centuries since 12h 1/1/2000 and 0h today
//    double T_c = (T - T_s - T_0)/3155760000.0;
//
//    // GMST at 0h today in seconds
//    double GMST0 = 6.0*3600.0 + 41.0*60.0 + 50.54841 + (8640184.812866
//            + 0.093104*T_c)*T_c;
//
//    // GMST now in seconds
//    double GMST = GMST0 + 1.002738*T_s;
//
//    // longitude defined as west positive
//    GLfloat alpha = (GMST/(24.0*3600.0))*360.0 - LONdeg;
//
//    return alpha;
    return 1.0f;
    }


/**
 * Draw the observatories at their zenith positions
 */
void Starsphere::generateObservatories(float dimFactor) {
    // sanity check
//    if(dimFactor < 0.0) dimFactor = 0.0;
//    if(dimFactor > 1.0) dimFactor = 1.0;
//
//    GLfloat Lat, Lon; // Latitute/Longitude of IFO is
//    GLfloat RAdeg, DEdeg; // converted to RA/DEC of sky sphere position
//    GLfloat radius; // radius of sphere for obs
//
//    GLfloat arm_len_deg=3.000; // lenght of arms, in degrees (not to scale)
//    GLfloat h2=0.400; // slight offset for H2 arms
//
//    // get current time and UTC offset (for zenith position)
//    m_ObservatoryDrawTimeLocal = dtime();
//    time_t local = m_ObservatoryDrawTimeLocal;
//    tm *utc = gmtime(&local);
//    double utcOffset = difftime(local, mktime(utc));
//    double observatoryDrawTimeGMT = m_ObservatoryDrawTimeLocal - utcOffset;
//
//    radius = 1.0*sphRadius; // radius of sphere on which they are drawn
//
//    float lineSize = 4.0;
//
//    /**
//     * LIGO Livingston Observatory:
//     */
//
//    Lat= 30.56377;
//    Lon= 90.77408;
//
//    RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//    DEdeg= Lat;
//
//    // delete existing, create new (required for windoze)
////    if(LLOmarker) glDeleteLists(LLOmarker, 1);
////    LLOmarker = glGenLists(1);
////    glNewList(LLOmarker, GL_COMPILE);
////
////        glColor3f(dimFactor * 0.0, dimFactor * 1.0, dimFactor * 0.0);
////        glLineWidth(lineSize);
////
////        glBegin(GL_LINE_STRIP);
////            //  North/South arm:
////            sphVertex3D(RAdeg, DEdeg-arm_len_deg, radius);
////            sphVertex3D(RAdeg, DEdeg, radius);
////            // East/West arm:
////            sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
////        glEnd();
////
////        // arm joint H2
////        glPointSize((GLfloat) lineSize);
////        glBegin(GL_POINTS);
////            sphVertex3D(RAdeg, DEdeg, radius);
////        glEnd();
////
////    glEndList();
//
//    /**
//     * LIGO Hanford Observatory: H1 and H2
//     */
//
//    Lat= 46.45510;
//    Lon= 119.40627;
//
//    RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//    DEdeg= Lat;
//
//    // delete existing, create new (required for windoze)
////    if(LHOmarker) glDeleteLists(LHOmarker, 1);
////    LHOmarker = glGenLists(1);
////    glNewList(LHOmarker, GL_COMPILE);
////
////        glColor3f(dimFactor * 0.0, dimFactor * 0.0, dimFactor * 1.0);
////        glLineWidth(lineSize);
////
////        glBegin(GL_LINE_STRIP);
////            // North/South arm:
////            sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
////            sphVertex3D(RAdeg, DEdeg, radius);
////            // East/West arm:
////            sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
////        glEnd();
////
////        glBegin(GL_LINE_STRIP);
////            // North/South arm, H2:
////            sphVertex3D(RAdeg-h2, DEdeg+arm_len_deg/2.0+h2/2.0, radius);
////            sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
////            // East/West arm, H2;
////            sphVertex3D(RAdeg-arm_len_deg/2.0-h2, DEdeg+h2/2.0, radius);
////        glEnd();
////
////        // arm joint H1
////        glPointSize((GLfloat) lineSize);
////        glBegin(GL_POINTS);
////            sphVertex3D(RAdeg, DEdeg, radius);
////        glEnd();
////
////        // arm joint H2
////        glPointSize((GLfloat) lineSize);
////        glBegin(GL_POINTS);
////            sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
////        glEnd();
////
////    glEndList();
//
//    /**
//     *  GEO600 Interferometer:
//     */
//
//    Lat= 52.24452;
//    Lon= -9.80683;
//    arm_len_deg=1.50; // not to scale
//
//    RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//    DEdeg= Lat;
//
//    // delete existing, create new (required for windoze)
////    if(GEOmarker) glDeleteLists(GEOmarker, 1);
////    GEOmarker = glGenLists(1);
////    glNewList(GEOmarker, GL_COMPILE);
////
////        glColor3f(dimFactor * 1.0, dimFactor * 0.0, dimFactor * 0.0);
////        glLineWidth(lineSize);
////
////        glBegin(GL_LINE_STRIP);
////            // North/South arm:
////            sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
////            sphVertex3D(RAdeg, DEdeg, radius);
////            // West/East arm:
////            sphVertex3D(RAdeg+arm_len_deg, DEdeg, radius);
////        glEnd();
////
////        // arm joint
////        glPointSize((GLfloat) lineSize);
////        glBegin(GL_POINTS);
////            sphVertex3D(RAdeg, DEdeg, radius);
////        glEnd();
////
////    glEndList();
//
//    /**
//     *  VIRGO Interferometer:
//     */
//
//    Lat= 43.63139;
//    Lon= -10.505;
//    arm_len_deg=3.000; // not to scale
//
//    RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//    DEdeg= Lat;
//
//    // delete existing, create new (required for windoze)
////    if(VIRGOmarker) glDeleteLists(VIRGOmarker, 1);
//    VIRGOmarker = glGenLists(1);
//    glNewList(VIRGOmarker, GL_COMPILE);
//
//        glColor3f(dimFactor * 1.0, dimFactor * 1.0, dimFactor * 1.0);
//        glLineWidth(lineSize);
//
//        glBegin(GL_LINE_STRIP);
//            // North/South arm:
//            sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
//            sphVertex3D(RAdeg, DEdeg, radius);
//            // West/East arm:
//            sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
//        glEnd();
//
//        // arm joint
//        glPointSize((GLfloat) lineSize);
//        glBegin(GL_POINTS);
//            sphVertex3D(RAdeg, DEdeg, radius);
//        glEnd();
//
//    glEndList();

    return;
    }

void Starsphere::make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size) {
//    GLfloat x, y;
//    GLfloat r1, r2, r3;
//    float theta;
//    int i, Nstep=20;
//
//    // r1 is inner circle, r2 is outer circle, r3 is crosshairs
//    r1 = size, r2=3*size, r3=4*size;

    // delete existing, create new (required for windoze)
//    if(SearchMarker) glDeleteLists(SearchMarker, 1);
//    SearchMarker = glGenLists(1);
//    glNewList(SearchMarker, GL_COMPILE);
//
//        // start gunsight drawing
//        glPushMatrix();
//
//        glLineWidth(3.0);
//        glColor3f(1.0, 0.5, 0.0); // Orange
//
//        // First rotate east  to the RA position around y
//        glRotatef(RAdeg, 0.0, 1.0, 0.0);
//        // Then rotate up to DEC position around z (not x)
//        glRotatef(DEdeg, 0.0, 0.0, 1.0);
//
//        // Inner circle
//        glBegin(GL_LINE_LOOP);
//            for (i=0; i<Nstep; i++) {
//                theta = i*360.0/Nstep;
//                x = r1*COS(theta);
//                y = r1*SIN(theta);
//                sphVertex(x, y);
//            }
//        glEnd();
//
//        // Outer circle
//        glBegin(GL_LINE_LOOP);
//            for (i=0; i<Nstep; i++) {
//                theta = i*360.0/Nstep;
//                x = r2*COS(theta);
//                y = r2*SIN(theta);
//                sphVertex(x, y);
//            }
//        glEnd();
//
//        // Arms that form the gunsight
//        glBegin(GL_LINES);
//            //  North arm:
//            sphVertex(0.0, +r1);
//            sphVertex(0.0, +r3);
//            //  South arm:
//            sphVertex(0.0, -r1);
//            sphVertex(0.0, -r3);
//            // East arm:
//            sphVertex(-r1, 0.0);
//            sphVertex(-r3, 0.0);
//            // West arm:
//            sphVertex(+r1, 0.0);
//            sphVertex(+r3, 0.0);
//        glEnd();
//
//        glPopMatrix();
//
//        // searchlight line out to marker (OFF!)
//        if(false) {
//            glBegin(GL_LINES);
//                sphVertex3D(RAdeg, DEdeg, 0.50*sphRadius);
//                sphVertex3D(RAdeg, DEdeg, 0.95*sphRadius);
//            glEnd();
//        }
//
//    glEndList();
    }

/**
 * XYZ coordinate axes: (if we want them - most useful for testing)
 */
void Starsphere::make_axes(void) {
//    GLfloat axl=10.0;

    // delete existing, create new (required for windoze)
//    if(Axes) glDeleteLists(Axes, 1);
//    Axes = glGenLists(1);
//    glNewList(Axes, GL_COMPILE);
//
//        glLineWidth(2.0);
//
//        glBegin(GL_LINES);
//            glColor3f(1.0, 0.0, 0.0);
//            glVertex3f(-axl, 0.0, 0.0);
//            glVertex3f(axl, 0.0, 0.0);
//
//            glColor3f(0.0, 1.0, 0.0);
//            glVertex3f(0.0, -axl, 0.0);
//            glVertex3f(0.0, axl, 0.0);
//
//            glColor3f(0.0, 0.0, 1.0);
//            glVertex3f(0.0, 0.0, -axl);
//            glVertex3f(0.0, 0.0, axl);
//        glEnd();
//
//    glEndList();
    }

/**
 * RA/DEC coordinate grid on the sphere
 */
void Starsphere::make_globe_mesh_lat_long(void) {
    // What are the steps in latitude and longitude for this globe? Decimal degrees.
    GLfloat LAT_STEP = 180.0f/(GLOBE_LATITUDE_LAYERS - 1);
    GLfloat LONG_STEP = 360.0f/GLOBE_LONGITUDE_SLICES;

    // Populate a vertex array.
    // Calculate the number of vertices. This is a full number of longitudinal slices for
    // each non-pole latitude layer, plus one for each pole.
    GLuint num_vertices = ((GLOBE_LATITUDE_LAYERS - 2) * GLOBE_LONGITUDE_SLICES) + 2;
    m_globe_vertices = num_vertices;

    // Allocate a temporary array for vertex positions in 3D ie. each has
    // an x, y and z component.
    GLfloat globe_vertex_data[num_vertices*3];

    // Do the North Pole vertex first. This will be at RA = 0, DEC = +90 times radius.
    // Set & remember the index of the North Pole.
    GLuint north_pole_index = 0;
    // Set and remember the index of the current vertex.
    GLuint vertex_counter = 0;
    GLfloat globe_radius = SPHERE_RADIUS*(1.02);
    glm::vec3 north_pole = sphVertex3D(0, 90, globe_radius);
    globe_vertex_data[north_pole_index*3] = north_pole.x;
    globe_vertex_data[north_pole_index*3 + 1] = north_pole.y;
    globe_vertex_data[north_pole_index*3 + 2] = north_pole.z;


    // For each non-pole latitude layer.
    for(GLuint lat_layer = 1; lat_layer < (GLOBE_LATITUDE_LAYERS - 1); ++lat_layer ){
    	// For each longitude layer.
    	for(GLuint long_slice = 0; long_slice < GLOBE_LONGITUDE_SLICES; ++long_slice){
    		// NB sphVertex3D() measures declination above/below equator, but here
    		// latitude is measured from the North Pole = 0 downwards to Equator = 90
    		// and then South Pole = 180.
    		glm::vec3 globe_vertex = sphVertex3D(long_slice*LONG_STEP, 90 - lat_layer*LAT_STEP, globe_radius);
    		++vertex_counter;
    		globe_vertex_data[vertex_counter*3] = globe_vertex.x;
    		globe_vertex_data[vertex_counter*3 + 1] = globe_vertex.y;
    		globe_vertex_data[vertex_counter*3 + 2] = globe_vertex.z;
    		}
    	}

    // Do the South Pole vertex last. This will be at RA = 0, DEC = -90 times radius.
    // Set & remember the index of the South Pole.
    ++vertex_counter;
    GLuint south_pole_index = vertex_counter;
    glm::vec3 south_pole = sphVertex3D(0, -90, globe_radius);
    globe_vertex_data[south_pole_index*3] = south_pole.x;
    globe_vertex_data[south_pole_index*3 + 1] = south_pole.y;
    globe_vertex_data[south_pole_index*3 + 2] = south_pole.z;

    // Populate an index array.
    // Calculate the number of line segments to render.
    // For latitude layers this is one for each step in longitude ie. GLOBE_LONGITUDE_SLICES.
    // For longitude slices this is one for each step in latitude ie. (GLOBE_LATITUDE_SLICES - 1).
    m_globe_lines = GLOBE_LONGITUDE_SLICES * (GLOBE_LATITUDE_LAYERS - 1)
                    + (GLOBE_LATITUDE_LAYERS - 2) * GLOBE_LONGITUDE_SLICES;

	// Allocate a temporary array for vertex buffer indices. Note the array
    // type is suitable for indices ie. unsigned integer. The GL_LINES enumerant is to be used at
    // rendering time.
    GLuint globe_index_data[m_globe_lines*2];

    // Keep track of which entry in the indicial array we are up to.
    GLuint indicial_index = 0;

    // Do longitudinal slices. These are semi-circles.
    for(GLuint longitude = 0; longitude < GLOBE_LONGITUDE_SLICES; ++longitude) {
        // Start at the north pole.
        globe_index_data[indicial_index] = north_pole_index;
        ++indicial_index;
        // For each point of latitude intermediate b/w poles.
        for(GLuint latitude = 1; latitude < (GLOBE_LATITUDE_LAYERS - 1); ++latitude) {
            // Point on first latitude slice on zero meridian.
            GLuint temp = north_pole_index + 1;
            // Point on first latitude slice on correct longitude.
            temp += longitude;
            // Now move down to correct latitude.
            temp += (latitude - 1) * (GLOBE_LONGITUDE_SLICES);
            // End of one line segment.
            globe_index_data[indicial_index] = temp;
            ++indicial_index;
            // Begining of next line segment.
            globe_index_data[indicial_index] = temp;
            ++indicial_index;
            }
        // End at the south pole.
        globe_index_data[indicial_index] = south_pole_index;
        ++indicial_index;
        }

    // Do latitudinal layers for non polar latitudes.
    for(GLuint latitude = 1; latitude < (GLOBE_LATITUDE_LAYERS - 1); ++latitude) {
        // Start at longitude zero, first layer.
        GLuint starter = 1;
        // Now move down to correct layer, longitude zero.
        starter += (latitude - 1) * GLOBE_LONGITUDE_SLICES;
        // First point in layer.
        globe_index_data[indicial_index] = starter;
        ++indicial_index;
        // Step along longitudes in current layer.
        for(GLuint longitude = 1; longitude < GLOBE_LONGITUDE_SLICES; ++longitude) {
            GLuint temp = starter + longitude;
            // End of one line segment.
            globe_index_data[indicial_index] = temp;
            ++indicial_index;
            // Begining of next line segment.
            globe_index_data[indicial_index] = temp;
            ++indicial_index;
            }
        // Finish back at first point of layer.
        globe_index_data[indicial_index] = starter;
        // Check as no more indices at the final line segment.
        if(latitude != (GLOBE_LATITUDE_LAYERS - 2)) {
            ++indicial_index;
            }
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Stars")->std_string(),
    		                             factory.createInstance("FragmentShader_Pass")->std_string()};

    // Populate data structure for vertices.
    RenderTask::vertex_buffer_group v_group1 = {globe_vertex_data,
    		 	 	 	 	 	 	 	 	 	GLuint(sizeof(globe_vertex_data)),
												num_vertices,
    		                                    GL_STATIC_DRAW,
    		                                    VertexBuffer::BY_VERTEX};
    // Populate data structure for indices.
    RenderTask::index_buffer_group i_group1 = {globe_index_data,
        		 	 	 	 	 	 	 	   GLuint(sizeof(globe_index_data)),
    										   m_globe_lines*2,
        		                               GL_STATIC_DRAW,
											   GL_UNSIGNED_INT};

	// Instantiate a rendering task with the provided information.
    m_render_task_globe = new RenderTask(s_group1, v_group1, i_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_globe->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_globe->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());;
    m_render_task_globe->setUniform("color", &m_globe_color);
    m_render_task_globe->setUniform("point_size", &m_globe_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_globe->acquire();
    }

/**
 * Globe as a texture covered mesh. The Earth actually.
 */

void Starsphere::make_globe_mesh_texture(void) {
    // What are the steps in latitude and longitude for this globe? Decimal degrees.
    GLfloat LAT_STEP = 180.0f/(GLOBE_LATITUDE_LAYERS - 1);
    GLfloat LONG_STEP = 360.0f/GLOBE_LONGITUDE_SLICES;
    GLfloat LAT_TEXTURE_STEP = 1.0f/(GLOBE_LATITUDE_LAYERS - 1);
    GLfloat LONG_TEXTURE_STEP = 1.0f/GLOBE_LONGITUDE_SLICES;
    GLuint COORDS_PER_VERTEX = 5;
    GLuint EARTH_TEXTURE_WIDTH = 2048;
    GLuint EARTH_TEXTURE_HEIGHT = 1024;
    GLuint EARTH_TEXTURE_COLOR_DEPTH = 3;

    // What is the radius compared to the celestial sphere.
    GLfloat EARTH_RADIUS = SPHERE_RADIUS * 0.4;

    // Populate a vertex array.
    // Calculate the number of vertices. This is a full number of longitudinal slices for
    // each latitude layer, including each pole.
    GLuint NUM_VERTICES = GLOBE_LATITUDE_LAYERS * (GLOBE_LONGITUDE_SLICES + 1);

    m_earth_triangles = GLOBE_LATITUDE_LAYERS * GLOBE_LONGITUDE_SLICES * 2;

    // Allocate a temporary array for vertex positions in 3D ie. each has
    // an x, y and z component. Plus two texture coordinates.
    GLfloat globe_vertex_data[NUM_VERTICES*COORDS_PER_VERTEX];

    // Do the North Pole vertex first. There is a coordinate singularity at
    // RA = 0, DEC = +90 times radius.
    // Set and remember the index of the current vertex.
    GLuint vertex_counter = 0;

    // For each latitude layer.
    for(GLuint latitude_layer = 0; latitude_layer < GLOBE_LATITUDE_LAYERS; ++latitude_layer ){
        // For each longitude layer.
    	for(GLuint longitudinal_slice = 0; longitudinal_slice < GLOBE_LONGITUDE_SLICES +1; ++longitudinal_slice){
    		// NB sphVertex3D() measures declination above/below equator, but here
    		// latitude is measured from the North Pole = 0 downwards to Equator = 90
    		// and then South Pole = 180.
    		GLfloat temp = float(longitudinal_slice);
    		if(longitudinal_slice == GLOBE_LONGITUDE_SLICES){
                // Line of longitude at 360 degrees is equivalent in
                // position to the line at zero degrees.
                temp = 0.0f;
                }
    		glm::vec3 globe_vertex = sphVertex3D(temp*LONG_STEP, 90 - latitude_layer*LAT_STEP, EARTH_RADIUS);
    		globe_vertex_data[vertex_counter*COORDS_PER_VERTEX] = globe_vertex.x;
    		globe_vertex_data[vertex_counter*COORDS_PER_VERTEX + 1] = globe_vertex.y;
    		globe_vertex_data[vertex_counter*COORDS_PER_VERTEX + 2] = globe_vertex.z;
    		// Line of 360 degrees longitude has horizontal texture coordinate of 1.0f
    		globe_vertex_data[vertex_counter*COORDS_PER_VERTEX + 3] = float(longitudinal_slice)*LONG_TEXTURE_STEP;
    		globe_vertex_data[vertex_counter*COORDS_PER_VERTEX + 4] = 1.0f - latitude_layer*LAT_TEXTURE_STEP;
    		++vertex_counter;
    		}
    	}

    // Allocate a temporary array for vertex buffer indices. Note the array
    // type is suitable for indices ie. unsigned integer. The GL_TRIANGLES enumerant is to be used at
    // rendering time.
    GLuint globe_index_data[m_earth_triangles*VERTICES_PER_TRIANGLE];

    // Keep track of which entry in the indicial array we are up to.
    GLuint indicial_index = 0;

    for(GLuint latitude_layer = 0; latitude_layer < GLOBE_LATITUDE_LAYERS; ++latitude_layer) {
        for(GLuint longitude = 0; longitude < GLOBE_LONGITUDE_SLICES; ++longitude) {
            globe_index_data[indicial_index] = longitude + latitude_layer*GLOBE_LONGITUDE_SLICES;
            ++indicial_index;
            globe_index_data[indicial_index] = (latitude_layer + 1)*GLOBE_LONGITUDE_SLICES + longitude;
            ++indicial_index;
            globe_index_data[indicial_index] = (latitude_layer + 1)*GLOBE_LONGITUDE_SLICES + longitude + 1;
            ++indicial_index;

            globe_index_data[indicial_index] = longitude + latitude_layer*GLOBE_LONGITUDE_SLICES;
            ++indicial_index;
            globe_index_data[indicial_index] = (latitude_layer + 1)*GLOBE_LONGITUDE_SLICES + longitude + 1;
            ++indicial_index;
            globe_index_data[indicial_index] = latitude_layer*GLOBE_LONGITUDE_SLICES + longitude + 1;
            ++indicial_index;
            }
        }

    // Create factory instance to then access the shader strings.
    ResourceFactory factory;

    // Populate data structure indicating GLSL code use.
    RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Texture")->std_string(),
    		                             factory.createInstance("FragmentShader_Texture")->std_string()};

    // Populate data structure for vertices.
    RenderTask::vertex_buffer_group v_group1 = {globe_vertex_data,
    		 	 	 	 	 	 	 	 	 	GLuint(sizeof(globe_vertex_data)),
												NUM_VERTICES,
    		                                    GL_STATIC_DRAW,
    		                                    VertexBuffer::BY_VERTEX};
    // Populate data structure for indices.
    RenderTask::index_buffer_group i_group1 = {globe_index_data,
        		 	 	 	 	 	 	 	   GLuint(sizeof(globe_index_data)),
    										   m_earth_triangles*VERTICES_PER_TRIANGLE,
        		                               GL_STATIC_DRAW,
											   GL_UNSIGNED_INT};

    // To get at the underlying texture data from a Resource instance, then cast it void ...
	RenderTask::texture_buffer_group t_group1 =	{(const GLvoid*)factory.createInstance("Earthmap")->data()->data(),
                                                EARTH_TEXTURE_WIDTH*EARTH_TEXTURE_HEIGHT*EARTH_TEXTURE_COLOR_DEPTH,
                                                EARTH_TEXTURE_WIDTH,
                                                EARTH_TEXTURE_HEIGHT,
                                                GL_RGB,
                                                GL_UNSIGNED_BYTE,
                                                GL_REPEAT,
                                                GL_REPEAT,
                                                true};

	// Instantiate a rendering task with the provided information.
    m_render_task_earth = new RenderTask(s_group1, v_group1, i_group1, t_group1);

    // For vertex input need to correlate with vertex shader code.
    m_render_task_earth->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});
    m_render_task_earth->addSpecification({1, "aTexCoord", 2, GL_FLOAT, GL_FALSE});

    // For program uniforms need client side pointers.
    m_render_task_earth->setUniform("CameraMatrix", TransformGlobals::getCameraTransformMatrix());;
    m_render_task_earth->setUniform("point_size", &m_globe_point_size);

    // Claim all required state machine resources for this rendering task.
    m_render_task_earth->acquire();

    }

/**
 * Window resize/remap
 */
void Starsphere::resize(const int width, const int height) {
    // Store current settings. Units are pixels.
    m_CurrentWidth = width;
    m_CurrentHeight = height;

    // Filter out non-positivity in either screen dimension.
    if((m_CurrentHeight > 0) && (m_CurrentWidth > 0)) {
        // Dimensions acceptable.
        m_aspect = (float)m_CurrentWidth / (float)m_CurrentHeight;
        std::stringstream msg1;
		msg1 << "Starsphere::resize() : m_aspect = " << m_CurrentWidth
			<< "/" << m_CurrentHeight << " = " << m_aspect;
		ErrorHandler::record(msg1.str(), ErrorHandler::INFORM);
		}
    else {
        // Negative or zero for one or both of width and height.
        std::stringstream msg2;
        msg2 << "Starsphere::resize() : screen height = " << m_CurrentHeight
            << " and width = " << m_CurrentWidth;
        ErrorHandler::record(msg2.str(), ErrorHandler::WARN);
        ErrorHandler::record("Starsphere::resize() : invalid screen dimensions!", ErrorHandler::FATAL);
        }

    // Remember screen dimensions in global state class.
    TransformGlobals::setClientScreenDimensions(height, width);

    // Adjust HUD config.
    m_YStartPosTop = m_CurrentHeight - 25;

    // Make sure the search marker is updated (conditional rendering!)
    m_RefreshSearchMarker = true;

    // Adjust aspect ratio and projection.
    glViewport(0, 0, m_CurrentWidth, m_CurrentHeight);
    configTransformMatrices();
    }

/**
 *  What to do when graphics are "initialized".
 */
void Starsphere::initialize(const int width, const int height, const Resource* font) {
    // Remember screen dimensions in global state class.
    TransformGlobals::setClientScreenDimensions(height, width);

    // Remember transform matrix locations in global state class.
    TransformGlobals::setCameraTransformMatrix(&m_camera);
    TransformGlobals::setOrthographicTransformMatrix(&m_orthographic_projection);

    // Setup initial dimensions.
    m_CurrentWidth = width;
    m_CurrentHeight = height;
    resize(m_CurrentWidth, m_CurrentHeight);

    m_FontResource = font;

    // Fatal error if no font resource supplied.
    if(!m_FontResource) {
        ErrorHandler::record("Starsphere::initialize() - no fontResource supplied!", ErrorHandler::WARN);
        }
    else {
        // NB : initialization of logo font instances is done in subclasses!
        // Create header and text font instances using font resource,
        // if not done already.
        if(m_FontHeader == NULL) {
            m_FontHeader = TTF_OpenFontRW(SDL_RWFromConstMem(&m_FontResource->data()->at(0),
                                                             m_FontResource->data()->size()),
                                          TTF_FREE_SOURCE,
                                          TTF_FONT_LOAD_HEADER_POINT_SIZE);

            if(m_FontHeader == NULL) {
                std::stringstream font_header_error;
                font_header_error << "Starsphere::initialize() : "
                                  << "Could not construct header font face from in memory resource!"
                                  << std::endl;
                ErrorHandler::record(font_header_error.str(), ErrorHandler::FATAL);
                }
            }

        if(m_FontText == NULL) {
            m_FontText = TTF_OpenFontRW(SDL_RWFromConstMem(&m_FontResource->data()->at(0),
                                                           m_FontResource->data()->size()),
                                        TTF_FREE_SOURCE,
                                        TTF_FONT_LOAD_TEXT_POINT_SIZE);

            if(m_FontText == NULL) {
                std::stringstream font_text_error;
                font_text_error << "Starsphere::initialize() : "
                                << "Could not construct text font face from in memory resource!"
                                << std::endl;
                ErrorHandler::record(font_text_error.str(), ErrorHandler::FATAL);
                }
            }
        }

    // Create rendering tasks for given features.
    make_constellations();
    make_gammas();
    make_globe_mesh_lat_long();
    make_globe_mesh_texture();
    make_pulsars();
    make_snrs();
    make_stars();

    // Begin with these visual features enabled.
    setFeature(CONSTELLATIONS, true);
    setFeature(OBSERVATORIES, true);
    setFeature(XRAYS, true);            /// TODO - we don't have this feature even designed yet.
    setFeature(PULSARS, true);
    setFeature(SNRS, true);
    setFeature(GAMMAS, true);
    setFeature(GLOBE, true);
    setFeature(AXES, false);
    setFeature(SEARCHINFO, true);
    setFeature(LOGO, true);
    setFeature(MARKER, true);
    setFeature(EARTH, true);
    setFeature(ROLL_STOP, true);

    // Here we set the byte alignment when unpacking data from memory
    // to a one byte boundary. This done mainly for optimizing font setup.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glActiveTexture(GL_TEXTURE0);

    // In space the background is black.
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Enable depth buffering for 3D graphics
    glClearDepth(DEFAULT_CLEAR_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // We need alpha blending for proper font rendering
    // and other anti-aliased objects.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Default point size.
    glPointSize(DEFAULT_POINT_SIZE);
    // However, currently point size in managed within vertex shaders using gl_PointSize.
    // Un-REM the following line to enable the above point size to apply.
    // glEnable(GL_PROGRAM_POINT_SIZE);

    // Default line width.
    glLineWidth(DEFAULT_LINE_WIDTH);

    // Some selected drawing quality choices.
    glEnable(GL_POINT_SMOOTH);                            // Jaggy reduction, but this per primitve
    glEnable(GL_LINE_SMOOTH);                            // anti-aliasing depends on hardware etc ...
    // glEnable(GL_CULL_FACE);                                // Culling of rear faces
    glFrontFace(GL_CCW);                                // Front facing is counterclockwise

    /// TODO - sort out this quality selection business ?
    // enable opt-in quality feature
//    if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
//        /// TODO - what OpenGL ES 2.x quality options available? In the shader code then ?
//        }
//
//    // enable opt-in quality feature
//    if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::MediumGraphicsQualitySetting ||
//       m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
//        // fog aids depth perception
//        /// TODO - what OpenGL ES 2.x fog options available? In the shader code then ?
//        }
    }

/**
 * Rendering routine:  this is what does the drawing:
 */
void Starsphere::render(const double timeOfDay) {
//    GLfloat xvp, yvp, zvp, vp_theta, vp_phi, vp_rad;
//    GLfloat Zrot = 0.0, Zobs=0.0;
//    double revs, t, dt = 0;
//    static double start_time=-1.0, last_time=-1.0;
//
//    // Calculate the real time t since we started (or reset) and the
//    // time dt since the last render() call.    Both may be useful
//    // for timing animations.  Note that time_of_day is dtime().
//
//    if (start_time < 0.0)
//        start_time = timeOfDay;
//    t = timeOfDay - start_time;
//
//    if (last_time < 0.0)
//        last_time = timeOfDay - 0.01;
//    dt = timeOfDay - last_time;
//
//    last_time = timeOfDay; // remember for next time
//
//    // Now determine the rotation angle based on the time since start
//    // It is negative to get the rotation direction correct (the sun
//    // rises in the East, so the sky sphere rotates E to W).
//
//    Zrot = t*rotation_speed/60.0;
//    revs = Zrot/360.0;
//    Zrot = -360.0 * (revs - (int)revs);

    // Start drawing with clearing the relevant buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // now draw the scene...

    // Vary the viewpoint with both a long period wobble of the elevation
    // of the view and a longer period zoom in/out that might even penetrate
    // The starsphere for a brief time.   Increase the power in pow(,) to
    // make the visit inside briefer.
//    vp_theta = 90.0 - viewpt_elev + wobble_amp*sin(PI2*t/(wobble_period*60.0));
//    vp_phi = viewpt_azimuth;
//    vp_rad = viewpt_radius - zoom_amp*sin(PI2*t/(zoom_period*60.0));
//    if(vp_rad <0.0) vp_rad = 0.0; // cannot pass origin (confusing)
//
//    // TRIED THIS TOO: -zoom_amp*pow(fabs(sin(PI2*t/(zoom_period*60.0))),3);
//    xvp = vp_rad * SIN(vp_theta) * SIN(vp_phi);
//    zvp = vp_rad * SIN(vp_theta) * COS(vp_phi);
//    yvp = vp_rad * COS(vp_theta);

//      gluLookAt(0.0, 0.0, 5.0, // eyes position

//              0.0, 0.0, 0.0, // looking toward here
//                0.0, 1.0, 0.0); // which way is up?  y axis!

    GLuint stagger = m_framecount % 300;

    if(stagger == 100) {
        m_axis = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    else if(stagger == 200){
        m_axis = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    else if(stagger == 0) {
        m_axis = glm::vec3(1.0f, 0.0f, 0.0f);
        }

    m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -viewpt_radius));

    if(!isFeature(ROLL_STOP)) {
        m_rotation = glm::rotate(m_rotation, 0.01f, m_axis);
        }

    m_camera = m_perspective_projection * m_view * m_rotation;

    /// TODO - place this block correctly
    // draw axes before any rotation so they stay put.
    if(isFeature(AXES)) {
        /// TODO - call to render axes;
        }

    // stars, pulsars, supernovae, grid
    if(isFeature(GAMMAS)) {
        m_render_task_gammas->trigger(GL_POINTS, Ngammas);
        }
    if(isFeature(PULSARS)) {
        m_render_task_psr->trigger(GL_POINTS, Npulsars);
        }
    if(isFeature(SNRS)) {
        m_render_task_snr->trigger(GL_POINTS, NSNRs);
        }
    if(isFeature(STARS)) {
        m_render_task_star->trigger(GL_POINTS, m_distinct_stars);
        }
    if(isFeature(CONSTELLATIONS)) {
        m_render_task_cons->trigger(GL_LINES, m_constellation_lines*2);
        }
    if(isFeature(GLOBE)) {
        m_render_task_globe->trigger(GL_LINES, m_globe_lines*2);
        }
    if(isFeature(EARTH)) {
        m_render_task_earth->trigger(GL_TRIANGLES, m_earth_triangles*VERTICES_PER_TRIANGLE);
        }

    // observatories move an extra 15 degrees/hr since they were drawn
    if(isFeature(OBSERVATORIES)) {
        /// TODO - call to render observatories;
//        glPushMatrix();
//        Zobs = (timeOfDay - m_ObservatoryDrawTimeLocal) * 15.0/3600.0;
//        glRotatef(Zobs, 0.0, 1.0, 0.0);
//        glCallList(LLOmarker);
//        glCallList(LHOmarker);
//        glCallList(GEOmarker);
//        glCallList(VIRGOmarker);
//        renderAdditionalObservatories();
//        glPopMatrix();
        }

    // draw the search marker (gunsight)
    if(isFeature(MARKER)) {
        /// TODO - call to render gunsight;
//        if(m_RefreshSearchMarker) {
//            make_search_marker(m_CurrentRightAscension, m_CurrentDeclination, 0.5);
//            m_RefreshSearchMarker = false;
//        }
//        else {
//            glCallList(SearchMarker);
//        }
        }


    // draw 2D vectorized HUD
    if(isFeature(LOGO) || isFeature(SEARCHINFO)) {
        // Disable depth testing since we're in 2D mode
        glDisable(GL_DEPTH_TEST);

        //if (isFeature(LOGO)) renderLogo();
        // if (isFeature(SEARCHINFO)) renderSearchInformation();

        // Enable depth testing since we're leaving 2D mode
        glEnable(GL_DEPTH_TEST);
        }

    // Mark off another frame done.
    ++m_framecount;
    }

void Starsphere::renderAdditionalObservatories() {
    // default implementation doesn't do anything
    }

void Starsphere::mouseButtonEvent(const int positionX, const int positionY,
                                  const AbstractGraphicsEngine::MouseButton buttonPressed) {
    }

void Starsphere::mouseMoveEvent(const int deltaX, const int deltaY,
                                const AbstractGraphicsEngine::MouseButton buttonPressed) {
    switch(buttonPressed) {
        case MouseButtonLeft:
            rotateSphere(deltaX, deltaY);
            break;
        case MouseButtonRight:
            zoomSphere(deltaY);
            break;
        default:
            break;
        }
    }

void Starsphere::mouseWheelEvent(const int pos) {
    // Mouse wheel sets angle of field of view for perspective transform.3.0
    if(pos > 0) {
        viewpt_fov += VIEWPOINT_MOUSEWHEEL_FOV_RATE;
        if(viewpt_fov > PERSPECTIVE_FOV_MAX) {
            viewpt_fov = PERSPECTIVE_FOV_MAX;
            }
        }
    else if (pos < 0) {
        viewpt_fov -= VIEWPOINT_MOUSEWHEEL_FOV_RATE;
        if(viewpt_fov < PERSPECTIVE_FOV_MIN) {
            viewpt_fov = PERSPECTIVE_FOV_MIN;
            }
        }
    // Re-compute transform matrix with new field of view angle.
    configTransformMatrices();
    }

void Starsphere::keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed) {
    switch(keyPressed) {
        case KeyS:
            setFeature(STARS, isFeature(STARS) ? false : true);
            break;
        case KeyC:
            setFeature(CONSTELLATIONS, isFeature(CONSTELLATIONS) ? false : true);
            break;
        case KeyO:
            setFeature(OBSERVATORIES, isFeature(OBSERVATORIES) ? false : true);
            break;
        case KeyX:
            setFeature(XRAYS, isFeature(XRAYS) ? false : true);
            break;
        case KeyP:
            setFeature(PULSARS, isFeature(PULSARS) ? false : true);
            break;
        case KeyR:
            setFeature(SNRS, isFeature(SNRS) ? false : true);
            break;
        case KeyG:
            setFeature(GLOBE, isFeature(GLOBE) ? false : true);
            break;
        case KeyA:
            setFeature(AXES, isFeature(AXES) ? false : true);
            break;
        case KeyI:
            setFeature(SEARCHINFO, isFeature(SEARCHINFO) ? false : true);
            break;
        case KeyL:
            setFeature(LOGO, isFeature(LOGO) ? false : true);
            break;
        case KeyM:
            setFeature(MARKER, isFeature(MARKER) ? false : true);
            break;
        case KeyU:
            setFeature(GAMMAS, isFeature(GAMMAS) ? false : true);
            break;
        case KeyE:
            setFeature(EARTH, isFeature(EARTH) ? false : true);
            break;
        case KeyH:
            setFeature(ROLL_STOP, isFeature(ROLL_STOP) ? false : true);
            break;
        default:
            break;
        }
    }

/**
 * View control
 */
void Starsphere::rotateSphere(const int relativeRotation,
                              const int relativeElevation) {
    // elevation
    viewpt_elev += relativeElevation/10.0;
    if (viewpt_elev > 89.0)
        viewpt_elev = +89.0;
    if (viewpt_elev < -89.0)
        viewpt_elev = -89.0;

    // rotation
    rotation_offset -= relativeRotation/10.0;
    }

void Starsphere::zoomSphere(const int relativeZoom) {
    // zoom
    viewpt_radius -= relativeZoom/VIEWPOINT_ZOOM_RATE;
    if (viewpt_radius > VIEWPOINT_MAX_ZOOM)
        viewpt_radius = VIEWPOINT_MAX_ZOOM;
    if (viewpt_radius < VIEWPOINT_MIN_ZOOM)
        viewpt_radius = VIEWPOINT_MIN_ZOOM;
    }

void Starsphere::configTransformMatrices(void) {
    // Create desired perspective projection matrix based upon a frustum model.
    m_perspective_projection = glm::perspective(viewpt_fov,
                                    m_aspect,
                                    PERSPECTIVE_NEAR_FRUSTUM_DISTANCE,
                                    PERSPECTIVE_FAR_FRUSTUM_DISTANCE);

    m_perspective_projection[3][3]= 1.0f;

    std::cout << "m_perspective_projection = [["
              << m_perspective_projection[0][0] << ", "
              << m_perspective_projection[0][1] << ", "
              << m_perspective_projection[0][2] << ", "
              << m_perspective_projection[0][3] << "], ["
              << m_perspective_projection[1][0] << ", "
              << m_perspective_projection[1][1] << ", "
              << m_perspective_projection[1][2] << ", "
              << m_perspective_projection[1][3] << "], ["
              << m_perspective_projection[2][0] << ", "
              << m_perspective_projection[2][1] << ", "
              << m_perspective_projection[2][2] << ", "
              << m_perspective_projection[2][3] << "], ["
              << m_perspective_projection[3][0] << ", "
              << m_perspective_projection[3][1] << ", "
              << m_perspective_projection[3][2] << ", "
              << m_perspective_projection[3][3] << "]]"
              << std::endl;

    // Create desired orthographic projection matrix based upon a frustum model.
    m_orthographic_projection = glm::ortho(GLfloat(0), GLfloat(TransformGlobals::getClientScreenWidth()),
                                           GLfloat(0), GLfloat(TransformGlobals::getClientScreenHeight()));

    std::cout << "m_orthographic_projection = [["
              << m_orthographic_projection[0][0] << ", "
              << m_orthographic_projection[0][1] << ", "
              << m_orthographic_projection[0][2] << ", "
              << m_orthographic_projection[0][3] << "], ["
              << m_orthographic_projection[1][0] << ", "
              << m_orthographic_projection[1][1] << ", "
              << m_orthographic_projection[1][2] << ", "
              << m_orthographic_projection[1][3] << "], ["
              << m_orthographic_projection[2][0] << ", "
              << m_orthographic_projection[2][1] << ", "
              << m_orthographic_projection[2][2] << ", "
              << m_orthographic_projection[2][3] << "], ["
              << m_orthographic_projection[3][0] << ", "
              << m_orthographic_projection[3][1] << ", "
              << m_orthographic_projection[3][2] << ", "
              << m_orthographic_projection[3][3] << "]]"
              << std::endl;
    }

/**
 * Feature control
 */
void Starsphere::setFeature(const Features feature, const bool enable) {
    featureFlags = enable ? (featureFlags | feature) : (featureFlags & ~feature);
    }

bool Starsphere::isFeature(const Features feature) {
    return ((featureFlags & feature) == feature ? true : false);
    }

void Starsphere::refreshLocalBOINCInformation() {
    // call base class implementation
    AbstractGraphicsEngine::refreshLocalBOINCInformation();

    // prepare conversion buffer
    std::stringstream buffer;
    buffer.precision(2);
    buffer.setf(ios::fixed, ios::floatfield);
    buffer.fill('0');
    buffer.setf(ios::right, ios::adjustfield);

    // store content required for our HUD (user info)
    m_UserName = "User: " + m_BoincAdapter.userName();
    m_TeamName = "Team: " + m_BoincAdapter.teamName();

    buffer << "Project Credit: " << fixed << m_BoincAdapter.userCredit() << ends;
    m_UserCredit = buffer.str();
    buffer.str("");

    buffer << "Project RAC: " << fixed << m_BoincAdapter.userRACredit() << ends;
    m_UserRACredit = buffer.str();
    buffer.str("");
    }
