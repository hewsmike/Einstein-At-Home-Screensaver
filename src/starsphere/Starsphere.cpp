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

#include "Starsphere.h"

const GLfloat Starsphere::FULL_CIRCLE_DEG(360);
const GLfloat Starsphere::HALF_CIRCLE_DEG(FULL_CIRCLE_DEG/2);
const GLfloat Starsphere::QUARTER_CIRCLE_DEG(FULL_CIRCLE_DEG/4);

const int Starsphere::RIGHT_ASCENSION_SLICES(24);
const int Starsphere::DECLINATION_STACKS(12);
const int Starsphere::RIGHT_ASCENSION_STEP_DEG(FULL_CIRCLE_DEG/RIGHT_ASCENSION_SLICES);
const int Starsphere::DECLINATION_STEP_DEG(HALF_CIRCLE_DEG/DECLINATION_STACKS);
const int Starsphere::GLOBE_GRID_STEPS(100);

const uint Starsphere::RA_ARR_POS(0);
const uint Starsphere::DEC_ARR_POS(1);

const GLfloat Starsphere::LLO_LAT(30.56377);
const GLfloat Starsphere::LLO_LONG(90.77408);
const GLfloat Starsphere::LLO_ARM_LEN_DEG(3.0);

const GLfloat Starsphere::LHO_LAT(46.45510);
const GLfloat Starsphere::LHO_LONG(119.40627);
const GLfloat Starsphere::LHO_ARM_LEN_DEG(3.0);
const GLfloat Starsphere::LHO_H2_OFFSET(0.4);

const GLfloat Starsphere::GEO_LAT(52.24452);
const GLfloat Starsphere::GEO_LONG(-9.80683);
const GLfloat Starsphere::GEO_ARM_LEN_DEG(1.5);

const GLfloat Starsphere::VIRGO_LAT(43.63139);
const GLfloat Starsphere::VIRGO_LONG(-10.505);
const GLfloat Starsphere::VIRGO_ARM_LEN_DEG(3.0);

const GLfloat Starsphere::ROTATION_SPEED(180.0);
const GLfloat Starsphere::WOBBLE_AMP(37.0);
const GLfloat Starsphere::WOBBLE_PERIOD(17.0);
const GLfloat Starsphere::ZOOM_AMP(0.0);
const GLfloat Starsphere::ZOOM_PERIOD(29.0);

const GLfloat Starsphere::SECONDS_PER_MINUTE(60.0);
const GLfloat Starsphere::MINUTES_PER_HOUR(60.0);
const GLfloat Starsphere::SECONDS_PER_HOUR(SECONDS_PER_MINUTE * MINUTES_PER_HOUR);
const GLfloat Starsphere::HOURS_PER_DAY(24.0);
const GLfloat Starsphere::SECS_PER_DAY(SECONDS_PER_HOUR * HOURS_PER_DAY);
const GLfloat Starsphere::MEAN_SIDERIAL_DAYS_PER_YEAR(366.24);
const GLfloat Starsphere::MEAN_SOLAR_DAYS_PER_YEAR(365.24);
const GLfloat Starsphere::YEARS_PER_CENTURY(100.0);
const GLfloat Starsphere::SECONDS_PER_CENTURY(SECS_PER_DAY *
                                              MEAN_SOLAR_DAYS_PER_YEAR *
                                              YEARS_PER_CENTURY);

const GLfloat Starsphere::AXES_LINE_WIDTH(2.0);
const GLfloat Starsphere::AXES_RATIO(1.1);

// Stars are white.
const GLfloat Starsphere::STAR_MAG_SIZE(2.0f);
const GLfloat Starsphere::STAR_RED(1.0f);
const GLfloat Starsphere::STAR_GREEN(1.0f);
const GLfloat Starsphere::STAR_BLUE(1.0f);

// _P_ulsars are _P_urple
const GLfloat Starsphere::PULSAR_MAG_SIZE(1.5f);
const GLfloat Starsphere::PULSAR_RED(0.8f);
const GLfloat Starsphere::PULSAR_GREEN(0.0f);
const GLfloat Starsphere::PULSAR_BLUE(0.85f);

// _S_NRs are _S_ienna
const GLfloat Starsphere::SNR_MAG_SIZE(2.0f);
const GLfloat Starsphere::SNR_RED(0.7f);
const GLfloat Starsphere::SNR_GREEN(0.176f);
const GLfloat Starsphere::SNR_BLUE(0.0f);

// Constellation links are light yellow.
const GLfloat Starsphere::CONS_LINK_SIZE(1.0f);
const GLfloat Starsphere::CONS_LINK_RED(0.7f);
const GLfloat Starsphere::CONS_LINK_GREEN(0.7f);
const GLfloat Starsphere::CONS_LINK_BLUE(0.0f);

// Globe meridians.
const GLfloat Starsphere::GLOBE_LINE_SIZE(1.0f);
const GLfloat Starsphere::MERID_RED(0.25f);
const GLfloat Starsphere::MERID_GREEN(0.25f);
const GLfloat Starsphere::MERID_BLUE(0.25f);
// Globe prime meridian
const GLfloat Starsphere::MERID_PRIME_RED(0.60f);
const GLfloat Starsphere::MERID_PRIME_GREEN(0.60f);
const GLfloat Starsphere::MERID_PRIME_BLUE(0.60f);

const GLfloat Starsphere::OBS_LINE_SIZE(4.0f);

// Livingstone is green
const GLfloat Starsphere::LLO_RED(0.0f);
const GLfloat Starsphere::LLO_GREEN(1.0f);
const GLfloat Starsphere::LLO_BLUE(0.0f);

// Hanford is blue
const GLfloat Starsphere::LHO_RED(0.0f);
const GLfloat Starsphere::LHO_GREEN(0.0f);
const GLfloat Starsphere::LHO_BLUE(1.0f);

// GEO is red
const GLfloat Starsphere::GEO_RED(1.0f);
const GLfloat Starsphere::GEO_GREEN(0.0f);
const GLfloat Starsphere::GEO_BLUE(0.0f);

// VIRGO is white
const GLfloat Starsphere::VIRGO_RED(1.0f);
const GLfloat Starsphere::VIRGO_GREEN(1.0f);
const GLfloat Starsphere::VIRGO_BLUE(1.0f);

// X-axis is red
const GLfloat Starsphere::X_AXIS_RED(1.0f);
const GLfloat Starsphere::X_AXIS_GREEN(0.0f);
const GLfloat Starsphere::X_AXIS_BLUE(0.0f);

// Y-axis is green
const GLfloat Starsphere::Y_AXIS_RED(0.0f);
const GLfloat Starsphere::Y_AXIS_GREEN(1.0f);
const GLfloat Starsphere::Y_AXIS_BLUE(0.0f);

// Z-axis is blue
const GLfloat Starsphere::Z_AXIS_RED(0.0f);
const GLfloat Starsphere::Z_AXIS_GREEN(0.0f);
const GLfloat Starsphere::Z_AXIS_BLUE(1.0f);

// Search marker is orange
const GLfloat Starsphere::MARKER_LINE_WIDTH(3.0);
const int Starsphere::MARKER_CURVE_STEPS(20);
const GLfloat Starsphere::MARKER_INNER_RADIUS_FACTOR(1.0f);
const GLfloat Starsphere::MARKER_OUTER_RADIUS_FACTOR(3.0f);
const GLfloat Starsphere::MARKER_CROSSHAIR_RADIUS_FACTOR(4.0f);
const GLfloat Starsphere::MARKER_ANGULAR_SIZE(0.5f);
const GLfloat Starsphere::MARKER_RED(1.0f);
const GLfloat Starsphere::MARKER_GREEN(0.5f);
const GLfloat Starsphere::MARKER_BLUE(0.0f);

