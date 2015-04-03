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
#include "VertexBuffer.h"
#include "VertexFetch.h"
#include "VertexShader.h"

Starsphere::Starsphere(string sharedMemoryAreaIdentifier) :
	AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
	m_framecount = 0;

	m_render_task_snr = NULL;

	m_rotation = glm::mat4(1.0);
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
	aspect = 0;
	m_XStartPosLeft = 0;
	m_YStartPosTop = 0;
	m_YOffsetLarge = 0;
	m_CurrentRightAscension = 0;
	m_CurrentDeclination = 0;
	m_RefreshSearchMarker = false;

	Axes=0, Stars=0, Constellations=0, Pulsars=0;
	LLOmarker=0, LHOmarker=0, GEOmarker=0, VIRGOmarker=0;
	sphGrid=0, SNRs=0, SearchMarker=0;

	/**
	 * Parameters and State info
	 */
	sphRadius = 5.5;
	featureFlags = 0;

	/**
	 * Viewpoint (can be changed with mouse)
	 */
	viewpt_azimuth = 30.0;
	viewpt_elev = 23.6;
	viewpt_radius = 7.6;

	wobble_amp = 37.0;
	wobble_period = 17.0;
	zoom_amp = 0.00;
	zoom_period = 29.0;

	rotation_offset = 0.0;
	rotation_speed = 180.0;

	m_CurrentRightAscension = -1.0;
	m_CurrentDeclination = -1.0;
	m_RefreshSearchMarker = true;
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
	if(m_render_task_snr) delete m_render_task_snr;
	}

glm::vec3 Starsphere::sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius) {
	GLfloat x = radius * COS(DEdeg) * COS(RAdeg);
	GLfloat z = -radius * COS(DEdeg) * SIN(RAdeg);
	GLfloat y = radius * SIN(DEdeg);

	return glm::vec3(x,y,z);
    }
void Starsphere::sphVertex(GLfloat RAdeg, GLfloat DEdeg) {
//	sphVertex3D(RAdeg, DEdeg, sphRadius);
    }

/**
 * Star Marker:
 * Makes a marker for one star at a given position and angular size.
 */
void Starsphere::star_marker(float RAdeg, float DEdeg, float size) {
//	glPointSize((GLfloat) size);
//	glBegin(GL_POINTS);
//	sphVertex((GLfloat) RAdeg, (GLfloat) DEdeg);
//	glEnd();
	return;
    }

/**
 *  Create Stars: markers for each star
 */
void Starsphere::make_stars() {
//	GLfloat mag_size;
//	int i, j;
//	bool is_dupe;
//	int Ndupes=0;

//		glColor3f(1.0, 1.0, 1.0);
//
//		/**
//		 * At some point in the future star_info[][] will also contain
//		 * star magnitude and the marker size will vary with this.
//		 */
//		for (i=0; i < Nstars; i++) {
//			// same stars appear more than once in constallations so ignore dupes
//			is_dupe=false;
//			for (j=0; j< i; j++) {
//				if (star_info[j][0] == star_info[i][0] && star_info[j][0]
//				        == star_info[i][0]) {
//					is_dupe=true;
//					Ndupes++;
//					break;
//				}
//			}
//			if (!is_dupe) {
//				// mag_size = 0.05 + 0.50*rand()/RAND_MAX;
//				mag_size = 4.0;
//				star_marker(star_info[i][0], star_info[i][1], mag_size);
//			}
//		}
    }

/**
 *  Pulsar Markers:
 */
void Starsphere::make_pulsars() {
//	GLfloat mag_size=3.0;
//	int i;

	// delete existing, create new (required for windoze)
//	if(Pulsars) glDeleteLists(Pulsars, 1);
//	Pulsars = glGenLists(1);
//	glNewList(Pulsars, GL_COMPILE);
//
//		glColor3f(0.80, 0.0, 0.85); // _P_ulsars are _P_urple
//
//		for (i=0; i < Npulsars; i++) {
//			star_marker(pulsar_info[i][0], pulsar_info[i][1], mag_size);
//		}
//
//	glEndList();
    }

/**
 * Super Novae Remenants (SNRs):
 */
