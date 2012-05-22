/***************************************************************************
 *   Copyright (C) 2011 by Mike Hewson                                     *
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

#include "Supernovae.h"

const GLfloat Supernovae::MAG_SIZE(2.0f);
const GLfloat Supernovae::RGB_RED(0.40f);
const GLfloat Supernovae::RGB_GREEN(0.35f);
const GLfloat Supernovae::RGB_BLUE(1.0f);

const GLint Supernovae::ARRAY_START(0);
const GLsizei Supernovae::ARRAY_STRIDE(0);
const GLvoid* Supernovae::BYTE_OFFSET(0);
const GLint Supernovae::COORDS_PER_VERTEX(3);

Supernovae::Supernovae(vec_t rad) : s_rad(rad) {
   supernova_list.push_back(Supernova( 266.433333f , -29.00000f ));
   supernova_list.push_back(Supernova( 266.562500f , -28.63333f ));
   supernova_list.push_back(Supernova( 266.837500f , -28.15000f ));
   supernova_list.push_back(Supernova( 267.125000f , -28.15000f ));
   supernova_list.push_back(Supernova( 267.412500f , -27.76667f ));
   supernova_list.push_back(Supernova( 267.187500f , -27.16667f ));
   supernova_list.push_back(Supernova( 268.858333f , -25.83333f ));
   supernova_list.push_back(Supernova( 268.229167f , -25.46667f ));
   supernova_list.push_back(Supernova( 272.229167f , -27.05000f ));
   supernova_list.push_back(Supernova( 262.675000f , -21.48333f ));
   supernova_list.push_back(Supernova( 263.354167f , -21.56667f ));
   supernova_list.push_back(Supernova( 271.875000f , -25.75000f ));
   supernova_list.push_back(Supernova( 270.541667f , -24.90000f ));
   supernova_list.push_back(Supernova( 266.833333f , -22.26667f ));
   supernova_list.push_back(Supernova( 268.729167f , -23.08333f ));
   supernova_list.push_back(Supernova( 270.125000f , -23.43333f ));
   supernova_list.push_back(Supernova( 266.291667f , -21.36667f ));
   supernova_list.push_back(Supernova( 270.458333f , -22.90000f ));
   supernova_list.push_back(Supernova( 274.354167f , -24.06667f ));
   supernova_list.push_back(Supernova( 276.041667f , -23.80000f ));
   supernova_list.push_back(Supernova( 271.375000f , -21.43333f ));
   supernova_list.push_back(Supernova( 271.283333f , -20.23333f ));
   supernova_list.push_back(Supernova( 272.862500f , -19.41667f ));
   supernova_list.push_back(Supernova( 272.695833f , -19.08333f ));
   supernova_list.push_back(Supernova( 273.045833f , -18.61667f ));
   supernova_list.push_back(Supernova( 274.833333f , -18.00000f ));
   supernova_list.push_back(Supernova( 273.558333f , -17.20000f ));
   supernova_list.push_back(Supernova( 276.000000f , -16.56667f ));
   supernova_list.push_back(Supernova( 274.716667f , -15.03333f ));
   supernova_list.push_back(Supernova( 277.208333f , -16.18333f ));
   supernova_list.push_back(Supernova( 275.233333f , -14.33333f ));
   supernova_list.push_back(Supernova( 276.333333f , -14.76667f ));
   supernova_list.push_back(Supernova( 277.729167f , -14.86667f ));
   supernova_list.push_back(Supernova( 278.208333f , -14.65000f ));
   supernova_list.push_back(Supernova( 275.991667f , -12.38333f ));
   supernova_list.push_back(Supernova( 277.458333f , -12.96667f ));
   supernova_list.push_back(Supernova( 277.029167f , -11.58333f ));
   supernova_list.push_back(Supernova( 278.387500f , -10.58333f ));
   supernova_list.push_back(Supernova( 278.187500f , -10.13333f ));
   supernova_list.push_back(Supernova( 278.312500f , -9.21667f ));
   supernova_list.push_back(Supernova( 278.687500f , -8.80000f ));
   supernova_list.push_back(Supernova( 278.262500f , -8.21667f ));
   supernova_list.push_back(Supernova( 279.679167f , -7.53333f ));
   supernova_list.push_back(Supernova( 278.541667f , -7.08333f ));
   supernova_list.push_back(Supernova( 280.329167f , -4.93333f ));
   supernova_list.push_back(Supernova( 279.958333f , -4.40000f ));
   supernova_list.push_back(Supernova( 280.979167f , -3.88333f ));
   supernova_list.push_back(Supernova( 279.750000f , -2.91667f ));
   supernova_list.push_back(Supernova( 281.216667f , -2.95000f ));
   supernova_list.push_back(Supernova( 281.604167f , -2.98333f ));
   supernova_list.push_back(Supernova( 283.604167f , -2.90000f ));
   supernova_list.push_back(Supernova( 281.000000f , -1.53333f ));
   supernova_list.push_back(Supernova( 282.791667f , -1.51667f ));
   supernova_list.push_back(Supernova( 282.354167f , 0.00000f ));
   supernova_list.push_back(Supernova( 286.500000f , -3.00000f ));
   supernova_list.push_back(Supernova( 283.291667f , -1.13333f ));
   supernova_list.push_back(Supernova( 282.854167f , 0.00000f ));
   supernova_list.push_back(Supernova( 283.458333f , 0.00000f ));
   supernova_list.push_back(Supernova( 283.200000f , 0.00000f ));
   supernova_list.push_back(Supernova( 284.000000f , 1.36667f ));
   supernova_list.push_back(Supernova( 285.145833f , 2.93333f ));
   supernova_list.push_back(Supernova( 282.204167f , 4.43333f ));
   supernova_list.push_back(Supernova( 286.033333f , 5.46667f ));
   supernova_list.push_back(Supernova( 288.083333f , 4.91667f ));
   supernova_list.push_back(Supernova( 286.791667f , 6.51667f ));
   supernova_list.push_back(Supernova( 286.891667f , 7.13333f ));
   supernova_list.push_back(Supernova( 286.833333f , 9.08333f ));
   supernova_list.push_back(Supernova( 287.783333f , 9.10000f ));
   supernova_list.push_back(Supernova( 286.458333f , 10.50000f ));
   supernova_list.push_back(Supernova( 289.104167f , 11.15000f ));
   supernova_list.push_back(Supernova( 289.541667f , 12.15000f ));
   supernova_list.push_back(Supernova( 290.958333f , 14.10000f ));
   supernova_list.push_back(Supernova( 294.708333f , 17.23333f ));
   supernova_list.push_back(Supernova( 292.629167f , 18.86667f ));
   supernova_list.push_back(Supernova( 293.333333f , 18.93333f ));
   supernova_list.push_back(Supernova( 293.000000f , 19.83333f ));
   supernova_list.push_back(Supernova( 290.333333f , 21.73333f ));
   supernova_list.push_back(Supernova( 293.745833f , 21.95000f ));
   supernova_list.push_back(Supernova( 295.637500f , 23.58333f ));
   supernova_list.push_back(Supernova( 294.729167f , 24.31667f ));
   supernova_list.push_back(Supernova( 296.966667f , 27.75000f ));
   supernova_list.push_back(Supernova( 298.666667f , 28.58333f ));
   supernova_list.push_back(Supernova( 293.250000f , 31.16667f ));
   supernova_list.push_back(Supernova( 298.041667f , 29.43333f ));
   supernova_list.push_back(Supernova( 298.633333f , 31.48333f ));
   supernova_list.push_back(Supernova( 302.166667f , 30.61667f ));
   supernova_list.push_back(Supernova( 298.333333f , 32.91667f ));
   supernova_list.push_back(Supernova( 300.666667f , 32.71667f ));
   supernova_list.push_back(Supernova( 303.562500f , 36.20000f ));
   supernova_list.push_back(Supernova( 312.750000f , 30.66667f ));
   supernova_list.push_back(Supernova( 304.008333f , 37.20000f ));
   supernova_list.push_back(Supernova( 305.583333f , 38.71667f ));
   supernova_list.push_back(Supernova( 305.208333f , 40.43333f ));
   supernova_list.push_back(Supernova( 304.750000f , 45.50000f ));
   supernova_list.push_back(Supernova( 313.333333f , 43.45000f ));
   supernova_list.push_back(Supernova( 312.625000f , 44.88333f ));
   supernova_list.push_back(Supernova( 312.666667f , 45.36667f ));
   supernova_list.push_back(Supernova( 314.666667f , 44.88333f ));
   supernova_list.push_back(Supernova( 311.250000f , 50.58333f ));
   supernova_list.push_back(Supernova( 313.104167f , 55.35000f ));
   supernova_list.push_back(Supernova( 322.333333f , 50.83333f ));
   supernova_list.push_back(Supernova( 321.208333f , 51.88333f ));
   supernova_list.push_back(Supernova( 336.875000f , 60.83333f ));
   supernova_list.push_back(Supernova( 345.395833f , 58.88333f ));
   supernova_list.push_back(Supernova( 350.858333f , 58.80000f ));
   supernova_list.push_back(Supernova( 354.250000f , 61.91667f ));
   supernova_list.push_back(Supernova( 358.416667f , 63.25000f ));
   supernova_list.push_back(Supernova( 359.791667f , 62.43333f ));
   supernova_list.push_back(Supernova(   1.666667f , 72.75000f ));
   supernova_list.push_back(Supernova(   6.325000f , 64.15000f ));
   supernova_list.push_back(Supernova(  20.500000f , 64.25000f ));
   supernova_list.push_back(Supernova(  22.083333f , 63.16667f ));
   supernova_list.push_back(Supernova(  31.420833f , 64.81667f ));
   supernova_list.push_back(Supernova(  34.416667f , 62.75000f ));
   supernova_list.push_back(Supernova(  74.666667f , 51.83333f ));
   supernova_list.push_back(Supernova(  75.250000f , 46.66667f ));
   supernova_list.push_back(Supernova(  81.625000f , 42.93333f ));
   supernova_list.push_back(Supernova(  79.750000f , 41.91667f ));
   supernova_list.push_back(Supernova(  88.416667f , 31.08333f ));
   supernova_list.push_back(Supernova(  84.750000f , 27.83333f ));
   supernova_list.push_back(Supernova(  92.041667f , 29.00000f ));
   supernova_list.push_back(Supernova(  83.629167f , 22.01667f ));
   supernova_list.push_back(Supernova(  94.250000f , 22.56667f ));
   supernova_list.push_back(Supernova(  92.333333f , 17.33333f ));
   supernova_list.push_back(Supernova(  99.750000f , 6.50000f ));
   supernova_list.push_back(Supernova( 102.166667f , 6.43333f ));
   supernova_list.push_back(Supernova( 125.541667f , -43.00000f ));
   supernova_list.push_back(Supernova( 136.083333f , -38.70000f ));
   supernova_list.push_back(Supernova( 128.500000f , -45.83333f ));
   supernova_list.push_back(Supernova( 133.000000f , -46.33333f ));
   supernova_list.push_back(Supernova( 136.708333f , -52.11667f ));
   supernova_list.push_back(Supernova( 149.416667f , -53.25000f ));
   supernova_list.push_back(Supernova( 154.562500f , -59.00000f ));
   supernova_list.push_back(Supernova( 158.916667f , -59.70000f ));
   supernova_list.push_back(Supernova( 165.312500f , -60.30000f ));
   supernova_list.push_back(Supernova( 165.770833f , -60.93333f ));
   supernova_list.push_back(Supernova( 167.975000f , -60.63333f ));
   supernova_list.push_back(Supernova( 171.150000f , -59.26667f ));
   supernova_list.push_back(Supernova( 169.833333f , -61.46667f ));
   supernova_list.push_back(Supernova( 173.750000f , -60.90000f ));
   supernova_list.push_back(Supernova( 174.041667f , -61.63333f ));
   supernova_list.push_back(Supernova( 177.791667f , -62.56667f ));
   supernova_list.push_back(Supernova( 182.416667f , -52.41667f ));
   supernova_list.push_back(Supernova( 179.625000f , -62.58333f ));
   supernova_list.push_back(Supernova( 183.166667f , -62.86667f ));
   supernova_list.push_back(Supernova( 183.420833f , -62.61667f ));
   supernova_list.push_back(Supernova( 183.804167f , -65.50000f ));
   supernova_list.push_back(Supernova( 185.437500f , -63.15000f ));
   supernova_list.push_back(Supernova( 189.479167f , -63.81667f ));
   supernova_list.push_back(Supernova( 191.479167f , -62.13333f ));
   supernova_list.push_back(Supernova( 196.495833f , -62.70000f ));
   supernova_list.push_back(Supernova( 204.404167f , -63.06667f ));
   supernova_list.push_back(Supernova( 205.625000f , -62.38333f ));
   supernova_list.push_back(Supernova( 206.629167f , -62.90000f ));
   supernova_list.push_back(Supernova( 207.625000f , -62.08333f ));
   supernova_list.push_back(Supernova( 209.500000f , -62.15000f ));
   supernova_list.push_back(Supernova( 210.000000f , -62.28333f ));
   supernova_list.push_back(Supernova( 211.408333f , -61.96667f ));
   supernova_list.push_back(Supernova( 213.250000f , -61.73333f ));
   supernova_list.push_back(Supernova( 215.250000f , -64.20000f ));
   supernova_list.push_back(Supernova( 220.750000f , -62.50000f ));
   supernova_list.push_back(Supernova( 218.979167f , -60.60000f ));
   supernova_list.push_back(Supernova( 219.604167f , -60.18333f ));
   supernova_list.push_back(Supernova( 220.375000f , -60.00000f ));
   supernova_list.push_back(Supernova( 222.416667f , -59.76667f ));
   supernova_list.push_back(Supernova( 223.708333f , -59.06667f ));
   supernova_list.push_back(Supernova( 224.625000f , -58.48333f ));
   supernova_list.push_back(Supernova( 228.625000f , -59.13333f ));
   supernova_list.push_back(Supernova( 229.458333f , -59.26667f ));
   supernova_list.push_back(Supernova( 230.937500f , -58.21667f ));
   supernova_list.push_back(Supernova( 230.166667f , -57.56667f ));
   supernova_list.push_back(Supernova( 230.845833f , -57.10000f ));
   supernova_list.push_back(Supernova( 232.175000f , -56.35000f ));
   supernova_list.push_back(Supernova( 238.250000f , -56.16667f ));
   supernova_list.push_back(Supernova( 238.604167f , -55.15000f ));
   supernova_list.push_back(Supernova( 237.083333f , -53.81667f ));
   supernova_list.push_back(Supernova( 236.700000f , -53.33333f ));
   supernova_list.push_back(Supernova( 225.708333f , -41.93333f ));
   supernova_list.push_back(Supernova( 238.875000f , -53.28333f ));
   supernova_list.push_back(Supernova( 240.333333f , -52.30000f ));
   supernova_list.push_back(Supernova( 227.500000f , -40.00000f ));
   supernova_list.push_back(Supernova( 240.275000f , -51.56667f ));
   supernova_list.push_back(Supernova( 243.320833f , -50.88333f ));
   supernova_list.push_back(Supernova( 244.387500f , -51.03333f ));
   supernova_list.push_back(Supernova( 243.833333f , -50.70000f ));
   supernova_list.push_back(Supernova( 246.937500f , -48.78333f ));
   supernova_list.push_back(Supernova( 248.045833f , -47.31667f ));
   supernova_list.push_back(Supernova( 248.987500f , -47.60000f ));
   supernova_list.push_back(Supernova( 249.866667f , -47.85000f ));
   supernova_list.push_back(Supernova( 248.162500f , -46.60000f ));
   supernova_list.push_back(Supernova( 249.754167f , -46.98333f ));
   supernova_list.push_back(Supernova( 249.495833f , -46.40000f ));
   supernova_list.push_back(Supernova( 250.250000f , -46.56667f ));
   supernova_list.push_back(Supernova( 250.287500f , -46.31667f ));
   supernova_list.push_back(Supernova( 251.629167f , -44.65000f ));
   supernova_list.push_back(Supernova( 251.920833f , -44.56667f ));
   supernova_list.push_back(Supernova( 251.895833f , -43.78333f ));
   supernova_list.push_back(Supernova( 253.754167f , -44.01667f ));
   supernova_list.push_back(Supernova( 253.708333f , -43.88333f ));
   supernova_list.push_back(Supernova( 252.679167f , -43.06667f ));
   supernova_list.push_back(Supernova( 261.250000f , -46.50000f ));
   supernova_list.push_back(Supernova( 257.000000f , -44.26667f ));
   supernova_list.push_back(Supernova( 255.104167f , -43.23333f ));
   supernova_list.push_back(Supernova( 255.962500f , -41.70000f ));
   supernova_list.push_back(Supernova( 256.833333f , -40.88333f ));
   supernova_list.push_back(Supernova( 257.579167f , -40.18333f ));
   supernova_list.push_back(Supernova( 258.458333f , -39.75000f ));
   supernova_list.push_back(Supernova( 258.858333f , -38.46667f ));
   supernova_list.push_back(Supernova( 258.525000f , -38.53333f ));
   supernova_list.push_back(Supernova( 258.479167f , -38.18333f ));
   supernova_list.push_back(Supernova( 259.312500f , -38.06667f ));
   supernova_list.push_back(Supernova( 259.495833f , -37.43333f ));
   supernova_list.push_back(Supernova( 261.958333f , -38.53333f ));
   supernova_list.push_back(Supernova( 260.612500f , -36.18333f ));
   supernova_list.push_back(Supernova( 260.250000f , -35.45000f ));
   supernova_list.push_back(Supernova( 262.216667f , -36.26667f ));
   supernova_list.push_back(Supernova( 261.916667f , -35.11667f ));
   supernova_list.push_back(Supernova( 264.729167f , -35.18333f ));
   supernova_list.push_back(Supernova( 262.616667f , -33.76667f ));
   supernova_list.push_back(Supernova( 264.000000f , -33.70000f ));
   supernova_list.push_back(Supernova( 263.816667f , -32.63333f ));
   supernova_list.push_back(Supernova( 266.470833f , -33.71667f ));
   supernova_list.push_back(Supernova( 259.750000f , -29.66667f ));
   supernova_list.push_back(Supernova( 264.483333f , -32.26667f ));
   supernova_list.push_back(Supernova( 265.645833f , -32.86667f ));
   supernova_list.push_back(Supernova( 265.120833f , -30.96667f ));
   supernova_list.push_back(Supernova( 264.645833f , -30.73333f ));
   supernova_list.push_back(Supernova( 261.500000f , -28.60000f ));
   supernova_list.push_back(Supernova( 266.708333f , -30.26667f ));
   supernova_list.push_back(Supernova( 266.375000f , -29.95000f ));
   supernova_list.push_back(Supernova( 264.900000f , -29.18333f ));
   };

Supernovae::~Supernovae() {
   release();
   }

void Supernovae::prepare(SolarSystemGlobals::render_quality rq) {
   // Get an OpenGL buffer object.
   buff_obj_points.acquire();
   /// TODO - Preparations depend upon the requested rendering quality level??
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST : {
         // What size allocation are we after? The size of a
         // supernova's worth times how many supernovae.
         GLsizeiptr buffer_size = sizeof(Position) * supernova_list.size();

         Position* buffer_base_ptr = new Position[supernova_list.size()];
         Position* buffer_ptr = buffer_base_ptr;

         // Traverse the listing of supernovae positions and
         // store such vertices in the buffer.
         for(std::vector<Supernova>::const_iterator sn = supernova_list.begin();
             sn < supernova_list.end();
             sn++ ) {
            // Take each supernova in spherical polar co-ordinate representation.
            VectorSP super = VectorSP(sn->right_ascension(), sn->declination(), s_rad);

            // But store in Cartesian co-ordinate representation..
            *(buffer_ptr) = {super.x(), super.y(), super.z()};
            buffer_ptr++;
            }

         buff_obj_points.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);
         delete[] buffer_base_ptr;
         break;
         }
      default :
         // Ought not get here !!
         std::string msg = "Supernovae::prepare() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void Supernovae::release(void) {
   // Release the buffer object's resources.
   buff_obj_points.release();
   }

void Supernovae::render(void) {
   // Set the point size and colour for rendering supernovae.
   glPointSize(MAG_SIZE);
   glColor3f(RGB_RED, RGB_GREEN, RGB_BLUE);

   // Make our buffer identifier OpenGL's current one.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   // We will use a vertex array within that buffer.
   glEnableClientState(GL_VERTEX_ARRAY);

   // The vertex array pointer points to the start of the buffer.
   glVertexPointer(COORDS_PER_VERTEX, GL_FLOAT, ARRAY_STRIDE, BYTE_OFFSET);

   // Finally we get to render the points.
   glDrawArrays(GL_POINTS, ARRAY_START, supernova_list.size());

   // Stop using vertex arrays.
   glDisableClientState(GL_VERTEX_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }
