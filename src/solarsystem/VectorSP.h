/***************************************************************************
 *   Copyright (C) 2010 by Mike Hewson                                     *
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

#ifndef VECTORSP_H_
#define VECTORSP_H_

#include <cmath>

#include "SolarSystemGlobals.h"
#include "Vector3D.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief %Solarsystem Three dimensional vector data type.
 *
 *    This class comprises a spherical polar co-ordinate triplet. Within the
 * current inheritance heirarchy this class acts as a converter of spherical
 * polar to Cartesian coordinates, but not vice-versa.
 *    A spherical coordinate system is classically described as having two
 * angles and a length for each point in 3D space. The length is that of the
 * radius vector from the origin to the point. For this discussion a 'great'
 * circle is one that is of a given radius and has it's centre at the origin.
 *    One angle is 'longitude' or angle measure along a great circle centered
 * upon the origin, where one point on that circle is designated as of zero
 * longitude. Call this great circle the 'equator' of the coordinate system.
 * An axis or line can then be drawn, perpendicular to that plane formed by this
 * great circle and the origin, projecting from the origin to distance of the
 * given radius but above and below the plane. This gives you points of 'north'
 * and 'south' poles at the ends of these line segments, where these points are
 * then given the latitude values of plus and minus 90 degrees respectively.
 * All points on the equator are given a latitude value of zero.
 *    Points not either polar or upon the equator are determined thus :
 *    - latitude is that angle between the radius vector to that point and
 *      the projection of that vector onto the equatorial plane
 *    - latitude is positive for points on the 'north' polar side of the
 *      equatorial plane, and negative if on the 'south' polar side
 *    - form a great circle ( there will only be one ) that passes through
 *      each pole and the point of interest. Call that circle a meridian.
 *    - that meridian will intersect the equator at two points
 *    - for that point of intersection nearest the point in question, it's
 *      longitude becomes the longitude of the said point.
 *    - the longitude of the poles needs to be either assigned or ignored,
 *      and in a special sense represents a ( coordinate ) singularity. :-)
 *    If one considers the radius as ( effectively ) infinite the entire sky
 * from the origin appears to be 'painted' upon the inside of a sphere
 * which encloses and is centred upon the origin. So now one can describe the
 * position of objects thereupon using the two angles ie. the sky becomes a
 * two dimensional area albeit curved and not flat.
 *
 * \author Mike Hewson\n
 */

class VectorSP : public Vector3D {
   public:
      /**
       * \brief Constructor, no arguments, gives the null vector
       */
      VectorSP();

      /**
       * \brief Constructor, giving fully specified member values
       * in spherical polar co-ordinates.
       *
       * \param ra the right ascension co-ordinate ( in decimal degrees )
       *
       * \param dec the declination co-ordinate ( in decimal degrees )
       *
       * \param rd the radius co-ordinate
       */
      VectorSP(vec_t ra, vec_t dec, vec_t rd);

      /**
       * \brief Destructor
       */
      virtual ~VectorSP();
   };

#endif // VECTORSP_H_