void Starsphere::make_snrs(RenderTask* task) {
	ResourceFactory factory;

	RenderTask::shader_group s_group1 = {factory.createInstance("VertexShader_Single_Color")->std_string(),
	                                     factory.createInstance("FragmentShader_Pass")->std_string(),
	                                     Program::KEEP_ON_GOOD_LINK};

	RenderTask::index_buffer_group i_group1 = {NULL, 0, 0, 0, 0};		// With no index data remaining fields irrelevant.

	GLfloat vertex_data[NSNRs * 3] = { };

	RenderTask::vertex_buffer_group v_group1 = {vertex_data,
	                                            sizeof(vertex_data),
	                                            NSNRs,
	                                            GL_STATIC_DRAW,
	                                            VertexBuffer::BY_VERTEX};



	glm::vec3 snr_color = glm::vec3(0.7, 0.176, 0.0);		// Supernovae are Sienna.

	// GLfloat mag_size=3.0;
	// int i;

	for(int i=0; i < NSNRs; ++i) {
		glm::vec3 temp = sphVertex3D(SNR_info[i][0], SNR_info[i][1], sphRadius);

		vertex_data[i*3] = temp.x;
		vertex_data[i*3 + 1] = temp.y;
		vertex_data[i*3 + 2] = temp.z;
	}

	task = new RenderTask(s_group1, i_group1, v_group1);

	task->addSpecification({0, "position", 3, GL_FLOAT, GL_FALSE});

	task->setUniformLoadPoint("color", &snr_color);

	task->setUniformLoadPoint("RotationMatrix", &m_rotation[0][0]);

	task->acquire();
	}

/**
 * Create Constellations:
 * draws line links between pairs of stars in the list.
 */
void Starsphere::make_constellations() {
//	GLint star_num=0;

	// delete existing, create new (required for windoze)
//	if(Constellations) glDeleteLists(Constellations, 1);
//	Constellations = glGenLists(1);
//	glNewList(Constellations, GL_COMPILE);
//
//		glLineWidth(1.0);
//		glColor3f(0.7, 0.7, 0.0); // light yellow
//
//		glBegin(GL_LINES); // draws lines between *pairs* of vertices
//			for (star_num=0; star_num < Nstars; ++star_num) {
//				sphVertex(star_info[star_num][0], star_info[star_num][1]);
//				star_num++;
//				sphVertex(star_info[star_num][0], star_info[star_num][1]);
//			}
//		glEnd();
//
//	glEndList();
    }

/**
 * Create markers on sky sphere for LLO, LHO, GEO and VIRGO
 * IFO corner positions are from Myers' personal GPS and are +/- 100m
 */

GLfloat Starsphere::RAofZenith(double T, GLfloat LONdeg) {

//	// unix epoch at 12h  1/1/2000
//	const double T_0 = 946728000.0;
//
//	//  UT seconds of the day
//	double T_s = fmod(T, 24.0*3600.0);
//
//	// Julian centuries since 12h 1/1/2000 and 0h today
//	double T_c = (T - T_s - T_0)/3155760000.0;
//
//	// GMST at 0h today in seconds
//	double GMST0 = 6.0*3600.0 + 41.0*60.0 + 50.54841 + (8640184.812866
//	        + 0.093104*T_c)*T_c;
//
//	// GMST now in seconds
//	double GMST = GMST0 + 1.002738*T_s;
//
//	// longitude defined as west positive
//	GLfloat alpha = (GMST/(24.0*3600.0))*360.0 - LONdeg;
//
//	return alpha;
    }

/**
 * Draw the observatories at their zenith positions
 */
