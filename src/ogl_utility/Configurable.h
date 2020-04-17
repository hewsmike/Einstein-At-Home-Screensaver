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

#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

#include "ogl_utility.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with those objects
 *        that require configuration/setup before first use.
 *
 * \author Mike Hewson\n
 */

class Configurable {
    public :
        /**
         * \brief Destructor.
         */
        virtual ~Configurable();

        /**
         * \brief Actually configure any underlying object(s). The configuration
         *        state must be set by any derived class via setConfigurationState().
         *
         * \return a boolean indicating success of configuration
         *              - true, the configuration as successful.
         *              - false, the configuration was not successful.
         */
        virtual bool configure(void) = 0;

        /**
         * \brief The configuration state of the object.
         *
         * \return a boolean indicating configuration state :
         *          true : the object has been configured
         *          false : the object has not been configured
         */
        bool isConfigured(void) const;

    protected :
        /**
         * \brief Constructor.
         */
        Configurable(void);

        /**
         * \brief Set the configuration state flag for this object.
         *
         * \param state : the desired configuration state.
         *                  true - the object has been configured
         *                  false - the object has not been configured
         */
        void setConfigurationState(bool state);

    private:
        bool m_configure_flag;
    };

/**
 * @}
 */

#endif /*CONFIGURABLE_H_*/
