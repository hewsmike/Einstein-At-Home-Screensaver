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

#ifndef PATH_H_
#define PATH_H_

#include <string>
#include <vector>

#include "CameraState.h"
#include "Curve.h"
#include "HUDImage.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This class encapsulates a path in 3D space with a camera
 *        state retrievable for any position along it and descriptions
 *        at the endpoints.
 *
 *      Intended to be used by the Traverse class as one Path in a
 * sequence. A parameter, here called 'lambda', specifies a camera
 * state between the start and finish of the Path inclusive. While
 * any value of lambda may be given, evaluation occurs after clamping
 * to within the range [0.0, 1.0]. Sets of descriptive messages may
 * be given and retrieved for either end of the Path.
 *
 * \see Curve
 * \see CameraState
 * \see Traverse
 *
 * \author Mike Hewson\n
 */

class Path {
    public:
        /// Enumerant for camera view state components.
        enum component {POSITION, FOCUS, ORIENTATION};

        /**
         * \brief Constructor ( no argument )
         *
         *      Gives a Path with defaults from the Curve class
         */

        Path(void);

        /**
         * \brief Constructor
         *
         * \param pos : the curve for determining camera position
         *
         * \param view : the curve for determining camera focus point
         *
         * \param orient : the curve for determining camera orientation
         */
        Path(const Curve& pos, const Curve& focus, const Curve& orient);

        /**
         * \brief Destructor
         */
        virtual ~Path();

        /**
         * \brief Obtain the camera state parameterised by the given lambda.
         *
         * \param lambda : a value indicating the desired point. This is
         *             clamped to the real number interval [0.0, 1.0],
         *             meaning that if given values less than 0.0 that leads
         *             to an evaluation at 0.0, likewise for given values
         *             greater than 1.0 will evaluate at 1.0 ....
         *
         * \return The camera state at the given lambda
         */
        CameraState value(float lambda) const;

        /**
         * \brief Obtain the length of a curve component
         *
         *      This evaluates the distance along the chosen curve component
         * between the points designated by lambda = 0.0 and 1.0
         *
         * \param comp : one of the camera view state components
         *
         * \return The length of the curve
         */
        float curveLength(Path::component comp) const;

        /**
         * \brief Specify a set of descriptive strings for the start of the path
         *
         * \param message : the set of strings
         */
        void setStartMessages(const std::vector<std::string>& message);

        /**
         * \brief Specify a set of descriptive strings for the finish of the path
         *
         * \param message : the set of strings
         */
        void setFinishMessages(const std::vector<std::string>& message);

        /**
         * \brief Specify an image resource name for the start of the path
         *
         * \param images : the image resource name
         */
        void setStartImageResourceName(std::string image);

        /**
         * \brief Specify an image resource name for the finish of the path
         *
         * \param image : the image resource name
         */
        void setFinishImageResourceName(std::string message);

        /**
         * \brief Obtain the set of descriptive strings for the start of the path
         *
         * \return the set of strings
         */
        const std::vector<std::string>& getStartMessages(void) const;

        /**
         * \brief Obtain the set of descriptive strings for the finish of the path
         *
         * \return the set of strings
         */
        const std::vector<std::string>& getFinishMessages(void) const;

        /**
         * \brief Obtain the image resource name for the start of the path
         *
         * \return the image resource name
         */
        std::string getStartImageResourceName(void) const;

        /**
         * \brief Obtain the image resource name for the finish of the path
         *
         * \return the image resource name
         */
        std::string getFinishImageResourceName(void) const;

        /// Evaluation of curves uses the 'lambda' parameter and needs limits.
        static const float LAMBDA_LOWER_BOUND;
        static const float LAMBDA_UPPER_BOUND;

    private:
        /// The geometric curves for camera position, focus and orientation.
        Curve where;
        Curve look_at;
        Curve up_dir;

        /// A set of descriptive strings for each end of the path.
        std::vector<std::string> start_msg;
        std::vector<std::string> finish_msg;

        /// The image resource names for each end of the path.
        std::string start_image;
        std::string finish_image;
    };

/**
 * @}
 */

#endif // PATH_H_