void Starsphere::generateObservatories(float dimFactor) {
	// sanity check
//	if(dimFactor < 0.0) dimFactor = 0.0;
//	if(dimFactor > 1.0) dimFactor = 1.0;
//
//	GLfloat Lat, Lon; // Latitute/Longitude of IFO is
//	GLfloat RAdeg, DEdeg; // converted to RA/DEC of sky sphere position
//	GLfloat radius; // radius of sphere for obs
//
//	GLfloat arm_len_deg=3.000; // lenght of arms, in degrees (not to scale)
//	GLfloat h2=0.400; // slight offset for H2 arms
//
//	// get current time and UTC offset (for zenith position)
//	m_ObservatoryDrawTimeLocal = dtime();
//	time_t local = m_ObservatoryDrawTimeLocal;
//	tm *utc = gmtime(&local);
//	double utcOffset = difftime(local, mktime(utc));
//	double observatoryDrawTimeGMT = m_ObservatoryDrawTimeLocal - utcOffset;
//
//	radius = 1.0*sphRadius; // radius of sphere on which they are drawn
//
//	float lineSize = 4.0;
//
//	/**
//	 * LIGO Livingston Observatory:
//	 */
//
//	Lat= 30.56377;
//	Lon= 90.77408;
//
//	RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//	DEdeg= Lat;
//
//	// delete existing, create new (required for windoze)
////	if(LLOmarker) glDeleteLists(LLOmarker, 1);
////	LLOmarker = glGenLists(1);
////	glNewList(LLOmarker, GL_COMPILE);
////
////		glColor3f(dimFactor * 0.0, dimFactor * 1.0, dimFactor * 0.0);
////		glLineWidth(lineSize);
////
////		glBegin(GL_LINE_STRIP);
////			//  North/South arm:
////			sphVertex3D(RAdeg, DEdeg-arm_len_deg, radius);
////			sphVertex3D(RAdeg, DEdeg, radius);
////			// East/West arm:
////			sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
////		glEnd();
////
////		// arm joint H2
////		glPointSize((GLfloat) lineSize);
////		glBegin(GL_POINTS);
////			sphVertex3D(RAdeg, DEdeg, radius);
////		glEnd();
////
////	glEndList();
//
//	/**
//	 * LIGO Hanford Observatory: H1 and H2
//	 */
//
//	Lat= 46.45510;
//	Lon= 119.40627;
//
//	RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//	DEdeg= Lat;
//
//	// delete existing, create new (required for windoze)
////	if(LHOmarker) glDeleteLists(LHOmarker, 1);
////	LHOmarker = glGenLists(1);
////	glNewList(LHOmarker, GL_COMPILE);
////
////		glColor3f(dimFactor * 0.0, dimFactor * 0.0, dimFactor * 1.0);
////		glLineWidth(lineSize);
////
////		glBegin(GL_LINE_STRIP);
////			// North/South arm:
////			sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
////			sphVertex3D(RAdeg, DEdeg, radius);
////			// East/West arm:
////			sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
////		glEnd();
////
////		glBegin(GL_LINE_STRIP);
////			// North/South arm, H2:
////			sphVertex3D(RAdeg-h2, DEdeg+arm_len_deg/2.0+h2/2.0, radius);
////			sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
////			// East/West arm, H2;
////			sphVertex3D(RAdeg-arm_len_deg/2.0-h2, DEdeg+h2/2.0, radius);
////		glEnd();
////
////		// arm joint H1
////		glPointSize((GLfloat) lineSize);
////		glBegin(GL_POINTS);
////			sphVertex3D(RAdeg, DEdeg, radius);
////		glEnd();
////
////		// arm joint H2
////		glPointSize((GLfloat) lineSize);
////		glBegin(GL_POINTS);
////			sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
////		glEnd();
////
////	glEndList();
//
//	/**
//	 *  GEO600 Interferometer:
//	 */
//
//	Lat= 52.24452;
//	Lon= -9.80683;
//	arm_len_deg=1.50; // not to scale
//
//	RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//	DEdeg= Lat;
//
//	// delete existing, create new (required for windoze)
////	if(GEOmarker) glDeleteLists(GEOmarker, 1);
////	GEOmarker = glGenLists(1);
////	glNewList(GEOmarker, GL_COMPILE);
////
////		glColor3f(dimFactor * 1.0, dimFactor * 0.0, dimFactor * 0.0);
////		glLineWidth(lineSize);
////
////		glBegin(GL_LINE_STRIP);
////			// North/South arm:
////			sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
////			sphVertex3D(RAdeg, DEdeg, radius);
////			// West/East arm:
////			sphVertex3D(RAdeg+arm_len_deg, DEdeg, radius);
////		glEnd();
////
////		// arm joint
////		glPointSize((GLfloat) lineSize);
////		glBegin(GL_POINTS);
////			sphVertex3D(RAdeg, DEdeg, radius);
////		glEnd();
////
////	glEndList();
//
//	/**
//	 *  VIRGO Interferometer:
//	 */
//
//	Lat= 43.63139;
//	Lon= -10.505;
//	arm_len_deg=3.000; // not to scale
//
//	RAdeg= RAofZenith(observatoryDrawTimeGMT, Lon);
//	DEdeg= Lat;
//
//	// delete existing, create new (required for windoze)
////	if(VIRGOmarker) glDeleteLists(VIRGOmarker, 1);
//	VIRGOmarker = glGenLists(1);
//	glNewList(VIRGOmarker, GL_COMPILE);
//
//		glColor3f(dimFactor * 1.0, dimFactor * 1.0, dimFactor * 1.0);
//		glLineWidth(lineSize);
//
//		glBegin(GL_LINE_STRIP);
//			// North/South arm:
//			sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
//			sphVertex3D(RAdeg, DEdeg, radius);
//			// West/East arm:
//			sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
//		glEnd();
//
//		// arm joint
//		glPointSize((GLfloat) lineSize);
//		glBegin(GL_POINTS);
//			sphVertex3D(RAdeg, DEdeg, radius);
//		glEnd();
//
//	glEndList();

	return;
    }

