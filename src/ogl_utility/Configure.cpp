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

#include "Configure.h"

#include "ErrorHandler.h"

Configure::Configure(void) :
        m_configure_flag(false) {
	}

Configure::~Configure() {
    }

Configure::utilise(void) {
    // Has a successful configuration been achieved ?
    if(m_configure_flag == false) {
        // No, so attempt to do that.
        bool check = m_configure();
        // Did configuration succeed ?
        if(check == false) {
            // No, so that's a FATAL.
            ErrorHandler::record("Configure::utilise() : object configuration failed", ErrorHandler::FATAL);
            }
        // Yes, so mark this aspect as having been done.
        m_configure_flag = true;
        }
    // Now the object must have been successfully configured, so use it.
    m_utilise();
    }
