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

#ifndef PULRARS_H_
#define PULSARS_H_

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class manages the rendering of EAH discoverd pulsars.
 *
 *
 *
 * \see Renderable
 * \see Star
 *
 * \author Mike Hewson\n
 */

class PulsarsEAH : public Stars, public Traversable {
    public:
        /**
         * \brief Constructor
         *
         * \param rad : the radius from the origin where the Stars map to.
         * \param magsize : the pixel radius to render a Star at.
         * \param red : the red component of the RGB color.
         * \param green : the green component of the RGB color.
         * \param blue : the blue component of the RGB color.
         */
        PulsarsEAH(vec_t rad, GLfloat magsize,
                   GLclampf red,
                   GLclampf green,
                   GLclampf blue);

        /// Virtual destructor
        virtual ~PulsarsEAH();

        virtual unsigned int numberOfWayPoints(void) const;

        virtual LookOut getView(unsigned int sequence) const;

    private:
        static const GLfloat PULSAR_EAH_RGB_RED;
        static const GLfloat PULSAR_EAH_RGB_GREEN;
        static const GLfloat PULSAR_EAH_RGB_BLUE;
        static const GLfloat PULSAR_EAH_MAG_SIZE;
        static const float VIEW_OFFSET;

        /// The radius from the origin at which the constellations will be drawn.
        vec_t radius;
    };


/**
 * @}
 */

#endif /* PULSARS_H_*/