Starsphere::Starsphere(string sharedMemoryAreaIdentifier) :
   AbstractGraphicsEngine(sharedMemoryAreaIdentifier) {
   static const GLfloat SPHERE_RADIUS_INIT(5.5);
   static const GLfloat VIEWPT_AZIMUTH_INIT(30.0);
   static const GLfloat VIEWPT_ELEVATION_INIT(23.6);
   static const GLfloat VIEWPT_RADIUS_INIT(7.6);
   static const GLfloat ROTATION_OFFSET_INIT(0.0);
   static const GLfloat CURRENT_RA_INIT(-1.0);
   static const GLfloat CURRENT_DEC_INIT(-1.0);
   static const GLuint FEATURE_INIT(0);
   static const GLuint FEATURE_FLAGS_INIT(0);
   static const GLuint SVG_COUNT_INIT(0);

	m_FontResource = NULL;
	m_FontLogo1 = NULL;
	m_FontLogo2 = NULL;
	m_FontHeader = NULL;
	m_FontText = NULL;

	Axes = FEATURE_INIT;
   Stars = FEATURE_INIT;
   Constellations = FEATURE_INIT;
   Pulsars = FEATURE_INIT;
   LLOmarker = FEATURE_INIT;
   LHOmarker = FEATURE_INIT;
   GEOmarker = FEATURE_INIT;
   VIRGOmarker = FEATURE_INIT;
   sphGrid = FEATURE_INIT;
   SNRs = FEATURE_INIT;
   SearchMarker = FEATURE_INIT;

	/**
	 * Parameters and State info
	 */
	sphRadius = SPHERE_RADIUS_INIT;
	featureFlags = FEATURE_FLAGS_INIT;

	/**
	 * Viewpoint (can be changed with mouse)
	 */
	viewpt_azimuth = VIEWPT_AZIMUTH_INIT;
	viewpt_elev = VIEWPT_ELEVATION_INIT;
	viewpt_radius = VIEWPT_RADIUS_INIT;

	rotation_offset = ROTATION_OFFSET_INIT;

   m_CurrentRightAscension = CURRENT_RA_INIT;
	m_CurrentDeclination = CURRENT_DEC_INIT;
	m_RefreshSearchMarker = true;

   // Set SVG file emit counter to zero, and awaiting trigger.
   SVG_emit_count = SVG_COUNT_INIT;
   SVGSample = false;
   // Start off with the starsphere rotating.
   rotate_sphere = true;
   }

Starsphere::~Starsphere() {
	if(m_FontLogo1) delete m_FontLogo1;
	if(m_FontLogo2) delete m_FontLogo2;
	if(m_FontHeader) delete m_FontHeader;
	if(m_FontText) delete m_FontText;
   }

void Starsphere::sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius) {
	GLfloat x, y, z;

	x = radius * COS(DEdeg) * COS(RAdeg);
	z = -radius * COS(DEdeg) * SIN(RAdeg);
	y = radius * SIN(DEdeg);
	glVertex3f(x, y, z);
	return;
   }

void Starsphere::sphVertex(GLfloat RAdeg, GLfloat DEdeg) {
	sphVertex3D(RAdeg, DEdeg, sphRadius);
   }

/**
 * Star Marker:
 * Makes a marker for one star at a given position and angular size.
 */
void Starsphere::star_marker(float RAdeg, float DEdeg, float size) {
	glPointSize((GLfloat) size);
	glBegin(GL_POINTS);
      sphVertex((GLfloat) RAdeg, (GLfloat) DEdeg);
	glEnd();
	return;
   }

/**
 * Create Stars: markers for each star
 */
void Starsphere::make_stars() {
   // delete existing, create new (required for windoze)
	if(Stars) glDeleteLists(Stars, 1);
	Stars = glGenLists(1);

	glNewList(Stars, GL_COMPILE);
		glColor3f(STAR_RED, STAR_GREEN, STAR_BLUE);

		/**
		 * At some point in the future star_info[][] will also contain
		 * star magnitude and the marker size will vary with this.
		 */
		for(int star = 0; star < Nstars; ++star) {
			// same stars appear more than once in constellations so ignore dupes
			bool is_dupe = false;
			for(int other_star = 0; other_star < star; ++other_star) {
            // This logical 'AND' will short circuit evaluate ...
				if((star_info[other_star][RA_ARR_POS] == star_info[star][RA_ARR_POS]) &&
               (star_info[other_star][DEC_ARR_POS] == star_info[star][DEC_ARR_POS])) {
					is_dupe = true;
					break;
               }
            }
			if(!is_dupe) {
				// mag_size = 0.05 + 0.50*rand()/RAND_MAX;
				star_marker(star_info[star][RA_ARR_POS],
                        star_info[star][DEC_ARR_POS],
                        STAR_MAG_SIZE);
            }
         }
	glEndList();
   }

/**
 * Pulsar Markers:
 */
void Starsphere::make_pulsars() {
   // delete existing, create new (required for windoze)
	if(Pulsars) glDeleteLists(Pulsars, 1);
	Pulsars = glGenLists(1);

	glNewList(Pulsars, GL_COMPILE);
		glColor3f(PULSAR_RED, PULSAR_GREEN, PULSAR_BLUE);
		for(int pulsar = 0; pulsar < Npulsars; ++pulsar) {
			star_marker(pulsar_info[pulsar][RA_ARR_POS],
                     pulsar_info[pulsar][DEC_ARR_POS],
                     PULSAR_MAG_SIZE);
         }
	glEndList();
   }

/**
 * Super Novae Remenants (SNRs):
 */
void Starsphere::make_snrs() {
   // delete existing, create new (required for windoze)
	if(SNRs) glDeleteLists(SNRs, 1);
	SNRs = glGenLists(1);

	glNewList(SNRs, GL_COMPILE);
		glColor3f(SNR_RED, SNR_GREEN, SNR_BLUE);
		for(int supernova = 0; supernova < NSNRs; ++supernova) {
			star_marker(SNR_info[supernova][RA_ARR_POS],
                     SNR_info[supernova][DEC_ARR_POS],
                     SNR_MAG_SIZE);
         }
	glEndList();
   }

/**
 * Create Constellations:
 * draws line links between pairs of stars in the list.
 */
void Starsphere::make_constellations() {
   // delete existing, create new (required for windoze)
	if(Constellations) glDeleteLists(Constellations, 1);
	Constellations = glGenLists(1);

   glNewList(Constellations, GL_COMPILE);
		glLineWidth(CONS_LINK_SIZE);
		glColor3f(CONS_LINK_RED, CONS_LINK_GREEN, CONS_LINK_BLUE);

		glBegin(GL_LINES); // draws lines between *pairs* of vertices
			for(int star_num = 0; star_num < Nstars; ++star_num) {
				sphVertex(star_info[star_num][RA_ARR_POS],
                      star_info[star_num][DEC_ARR_POS]);
				star_num++;
				sphVertex(star_info[star_num][RA_ARR_POS],
                      star_info[star_num][DEC_ARR_POS]);
            }
		glEnd();
	glEndList();
   }

/**
 * Create markers on sky sphere for LLO, LHO, GEO and VIRGO
 * IFO corner positions are from Myers' personal GPS and are +/- 100m
 */

GLfloat Starsphere::RAofZenith(double time_now, GLfloat longitude_deg) {
   // unix epoch at 12h on 1/1/2000 - measured in seconds since 01/01/1970.
   static const GLfloat UNIX_EPOCH_DATUM(946728000.0);

   // Right ascension offset of Greenwich from the First Point of Aries
   // at UTC 00:00:00 on January 1st ( GHAA = Greenwich Hour Angle Aries ).
   // This mildly varies year to year, equivalent ~ 100 degrees of Earth's
   // rotation eastward.
   static const GLfloat GHAA_HOURS(6.0);
   static const GLfloat GHAA_MINUTES(41.0);
   static const GLfloat GHAA_SECONDS(50.54841);

   // Components of a quadratic model of Earth's rotational slowing.
   static const GLfloat EARTH_ROT_BASE(8640184.812866);
   static const GLfloat EARTH_ROT_SHIFT(0.093104);

	//  Seconds of this day since midnite UTC
	double seconds_since_midnite = fmod(time_now, static_cast<double>(SECS_PER_DAY));

	// Julian centuries since 12h 1/1/2000 and 0h today
	double centuries_since_datum = (time_now - seconds_since_midnite - UNIX_EPOCH_DATUM)/SECONDS_PER_CENTURY;

	// GMST at 0h today in seconds. Derived from a formula accepted by official
   // astronomy bodies. The first three terms account for the fact that at GMT
   // 00:00:00 on 1/1/2000 the zero of celestial longitude ( First Point of
   // Aries, now actually in Pisces ) was overhead nearby Mexico City. Which is
   // about 100 degrees West of Greenwich in terrestrial longitude, or if you
   // like a certain number of hours, minutes and seconds of right ascension.
   // The second line of terms is a quadratic approximation indicating that
   // as the centuries go by : the year ( siderial or solar ) gets longer ( ie.
   // containing more seconds - assuming seconds are defined elsewhere by a method
   // not related to Earth's rotation ) due to the slowing of the Earth's rotation ....
	double GMST0 = GHAA_HOURS * SECONDS_PER_HOUR +
                  GHAA_MINUTES * MINUTES_PER_HOUR +
                  GHAA_SECONDS +
                  (EARTH_ROT_BASE + EARTH_ROT_SHIFT * centuries_since_datum) *
                  centuries_since_datum;

	// GMST now in seconds
	double GMST = GMST0 +
                 (MEAN_SIDERIAL_DAYS_PER_YEAR/MEAN_SOLAR_DAYS_PER_YEAR) *
                 seconds_since_midnite;

	// longitude defined as west positive
	return (GMST/(SECS_PER_DAY)) * FULL_CIRCLE_DEG - longitude_deg;
   }

