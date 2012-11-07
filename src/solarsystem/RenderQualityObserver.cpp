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

#include "RenderQualityObserver.h"
#include "SolarSystemGlobals.h"

RenderQualityObserver::RenderQualityObserver() {
    /// Automatically attach on construction.
    attachRender();
    }

RenderQualityObserver::~RenderQualityObserver() {
    /// Automatically detach on destruction.
    detachRender();
    }

void RenderQualityObserver::attachRender(void) {
    SolarSystemGlobals::registerRenderQualityObserver(this);
    attach_flag = true;
    }

void RenderQualityObserver::detachRender(void) {
    SolarSystemGlobals::unRegisterRenderQualityObserver(this);
    attach_flag = false;
    }

bool RenderQualityObserver::isAttachedRender(void) const {
    return attach_flag;
    }

void RenderQualityObserver::renderQualityUpdate(void) {
    // Does nothing, override in derived class otherwise.
    }
