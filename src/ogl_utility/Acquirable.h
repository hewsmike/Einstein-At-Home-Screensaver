/***************************************************************************
 *   Copyright (C) 2016 by Mike Hewson                                     *
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

#ifndef ACQUIRABLE_H_
#define ACQUIRABLE_H_

#include "ogl_utility.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with objects
 *        that involve acquisition and release of resources.
 *
 * \author Mike Hewson\n
 */

class Acquirable {
    public:
        /**
         * \brief Destructor.
         */
        virtual ~Acquirable();

        /**
         * \brief Obtains the resources. The acquisition state ought be
         *        set by any derived class via setAcquisitionState().
         *
         * \return a boolean indicating success of acquisition
         *              true - resources acquired without error
         *              false - resources were not acquired
         */
        virtual bool acquire(void) = 0;

        /**
         * \brief Releases the object resources. The acquisition state ought be
         *        set by any derived class via setAcquisitionState().
         */
        virtual void release(void) = 0;

        /**
         * \brief The acquisition state of the object with respect to
         *        resources.
         *
         * \return a boolean indicating acquisition state :
         *          true : the object has acquired resources
         *          false : the object does not have acquired resources
         */
        bool isAcquired(void) const;

    protected :
         /**
         * \brief Constructor.
         */
        Acquirable(void);

        /**
         * \brief Set the acquisition state flag for this object.
         *
         * \param state : the desired acquisition state.
         *                  true - the object has acquired resources
         *                  false - the object does not have acquired resources
         */
        void setAcquisitionState(bool state);

    private:
        /// Flag indicating if resources have been acquired.
        bool m_acquire_flag;
    };

/**
 * @}
 */

#endif /*ACQUIRABLE_H_*/