/**
 * Draw the observatories at their zenith positions
 */
void Starsphere::generateObservatories(float dimFactor) {
   static const GLfloat DIM_FACTOR_LOW(0.0f);
   static const GLfloat DIM_FACTOR_HIGH(1.0f);

	// sanity check
	if(dimFactor < DIM_FACTOR_LOW) dimFactor = DIM_FACTOR_LOW;
	if(dimFactor > DIM_FACTOR_HIGH) dimFactor = DIM_FACTOR_HIGH;

	GLfloat RAdeg, DEdeg; // converted to RA/DEC of sky sphere position

	// get current time and UTC offset (for zenith position)
	m_ObservatoryDrawTimeLocal = dtime();
	time_t local = m_ObservatoryDrawTimeLocal;
	tm *utc = gmtime(&local);
	double utcOffset = difftime(local, mktime(utc));
	observatoryDrawTimeGMT = m_ObservatoryDrawTimeLocal - utcOffset;

	/**
	 * LIGO Livingston Observatory:
	 */

	RAdeg = RAofZenith(observatoryDrawTimeGMT, LLO_LONG);
	DEdeg = LLO_LAT;

	// delete existing, create new (required for windoze)
	if(LLOmarker) glDeleteLists(LLOmarker, 1);
	LLOmarker = glGenLists(1);
	glNewList(LLOmarker, GL_COMPILE);
		glColor3f(dimFactor * LLO_RED, dimFactor * LLO_GREEN, dimFactor * LLO_BLUE);
		glLineWidth(OBS_LINE_SIZE);

		glBegin(GL_LINE_STRIP);
			//  North/South arm:
			sphVertex3D(RAdeg, DEdeg - LLO_ARM_LEN_DEG, sphRadius);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
			// East/West arm:
			sphVertex3D(RAdeg - LLO_ARM_LEN_DEG, DEdeg, sphRadius);
		glEnd();

      // arm joint H2
		glPointSize(OBS_LINE_SIZE);
		glBegin(GL_POINTS);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
		glEnd();
	glEndList();

	/**
	 * LIGO Hanford Observatory: H1 and H2
	 */

	RAdeg = RAofZenith(observatoryDrawTimeGMT, LHO_LONG);
	DEdeg = LHO_LAT;

	// delete existing, create new (required for windoze)
	if(LHOmarker) glDeleteLists(LHOmarker, 1);
	LHOmarker = glGenLists(1);
	glNewList(LHOmarker, GL_COMPILE);
		glColor3f(dimFactor * LHO_RED, dimFactor * LHO_GREEN, dimFactor * LHO_BLUE);
		glLineWidth(OBS_LINE_SIZE);

		glBegin(GL_LINE_STRIP);
			// North/South arm:
			sphVertex3D(RAdeg, DEdeg + LHO_ARM_LEN_DEG, sphRadius);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
			// East/West arm:
			sphVertex3D(RAdeg - LHO_ARM_LEN_DEG, DEdeg, sphRadius);
		glEnd();

		glBegin(GL_LINE_STRIP);
			// North/South arm, H2:
			sphVertex3D(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_ARM_LEN_DEG/2.0 + LHO_H2_OFFSET, sphRadius);
			sphVertex3D(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_H2_OFFSET, sphRadius);
			// East/West arm, H2;
			sphVertex3D(RAdeg - LHO_ARM_LEN_DEG/2.0 - LHO_H2_OFFSET, DEdeg + LHO_H2_OFFSET, sphRadius);
		glEnd();

		// arm joint H1
		glPointSize(OBS_LINE_SIZE);
		glBegin(GL_POINTS);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
		glEnd();

		// arm joint H2
		glPointSize(OBS_LINE_SIZE);
		glBegin(GL_POINTS);
			sphVertex3D(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_H2_OFFSET, sphRadius);
      glEnd();
	glEndList();

	/**
	 *  GEO600 Interferometer:
	 */

	RAdeg = RAofZenith(observatoryDrawTimeGMT, GEO_LONG);
	DEdeg = GEO_LAT;

	// delete existing, create new (required for windoze)
	if(GEOmarker) glDeleteLists(GEOmarker, 1);
	GEOmarker = glGenLists(1);
	glNewList(GEOmarker, GL_COMPILE);
		glColor3f(dimFactor * GEO_RED, dimFactor * GEO_GREEN, dimFactor * GEO_BLUE);
		glLineWidth(OBS_LINE_SIZE);

		glBegin(GL_LINE_STRIP);
			// North/South arm:
			sphVertex3D(RAdeg, DEdeg + GEO_ARM_LEN_DEG, sphRadius);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
			// West/East arm:
			sphVertex3D(RAdeg + GEO_ARM_LEN_DEG, DEdeg, sphRadius);
		glEnd();

		// arm joint
		glPointSize(OBS_LINE_SIZE);
		glBegin(GL_POINTS);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
		glEnd();
	glEndList();

	/**
	 *  VIRGO Interferometer:
	 */

	RAdeg = RAofZenith(observatoryDrawTimeGMT, VIRGO_LONG);
	DEdeg = VIRGO_LAT;

	// delete existing, create new (required for windoze)
	if(VIRGOmarker) glDeleteLists(VIRGOmarker, 1);
	VIRGOmarker = glGenLists(1);
	glNewList(VIRGOmarker, GL_COMPILE);
      glColor3f(dimFactor * VIRGO_RED, dimFactor * VIRGO_GREEN, dimFactor * VIRGO_BLUE);
		glLineWidth(OBS_LINE_SIZE);

		glBegin(GL_LINE_STRIP);
			// North/South arm:
			sphVertex3D(RAdeg, DEdeg + VIRGO_ARM_LEN_DEG, sphRadius);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
			// West/East arm:
			sphVertex3D(RAdeg - VIRGO_ARM_LEN_DEG, DEdeg, sphRadius);
		glEnd();

		// arm joint
		glPointSize(OBS_LINE_SIZE);
		glBegin(GL_POINTS);
			sphVertex3D(RAdeg, DEdeg, sphRadius);
		glEnd();
	glEndList();

	return;
   }

