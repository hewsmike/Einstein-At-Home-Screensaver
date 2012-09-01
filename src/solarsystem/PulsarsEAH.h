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

#ifndef PULSARS_H_
#define PULSARS_H_

#include "Stars.h"
#include "Traversable.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class manages the rendering of EAH discovered pulsars.
 *
 *
 *
 * \see Renderable
 * \see Star
 *
 * \author Mike Hewson\n
 */

template<class T> class PulsarsEAH : public Stars<T>, public Traversable {
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
        PulsarsEAH(GLfloat rad, GLfloat magsize,
                   GLfloat red,
                   GLfloat green,
                   GLfloat blue) {
                Stars<T>(rad, magsize, red, green, blue);
            }

        /// Virtual destructor
        virtual ~PulsarsEAH() {
            }

        virtual unsigned int numberOfWayPoints(void) const {
            return listing().size();
            }

        virtual LookOut getView(unsigned int sequence) const {
            // Default view - in absence of any pulsars - the
            // simulation's initial view.
            LookOut ret_val;

            const std::vector<PulsarEAH>& list = listing();

            // Assuming there is at least one pulsar.
            if(list.size() > 0) {
                if(sequence > (list.size() - 1)) {
                    sequence = list.size() - 1;
                    }

                PulsarEAH current_pulsar = list.at(sequence);

                VectorSP pulsar_position = VectorSP(current_pulsar.right_ascension(),
                                                    current_pulsar.declination(),
                                                    radius);

                Vector3D viewpoint = pulsar_position.unit() * (radius - VIEW_OFFSET);

                ret_val.setPosition(viewpoint);
                ret_val.setFocus(pulsar_position);
                ret_val.setOrientation(VIEW_UP);

                const std::vector<std::string>& current_description = current_pulsar.getDescription();

                for(unsigned int index = 0; index < current_description.size(); ++index) {
                    ret_val.addToDescription(current_description[index]);
                    }
                }

            return ret_val;
            }
    private:
    };

/**
 * @}
 */

#endif /* PULSARS_H_*/
