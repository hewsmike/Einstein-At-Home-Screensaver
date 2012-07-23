/***************************************************************************
 *   Copyright (C) 2012 by Mike Hewson                                     *
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

#ifndef HUD_TEXT_BOX_H_
#define HUD_TEXT_BOX_H_

#include "framework.h"

#include <string>

#include <oglft/OGLFT.h>

#include "HUDContent.h"
#include "HUDTextLine.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief A box of text on the HUD.
 *
 *      A given number of lines of a given width, stated as a
 * number of characters, arranged one on top of the other to
 * produce a box-like block of text. The actual dimensions on
 * the HUD, in pixels, at any particular time depends upon the
 * choice of font and the current contents of the strings
 * defining the text to be displayed. The padding parameter is
 * to make a clear separation between the descenders of one line
 * and the ascenders of the one below ( probably a couple of
 * pixels is fine ).
 *
 * \see HUDContent
 *
 * \author Mike Hewson\n
 */

class HUDTextBox : public HUDContent {
    public:
        /**
         * \brief Constructor
         *
         *      All of the following parameters represent box features
         * that remain constant for the lifetime of the box.
         *
         * \param width : the number of characters per line
         * \param height : the number of lines
         * \param font : pointer to an OGLFT font instance
         * \param padding : the margin in pixels to insert between lines
         * \param horizontalMargin : the horizontal pixel margin to be applied
         *                           either side of the enclosed content
         * \param verticalMargin : the vertical pixel margin to be applied
         *                         above and below the enclosed content
         */
        HUDTextBox(GLuint width, GLuint height,
                   OGLFT_ft* font, GLuint padding,
                   GLuint horizontalMargin, GLuint verticalMargin);

        /**
         * \brief Destructor
         */
        virtual ~HUDTextBox();

        /**
         * \brief Get the maximum width, in characters, of any text line.
         *
         *      This is independent of whether there is any text content
         * at all within any line. It returns the value given upon
         * construction.
         *
         * \return the number of characters.
         */
        GLuint widthInChars(void) const;

        /**
         * \brief Get the maximum number of lines in the box.
         *
         *      This is independent of whether there is any text content
         * at all within any line. It returns the value given upon
         * construction.
         *
         * \return the number of lines.
         */
        GLuint heightInLines(void) const;

        /**
         * \brief Get the maximum width in pixels of the box.
         *
         *      This is determined by the font choice and the current
         * box text contents. All lines are examined and that with the
         * greatest number of pixels required to display it then represents
         * the maximum width of the entire box.
         *
         * \return the width
         */
        GLuint widthInPixels(void) const;

        /**
         * \brief Get the height in pixels of the entire box.
         *
         *      This is determined by the font choice and the current
         * box text contents. All lines are examined and the sum of
         * the heights of all lines is returned. Each line height is
         * determined by the greatest number of pixels required to
         * display that line, which includes consideration of ascenders
         * and descenders of the specific characters within.
         *
         * \return the height
         */
        GLuint heightInPixels(void) const;

        /**
         * \brief Set the entire box's text content.
         *
         *      This will clear any pre-existing text from the box
         * and engage 'best-fit' formatting. Any characters over the
         * capacity of the box are not displayed and are lost. That
         * is, the string is truncated to fit the box without retaining
         * any record of the excess characters.
         *
         * \param text : a string containing the desired text
         */
        void setText(const std::string& text);

        /**
         * \brief Obtain the entire box's text content.
         *
         *      This will retrieve the concatenation of all text
         * over all lines, regardless of how the text was originally
         * presented to the class. It is a bland concatenation with
         * no especial handling of line breaks etc, thus possibly
         * producing inappropriately adjoined words.
         *
         * \return a string containing the text
         */
        std::string text(void) const;

        /**
         * \brief Set a line's text content.
         *
         *      This will clear any pre-existing text from the line,
         * with any characters over the line width in characters not
         * displayed. That is, the string is truncated to fit the line
         * without retaining any record of the excess characters. If
         * the requested line index is out of range then no alteration
         * occurs, without error report.
         *
         * \param text : an string containing the desired text
         * \param line : the number of the line, indexed from zero
         *               ( for the first line ) to the box height in
         *               lines minus one ( for the last line ).
         */
        void setLineText(const std::string& text, GLuint line);

        /**
         * \brief Obtain a line's text content.
         *
         *      This will retrieve the contents of a given line
         * regardless of how the text was originally presented to
         * the class. If the requested line index is out of range
         * then an empty string is returned, without error report.
         *
         * \param line : the index of the desired line from zero
         *               ( for the first line ) to one less than
         *               the total number of lines in the box ( for
         *               the last line ).
         *
         * \return a string containing the text
         */
        std::string lineText(GLuint line) const;

    protected:
        /// These three routines below satisfy the Renderable interface.

        /// Provide OpenGL code to prepare for rendering.
        virtual void prepare(SolarSystemGlobals::render_quality rq);

        /// Provide OpenGL code to release any resources used.
        virtual void release(void);

        /// Provide OpenGL code to render the object.
        virtual void render(void);

    private:
        /// The box parameters as given upon construction.
        GLuint char_width;
        GLuint lines;
        OGLFT_ft* box_font;
        GLuint pad;
        GLuint horzMargin;
        GLuint vertMargin;

        std::vector<std::string> text_content;
    };

/**
 * @}
 */

#endif /* HUD_TEXT_BOX_H_ */
