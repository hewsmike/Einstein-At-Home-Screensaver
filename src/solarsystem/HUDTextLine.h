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

#ifndef HUD_TEXT_LINE_H_
#define HUD_TEXT_LINE_H_

#include "framework.h"

#include <string>

#include <oglft/OGLFT.h>

#include "HUDContent.h"
#include "Simulation.h"
#include "SolarSystemGlobals.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A single horizontal line of text on the HUD.
 *
 * \see HUDContent
 *
 * \author Mike Hewson\n
 */

class HUDTextLine : public HUDContent {
    public:
        /**
         * \brief Constructor
         *
         * \param length : the maximum length in characters to be displayed
         * \param element : the scene element type as per Simulation class
         * \param text : a string containing the desired text
         * \param horizontalMargin : the horizontal pixel margin to be applied
         *                           either side of the enclosed content
         * \param verticalMargin : the vertical pixel margin to be applied
         *                         above and below the enclosed content
         */
        HUDTextLine(GLuint length, Simulation::content element,
                    const std::string& text,
                    GLuint horizontalMargin, GLuint verticalMargin);

        /**
         * \brief Destructor
         */
        virtual ~HUDTextLine();

        /**
         * \brief Get the maximum length, in characters, of the text line
         *
         * \return the length
         */
        GLuint maxLength(void) const;

        /**
         * \brief Get the width in pixels of the text line, as determined
         *        by the font choice and current contents.
         *
         * \return the width
         */
        GLuint width(void) const;

        /**
         * \brief Get the height in pixels of the text line, as determined
         *        by the font choice, ascenders and descenders.
         *
         * \return the height
         */
        GLuint height(void) const;

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

        std::string text(void) const;

        void setText(const std::string& text);

    private:
        // The line's maximum number of characters.
        GLuint len;

        // The font to be used in rendering.
        OGLFT_ft* lineFont;

        // The text itself.
        std::string txt;
    };

/**
 * @}
 */

#endif /* HUD_TEXT_LINE_H_ */