void Starsphere::make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size) {
   // Relative dimensions of marker components
	GLfloat inner_rad = MARKER_INNER_RADIUS_FACTOR * size;
   GLfloat outer_rad = MARKER_OUTER_RADIUS_FACTOR * size;
   GLfloat xhair_rad = MARKER_CROSSHAIR_RADIUS_FACTOR * size;

	// delete existing, create new (required for windoze)
	if(SearchMarker) glDeleteLists(SearchMarker, 1);
	SearchMarker = glGenLists(1);

	glNewList(SearchMarker, GL_COMPILE);
		// start gunsight drawing
		glPushMatrix();

		glLineWidth(MARKER_LINE_WIDTH);
		glColor3f(MARKER_RED, MARKER_GREEN, MARKER_BLUE);

		// First rotate east  to the RA position around y
		glRotatef(RAdeg, 0.0, 1.0, 0.0);
		// Then rotate up to DEC position around z (not x)
		glRotatef(DEdeg, 0.0, 0.0, 1.0);

		// Inner circle
		glBegin(GL_LINE_LOOP);
			for(int step = 0; step < MARKER_CURVE_STEPS; ++step) {
				sphVertex(inner_rad * COS(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                      inner_rad * SIN(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS ));
            }
		glEnd();

		// Outer circle
		glBegin(GL_LINE_LOOP);
         for(int step = 0; step < MARKER_CURVE_STEPS; ++step) {
				sphVertex(outer_rad * COS(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                      outer_rad * SIN(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS ));
            }
		glEnd();

		// Arms that form the gunsight
		glBegin(GL_LINES);
			//  North arm:
			sphVertex(0.0, inner_rad);
			sphVertex(0.0, xhair_rad);
			//  South arm:
			sphVertex(0.0, -inner_rad);
			sphVertex(0.0, -xhair_rad);
			// East arm:
			sphVertex(-inner_rad, 0.0);
			sphVertex(-xhair_rad, 0.0);
			// West arm:
			sphVertex(inner_rad, 0.0);
			sphVertex(xhair_rad, 0.0);
		glEnd();

		glPopMatrix();
	glEndList();
   }

/**
 * XYZ coordinate axes: (if we want them - most useful for testing)
 */
void Starsphere::make_axes() {
	// delete existing, create new (required for windoze)
	if(Axes) glDeleteLists(Axes, 1);
	Axes = glGenLists(1);

   // minimum of sphRadius and viewpt_radius
   GLfloat axes_length = AXES_RATIO * sphRadius;

	glNewList(Axes, GL_COMPILE);
		glLineWidth(AXES_LINE_WIDTH);

		glBegin(GL_LINES);
			glColor3f(X_AXIS_RED, X_AXIS_GREEN, X_AXIS_BLUE);
			glVertex3f(- axes_length, 0.0, 0.0);
			glVertex3f(axes_length, 0.0, 0.0);

			glColor3f(Y_AXIS_RED, Y_AXIS_GREEN, Y_AXIS_BLUE);
			glVertex3f(0.0, - axes_length, 0.0);
			glVertex3f(0.0, axes_length, 0.0);

			glColor3f(Z_AXIS_RED, Z_AXIS_GREEN, Z_AXIS_BLUE);
			glVertex3f(0.0, 0.0, - axes_length);
			glVertex3f(0.0, 0.0, axes_length);
		glEnd();
	glEndList();
   }

/**
 * RA/DEC coordinate grid on the sphere
 */
void Starsphere::make_globe() {

	GLfloat RAdeg, DEdeg;

	// delete existing, create new (required for windoze)
	if(sphGrid) glDeleteLists(sphGrid, 1);
	sphGrid = glGenLists(1);
	glNewList(sphGrid, GL_COMPILE);
		glLineWidth(GLOBE_LINE_SIZE);

		// Lines of constant Right Ascension ( East Longitude )
		for(int asc_step = 0; asc_step < RIGHT_ASCENSION_SLICES; ++asc_step) {
			RAdeg = asc_step * RIGHT_ASCENSION_STEP_DEG;
			glColor3f(MERID_RED, MERID_GREEN, MERID_BLUE);

			// mark meridian
			if(asc_step == 0) glColor3f(MERID_PRIME_RED, MERID_PRIME_GREEN, MERID_PRIME_BLUE);

			glBegin(GL_LINE_STRIP);
            for(int dec_step = 0; dec_step <= GLOBE_GRID_STEPS; ++dec_step) {
					DEdeg = dec_step * HALF_CIRCLE_DEG/GLOBE_GRID_STEPS - QUARTER_CIRCLE_DEG;
					sphVertex(RAdeg, DEdeg);
               }
			glEnd();
         }

		// Lines of constant Declination ( latitude )
		for(int dec_step = 1; dec_step < DECLINATION_STACKS; ++dec_step) {
			DEdeg = QUARTER_CIRCLE_DEG - dec_step * DECLINATION_STEP_DEG;
			glBegin(GL_LINE_STRIP);
				for(int asc_step = 0; asc_step <= GLOBE_GRID_STEPS; ++asc_step) {
					RAdeg = asc_step * FULL_CIRCLE_DEG/GLOBE_GRID_STEPS;
					sphVertex(RAdeg, DEdeg);
               }
			glEnd();
         }
	glEndList();
   }

/**
 * Window resize/remap
 */
void Starsphere::resize(const int width, const int height) {
	// store current settings
	m_CurrentWidth = width;
	m_CurrentHeight = height;
	aspect = (float)width / (float)height;

	// adjust HUD config
	m_YStartPosTop = height - 25;

	// make sure the search marker is updated (conditional rendering!)
	m_RefreshSearchMarker = true;

	// adjust aspect ratio and projection
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(95.0, aspect, 0.50, 5000.0);
	glMatrixMode(GL_MODELVIEW);
   }

/**
 *  What to do when graphics are "initialized".
 */
void Starsphere::initialize(const int width,
                            const int height,
                            const Resource *font,
                            const bool recycle) {
	// check whether we initialize the first time or have to recycle (required for windoze)
	if(!recycle) {
		// store the font resource
		if(font) m_FontResource = font;

		// initialize the BOINC client adapter
		m_BoincAdapter.initialize();

		// inital HUD offset setup
		m_XStartPosLeft = 5;
		m_YOffsetLarge = 18;

		setFeature(STARS, true);
		setFeature(CONSTELLATIONS, true);
		setFeature(PULSARS, true);
		setFeature(SNRS, true);
		setFeature(OBSERVATORIES, true);
		setFeature(SEARCHINFO, true);
		setFeature(LOGO, true);
		setFeature(MARKER, true);
      }
	else {
		// seems that windoze also "resets" our OpenGL fonts
		// let's clean up before reinitializing them
		if(m_FontLogo1) delete m_FontLogo1;
		if(m_FontLogo2) delete m_FontLogo2;
		if(m_FontHeader)delete m_FontHeader;
		if(m_FontText)  delete m_FontText;
      }

	// we might be called to recycle even before initialization
	if(!m_FontResource) {
		// display a warning, this could be unintentionally
		cerr << "Warning: font resource still unknown! You might want to recycle at a later stage..." << endl;
      }
	else {
		// note: initialization of logo font instances is done in subclasses!

		// create medium font instances using font resource (base address + size)
		m_FontHeader = new OGLFT::TranslucentTexture(&m_FontResource->data()->at(0),
                                                   m_FontResource->data()->size(),
                                                   13, 78 );

		if(m_FontHeader == 0 || !m_FontHeader->isValid()) {
		     cerr << "Could not construct header font face from in memory resource!" << endl;
		     return;
         }

		m_FontHeader->setForegroundColor(1.0, 1.0, 0.0, 1.0);

		// create small font instances using font resource (base address + size)
		m_FontText = new OGLFT::TranslucentTexture(&m_FontResource->data()->at(0),
                                                 m_FontResource->data()->size(),
                                                 11, 72 );

		if(m_FontText == 0 || !m_FontText->isValid()) {
		     cerr << "Could not construct text font face from in memory resource!" << endl;
		     return;
         }

		m_FontText->setForegroundColor(0.75, 0.75, 0.75, 1.0);
      }

	// setup initial dimensions
	resize(width, height);

	// more font setup and optimizations
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
#if defined( GL_RASTER_POSITION_UNCLIPPED_IBM )
	glEnable( GL_RASTER_POSITION_UNCLIPPED_IBM );
#endif

	// drawing setup:
	glClearColor(0.0, 0.0, 0.0, 0.0); // background is black
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// enable opt-in quality feature
	if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
		// some polishing
		glShadeModel(GL_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      }

	// FSAA will be enabled explicitly when needed!
	glDisable(GL_MULTISAMPLE_ARB);

	// we need alpha blending for proper font rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable depth buffering for 3D graphics
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// enable opt-in quality feature
	if(m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::MediumGraphicsQualitySetting ||
	   m_BoincAdapter.graphicsQualitySetting() == BOINCClientAdapter::HighGraphicsQualitySetting) {
		// fog aids depth perception
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogf(GL_FOG_DENSITY, 0.085);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
      }

	// create pre-drawn display lists
	make_stars();
	make_constellations();
	make_pulsars();
	make_snrs();
	make_axes();
	make_globe();

	glDisable(GL_CLIP_PLANE0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   }

/**
 * Rendering routine:  this is what does the drawing:
 */
void Starsphere::render(const double timeOfDay) {
   // If a sample is desired since last render, then capture it
   // first before Starsphere is evolved.
   if(SVGSample == true) {
      loadForSVG();
      sampleForSVG();
      emitSVG();
      // Reset sampling flag ie. it has to be manually
      // re-triggered by the relevant keystroke.
      SVGSample = false;
      }

   GLfloat vp_theta, vp_phi, vp_rad;
	GLfloat Zobs = 0.0;
	double revs, t, dt = 0;
	static double start_time = -1.0, last_time = -1.0;

	// Calculate the real time t since we started (or reset) and the
	// time dt since the last render() call. Both may be useful
	// for timing animations. Note that time_of_day is dtime().

   if(start_time < 0.0) {
      start_time = timeOfDay;
      }

   if(rotate_sphere == true){
      t = timeOfDay;
      }
   else{
      t = last_time;
      }

   t -= start_time;

   if(last_time < 0.0) {
      last_time = timeOfDay - 0.01;
      }

   dt = timeOfDay - last_time;

   if(rotate_sphere == true){
      last_time = timeOfDay; // remember for next time
      }

	// Now determine the rotation angle based on the time since start.
	// It is negative to get the rotation direction correct (the sun
	// rises in the East, so the sky sphere rotates E to W).

   Zrot = t*ROTATION_SPEED/60.0;
   revs = Zrot/FULL_CIRCLE_DEG;
	Zrot = - FULL_CIRCLE_DEG * (revs - (int)revs);

	// and start drawing...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// now draw the scene...
	glLoadIdentity();

	// Vary the viewpoint with both a long period wobble of the elevation
	// of the view and a longer period zoom in/out that might even penetrate
	// the starsphere for a brief time.   Increase the power in pow(,) to
	// make the visit inside briefer.

   vp_theta = QUARTER_CIRCLE_DEG - viewpt_elev + WOBBLE_AMP*sin(PI2*t/(WOBBLE_PERIOD * 60.0));
   vp_rad = viewpt_radius - ZOOM_AMP*sin(PI2*t/(ZOOM_PERIOD*60.0));

   vp_phi = viewpt_azimuth;

   if(vp_rad <0.0) vp_rad = 0.0; // cannot pass origin (confusing)

	// TRIED THIS TOO: -zoom_amp*pow(fabs(sin(PI2*t/(zoom_period*60.0))),3);
	xvp = vp_rad * SIN(vp_theta) * SIN(vp_phi);
	zvp = vp_rad * SIN(vp_theta) * COS(vp_phi);
	yvp = vp_rad * COS(vp_theta);

   gluLookAt(xvp, yvp, zvp, // eyes position
	          0.0, 0.0, 0.0, // looking toward here
	          0.0, 1.0, 0.0); // which way is up?  y axis!

	// draw axes before any rotation so they stay put
	if(isFeature(AXES)) glCallList(Axes);

	// draw the sky sphere, with rotation:
	glPushMatrix();
	glRotatef(Zrot - rotation_offset, 0.0, 1.0, 0.0);

   // stars, pulsars, supernovae, grid
	if(isFeature(STARS))			glCallList(Stars);
	if(isFeature(PULSARS))			glCallList(Pulsars);
	if(isFeature(SNRS))			glCallList(SNRs);
	if(isFeature(CONSTELLATIONS))	glCallList(Constellations);
	if(isFeature(GLOBE))			glCallList(sphGrid);

	// observatories move an extra 15 degrees/hr since they were drawn
	if (isFeature(OBSERVATORIES)) {
		glPushMatrix();
		Zobs = (timeOfDay - m_ObservatoryDrawTimeLocal) * 15.0/3600.0;
		glRotatef(Zobs, 0.0, 1.0, 0.0);
		glCallList(LLOmarker);
		glCallList(LHOmarker);
		glCallList(GEOmarker);
		glCallList(VIRGOmarker);
		renderAdditionalObservatories();
		glPopMatrix();
      }

	// draw the search marker (gunsight)
	if (isFeature(MARKER)) {
		if(m_RefreshSearchMarker) {
			make_search_marker(m_CurrentRightAscension, m_CurrentDeclination, MARKER_ANGULAR_SIZE);
			m_RefreshSearchMarker = false;
         }
		else {
			glCallList(SearchMarker);
         }
      }

	glPopMatrix();

	// draw 2D vectorized HUD
	if(isFeature(LOGO) || isFeature(SEARCHINFO)) {
		// disable depth testing since we're in 2D mode
		glDisable(GL_DEPTH_TEST);

		// enable textured fonts
		glEnable(GL_TEXTURE_2D);

		// save current state
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, m_CurrentWidth, 0, m_CurrentHeight, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		if (isFeature(LOGO)) renderLogo();
		if (isFeature(SEARCHINFO)) renderSearchInformation();

		// restore original state
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// disable font textures
		glDisable(GL_TEXTURE_2D);

		// enable depth testing since we're leaving 2D mode
		glEnable(GL_DEPTH_TEST);
      }

	SDL_GL_SwapBuffers();
   }

void Starsphere::renderAdditionalObservatories() {
	// default implementation doesn't do anything
   }

void Starsphere::mouseButtonEvent(const int positionX,
                                  const int positionY,
                                  const AbstractGraphicsEngine::MouseButton buttonPressed) {
   }

void Starsphere::mouseMoveEvent(const int deltaX,
                                const int deltaY,
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
      // TODO/CONTROL : point of control for key used to trigger SVG production
      case KeyF1:
         SVGSample = true;
         break;
      case KeySpace:
         toggleRotation();
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
	if(viewpt_elev > 89.0) {
		viewpt_elev = +89.0;
      }
	if(viewpt_elev < -89.0) {
		viewpt_elev = -89.0;
      }

	// rotation
	rotation_offset -= relativeRotation/10.0;
   }

void Starsphere::zoomSphere(const int relativeZoom) {
	// zoom
	viewpt_radius -= relativeZoom/10.0;
	if(viewpt_radius > 15.0) {
		viewpt_radius = 15.0;
      }
   if(viewpt_radius < 0.5) {
		viewpt_radius = 0.5;
      }
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

void Starsphere::loadForSVG(void) {
   // Do the axes, if enabled.
   if(isFeature(AXES)) {
      // minimum of sphRadius and viewpt_radius
      GLfloat axes_length = AXES_RATIO * sphRadius;

      // x axis is at RA of 0 and 180 degrees in the x-y plane.
      axes.push_back(std::pair<VectorSPR, VectorSPR>(VectorSPR(0, 0, axes_length),
                                                     VectorSPR(180, 0, axes_length)));
      // y axis is at DEC of 90 and -90 degrees.
      axes.push_back(std::pair<VectorSPR, VectorSPR>(VectorSPR(0, 90, axes_length),
                                                     VectorSPR(0, -90, axes_length)));
      // z axis is at RA of 90 and 270 degrees in the x-y plane.
      axes.push_back(std::pair<VectorSPR, VectorSPR>(VectorSPR(90, 0, axes_length),
                                                     VectorSPR(270, 0, axes_length)));
      }

   // Do the stars, if enabled.
   if(isFeature(STARS)) {
      stars.clear();
      for(int index = 0; index < Nstars; index ++) {
         VectorSPR star = VectorSPR(star_info[index][RA_ARR_POS],
                                    star_info[index][DEC_ARR_POS],
                                    sphRadius);

         // But we have to exclude repeated entries ( star_info[] array has many )

         // Assume it's not already entered into our container of star coordinates.
         bool already_in = false;

         // Iterate over all entries currently within our container
         for(std::vector<VectorSPR>::const_iterator search_star = stars.begin();
             search_star < stars.end();
             search_star++) {
             // Have we found a matching entry?
             if(search_star->isEqual(star)) {
               // Yes, we have found a match. Set the flag and exit this search.
               already_in = true;
               break;
               }
            // No, keep looking .....
            }

         // If, after looking through all of what is currently in our container ...
         if(!already_in) {
            // ..... and it's not mentioned, then include it.
            stars.push_back(star);
            }
         }
      }

   // Do the pulsars, if enabled.
   if(isFeature(PULSARS)) {
      pulsars.clear();
      for(int index = 0; index < Npulsars; index ++) {
         VectorSPR pulsar = VectorSPR(pulsar_info[index][RA_ARR_POS],
                                      pulsar_info[index][DEC_ARR_POS],
                                      sphRadius);
         pulsars.push_back(pulsar);
         }
      }

   // Do the supernovae, if enabled.
   if(isFeature(SNRS)) {
      supernovae.clear();
      for(int index = 0; index < NSNRs; index ++) {
         VectorSPR supernova = VectorSPR(SNR_info[index][RA_ARR_POS],
                                         SNR_info[index][DEC_ARR_POS],
                                         sphRadius);
         supernovae.push_back(supernova);
         }
      }

   // Do the constellations, if enabled.
   if(isFeature(CONSTELLATIONS)) {
      constellations.clear();

      // This ensures only pairs of star positions will be
      // taken up. If there's one left over, it will be ignored.
      int NLinks = Nstars/2;
      // Examine star pairings and form the links.
      for(int index = 0; index < NLinks; index ++) {
         VectorSPR link1 = VectorSPR(star_info[index*2][RA_ARR_POS],
                                     star_info[index*2][DEC_ARR_POS],
                                     sphRadius);
         VectorSPR link2 = VectorSPR(star_info[index*2 + 1][RA_ARR_POS],
                                     star_info[index*2 + 1][DEC_ARR_POS],
                                     sphRadius);
         constellations.push_back(std::pair<VectorSPR, VectorSPR>(link1, link2));
         }
      }

   // Do the globe, if enabled.
   if(isFeature(GLOBE)) {
      // Lines of constant Right Ascension ( East Longitude )
      globe_slices.clear();
		for(int asc_step = 0; asc_step < RIGHT_ASCENSION_SLICES; ++asc_step) {
			GLfloat RAdeg = asc_step * RIGHT_ASCENSION_STEP_DEG;
         std::vector<VectorSPR> slice;

			for(int dec_step = 0; dec_step <= GLOBE_GRID_STEPS; ++dec_step) {
            GLfloat DEdeg = dec_step * HALF_CIRCLE_DEG/GLOBE_GRID_STEPS - QUARTER_CIRCLE_DEG;
            slice.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
            }
         globe_slices.push_back(slice);
			}

      // Lines of constant Declination ( latitude )
      globe_stacks.clear();
		for(int dec_step = 1; dec_step < DECLINATION_STACKS; ++dec_step) {
			GLfloat DEdeg = QUARTER_CIRCLE_DEG - dec_step * DECLINATION_STEP_DEG;
         std::vector<VectorSPR> stack;

			for(int asc_step = 0; asc_step <= GLOBE_GRID_STEPS; ++asc_step) {
			   GLfloat RAdeg = asc_step * FULL_CIRCLE_DEG/GLOBE_GRID_STEPS;
				stack.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
            }
         globe_stacks.push_back(stack);
			}
      }

   if(isFeature(OBSERVATORIES)) {
      GLfloat RAdeg;
      GLfloat DEdeg;
      // LLOmarker
      LLO.clear();
      RAdeg = RAofZenith(observatoryDrawTimeGMT, LLO_LONG);
      DEdeg = LLO_LAT;

      LLO.push_back(VectorSPR(RAdeg, DEdeg - LLO_ARM_LEN_DEG, sphRadius));
      LLO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
      LLO.push_back(VectorSPR(RAdeg - LLO_ARM_LEN_DEG, DEdeg, sphRadius));
      LLO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));

		// LHOmarker
      LH1.clear();
      RAdeg = RAofZenith(observatoryDrawTimeGMT, LHO_LONG);
      DEdeg = LHO_LAT;

      LH1.push_back(VectorSPR(RAdeg, DEdeg + LHO_ARM_LEN_DEG, sphRadius));
      LH1.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
      LH1.push_back(VectorSPR(RAdeg - LHO_ARM_LEN_DEG, DEdeg, sphRadius));
      LH1.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));

      LH2.push_back(VectorSPR(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_ARM_LEN_DEG/2 + LHO_H2_OFFSET, sphRadius));
      LH2.push_back(VectorSPR(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_H2_OFFSET, sphRadius));
      LH2.push_back(VectorSPR(RAdeg - LHO_ARM_LEN_DEG/2  - LHO_H2_OFFSET, DEdeg  + LHO_H2_OFFSET, sphRadius));
      LH2.push_back(VectorSPR(RAdeg - LHO_H2_OFFSET, DEdeg + LHO_H2_OFFSET, sphRadius));

      // GEOmarker
      GEO.clear();
      RAdeg = RAofZenith(observatoryDrawTimeGMT, GEO_LONG);
      DEdeg = GEO_LAT;

      GEO.push_back(VectorSPR(RAdeg, DEdeg + GEO_ARM_LEN_DEG, sphRadius));
      GEO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
      GEO.push_back(VectorSPR(RAdeg + GEO_ARM_LEN_DEG, DEdeg, sphRadius));
      GEO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));

      // VIRGOmarker
      VIRGO.clear();
      RAdeg = RAofZenith(observatoryDrawTimeGMT, VIRGO_LONG);
      DEdeg = VIRGO_LAT;

      VIRGO.push_back(VectorSPR(RAdeg, DEdeg + VIRGO_ARM_LEN_DEG, sphRadius));
      VIRGO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
      VIRGO.push_back(VectorSPR(RAdeg - VIRGO_ARM_LEN_DEG, DEdeg, sphRadius));
      VIRGO.push_back(VectorSPR(RAdeg, DEdeg, sphRadius));
      }

   // Do the search marker, if enabled.
   if(isFeature(MARKER)) {
      GLfloat inner_rad = MARKER_INNER_RADIUS_FACTOR * MARKER_ANGULAR_SIZE;
      GLfloat outer_rad = MARKER_OUTER_RADIUS_FACTOR * MARKER_ANGULAR_SIZE;
      GLfloat xhair_rad = MARKER_CROSSHAIR_RADIUS_FACTOR * MARKER_ANGULAR_SIZE;

      // Inner circle
      for(int step = 0; step < MARKER_CURVE_STEPS; ++step) {
         marker_inner.push_back(VectorSPR(inner_rad * COS(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                                          inner_rad * SIN(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                                          sphRadius));
         }

		// Outer circle
      for(int step = 0; step < MARKER_CURVE_STEPS; ++step) {
         marker_outer.push_back(VectorSPR(outer_rad * COS(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                                          outer_rad * SIN(step*FULL_CIRCLE_DEG/MARKER_CURVE_STEPS),
                                          sphRadius));
         }

		// Arms that form the gunsight
      // North arm
      marker_xhair.push_back(VectorSPR(0.0, inner_rad, sphRadius));
      marker_xhair.push_back(VectorSPR(0.0, xhair_rad, sphRadius));

      // South arm
      marker_xhair.push_back(VectorSPR(0.0, -inner_rad, sphRadius));
      marker_xhair.push_back(VectorSPR(0.0, -xhair_rad, sphRadius));

      // East arm
      marker_xhair.push_back(VectorSPR(-inner_rad, 0.0, sphRadius));
      marker_xhair.push_back(VectorSPR(-xhair_rad, 0.0, sphRadius));

      // West arm
      marker_xhair.push_back(VectorSPR(inner_rad, 0.0, sphRadius));
      marker_xhair.push_back(VectorSPR(xhair_rad, 0.0, sphRadius));
      }
   }

void Starsphere::sampleForSVG(void) {
   // Isolating the any effects using the matrix stack.
   glPushMatrix();
	glLoadIdentity();

   // Reproduce the transform sequence as used for rendering.
   gluLookAt(xvp, yvp, zvp, // eyes position
	          0.0, 0.0, 0.0, // looking toward here
	          0.0, 1.0, 0.0); // which way is up?  y axis!

   // Take a snapshot of the transform matrices by inquiring of OpenGL.
   glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
   glGetIntegerv(GL_VIEWPORT, viewport);

   // Do the axes, if enabled but before any coordinate rotation.
   if(isFeature(AXES)) {
      axes_trans.clear();
      for(std::vector<std::pair<VectorSPR, VectorSPR> >::const_iterator axis = axes.begin();
         axis != axes.end();
         axis++) {
         Vector3D one = transformPoint(axis->first);
         Vector3D two = transformPoint(axis->second);
         axes_trans.push_back(std::pair<Vector3D, Vector3D>(one,two));
         }
      }

   // Now rotate.
   glRotatef(Zrot - rotation_offset, 0.0, 1.0, 0.0);

   // Take a snapshot of the transform matrices by inquiring of OpenGL.
   glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
   glGetIntegerv(GL_VIEWPORT, viewport);

   // Do the stars, if enabled.
   if(isFeature(STARS)) {
      transformContainer(stars, stars_trans);
      }

   // Do the pulsars, if enabled.
   if(isFeature(PULSARS)) {
      transformContainer(pulsars, pulsars_trans);
      }

   // Do the supernovae, if enabled.
   if(isFeature(SNRS)) {
      transformContainer(supernovae, supernovae_trans);
      }

   // Do the constellations, if enabled.
   if(isFeature(CONSTELLATIONS)) {
      cons_trans.clear();
      for(std::vector<std::pair<VectorSPR, VectorSPR> >::const_iterator link = constellations.begin();
         link != constellations.end();
         link++) {
         cons_trans.push_back(std::pair<Vector3D, Vector3D>(transformPoint(link->first),
                                                            transformPoint(link->second)));
         }
      }

   // Do the globe, if enabled.
   if(isFeature(GLOBE)) {
      // Lines of constant Right Ascension ( East Longitude )
      globe_slices_trans.clear();
      for(std::vector<std::vector<VectorSPR> >::const_iterator slice = globe_slices.begin();
         slice != globe_slices.end();
         ++slice) {
         std::vector<Vector3D> slice_trans;
         transformContainer(*slice, slice_trans);
         globe_slices_trans.push_back(slice_trans);
         }

      // Lines of constant Declination ( latitude )
      globe_stacks_trans.clear();
      for(std::vector<std::vector<VectorSPR> >::const_iterator stack = globe_stacks.begin();
         stack != globe_stacks.end();
         ++stack) {
         std::vector<Vector3D> stack_trans;
         transformContainer(*stack, stack_trans);
         globe_stacks_trans.push_back(stack_trans);
         }
      }

   if(isFeature(OBSERVATORIES)) {
      // LLO markers
      transformContainer(LLO, LLO_trans);

      // LHO markers
      transformContainer(LH1, LH1_trans);
      transformContainer(LH2, LH2_trans);

      // GEOmarker
      transformContainer(GEO, GEO_trans);

      // VIRGOmarker
      transformContainer(VIRGO, VIRGO_trans);
      }

   // Do the search marker, if enabled.
   if(isFeature(MARKER)) {
      // First rotate east to the right ascension position, around the y-axis
      glRotatef(m_CurrentRightAscension, 0.0, 1.0, 0.0);

      // Then rotate up to the declination position around the z-axis
		glRotatef(m_CurrentDeclination, 0.0, 0.0, 1.0);

      // Take a snapshot of the transform matrices by inquiring of OpenGL.
      glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
      glGetIntegerv(GL_VIEWPORT, viewport);

      transformContainer(marker_inner, marker_inner_trans);
      transformContainer(marker_outer, marker_outer_trans);
      transformContainer(marker_xhair, marker_xhair_trans);
      }

   // Restore the matrix stack.
   glPopMatrix();
   }

void Starsphere::emitSVG(void) {
   // For point objects we need the SVG radius to be half of the OpenGL size.

   // TODO/CONTROL : point of control for color specification. Substitute other acceptable
   // SVG versions of color declaration here .... for stroke and fill attributes.
   std::string star_color(SVGColorAttribute(STAR_RED, STAR_GREEN, STAR_BLUE));
   std::string pulsar_color(SVGColorAttribute(PULSAR_RED, PULSAR_GREEN, PULSAR_BLUE));
   std::string supernova_color(SVGColorAttribute(SNR_RED, SNR_GREEN, SNR_BLUE));
   std::string link_color(SVGColorAttribute(CONS_LINK_RED, CONS_LINK_GREEN, CONS_LINK_BLUE));
   std::string globe_meridian_color(SVGColorAttribute(MERID_RED, MERID_GREEN, MERID_BLUE));
   std::string globe_prime_meridian_color(SVGColorAttribute(MERID_PRIME_RED, MERID_PRIME_GREEN, MERID_PRIME_BLUE));
   std::string LLO_color(SVGColorAttribute(LLO_RED, LLO_GREEN, LLO_BLUE));
   std::string LHO_color(SVGColorAttribute(LHO_RED, LHO_GREEN, LHO_BLUE));
   std::string GEO_color(SVGColorAttribute(GEO_RED, GEO_GREEN, GEO_BLUE));
   std::string VIRGO_color(SVGColorAttribute(VIRGO_RED, VIRGO_GREEN, VIRGO_BLUE));
   std::string X_AXIS_color(SVGColorAttribute(X_AXIS_RED, X_AXIS_GREEN, X_AXIS_BLUE));
   std::string Y_AXIS_color(SVGColorAttribute(Y_AXIS_RED, Y_AXIS_GREEN, Y_AXIS_BLUE));
   std::string Z_AXIS_color(SVGColorAttribute(Z_AXIS_RED, Z_AXIS_GREEN, Z_AXIS_BLUE));
   std::string marker_color(SVGColorAttribute(MARKER_RED, MARKER_GREEN, MARKER_BLUE));

   // Set start and end of filename pattern.
   // TODO/CONTROL : point of control for output file base name.
   static const std::string filename_stub = "EAH_starsphere_sample_";
   // TODO/CONTROL : point of control for output file extension.
   static const std::string filename_ext = ".svg";

   // Construct string to represent file emission counter.
   std::stringstream filename_postfix(stringstream::in | stringstream::out);
   filename_postfix << SVG_emit_count;

   // Construct the full output filename for this instance.
   std::string output_filename = filename_stub +
                                 filename_postfix.str() +
                                 filename_ext;

   // Create the output stream to write to.
   ofstream outfile(output_filename.c_str());

   // Confusion can reign here due to need to embed double quotes via escapes,
   // so \" inside a string delimited by " gets you a lone " in the file.
   // TODO/CONTROL for data type declarations, xml version etc
   outfile << "<?xml version=\"1.0\"?>\n";

   // Apparently the latest and greatest.
   outfile << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" "
           << "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n";

   // SVG document dimensions
   outfile << "<svg width=\""
           << m_CurrentWidth
           << "\" height=\""
           << m_CurrentHeight
           << "\">\n";

   // SVG document title
   outfile << "\t<title>Einstein At Home Starsphere</title>\n";

   // SVG short description
   outfile << "\t<desc>Screensaver View Sample</desc>\n";

   // FWIW features are included in the file in the order in which they
   // were presented to OpenGL for rendering.

   // Do the axes, if enabled.
   if(isFeature(AXES)) {
      outfile << "\t<!-- axes START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(axes_trans[0].first, axes_trans[0].second),
                            AXES_LINE_WIDTH,
                            X_AXIS_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(axes_trans[1].first, axes_trans[1].second),
                            AXES_LINE_WIDTH,
                            Y_AXIS_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(axes_trans[2].first, axes_trans[2].second),
                            AXES_LINE_WIDTH,
                            Z_AXIS_color);
      outfile << "\t<!-- axes END -->\n";
      }

   // Do the stars, if enabled.
   if(isFeature(STARS)) {
      outfile << "\t<!-- stars START -->\n";
      for(std::vector<Vector3D>::const_iterator star = stars_trans.begin();
         star != stars_trans.end();
         ++star) {
         outfile << "\t\t"
                 << SVGCircleTag(*star,
                                 STAR_MAG_SIZE/2,
                                 star_color);
         }
      outfile << "\t<!-- stars END -->\n";
      }

   // Do the pulsars, if enabled.
   if(isFeature(PULSARS)) {
      outfile << "\t<!-- pulsars START -->\n";
      for(std::vector<Vector3D>::const_iterator pulsar = pulsars_trans.begin();
         pulsar != pulsars_trans.end();
         ++pulsar) {
         outfile << "\t\t"
                 << SVGCircleTag(*pulsar,
                                 PULSAR_MAG_SIZE/2,
                                 pulsar_color);
         }
      outfile << "\t<!-- pulsars END -->\n";
      }

   // Do the supernovae, if enabled.
   if(isFeature(SNRS)) {
      outfile << "\t<!-- supernovae START -->\n";
      for(std::vector<Vector3D>::const_iterator supernova = supernovae_trans.begin();
         supernova != supernovae_trans.end();
         ++supernova) {
         outfile << "\t\t"
                 << SVGCircleTag(*supernova,
                                 SNR_MAG_SIZE/2,
                                 supernova_color);
         }
      outfile << "\t<!-- supernovae END -->\n";
      }

   // Do the constellations, if enabled.
   if(isFeature(CONSTELLATIONS)) {
      outfile << "\t<!-- constellation links START -->\n";
      for(std::vector<std::pair<Vector3D, Vector3D> >::const_iterator link = cons_trans.begin();
         link != cons_trans.end();
         ++link) {
         outfile << "\t\t\t"
                 << SVGLineTag(std::pair<Vector3D, Vector3D>(link->first, link->second),
                               CONS_LINK_SIZE,
                               link_color);
         }
      outfile << "\t<!-- constellation links END -->\n";
      }

   if(isFeature(GLOBE)) {
      outfile << "\t<!-- globe grid START -->\n";

      outfile << "\t\t<!-- globe grid slices START -->\n";
      for(std::vector<std::vector<Vector3D> >::const_iterator slice = globe_slices_trans.begin();
         slice != globe_slices_trans.end();
         ++slice) {
         // Set the slice color.
         std::string slice_color = globe_meridian_color;

         // Special case is meridian or slice zero.
         if(slice == globe_slices_trans.begin()) {
            slice_color = globe_prime_meridian_color;
            }

         for(std::vector<Vector3D>::const_iterator point = slice->begin();
            point != slice->end();
            ++point) {

            // Need a distinct iterator to the next point, if any.
            std::vector<Vector3D>::const_iterator next = point;
            ++next;
            // Have we over-reached the container using 'next'? Or put another way,
            // was 'point' therefore the last item?
            if(next != slice->end()) {
               // No, it wasn't, so use both iterators to dereference.
               outfile << "\t\t\t"
                       << SVGLineTag(std::pair<Vector3D, Vector3D>(*point, *next),
                                     GLOBE_LINE_SIZE,
                                     slice_color);
              }
            }
         }
      outfile << "\t\t<!-- globe grid slices END -->\n";

      outfile << "\t\t<!-- globe grid stacks START -->\n";
      for(std::vector<std::vector<Vector3D> >::const_iterator stack = globe_stacks_trans.begin();
         stack != globe_stacks_trans.end();
         ++stack) {
         for(std::vector<Vector3D>::const_iterator point = stack->begin();
            point != stack->end();
            ++point) {

            // Need a distinct iterator to the next point, if any.
            std::vector<Vector3D>::const_iterator next = point;
            ++next;
            // Have we over-reached the container using 'next'? Or put another way,
            // was 'point' therefore the last item?
            if(next == stack->end()) {
               // Yes, so to make a line loop, hook to first point.
               next = stack->begin();
               }
            outfile << "\t\t\t"
                    << SVGLineTag(std::pair<Vector3D, Vector3D>(*point, *next),
                                  GLOBE_LINE_SIZE,
                                  globe_meridian_color);
            }
         }
      outfile << "\t\t<!-- globe grid stacks END -->\n";
      outfile << "\t<!-- globe grid END -->\n";
      }

   if(isFeature(OBSERVATORIES)) {
      outfile << "\t<!-- observatories START -->\n";
      // LLO markers
      outfile << "\t\t<!-- LLO marker START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LLO_trans[0], LLO_trans[1]),
                            OBS_LINE_SIZE,
                            LLO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LLO_trans[1], LLO_trans[2]),
                            OBS_LINE_SIZE,
                            LLO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LLO_trans[3], LLO_trans[3]),
                            OBS_LINE_SIZE,
                            LLO_color);
      outfile << "\t\t<!-- LLO marker END -->\n";

      // LHO markers
      outfile << "\t\t<!-- LHO markers START -->\n";
      outfile << "\t\t\t<!-- LH1 marker START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH1_trans[0], LH1_trans[1]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH1_trans[1], LH1_trans[2]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH1_trans[3], LH1_trans[3]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t\t<!-- LH1 marker END -->\n";
      outfile << "\t\t\t<!-- LH2 marker START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH2_trans[0], LH2_trans[1]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH2_trans[1], LH2_trans[2]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(LH2_trans[3], LH2_trans[3]),
                            OBS_LINE_SIZE,
                            LHO_color);
      outfile << "\t\t\t<!-- LH2 marker END -->\n";
      outfile << "\t\t<!-- LHO markers END -->\n";

      // GEO marker
      outfile << "\t\t<!-- GEO marker START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(GEO_trans[0], GEO_trans[1]),
                            OBS_LINE_SIZE,
                            GEO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(GEO_trans[1], GEO_trans[2]),
                            OBS_LINE_SIZE,
                            GEO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(GEO_trans[3], GEO_trans[3]),
                            OBS_LINE_SIZE,
                            GEO_color);
      outfile << "\t\t<!-- GEO marker END -->\n";

      // VIRGO marker
      outfile << "\t\t<!-- VIRGO marker START -->\n";
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(VIRGO_trans[0], VIRGO_trans[1]),
                            OBS_LINE_SIZE,
                            VIRGO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(VIRGO_trans[1], VIRGO_trans[2]),
                            OBS_LINE_SIZE,
                            VIRGO_color);
      outfile << "\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(VIRGO_trans[3], VIRGO_trans[3]),
                            OBS_LINE_SIZE,
                            VIRGO_color);
      outfile << "\t\t<!-- VIRGO marker END -->\n";
      outfile << "\t<!-- observatories END -->\n";
      }

   // Do the search marker, if enabled.
   if(isFeature(MARKER)) {
      outfile << "\t<!-- search marker START -->\n";
      outfile << "\t\t<!-- inner marker START -->\n";
      for(std::vector<Vector3D>::const_iterator point = marker_inner_trans.begin();
          point != marker_inner_trans.end();
          ++point) {
         // Need a distinct iterator to the next point, if any.
         std::vector<Vector3D>::const_iterator next = point;
         ++next;
         // Have we over-reached the container using 'next'? Or put another way,
         // was 'point' therefore the last item?
         if(next == marker_inner_trans.end()) {
            // Yes, so to make a line loop, hook to first point.
            next = marker_inner_trans.begin();
            }
         outfile << "\t\t\t"
                 << SVGLineTag(std::pair<Vector3D, Vector3D>(*point, *next),
                               MARKER_LINE_WIDTH,
                               marker_color);
         }
      outfile << "\t\t<!-- inner marker END -->\n";
      outfile << "\t\t<!-- outer marker START -->\n";
      for(std::vector<Vector3D>::const_iterator point = marker_outer_trans.begin();
          point != marker_outer_trans.end();
          ++point) {
         // Need a distinct iterator to the next point, if any.
         std::vector<Vector3D>::const_iterator next = point;
         ++next;
         // Have we over-reached the container using 'next'? Or put another way,
         // was 'point' therefore the last item?
         if(next == marker_outer_trans.end()) {
            // Yes, so to make a line loop, hook to first point.
            next = marker_outer_trans.begin();
            }
         outfile << "\t\t\t"
                 << SVGLineTag(std::pair<Vector3D, Vector3D>(*point, *next),
                               MARKER_LINE_WIDTH,
                               marker_color);
         }
      outfile << "\t\t<!-- outer marker END -->\n";
      outfile << "\t\t<!-- cross-hair marker START -->\n";
      outfile << "\t\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(marker_xhair_trans[0], marker_xhair_trans[1]),
                            MARKER_LINE_WIDTH,
                            marker_color);
      outfile << "\t\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(marker_xhair_trans[2], marker_xhair_trans[3]),
                            MARKER_LINE_WIDTH,
                            marker_color);
      outfile << "\t\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(marker_xhair_trans[4], marker_xhair_trans[5]),
                            MARKER_LINE_WIDTH,
                            marker_color);
      outfile << "\t\t\t"
              << SVGLineTag(std::pair<Vector3D, Vector3D>(marker_xhair_trans[6], marker_xhair_trans[7]),
                            MARKER_LINE_WIDTH,
                            marker_color);
      outfile << "\t\t<!-- cross-hair marker END -->\n";

      outfile << "\t<!-- search marker END -->\n";
      }

   // Close off SVG/XML construct for well-formedness, then close the file.
   outfile << "</svg>\n";
   outfile.close();

   // Bump up file emission counter.
   SVG_emit_count ++;
   }

