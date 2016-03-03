/***************************************************************************
 *   Copyright (C) 2014 by Mike Hewson                                     *
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

#ifndef OGL_ID_H_
#define OGL_ID_H_

#include "ogl_utility.h"

#include "framework.h"

/**
 * \addtogroup ogl_utility OGL_Utility
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        objects that use runtime allocated identifiers.
 *
 *      Many OpenGL types have a common functionality for which this
 * class is a base of. The detailed acquisition and release of OpenGL
 * resources is to be provided in subclasses. NOTE CAREFULLY that a
 * suitable derived class destructor MUST call release() !!!
 *
 * \author Mike Hewson\n
 */

class OGL_ID {
    public:
        /// Default initialiser for the identifier.
        static const GLuint NO_ID;

        /**
         * \brief Constructor ( no argument )
         */
        OGL_ID(void);

        /**
         * \brief Destructor - do NOT call release() here, a suitable derived
         *        class destructor MUST call it.
         */
        virtual ~OGL_ID();

        /**
         * \brief Obtains the underlying OpenGL buffer object resources
         *        from the OpenGL state machine.
         *
         * \return a boolean indicating success of acquisition
         *              true - resources acquired without error
         *              false - resources were not acquired
         */
        virtual bool acquire(void) = 0;

        /**
         * \brief Releases the underlying OpenGL buffer object resources
         *        from the OpenGL state machine.
         */
        virtual void release(void) = 0;

        /**
         * \brief The acquisition state of the underlying OpenGL buffer
         *        object with respect to the OpenGL state machine. That is,
         *        have the OpenGL state machine resources been acquired ?
         *
         * \return a boolean indicating acquisition state :
         *          true : the object has acquired resources
         *          false : the object does not have acquired resources
         */
        bool isAcquired(void) const;

        /**
         * \brief Obtain the OpenGL resource identifier.
         *
         * \return the identifier.
         */
        GLuint ID(void) const;

    protected :
        /**
         * \brief Set the OpenGL resource identifier.
         *
         * \param value : the identifier value
         */
        void set_ID(GLuint value);

        /**
         * \brief Set the acquisition state flag for this object.
         *
         * \param state : the desired acquisition state.
         *                  true - the object has acquired resources
         *                  false - the object does not have acquired resources
         */
        void setAcquisitionState(bool state);

    private:
        /// The identifier as allocated by OpenGL.
        GLuint m_ident;

        /// Flag indicating if OpenGL resources have been acquired.
        bool m_acquire_flag;
    };

/**
 * @}
 */

#endif /*OGL_ID_H_*/