void Starsphere::make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size) {
//	GLfloat x, y;
//	GLfloat r1, r2, r3;
//	float theta;
//	int i, Nstep=20;
//
//	// r1 is inner circle, r2 is outer circle, r3 is crosshairs
//	r1 = size, r2=3*size, r3=4*size;

	// delete existing, create new (required for windoze)
//	if(SearchMarker) glDeleteLists(SearchMarker, 1);
//	SearchMarker = glGenLists(1);
//	glNewList(SearchMarker, GL_COMPILE);
//
//		// start gunsight drawing
//		glPushMatrix();
//
//		glLineWidth(3.0);
//		glColor3f(1.0, 0.5, 0.0); // Orange
//
//		// First rotate east  to the RA position around y
//		glRotatef(RAdeg, 0.0, 1.0, 0.0);
//		// Then rotate up to DEC position around z (not x)
//		glRotatef(DEdeg, 0.0, 0.0, 1.0);
//
//		// Inner circle
//		glBegin(GL_LINE_LOOP);
//			for (i=0; i<Nstep; i++) {
//				theta = i*360.0/Nstep;
//				x = r1*COS(theta);
//				y = r1*SIN(theta);
//				sphVertex(x, y);
//			}
//		glEnd();
//
//		// Outer circle
//		glBegin(GL_LINE_LOOP);
//			for (i=0; i<Nstep; i++) {
//				theta = i*360.0/Nstep;
//				x = r2*COS(theta);
//				y = r2*SIN(theta);
//				sphVertex(x, y);
//			}
//		glEnd();
//
//		// Arms that form the gunsight
//		glBegin(GL_LINES);
//			//  North arm:
//			sphVertex(0.0, +r1);
//			sphVertex(0.0, +r3);
//			//  South arm:
//			sphVertex(0.0, -r1);
//			sphVertex(0.0, -r3);
//			// East arm:
//			sphVertex(-r1, 0.0);
//			sphVertex(-r3, 0.0);
//			// West arm:
//			sphVertex(+r1, 0.0);
//			sphVertex(+r3, 0.0);
//		glEnd();
//
//		glPopMatrix();
//
//		// searchlight line out to marker (OFF!)
//		if(false) {
//			glBegin(GL_LINES);
//				sphVertex3D(RAdeg, DEdeg, 0.50*sphRadius);
//				sphVertex3D(RAdeg, DEdeg, 0.95*sphRadius);
//			glEnd();
//		}
//
//	glEndList();
    }

/**
 * XYZ coordinate axes: (if we want them - most useful for testing)
 */
