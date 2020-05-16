/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
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

#include "GraphicsEngineFactory.h"

#include "ErrorHandler.h"

GraphicsEngineFactory::~GraphicsEngineFactory() {
    }

GraphicsEngineFactory::GraphicsEngineFactory() {
    }

AbstractGraphicsEngine* GraphicsEngineFactory::createInstance(
                        GraphicsEngineFactory::Engines engine,
                        GraphicsEngineFactory::Applications application) {
    // Assume failure.
    AbstractGraphicsEngine* ret_val = NULL;

    // First switch upon engine type, then variants.
    // A NULL return indicates failure to instantiate.
    switch(engine) {
        case Starsphere:
            switch(application) {
                case EinsteinGravity :
                    ret_val = new StarsphereGravity();
                    break;
                case EinsteinRadio :
                    ret_val = new StarsphereRadio();
                    break;
                case EinsteinGamma :
                    ret_val = new StarsphereGamma();
                    break;
                default:
                    // Ideal use ought not arrive here. Big problem if so.
                    ErrorHandler::record("GraphicsEngineFactory::createInstance() : bad switch case ( Starsphere engine default )", ErrorHandler::FATAL);
                    break;
                }
            break;
        default:
            // Ideal use ought not arrive here. Big problem if so.
            ErrorHandler::record("GraphicsEngineFactory::createInstance() : bad switch case ( no engine default )", ErrorHandler::FATAL);
            break;
        }

    return ret_val;
    }
