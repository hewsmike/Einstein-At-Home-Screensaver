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

#ifndef PULSARS_H_
#define PULSARS_H_

#include <iostream>
#include <sstream>
#include <string>

#include "Stars.h"
#include "Traversable.h"
#include "Vector3D.h"
#include "VectorSP.h"

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
                   GLfloat blue) :
                    Stars<T>::Stars(rad, magsize, red, green, blue) {
            }

        /// Virtual destructor
        virtual ~PulsarsEAH() {
            }

        virtual unsigned int numberOfWayPoints(void) const {
            return Stars<T>::listing().size();
            }

        virtual LookOut getView(unsigned int sequence) const {
            static const GLuint VIEW_OFFSET = 3000;
            // Default view - in absence of any pulsars - the
            // simulation's initial view.
            LookOut ret_val;

            const std::vector<T>& list = Stars<T>::listing();

            // Assuming there is at least one pulsar.
            if(list.size() > 0) {
                if(sequence > (list.size() - 1)) {
                    sequence = list.size() - 1;
                    }

                T current_pulsar = list.at(sequence);

                VectorSP pulsar_position = VectorSP(current_pulsar.right_ascension(),
                                                    current_pulsar.declination(),
                                                    Stars<T>::getRadius());

                Vector3D viewpoint = pulsar_position.unit() * (Stars<T>::getRadius() - VIEW_OFFSET);

                ret_val.setPosition(viewpoint);
                ret_val.setFocus(pulsar_position);
                ret_val.setOrientation(Vector3D(0, 0, 1));

                ret_val.addToDescription(current_pulsar.name());

                std::stringstream pd;
                pd << "Period = " << current_pulsar.period() << " (ms)";
                ret_val.addToDescription(pd.str());

                std::stringstream dm;
                dm << "Dispersion Modulus = " << current_pulsar.dispersionModulus();
                ret_val.addToDescription(dm.str());

                std::stringstream ds;
                ds << "Distance = " << current_pulsar.distance() << " (kpc)";
                ret_val.addToDescription(ds.str());

                ret_val.addToDescription(current_pulsar.discoverers());

                std::string imageResourceName(current_pulsar.name());
                imageResourceName += "TGA";

                ret_val.addImageResource(imageResourceName);
                }
            return ret_val;
            }
    private:
    };

/**
 * @}
 */

#endif /* PULSARS_H_*/