void Starsphere::make_axes() {
//	GLfloat axl=10.0;

	// delete existing, create new (required for windoze)
//	if(Axes) glDeleteLists(Axes, 1);
//	Axes = glGenLists(1);
//	glNewList(Axes, GL_COMPILE);
//
//		glLineWidth(2.0);
//
//		glBegin(GL_LINES);
//			glColor3f(1.0, 0.0, 0.0);
//			glVertex3f(-axl, 0.0, 0.0);
//			glVertex3f(axl, 0.0, 0.0);
//
//			glColor3f(0.0, 1.0, 0.0);
//			glVertex3f(0.0, -axl, 0.0);
//			glVertex3f(0.0, axl, 0.0);
//
//			glColor3f(0.0, 0.0, 1.0);
//			glVertex3f(0.0, 0.0, -axl);
//			glVertex3f(0.0, 0.0, axl);
//		glEnd();
//
//	glEndList();
    }

/**
 * RA/DEC coordinate grid on the sphere
 */
void Starsphere::make_globe() {
//	int hr, j, i, iMax=100;
//	GLfloat RAdeg, DEdeg;

	// delete existing, create new (required for windoze)
//	if(sphGrid) glDeleteLists(sphGrid, 1);
//	sphGrid = glGenLists(1);
//	glNewList(sphGrid, GL_COMPILE);
//
//		glLineWidth(1.0);
//
//		// Lines of constant Right Ascencion (East Longitude)
//		for (hr=0; hr<24; hr++) {
//			RAdeg=hr*15.0;
//			glColor3f(0.25, 0.25, 0.25);
//
//			// mark median
//			if(hr==0) glColor3f(0.55, 0.55, 0.55);
//
//			glBegin(GL_LINE_STRIP);
//				for (i=0; i<=iMax; i++) {
//					DEdeg = i*180.0/iMax - 90.0;
//					sphVertex(RAdeg, DEdeg);
//				}
//			glEnd();
//		}
//
//		// Lines of constant Declination (Lattitude)
//		for (j=1; j<=12; j++) {
//			DEdeg = 90.0 - j*15.0;
//
//			glBegin(GL_LINE_STRIP);
//				for (i=0; i<=iMax; i++) {
//					RAdeg = i*360.0/iMax;
//					sphVertex(RAdeg, DEdeg);
//				}
//			glEnd();
//		}
//
//	glEndList();
    }

/**
 * Window resize/remap
 */
void Starsphere::resize(const int width, const int height) {
	// store current settings
	m_CurrentWidth = width;
	m_CurrentHeight = height;
	aspect = (float)m_CurrentWidth / (float)m_CurrentHeight;

	// adjust HUD config
	m_YStartPosTop = m_CurrentHeight - 25;

	// make sure the search marker is updated (conditional rendering!)
	m_RefreshSearchMarker = true;

    // adjust aspect ratio and projection
	glViewport(0, 0, m_CurrentWidth, m_CurrentHeight);
    // gluPerspective(95.0, aspect, 0.50, 25.0);
    }

/**
 *  What to do when graphics are "initialized".
 */