void Starsphere::toggleRotation(void) {
   rotate_sphere = !rotate_sphere;
   }

void Starsphere::transformContainer(const std::vector<VectorSPR>& input, std::vector<Vector3D>& output) {
   // Erase the output container.
   output.clear();

   for(std::vector<VectorSPR>::const_iterator point = input.begin();
       point != input.end();
       ++point) {
      output.push_back(transformPoint(*point));
      }
   }

Vector3D Starsphere::transformPoint(const VectorSPR& input) {
   Vector3D ret_val;

   // Holders for window coordinate values returned by gluProject().
   GLdouble winx;
   GLdouble winy;
   GLdouble winz;

   // Flag to test success of transform effort.
   int transform_flag = GL_TRUE;

   // Perform the transform.
   transform_flag = gluProject(input.x(), input.y(), input.z(),
                               modelMatrix,
                               projMatrix,
                               viewport,
                               &winx, &winy, &winz);

   // Check the outcome.
   if(transform_flag == GL_TRUE) {
      // Transform has succeeded, store the result.
      ret_val.set_x(winx);
      ret_val.set_y(winy);
      ret_val.set_z(winz);
      }
   else {
      // Notify failure. NB the default constructed value of a Vector3D will
      // now be returned : this is the null vector.
      std::cerr << "Starsphere::transform_point() : failed point transform"
                << std::endl;
      }

   return ret_val;
   }

