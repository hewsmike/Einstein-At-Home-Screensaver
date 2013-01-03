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

GraphicsEngineFactory::~GraphicsEngineFactory() {
    }

GraphicsEngineFactory::GraphicsEngineFactory() {
    }

AbstractGraphicsEngine* GraphicsEngineFactory::createInstance(
                        GraphicsEngineFactory::Engines engine,
                        GraphicsEngineFactory::Applications application) {
    // First switch upon engine type, then variants.
    // A NULL return indicates failure to instantiate.
    switch(engine) {
#ifdef SOLARSYSTEM
        case SolarSystem :
            switch(application) {
                case EinsteinGravity :
                    return new SolarSystemGravity();
                    break;
                case EinsteinRadio :
                    return new SolarSystemRadio();
                    break;
                case EinsteinGamma :
                    return new SolarSystemGamma();
                    break;
                default:
                    return NULL;
                    break;
                }
            break;
#endif
#ifdef STARSPHERE
        case Starsphere:
            switch(application) {
                case EinsteinGravity :
                    return new StarsphereGravity();
                    break;
                case EinsteinRadio :
                    return new StarsphereRadio();
                    break;
                default:
                    return NULL;
                    break;
            }
            break;
#endif
        default:
            return NULL;
            break;
        }
    }