void Starsphere::initialize(const int width, const int height, const Resource* font) {
    ResourceFactory factory;

    // The vertex data in client space.
    //GLfloat vertex_data[] = {
        // position ( x  ,   y  ) color ( R,    G,    B  )
    //                 0.0f,  0.5f,        1.0f, 0.0f, 0.0f,
     //               -0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
      //               0.5f, -0.5f,        0.0f, 0.0f, 1.0f,
       // };

    // The index data in client space ie. the order in which the above vertices are rendered.
    //GLuint index_data[] = {
      //  0, 1, 2,
//    	};

//    RenderTask::shader_group s_group1 = {factory.createInstance("VertexTestShader")->std_string(),
//                                         factory.createInstance("FragmentTestShader")->std_string(),
//                                         Program::KEEP_ON_GOOD_LINK};
//
//    RenderTask::index_buffer_group i_group1 = {index_data,
//                                               sizeof(index_data),
//                                               3,
//                                               GL_STATIC_DRAW,
//                                               GL_UNSIGNED_INT};
//
//    RenderTask::vertex_buffer_group v_group1 = {vertex_data,
//                                               sizeof(vertex_data),
//                                               3,
//                                               GL_STATIC_DRAW,
//                                               VertexBuffer::BY_VERTEX};
//
//    m_render_task_snr = new RenderTask(s_group1, i_group1, v_group1);
//
//    m_render_task_snr->addSpecification({0, "position", 2, GL_FLOAT, GL_FALSE});
//    m_render_task_snr->addSpecification({1, "color", 3, GL_FLOAT, GL_FALSE});
//
//    m_render_task_snr->setUniformLoadPoint("RotationMatrix", &m_rotation[0][0]);
//
//    m_render_task_snr->acquire();

    m_CurrentWidth = width;
    m_CurrentHeight = height;
    m_FontResource = font;

    // Fatal error if no font resource supplied.
    if(!m_FontResource) {
        ErrorHandler::record("Starsphere::initialize()", ErrorHandler::FATAL);
        }
	else {
        // NB : initialization of logo font instances is done in subclasses!
		// Create header and text font instances using font resource,
		// if not done already.
		if(m_FontHeader == NULL) {
            m_FontHeader = TTF_OpenFontRW(SDL_RWFromConstMem(&m_FontResource->data()->at(0),
                                                             m_FontResource->data()->size()),
                                          0,
                                          13);

            if(m_FontHeader == NULL) {
                stringstream font_header_error;
                font_header_error << "Starsphere::initialize() : "
                                  << "Could not construct header font face from in memory resource!"
                                  << std::endl;
                ErrorHandler::record(font_header_error.str(), ErrorHandler::FATAL);
                }
            }

        if(m_FontText == NULL) {
            m_FontText = TTF_OpenFontRW(SDL_RWFromConstMem(&m_FontResource->data()->at(0),
                                                           m_FontResource->data()->size()),
                                        0,
                                        11);

            if(m_FontText == NULL) {
                stringstream font_text_error;
                font_text_error << "Starsphere::initialize() : "
                                << "Could not construct text font face from in memory resource!"
                                << std::endl;
                ErrorHandler::record(font_text_error.str(), ErrorHandler::FATAL);
                }
            }
        }

	// setup initial dimensions
	// resize(m_CurrentWidth, m_CurrentHeight);

	// more font setup and optimizations
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	// drawing setup:
//	glClearColor(1.0, 1.0, 1.0, 0.0); // background is black
////	glEnable(GL_CULL_FACE;
//	glFrontFace(GL_CCW;
////	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST;
//
//	// enable opt-in quality feature
//	if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
//		/// TODO - what OpenGL ES 2.x quality options available? In the shader code then ?
//        }
//
//	// we need alpha blending for proper font rendering
//	glEnable(GL_BLEND;
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA;
//
//	// enable depth buffering for 3D graphics
//	glClearDepthf(1.0f);
//	glEnable(GL_DEPTH_TEST;
//	glDepthFunc(GL_LEQUAL;
//
//	glEnable(GL_PROGRAM_POINT_SIZE;
//	glPointSize(40);

	// enable opt-in quality feature
	if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::MediumGraphicsQualitySetting ||
	   m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
		// fog aids depth perception
		/// TODO - what OpenGL ES 2.x fog options available? In the shader code then ?
        }

	// create pre-drawn display lists
//	make_stars();
//	make_constellations();
//	make_pulsars();
//	make_snrs();
//	make_axes();
//	make_globe();
	}

/**
 * Rendering routine:  this is what does the drawing:
 */
