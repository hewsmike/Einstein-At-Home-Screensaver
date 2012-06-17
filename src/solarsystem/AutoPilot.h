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

#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

#include <vector>

#include "CameraState.h"
#include "Path.h"
#include "Traversable.h"
#include "Traverse.h"

/**
 * \addtogroup solarsystem Solarsystem
 * @{
 */

/**
 * \brief This interface declares public methods to deal with OpenGL
 *        buffer objects. It's a wrapper.
 *
 * \author Mike Hewson\n
 */

class AutoPilot {
   public :
      /**
      * \brief Constructor
      */
      AutoPilot(void);

      /**
       * \brief Destructor
       */
      ~AutoPilot();

      void activate(const Traversable& trav, const CameraState& cam);

      void inactivate(void);

      bool isActive(void) const;

      CameraState getViewState(void);

      const std::vector<std::string>& getDescription(void) const;

      bool hasDescriptionChanged(void) const;

   private:
      enum path_stage {EARLY, MIDDLE, LATE};

      static const float PATH_EARLY_BOUNDARY;
      static const float PATH_LATE_BOUNDARY;

      static const float LENGTH_PER_FRAME;
      static const float LEAST_PATH_LENGTH;
      static const unsigned int PAUSE_FRAME_COUNT;

      bool active_flag;

      CameraState view;

      Traverse current_traverse;

      Path current_path;

      float lambda;

      float current_delta_lambda;

      std::vector<std::string> current_description;

      path_stage path_stage_flag;

      bool description_change_flag;

      void set_delta_lambda(void);

      void getTraverse(const Traversable& trav, const CameraState& cam);
   };

/**
 * @}
 */

#endif /*AUTOPILOT_H_*/