std::string Starsphere::SVGColorAttribute(GLfloat red, GLfloat green, GLfloat blue) const {
   static const uint FULL_COLOR(255);

   stringstream tag;

   tag << "rgb("
       << static_cast<uint>(red * FULL_COLOR)
       << ", "
       << static_cast<uint>(green * FULL_COLOR)
       << ", "
       << static_cast<uint>(blue * FULL_COLOR)
       << ")";

   return tag.str();
   }

std::string Starsphere::SVGLineTag(std::pair<Vector3D, Vector3D> ends,
                                   GLfloat width,
                                   std::string color) const {
   stringstream tag;

   tag << "<line x1=\""
       << ends.first.x()
       << "\" y1=\""
       << ends.first.y()
       << "\" x2=\""
       << ends.second.x()
       << "\" y2=\""
       << ends.second.y()
       << "\" stroke-width=\""
       << width
       << "\" stroke=\""
       << color.c_str()
       << "\" fill=\""
       << color.c_str()
       << "\"/>\n";

   return tag.str();
   }

std::string Starsphere::SVGCircleTag(Vector3D centre,
                                     GLfloat radius,
                                     std::string color) const {
   stringstream tag;

   tag << "<circle cx=\""
       << centre.x()
       << "\" cy=\""
       << centre.y()
       << "\" r=\""
       << radius
       << "\""
       << " stroke=\""
       << color.c_str()
       << "\" fill=\""
       << color.c_str()
       << "\"/>\n";

   return tag.str();
   }