void Starsphere::render(const double timeOfDay) {
//    GLfloat xvp, yvp, zvp, vp_theta, vp_phi, vp_rad;
//	GLfloat Zrot = 0.0, Zobs=0.0;
//	double revs, t, dt = 0;
//	static double start_time=-1.0, last_time=-1.0;
//
//	// Calculate the real time t since we started (or reset) and the
//	// time dt since the last render() call.    Both may be useful
//	// for timing animations.  Note that time_of_day is dtime().
//
//	if (start_time < 0.0)
//		start_time = timeOfDay;
//	t = timeOfDay - start_time;
//
//	if (last_time < 0.0)
//		last_time = timeOfDay - 0.01;
//	dt = timeOfDay - last_time;
//
//	last_time = timeOfDay; // remember for next time
//
//	// Now determine the rotation angle based on the time since start
//	// It is negative to get the rotation direction correct (the sun
//	// rises in the East, so the sky sphere rotates E to W).
//
//	Zrot = t*rotation_speed/60.0;
//	revs = Zrot/360.0;
//	Zrot = -360.0 * (revs - (int)revs);

	// and start drawing...
	glClear(GL_COLOR_BUFFER_BIT);

	// now draw the scene...

	// Vary the viewpoint with both a long period wobble of the elevation
	// of the view and a longer period zoom in/out that might even penetrate
	// The starsphere for a brief time.   Increase the power in pow(,) to
	// make the visit inside briefer.
//	vp_theta = 90.0 - viewpt_elev + wobble_amp*sin(PI2*t/(wobble_period*60.0));
//	vp_phi = viewpt_azimuth;
//	vp_rad = viewpt_radius - zoom_amp*sin(PI2*t/(zoom_period*60.0));
//	if(vp_rad <0.0) vp_rad = 0.0; // cannot pass origin (confusing)
//
//	// TRIED THIS TOO: -zoom_amp*pow(fabs(sin(PI2*t/(zoom_period*60.0))),3);
//	xvp = vp_rad * SIN(vp_theta) * SIN(vp_phi);
//	zvp = vp_rad * SIN(vp_theta) * COS(vp_phi);
//	yvp = vp_rad * COS(vp_theta);

//  	gluLookAt(0.0, 0.0, 5.0, // eyes position

//              0.0, 0.0, 0.0, // looking toward here
//  	          0.0, 1.0, 0.0); // which way is up?  y axis!

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

	m_rotation = glm::rotate(m_rotation, 0.01f, m_axis);

  	m_render_task_snr->utilise(GL_POINTS, NSNRs);

	// draw axes before any rotation so they stay put
//	if (isFeature(AXES)) glCallList(Axes);
//
//	// draw the sky sphere, with rotation:
//	glPushMatrix();
//	glRotatef(Zrot - rotation_offset, 0.0, 1.0, 0.0);

	// stars, pulsars, supernovae, grid
//	if (isFeature(STARS))			glCallList(Stars);
//	if (isFeature(PULSARS))			glCallList(Pulsars);
//	if (isFeature(SNRS))			glCallList(SNRs);
//	if (isFeature(CONSTELLATIONS))	glCallList(Constellations);
//	if (isFeature(GLOBE))			glCallList(sphGrid);

	// observatories move an extra 15 degrees/hr since they were drawn
//	if (isFeature(OBSERVATORIES)) {
//		glPushMatrix();
//		Zobs = (timeOfDay - m_ObservatoryDrawTimeLocal) * 15.0/3600.0;
//		glRotatef(Zobs, 0.0, 1.0, 0.0);
//		glCallList(LLOmarker);
//		glCallList(LHOmarker);
//		glCallList(GEOmarker);
//		glCallList(VIRGOmarker);
//		renderAdditionalObservatories();
//		glPopMatrix();
//	}

	// draw the search marker (gunsight)
//	if (isFeature(MARKER)) {
//		if(m_RefreshSearchMarker) {
//			make_search_marker(m_CurrentRightAscension, m_CurrentDeclination, 0.5);
//			m_RefreshSearchMarker = false;
//		}
//		else {
//			glCallList(SearchMarker);
//		}
//	}

//	glPopMatrix();

	// draw 2D vectorized HUD
//    if(isFeature(LOGO) || isFeature(SEARCHINFO)) {
//
//		// disable depth testing since we're in 2D mode
//    	glDisable(GL_DEPTH_TEST);
//
////		if (isFeature(LOGO)) renderLogo();
////		if (isFeature(SEARCHINFO)) renderSearchInformation();
//
////		// enable depth testing since we're leaving 2D mode
//    	glEnable(GL_DEPTH_TEST);
//        }

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
    // Currently no behaviours attached to mouse wheel events.
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
	viewpt_radius -= relativeZoom/10.0;
	if (viewpt_radius > 15.0)
		viewpt_radius = 15.0;
	if (viewpt_radius < 0.5)
		viewpt_radius = 0.5;
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
	stringstream buffer;
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
