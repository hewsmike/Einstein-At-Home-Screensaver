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

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

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

class Configure {
    public :
        /**
         * \brief Constructor.
         */
        Configure(void);

        /**
         * \brief Destructor.
         */
        virtual ~Configure();

        /**
         * \brief Use this object. Configuration is ensured.
         */
        void utilise();

    private:
        bool m_configure_flag;

        /**
         * \brief actually configure the underlying object.
         *
         * \return a boolean indicating success of configuration
         *              - true, the configurationw as successful.
         *              - false, the configuration was not successful.
         */
        virtual bool m_configure(void) = 0;

        virtual void m_utilise(void) = 0;
    };

/**
 * @}
 */

#endif /*CONFIGURE_H_*/
