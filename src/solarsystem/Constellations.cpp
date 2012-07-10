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

#include "Constellations.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "Craft.h"
#include "ErrorHandler.h"
#include "VectorSP.h"
#include "Vector3D.h"

const GLfloat Constellations::OFFSET(-20.0f);
const GLfloat Constellations::LINK_WIDTH(1.0f);
const GLushort Constellations::LINK_STIPPLE_PATTERN(0x8888);
const GLint Constellations::LINK_STIPPLE_FACTOR(3.0f);
const GLfloat Constellations::LINK_RGB_RED(0.06f);
const GLfloat Constellations::LINK_RGB_GREEN(0.22f);
const GLfloat Constellations::LINK_RGB_BLUE(0.32f);

const GLfloat Constellations::MAG_SIZE(3.0f);

const GLuint Constellations::INDICES_PER_LINK(2);
const GLuint Constellations::COLORS_PER_VERTEX(3);
const GLuint Constellations::COORDS_PER_VERTEX(3);
const GLuint Constellations::BYTE_STRIDE_PER_VERTEX(Constellations::COLORS_PER_VERTEX + Constellations::COORDS_PER_VERTEX);

const GLfloat Constellations::TEXT_RATIO(1000);

const float Constellations::VIEW_OFFSET(3000);
const Vector3D Constellations::VIEW_UP(0, 0, 1);

// Don't alter this initial state !!
const Constellations::state Constellations::INITIAL_CYCLE_STATE(ALL_ON);

Constellations::Constellations(vec_t rad) : radius(rad) {
   // Set initial display cycle state.
   current_cycle_state = INITIAL_CYCLE_STATE;

   tot_stars = 0;
   tot_links = 0;

   // TODO - There's a HUGE 'BEWARE' with this method. The link numbers are used as
   // indices to a vector that stores the star data AS PER THE LISTED ORDER in
   // which they are added to the constellation. Do not exceed bounds, and
   // always enter a link AFTER the addition of the stars which it connects.
   // Actually I'm not happy generally as there's a dependency in ordering here.

   Constellation andromeda("Andromeda");
   andromeda.addToDescription("The princess chained to a rock");
   andromeda.add_star(OrdStar(2.01f, 29.09f, 2.07f, OrdStar::SPEC_B, "Alpheratz"));
   andromeda.add_star(OrdStar(17.26f, 35.62f, 2.07f, OrdStar::SPEC_M, "Mirach"));
   andromeda.add_star(OrdStar(30.76f, 42.33f, 2.10f, OrdStar::SPEC_B, "Almach"));
   andromeda.add_star(OrdStar(9.76f, 30.86f, 3.27f, OrdStar::SPEC_K, "Delta"));
   andromeda.add_star(OrdStar(24.27f, 48.63f, 3.59f, OrdStar::SPEC_K, "Nembus"));
   andromeda.add_star(OrdStar(345.27f, 42.33f, 3.62f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(354.26f, 46.46f, 3.81f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(14.01f, 38.50f, 3.86f, OrdStar::SPEC_A, ""));
   andromeda.add_star(OrdStar(11.76f, 24.27f, 4.08f, OrdStar::SPEC_K, ""));
   andromeda.add_star(OrdStar(12.45f, 41.06f, 4.10f, OrdStar::SPEC_F, ""));
   andromeda.add_star(OrdStar(355.01f, 44.33f, 4.15f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(17.26f, 47.24f, 4.26f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(354.50f, 43.27f, 4.29f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(9.01f, 33.72f, 4.34f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(9.51f, 29.31f, 4.34f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(14.25f, 23.42f, 4.40f, OrdStar::SPEC_G, ""));
   andromeda.add_link(0, 3);
   andromeda.add_link(1, 2);
   andromeda.add_link(1, 3);
   andromeda.add_link(1, 7);
   andromeda.add_link(1, 13);
   andromeda.add_link(3, 13);
   andromeda.add_link(3, 14);
   andromeda.add_link(4, 11);
   andromeda.add_link(5, 12);
   andromeda.add_link(6, 10);
   andromeda.add_link(7, 9);
   andromeda.add_link(8, 14);
   andromeda.add_link(8, 15);
   andromeda.add_link(9, 11);
   andromeda.add_link(10, 12);
   andromeda.add_link(12, 13);
   cons_list.push_back(andromeda);
   tot_stars += andromeda.total_stars();
   tot_links += andromeda.total_links();

   Constellation antlia("Antlia");
   antlia.addToDescription("The air pump");
   antlia.add_star(OrdStar(142.25f, -35.95f, 4.51f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(156.75f, -31.07f, 4.28f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(164.01f, -37.14f, 4.60f, OrdStar::SPEC_K, ""));
   antlia.add_link(0, 1);
   antlia.add_link(1, 2);
   cons_list.push_back(antlia);
   tot_stars += antlia.total_stars();
   tot_links += antlia.total_links();

   Constellation apus("Apus");
   apus.addToDescription("The bird of paradise");
   apus.add_star(OrdStar(221.76f, -79.03f, 3.83f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(248.26f, -78.88f, 3.86f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(250.75f, -77.50f, 4.23f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(245.01f, -78.68f, 4.68f, OrdStar::SPEC_M, ""));
   apus.add_link(0, 3);
   apus.add_link(1, 2);
   apus.add_link(2, 3);
   cons_list.push_back(apus);
   tot_stars += apus.total_stars();
   tot_links += apus.total_links();

   Constellation aquarius("Aquarius");
   aquarius.addToDescription("The water bearer");
   aquarius.add_star(OrdStar(322.76f, -5.56f, 2.90f, OrdStar::SPEC_G, "Sadalsuud"));
   aquarius.add_star(OrdStar(331.50f, -0.32f, 2.95f, OrdStar::SPEC_G, "Sadalmelik"));
   aquarius.add_star(OrdStar(343.75f, -15.81f, 3.27f, OrdStar::SPEC_A, "Skat"));
   aquarius.add_star(OrdStar(337.25f, -0.02f, 3.65f, OrdStar::SPEC_F, "Sadaltager"));
   aquarius.add_star(OrdStar(347.25f, -21.17f, 3.68f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(343.25f, -7.57f, 3.73f, OrdStar::SPEC_M, "Hydor"));
   aquarius.add_star(OrdStar(312.00f, -9.50f, 3.78f, OrdStar::SPEC_A, "Albali"));
   aquarius.add_star(OrdStar(335.50f, -1.38f, 3.86f, OrdStar::SPEC_A, "Sadachbia"));
   aquarius.add_star(OrdStar(350.75f, -20.10f, 3.96f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(338.75f, -0.12f, 4.04f, OrdStar::SPEC_B, ""));
   aquarius.add_star(OrdStar(342.50f, -13.60f, 4.05f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(334.25f, -7.78f, 4.17f, OrdStar::SPEC_G, "Ancha"));
   aquarius.add_star(OrdStar(348.75f, -6.05f, 4.22f, OrdStar::SPEC_M, ""));
   aquarius.add_star(OrdStar(349.00f, -9.08f, 4.24f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(331.50f, -13.87f, 4.29f, OrdStar::SPEC_B, ""));
   aquarius.add_star(OrdStar(336.25f, 1.38f, 4.80f, OrdStar::SPEC_B, "Seat"));
   aquarius.add_link(0, 1);
   aquarius.add_link(0, 6);
   aquarius.add_link(0, 14);
   aquarius.add_link(1, 7);
   aquarius.add_link(1, 11);
   aquarius.add_link(1, 15);
   aquarius.add_link(2, 10);
   aquarius.add_link(2, 13);
   aquarius.add_link(3, 7);
   aquarius.add_link(3, 9);
   aquarius.add_link(3, 15);
   aquarius.add_link(4, 13);
   aquarius.add_link(5, 10);
   aquarius.add_link(5, 11);
   aquarius.add_link(5, 12);
   aquarius.add_link(8, 13);
   aquarius.add_link(12, 13);
   cons_list.push_back(aquarius);
   tot_stars += aquarius.total_stars();
   tot_links += aquarius.total_links();

   Constellation aquila("Aquila");
   aquila.addToDescription("The eagle");
   aquila.add_star(OrdStar(297.51f, +8.87f, 0.76f, OrdStar::SPEC_A, "Altair"));
   aquila.add_star(OrdStar(296.50f, +10.62f, 2.72f, OrdStar::SPEC_K, "Tarazed"));
   aquila.add_star(OrdStar(286.26f, +13.87f, 2.99f, OrdStar::SPEC_A, "Deneb"));
   aquila.add_star(OrdStar(302.76f, -0.82f, 3.24f, OrdStar::SPEC_B, "Tseen Foo"));
   aquila.add_star(OrdStar(291.50f, +3.12f, 3.36f, OrdStar::SPEC_F, "Denebokab"));
   aquila.add_star(OrdStar(286.50f, -4.88f, 3.43f, OrdStar::SPEC_B, "Al Thalimain"));
   aquila.add_star(OrdStar(298.76f, +6.42f, 3.71f, OrdStar::SPEC_G, "Alshain"));
   aquila.add_star(OrdStar(298.01f, +1.00f, 3.87f, OrdStar::SPEC_F, ""));
   aquila.add_star(OrdStar(285.00f, +15.06f, 4.02f, OrdStar::SPEC_K, "Deneb el Okab"));
   aquila.add_star(OrdStar(294.25f, -1.28f, 4.36f, OrdStar::SPEC_B, "Al Thalimain"));
   aquila.add_link(0, 1);
   aquila.add_link(0, 6);
   aquila.add_link(1, 4);
   aquila.add_link(2, 4);
   aquila.add_link(2, 5);
   aquila.add_link(2, 8);
   aquila.add_link(3, 7);
   aquila.add_link(3, 9);
   aquila.add_link(4, 5);
   aquila.add_link(4, 7);
   aquila.add_link(5, 9);
   cons_list.push_back(aquila);
   tot_stars += aquila.total_stars();
   tot_links += aquila.total_links();

   Constellation ara("Ara");
   ara.addToDescription("The altar");
   ara.add_star(OrdStar(261.25f, -54.53f, 2.84f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(263.00f, -49.88f, 2.84f, OrdStar::SPEC_B, "Choo"));
   ara.add_star(OrdStar(254.75f, -55.98f, 3.12f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(261.25f, -56.38f, 3.31f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(262.75f, -60.68f, 3.60f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(271.75f, -50.08f, 3.65f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(252.50f, -59.03f, 3.77f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(255.00f, -53.16f, 4.06f, OrdStar::SPEC_K, ""));
   ara.add_link(0, 1);
   ara.add_link(0, 3);
   ara.add_link(1, 5);
   ara.add_link(1, 7);
   ara.add_link(2, 6);
   ara.add_link(2, 7);
   ara.add_link(3, 4);
   ara.add_link(4, 6);
   cons_list.push_back(ara);
   tot_stars += ara.total_stars();
   tot_links += ara.total_links();

   Constellation aries("Aries");
   aries.addToDescription("The ram");
   aries.add_star(OrdStar(31.75f, +23.47f, 2.01f, OrdStar::SPEC_K, "Hamal"));
   aries.add_star(OrdStar(28.51f, +20.81f, 2.64f, OrdStar::SPEC_A, "Sheratan"));
   aries.add_star(OrdStar(42.27f, +27.26f, 3.61f, OrdStar::SPEC_B, "Bharani"));
   aries.add_star(OrdStar(28.25f, +19.29f, 3.88f, OrdStar::SPEC_B, "Mesarthim"));
   aries.add_star(OrdStar(47.76f, +19.73f, 4.35f, OrdStar::SPEC_K, "Botein"));
   aries.add_link(0, 1);
   aries.add_link(0, 2);
   aries.add_link(1, 3);
   aries.add_link(2, 4);
   cons_list.push_back(aries);
   tot_stars += aries.total_stars();
   tot_links += aries.total_links();

   Constellation auriga("Auriga");
   auriga.addToDescription("The charioteer");
   auriga.add_star(OrdStar(79.25f, 45.99f, 0.08f, OrdStar::SPEC_G, "Capella"));
   auriga.add_star(OrdStar(89.87f, 44.95f, 1.90f, OrdStar::SPEC_A, "Menkalinan"));
   auriga.add_star(OrdStar(89.94f, 37.21f, 2.65f, OrdStar::SPEC_A, "Mahasim"));
   auriga.add_star(OrdStar(74.25f, 33.17f, 2.69f, OrdStar::SPEC_K, "Kabdhilinan"));
   auriga.add_star(OrdStar(75.27f, 43.82f, 3.03f, OrdStar::SPEC_F, "Almaaz"));
   auriga.add_star(OrdStar(76.62f, 41.23f, 3.18f, OrdStar::SPEC_B, "Haedus II"));
   auriga.add_star(OrdStar(75.62f, 41.08f, 3.69f, OrdStar::SPEC_K, "Sadatoni"));
   auriga.add_star(OrdStar(89.87f, 54.28f, 3.72f, OrdStar::SPEC_K, "Prijipati"));
   // This is Elnath or Beta-Tauri included to connect this Charioteer to that Bull.
   auriga.add_star(OrdStar(81.56f, 28.60f, 1.65f, OrdStar::SPEC_B, "Elnath"));
   auriga.add_link(0, 1);
   auriga.add_link(0, 4);
   auriga.add_link(0, 5);
   auriga.add_link(0, 7);
   auriga.add_link(1, 2);
   auriga.add_link(1, 7);
   auriga.add_link(2, 8);
   auriga.add_link(3, 5);
   auriga.add_link(3, 8);
   auriga.add_link(4, 6);
   cons_list.push_back(auriga);
   tot_stars += auriga.total_stars();
   tot_links += auriga.total_links();

   Constellation bootes("Bootes");
   bootes.addToDescription("The herdsman");
   bootes.add_star(OrdStar(213.94f, 19.18f, -0.05f, OrdStar::SPEC_K, "Arcturus"));
   bootes.add_star(OrdStar(221.25f, 27.08f, 2.35f, OrdStar::SPEC_A, "Izar"));
   bootes.add_star(OrdStar(208.67f, 18.40f, 2.68f, OrdStar::SPEC_G, "Muphrid"));
   bootes.add_star(OrdStar(218.00f, 38.31f, 3.04f, OrdStar::SPEC_A, "Seginus"));
   bootes.add_star(OrdStar(228.76f, 33.32, 3.46f, OrdStar::SPEC_G, "Princeps"));
   bootes.add_star(OrdStar(225.27f, 40.39f, 3.49f, OrdStar::SPEC_G, "Nekkar"));
   bootes.add_star(OrdStar(218.00f, 30.37f, 3.57f, OrdStar::SPEC_K, ""));
   bootes.add_star(OrdStar(220.25f, 13.73f, 3.78f, OrdStar::SPEC_A, ""));
   bootes.add_star(OrdStar(216.25f, 51.85f, 4.04f, OrdStar::SPEC_F, "Asellus Primus"));
   bootes.add_star(OrdStar(214.60f, 46.09f, 4.18f, OrdStar::SPEC_A, "Xuange"));
   bootes.add_star(OrdStar(206.81f, 17.46f, 4.50f, OrdStar::SPEC_F, ""));
   bootes.add_star(OrdStar(213.38f, 51.79f, 4.53f, OrdStar::SPEC_A, "Asellus Tertius"));
   bootes.add_link(0, 1);
   bootes.add_link(0, 2);
   bootes.add_link(0, 6);
   bootes.add_link(0, 7);
   bootes.add_link(1, 4);
   bootes.add_link(2, 10);
   bootes.add_link(3, 5);
   bootes.add_link(3, 6);
   bootes.add_link(3, 9);
   bootes.add_link(4, 5);
   bootes.add_link(8, 9);
   bootes.add_link(8, 11);
   bootes.add_link(9, 11);
   cons_list.push_back(bootes);
   tot_stars += bootes.total_stars();
   tot_links += bootes.total_links();

   Constellation caelum("Caelum");
   caelum.addToDescription("The chisel");
   caelum.add_star(OrdStar(70.01f, -41.87f, 4.44f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(76.09f, -35.48f, 4.55f, OrdStar::SPEC_K, ""));
   caelum.add_star(OrdStar(70.50f, -37.14f, 5.04f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(67.62f, -44.95f, 5.07f, OrdStar::SPEC_B, ""));
   caelum.add_link(0, 2);
   caelum.add_link(0, 3);
   caelum.add_link(1, 2);
   cons_list.push_back(caelum);
   tot_stars += caelum.total_stars();
   tot_links += caelum.total_links();

   Constellation camelopardalis("Camelopardalis");
   camelopardalis.addToDescription("The camel");
   camelopardalis.add_star(OrdStar(75.87f, 60.44f, 4.03f, OrdStar::SPEC_G, ""));
   camelopardalis.add_star(OrdStar(52.25f, 59.93f, 4.21f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(73.50f, 66.34f, 4.26f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(57.25f, 65.53f, 4.39f, OrdStar::SPEC_M, ""));
   camelopardalis.add_star(OrdStar(74.25f, 53.75f, 4.43f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(105.00f, 76.98f, 4.55f, OrdStar::SPEC_K, ""));
   camelopardalis.add_star(OrdStar(57.50f, 71.33f, 4.59f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(94.51f, 69.32f, 4.76f, OrdStar::SPEC_A, ""));
   camelopardalis.add_link(0, 2);
   camelopardalis.add_link(0, 4);
   camelopardalis.add_link(1, 3);
   camelopardalis.add_link(2, 6);
   camelopardalis.add_link(2, 7);
   camelopardalis.add_link(3, 6);
   camelopardalis.add_link(5, 7);
   cons_list.push_back(camelopardalis);
   tot_stars += camelopardalis.total_stars();
   tot_links += camelopardalis.total_links();

   Constellation cancer("Cancer");
   cancer.addToDescription("The crab");
   cancer.add_star(OrdStar(124.12f, 9.18f, 3.53f, OrdStar::SPEC_K, "Al Tarf"));
   cancer.add_star(OrdStar(131.18f, 18.15f, 3.94f, OrdStar::SPEC_K, "Asellus Australis"));
   cancer.add_star(OrdStar(131.67f, 28.76f, 4.03f, OrdStar::SPEC_G, ""));
   cancer.add_star(OrdStar(134.63f, 11.86f, 4.26f, OrdStar::SPEC_A, "Acubens"));
   cancer.add_star(OrdStar(130.81f, 21.47f, 4.66f, OrdStar::SPEC_A, "Asellus Borealis"));
   cancer.add_link(0, 1);
   cancer.add_link(1, 3);
   cancer.add_link(1, 4);
   cancer.add_link(2, 4);
   cons_list.push_back(cancer);
   tot_stars += cancer.total_stars();
   tot_links += cancer.total_links();

   Constellation canes_venatici("Canes Venatici");
   canes_venatici.addToDescription("The hunting dogs");
   canes_venatici.add_star(OrdStar(194.00f, 38.32f, 2.89f, OrdStar::SPEC_A, "Cor Caroli"));
   canes_venatici.add_star(OrdStar(188.44f, 41.36f, 4.24f, OrdStar::SPEC_G, "Chara"));
   canes_venatici.add_link(0, 1);
   cons_list.push_back(canes_venatici);
   tot_stars += canes_venatici.total_stars();
   tot_links += canes_venatici.total_links();

   Constellation canis_major("Canis Major");
   canis_major.addToDescription("The greater dog");
   canis_major.add_star(OrdStar(101.25f, -16.71f, 1.46f, OrdStar::SPEC_A, "Sirius"));
   canis_major.add_star(OrdStar(104.51f, -28.97f, 1.50f, OrdStar::SPEC_B, "Adhara"));
   canis_major.add_star(OrdStar(107.00f, -26.39f, 1.83f, OrdStar::SPEC_F, "Wezen"));
   canis_major.add_star(OrdStar(95.51f, -17.96f, 1.98f, OrdStar::SPEC_B, "Mirzam"));
   canis_major.add_star(OrdStar(111.00f, -29.30f, 2.45f, OrdStar::SPEC_B, "Aludra"));
   canis_major.add_star(OrdStar(95.01f, -30.06f, 3.02f, OrdStar::SPEC_B, "Furud"));
   canis_major.add_star(OrdStar(105.75f, -23.83f, 3.02f, OrdStar::SPEC_B, "Thanih al Adzari"));
   canis_major.add_link(0, 3);
   canis_major.add_link(0, 6);
   canis_major.add_link(1, 2);
   canis_major.add_link(1, 3);
   canis_major.add_link(2, 4);
   canis_major.add_link(2, 6);
   cons_list.push_back(canis_major);
   tot_stars += canis_major.total_stars();
   tot_links += canis_major.total_links();

   Constellation canis_minor("Canis Minor");
   canis_minor.addToDescription("The lesser dog");
   canis_minor.add_star(OrdStar(114.76f, 5.23f, 0.34f, OrdStar::SPEC_F, "Procyon"));
   canis_minor.add_star(OrdStar(111.75f, 8.29f, 2.89f, OrdStar::SPEC_B, "Gomeisa"));
   canis_minor.add_link(0, 1);
   cons_list.push_back(canis_minor);
   tot_stars += canis_minor.total_stars();
   tot_links += canis_minor.total_links();

   Constellation capricornus("Capricornus");
   capricornus.addToDescription("The sea goat");
   capricornus.add_star(OrdStar(326.75f, -16.13f, 2.85f, OrdStar::SPEC_A, "Deneb"));
   capricornus.add_star(OrdStar(305.25f, -14.78f, 3.05f, OrdStar::SPEC_A, "Dabih"));
   capricornus.add_star(OrdStar(304.50f, -12.54f, 3.58f, OrdStar::SPEC_G, "Algedi Secunda"));
   capricornus.add_star(OrdStar(325.00f, -16.66f, 3.69f, OrdStar::SPEC_A, "Nashira"));
   capricornus.add_star(OrdStar(321.51f, -22.41f, 3.77f, OrdStar::SPEC_G, "Yan"));
   capricornus.add_star(OrdStar(316.27f, -16.76f, 4.08f, OrdStar::SPEC_A, "Qin"));
   capricornus.add_star(OrdStar(312.76f, -26.92f, 4.12f, OrdStar::SPEC_K, "Baten Algiedi"));
   capricornus.add_star(OrdStar(311.50f, -25.27f, 4.13f, OrdStar::SPEC_F, "Yue"));
   capricornus.add_star(OrdStar(320.50f, -16.83f, 4.28f, OrdStar::SPEC_G, "Dae"));
   capricornus.add_link(0, 3);
   capricornus.add_link(0, 4);
   capricornus.add_link(1, 2);
   capricornus.add_link(1, 6);
   capricornus.add_link(2, 5);
   capricornus.add_link(3, 8);
   capricornus.add_link(4, 6);
   capricornus.add_link(5, 8);
   capricornus.add_link(6, 7);
   cons_list.push_back(capricornus);
   tot_stars += capricornus.total_stars();
   tot_links += capricornus.total_links();

   Constellation carina("Carina");
   carina.addToDescription("The keel");
   carina.add_star(OrdStar(95.77f, -52.68f, -0.72f, OrdStar::SPEC_F, "Canopus"));
   carina.add_star(OrdStar(138.25f, -69.72f, 1.67f, OrdStar::SPEC_A, "Miaplacidus"));

   carina.add_star(OrdStar(125.51f, -59.52f, 1.86f, OrdStar::SPEC_K, "Avior"));

   carina.add_star(OrdStar(139.25f, -59.28f, 2.21f, OrdStar::SPEC_A, "Aspidiske"));

   carina.add_star(OrdStar(160.52f, -64.40f, 2.74f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(146.75f, -65.07f, 2.92f, OrdStar::SPEC_A, ""));
   carina.add_star(OrdStar(153.26f, -70.04f, 3.29f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(158.00f, -61.68f, 3.30f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(154.25f, -61.33f, 3.39f, OrdStar::SPEC_K, ""));
   carina.add_star(OrdStar(119.19f, -52.98f, 3.46f, OrdStar::SPEC_B, ""));
   carina.add_link(0, 1);
   carina.add_link(1, 6);
   carina.add_link(2, 3);
   carina.add_link(2, 9);
   carina.add_link(3, 8);
   carina.add_link(4, 6);
   carina.add_link(4, 7);
   carina.add_link(7, 8);
   cons_list.push_back(carina);
   tot_stars += carina.total_stars();
   tot_links += carina.total_links();

   Constellation cassiopeia("Cassiopeia");
   cassiopeia.addToDescription("The lady tied to a chair");
   cassiopeia.add_star(OrdStar(14.01f, 60.72f, 2.15f, OrdStar::SPEC_B, "Tsih"));
   cassiopeia.add_star(OrdStar(10.01f, 56.54f, 2.24f, OrdStar::SPEC_K, "Schedar"));
   cassiopeia.add_star(OrdStar(2.25f, 59.15f, 2.28f, OrdStar::SPEC_F, "Caph"));
   cassiopeia.add_star(OrdStar(21.26f, 60.24f, 2.66f, OrdStar::SPEC_A, "Ruchbah"));
   cassiopeia.add_star(OrdStar(28.51f, 63.67f, 3.35f, OrdStar::SPEC_B, "Segin"));
   cassiopeia.add_star(OrdStar(12.25f, 57.82f, 3.46f, OrdStar::SPEC_G, "Achird"));
   cassiopeia.add_star(OrdStar(9.02f, 53.90f, 3.69f, OrdStar::SPEC_B, "Foo Loo"));
   cassiopeia.add_link(0, 3);
   cassiopeia.add_link(0, 5);
   cassiopeia.add_link(1, 2);
   cassiopeia.add_link(1, 5);
   cassiopeia.add_link(1, 6);
   cassiopeia.add_link(3, 4);
   cons_list.push_back(cassiopeia);
   tot_stars += cassiopeia.total_stars();
   tot_links += cassiopeia.total_links();

   Constellation centaurus("Centaurus");
   centaurus.addToDescription("The centaur");
   centaurus.add_star(OrdStar(219.91f, -60.83f, -0.01f, OrdStar::SPEC_G, "Rigil Kentaurus"));
   centaurus.add_star(OrdStar(210.96f, -60.36f, 0.61f, OrdStar::SPEC_B, "Hadar"));
   centaurus.add_star(OrdStar(211.67f, -36.37f, 2.06f, OrdStar::SPEC_K, "Menkent"));
   centaurus.add_star(OrdStar(190.37f, -48.96f, 2.20f, OrdStar::SPEC_A, "Muhlifain"));
   centaurus.add_star(OrdStar(204.76f, -53.47f, 2.29f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(218.87f, -42.16f, 2.33f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(208.88f, -47.28f, 2.55f, OrdStar::SPEC_B, "Alnair"));
   centaurus.add_star(OrdStar(182.08f, -50.72f, 2.58f, OrdStar::SPEC_B, "Ma Wei"));
   centaurus.add_star(OrdStar(200.12f, -36.72f, 2.75f, OrdStar::SPEC_A, ""));
   centaurus.add_star(OrdStar(224.75f, -42.10f, 3.13f, OrdStar::SPEC_B, "Ke Kwan"));
   centaurus.add_star(OrdStar(207.41f, -42.47f, 3.47f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(209.56f, -42.10f, 3.83f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(170.25f, -54.49f, 3.90f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(187.00f, -50.23f, 3.91f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(183.00f, -52.37f, 3.97f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(215.12f, -37.88f, 4.05f, OrdStar::SPEC_A, ""));
   centaurus.add_star(OrdStar(189.00f, -39.98f, 4.63f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(172.94f, -59.44f, 5.07f, OrdStar::SPEC_B, ""));
   centaurus.add_link(0, 1);
   centaurus.add_link(1, 4);
   centaurus.add_link(2, 10);
   centaurus.add_link(2, 15);
   centaurus.add_link(3, 4);
   centaurus.add_link(3, 6);
   centaurus.add_link(3, 13);
   centaurus.add_link(4, 6);
   centaurus.add_link(5, 9);
   centaurus.add_link(5, 11);
   centaurus.add_link(6, 10);
   centaurus.add_link(6, 11);
   centaurus.add_link(7, 12);
   centaurus.add_link(7, 13);
   centaurus.add_link(8, 10);
   centaurus.add_link(8, 16);
   centaurus.add_link(11, 15);
   centaurus.add_link(13, 14);
   centaurus.add_link(14, 17);
   cons_list.push_back(centaurus);
   tot_stars += centaurus.total_stars();
   tot_links += centaurus.total_links();

   Constellation cepheus("Cepheus");
   cepheus.addToDescription("The husband of Cassiopeia and father of Andromeda");
   cepheus.add_star(OrdStar(319.62f, 62.59f, 2.45f, OrdStar::SPEC_A, "Alderamin"));
   cepheus.add_star(OrdStar(354.83f, 77.63f, 3.21f, OrdStar::SPEC_K, "Arrai"));
   cepheus.add_star(OrdStar(322.17f, 70.56f, 3.23f, OrdStar::SPEC_B, "Alfirk"));
   cepheus.add_star(OrdStar(332.75f, 58.20f, 3.39f, OrdStar::SPEC_K, "Tsao Fu"));
   cepheus.add_star(OrdStar(311.31f, 61.83f, 3.41f, OrdStar::SPEC_K, "Al Kidr"));
   cepheus.add_star(OrdStar(342.41f, 66.20f, 3.50f, OrdStar::SPEC_K, ""));
   cepheus.add_star(OrdStar(337.25f, 58.42f, 4.07f, OrdStar::SPEC_G, "Alrediph"));
   cepheus.add_star(OrdStar(333.75f, 57.04f, 4.18f, OrdStar::SPEC_F, ""));
   cepheus.add_star(OrdStar(307.37f, 62.99f, 4.21f, OrdStar::SPEC_A, "Al Kidr"));
   cepheus.add_star(OrdStar(325.88f, 58.78f, 4.23f, OrdStar::SPEC_M, "Herschel's Garnet Star"));
   cepheus.add_link(0, 2);
   cepheus.add_link(0, 4);
   cepheus.add_link(0, 9);
   cepheus.add_link(1, 2);
   cepheus.add_link(1, 5);
   cepheus.add_link(2, 5);
   cepheus.add_link(3, 6);
   cepheus.add_link(3, 7);
   cepheus.add_link(4, 8);
   cepheus.add_link(5, 6);
   cepheus.add_link(7, 9);
   cons_list.push_back(cepheus);
   tot_stars += cepheus.total_stars();
   tot_links += cepheus.total_links();

   Constellation cetus("Cetus");
   cetus.addToDescription("The whale");
   cetus.add_star(OrdStar(10.88f, -17.98f, 2.04f, OrdStar::SPEC_K, "Deneb Kaitos"));
   cetus.add_star(OrdStar(45.56f, 4.09f, 2.54f, OrdStar::SPEC_M, "Menkar"));
   cetus.add_star(OrdStar(34.83f, -2.98, 3.04f, OrdStar::SPEC_M, "Mira Ceti"));
   cetus.add_star(OrdStar(17.12f, -10.18f, 3.46f, OrdStar::SPEC_K, "Deneb Algenubi"));
   cetus.add_star(OrdStar(40.83f, 3.23f, 3.47f, OrdStar::SPEC_A, "Kaffaljidhmah"));
   cetus.add_star(OrdStar(26.00f, -15.93f, 3.49f, OrdStar::SPEC_G, "Durre Menthor"));
   cetus.add_star(OrdStar(4.88f, -8.83f, 3.56f, OrdStar::SPEC_K, "Schemali"));
   cetus.add_star(OrdStar(21.00f, -8.18f, 3.60f, OrdStar::SPEC_K,  "Thanih al Naamat"));
   cetus.add_star(OrdStar(27.87f, -10.33f, 3.74f, OrdStar::SPEC_K, "Baten Kaitos"));
   cetus.add_star(OrdStar(39.87f, 0.33f, 4.08f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(41.25f, 10.12f, 4.27f, OrdStar::SPEC_F,  ""));
   cetus.add_star(OrdStar(37.00f, 8.45f, 4.30f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(44.92f, 8.91f, 4.71f, OrdStar::SPEC_B, "Menkar"));
   cetus.add_star(OrdStar(39.00f, 5.59f, 4.87f, OrdStar::SPEC_G,  ""));
   cetus.add_link(0, 5);
   cetus.add_link(0, 6);
   cetus.add_link(1, 4);
   cetus.add_link(1, 12);
   cetus.add_link(2, 8);
   cetus.add_link(2, 9);
   cetus.add_link(3, 6);
   cetus.add_link(3, 7);
   cetus.add_link(4, 9);
   cetus.add_link(4, 13);
   cetus.add_link(5, 8);
   cetus.add_link(7, 8);
   cetus.add_link(10, 11);
   cetus.add_link(10, 12);
   cetus.add_link(11, 13);
   cons_list.push_back(cetus);
   tot_stars += cetus.total_stars();
   tot_links += cetus.total_links();

   Constellation chamaeleon("Chamaeleon");
   chamaeleon.addToDescription("The chamaeleon");
   chamaeleon.add_star(OrdStar(124.62f, -76.92f, 4.05f, OrdStar::SPEC_F, ""));
   chamaeleon.add_star(OrdStar(158.87f, -78.61f, 4.11f, OrdStar::SPEC_M, ""));
   chamaeleon.add_star(OrdStar(184.58f, -79.31f, 4.24f, OrdStar::SPEC_B, ""));
   chamaeleon.add_star(OrdStar(161.44f, -80.53f, 4.45f, OrdStar::SPEC_B, ""));
   chamaeleon.add_star(OrdStar(179.92f, -78.22f, 4.88f, OrdStar::SPEC_B, ""));
   chamaeleon.add_link(0, 1);
   chamaeleon.add_link(1, 3);
   chamaeleon.add_link(1, 4);
   chamaeleon.add_link(2, 3);
   chamaeleon.add_link(2, 4);
   cons_list.push_back(chamaeleon);
   tot_stars += chamaeleon.total_stars();
   tot_links += chamaeleon.total_links();

   Constellation circinus("Circinus");
   circinus.addToDescription("The draftsman's compass");
   circinus.add_star(OrdStar(220.62f, -64.98f, 3.18f, OrdStar::SPEC_F, ""));
   circinus.add_star(OrdStar(229.37f, -58.80f, 4.07f, OrdStar::SPEC_A, ""));
   circinus.add_star(OrdStar(230.83f, -59.32f, 4.48f, OrdStar::SPEC_B, ""));
   circinus.add_link(0, 1);
   circinus.add_link(0, 2);
   cons_list.push_back(circinus);
   tot_stars += circinus.total_stars();
   tot_links += circinus.total_links();

   Constellation columba("Columba");
   columba.addToDescription("The dove");
   columba.add_star(OrdStar(84.92f, -34.08f, 2.65f, OrdStar::SPEC_B, "Phact"));
   columba.add_star(OrdStar(87.75f, -35.77f, 3.12f, OrdStar::SPEC_K, "Wezn"));
   columba.add_star(OrdStar(95.50f, -33.43f, 3.85f, OrdStar::SPEC_G, ""));
   columba.add_star(OrdStar(82.80f, -35.47f, 3.86f, OrdStar::SPEC_K, ""));
   columba.add_star(OrdStar(89.75f, -42.82f, 3.96f, OrdStar::SPEC_K, ""));
   columba.add_star(OrdStar(89.37f, -35.28f, 4.36f, OrdStar::SPEC_B, ""));
   columba.add_link(0, 1);
   columba.add_link(0, 3);
   columba.add_link(1, 4);
   columba.add_link(1, 5);
   columba.add_link(2, 5);
   cons_list.push_back(columba);
   tot_stars += columba.total_stars();
   tot_links += columba.total_links();

   Constellation coma_berenices("Coma Berenices");
   coma_berenices.addToDescription("Berenice's hair");
   coma_berenices.add_star(OrdStar(197.76f, 27.88f, 4.23f, OrdStar::SPEC_G, ""));
   coma_berenices.add_star(OrdStar(197.46f, 17.53f, 4.32f, OrdStar::SPEC_F, "Diadem"));
   coma_berenices.add_star(OrdStar(186.52f, 28.27f, 4.35f, OrdStar::SPEC_K, ""));
   coma_berenices.add_link(0, 1);
   coma_berenices.add_link(0, 2);
   cons_list.push_back(coma_berenices);
   tot_stars += coma_berenices.total_stars();
   tot_links += coma_berenices.total_links();

   Constellation corona_australis("Corona Australis");
   corona_australis.addToDescription("The southern crown");
   corona_australis.add_star(OrdStar(287.50f, -39.34f, 4.10f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(287.37f, -37.90f, 4.11f, OrdStar::SPEC_A, "Alphekka Meridiana"));
   corona_australis.add_star(OrdStar(286.62f, -37.06f, 4.23f, OrdStar::SPEC_F, ""));
   corona_australis.add_star(OrdStar(287.08f, -40.49f, 4.57f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(278.37f, -42.31f, 4.62f, OrdStar::SPEC_G, ""));
   corona_australis.add_star(OrdStar(285.77f, -42.09f, 4.74f, OrdStar::SPEC_A, ""));
   corona_australis.add_star(OrdStar(284.69f, -37.11f, 4.83f, OrdStar::SPEC_F, ""));
   corona_australis.add_link(0, 1);
   corona_australis.add_link(0, 3);
   corona_australis.add_link(1, 2);
   corona_australis.add_link(2, 6);
   corona_australis.add_link(3, 5);
   corona_australis.add_link(4, 5);
   cons_list.push_back(corona_australis);
   tot_stars += corona_australis.total_stars();
   tot_links += corona_australis.total_links();

   Constellation corona_borealis("Corona Borealis");
   corona_borealis.addToDescription("The northern crown");
   corona_borealis.add_star(OrdStar(233.67f, 26.71f, 2.22f, OrdStar::SPEC_A, "Alphecca"));
   corona_borealis.add_star(OrdStar(231.95f, 29.11f, 3.66f, OrdStar::SPEC_F, "Nusakan"));
   corona_borealis.add_star(OrdStar(235.69f, 26.30f, 3.81f, OrdStar::SPEC_A, ""));
   corona_borealis.add_star(OrdStar(233.25f, 31.36f, 4.14f, OrdStar::SPEC_B, ""));
   corona_borealis.add_star(OrdStar(239.37f, 26.88f, 4.14f, OrdStar::SPEC_K, ""));
   corona_borealis.add_star(OrdStar(237.38f, 26.07f, 4.59f, OrdStar::SPEC_G, ""));
   corona_borealis.add_star(OrdStar(245.50f, 30.89f, 4.86f, OrdStar::SPEC_K, ""));
   corona_borealis.add_link(0, 1);
   corona_borealis.add_link(0, 2);
   corona_borealis.add_link(1, 3);
   corona_borealis.add_link(2, 5);
   corona_borealis.add_link(4, 5);
   corona_borealis.add_link(4, 6);
   cons_list.push_back(corona_borealis);
   tot_stars += corona_borealis.total_stars();
   tot_links += corona_borealis.total_links();

   Constellation corvus("Corvus");
   corvus.addToDescription("The crow");
   corvus.add_star(OrdStar(183.95f, -17.54f, 2.58f, OrdStar::SPEC_B, "Gienah Corvi"));
   corvus.add_star(OrdStar(188.60f, -23.39f, 2.65f, OrdStar::SPEC_G, "Kraz"));
   corvus.add_star(OrdStar(187.47f, -16.52f, 2.94f, OrdStar::SPEC_B, "Algorab"));
   corvus.add_star(OrdStar(182.52f, -22.62f, 3.02f, OrdStar::SPEC_K, "Minkar"));
   corvus.add_star(OrdStar(182.10f, -24.73f, 4.02f, OrdStar::SPEC_F, "Al Chiba"));
   corvus.add_link(0, 2);
   corvus.add_link(0, 3);
   corvus.add_link(1, 2);
   corvus.add_link(1, 3);
   corvus.add_link(3, 4);
   cons_list.push_back(corvus);
   tot_stars += corvus.total_stars();
   tot_links += corvus.total_links();

   Constellation crater("Crater");
   crater.addToDescription("The cup");
   crater.add_star(OrdStar(169.83f, -14.78f, 3.56f, OrdStar::SPEC_K, "Labrum"));
   crater.add_star(OrdStar(171.22f, -17.68f, 4.06f, OrdStar::SPEC_A, ""));
   crater.add_star(OrdStar(164.94f, -18.30f, 4.08f, OrdStar::SPEC_K, "Alkes"));
   crater.add_star(OrdStar(167.91f, -22.83f, 4.46f, OrdStar::SPEC_A, "Al Sharasif"));
   crater.add_star(OrdStar(174.15f, -9.80f, 4.70f, OrdStar::SPEC_B, ""));
   crater.add_star(OrdStar(176.19f, -18.35f, 4.71f, OrdStar::SPEC_G, ""));
   crater.add_star(OrdStar(171.15f, -10.85f, 4.81f, OrdStar::SPEC_K, ""));
   crater.add_star(OrdStar(179.00f, -17.15f, 5.17f, OrdStar::SPEC_A, ""));
   crater.add_link(0, 1);
   crater.add_link(0, 2);
   crater.add_link(0, 6);
   crater.add_link(1, 3);
   crater.add_link(1, 5);
   crater.add_link(2, 3);
   crater.add_link(4, 6);
   crater.add_link(5, 7);
   cons_list.push_back(crater);
   tot_stars += crater.total_stars();
   tot_links += crater.total_links();

   Constellation crux("Crux");
   crux.addToDescription("The southern cross");
   crux.add_star(OrdStar(191.94f, -59.68f, 1.25f, OrdStar::SPEC_B, "Mimosa"));
   crux.add_star(OrdStar(186.65f, -63.10f, 1.4f, OrdStar::SPEC_B, "Acrux"));
   crux.add_star(OrdStar(187.80f, -57.11f, 1.59f, OrdStar::SPEC_M, "Gacrux"));
   crux.add_star(OrdStar(183.80f, -58.75f, 2.79f, OrdStar::SPEC_B, "Palida"));
   crux.add_star(OrdStar(185.33f, -60.40f, 3.59f, OrdStar::SPEC_K, "Intrometida"));
   crux.add_link(0, 3);
   crux.add_link(1, 2);
   cons_list.push_back(crux);
   tot_stars += crux.total_stars();
   tot_links += crux.total_links();

   Constellation cygnus("Cygnus");
   cygnus.addToDescription("The swan");
   cygnus.add_star(OrdStar(310.35f, 45.28f, 1.25f, OrdStar::SPEC_A, "Deneb Cygni"));
   cygnus.add_star(OrdStar(305.55f, 40.26f, 2.23f, OrdStar::SPEC_F, "Sadr"));
   cygnus.add_star(OrdStar(311.55f, 33.97f, 2.48f, OrdStar::SPEC_K, "Gienah"));
   cygnus.add_star(OrdStar(296.25f, 45.13f, 2.86f, OrdStar::SPEC_B, "Ruc"));
   cygnus.add_star(OrdStar(292.67f, 27.95f, 3.05f, OrdStar::SPEC_K, "Al Minhar al Dajajah"));
   cygnus.add_star(OrdStar(318.25f, 30.22f, 3.21f, OrdStar::SPEC_G, ""));
   cygnus.add_star(OrdStar(292.43f, 51.73f, 3.76f, OrdStar::SPEC_A, ""));
   cygnus.add_star(OrdStar(289.28f, 53.37f, 3.80f, OrdStar::SPEC_K, ""));
   cygnus.add_star(OrdStar(303.40f, 46.74f, 3.80f, OrdStar::SPEC_K, ""));
   cygnus.add_star(OrdStar(299.07f, 35.08f, 3.89f, OrdStar::SPEC_K, ""));
   cygnus.add_star(OrdStar(314.29f, 41.17f, 3.94f, OrdStar::SPEC_A, ""));
   cygnus.add_link(0, 1);
   cygnus.add_link(0, 8);
   cygnus.add_link(0, 10);
   cygnus.add_link(1, 2);
   cygnus.add_link(1, 3);
   cygnus.add_link(1, 9);
   cygnus.add_link(2, 5);
   cygnus.add_link(3, 6);
   cygnus.add_link(4, 9);
   cygnus.add_link(5, 10);
   cygnus.add_link(6, 7);
   cygnus.add_link(6, 8);
   cons_list.push_back(cygnus);
   tot_stars += cygnus.total_stars();
   tot_links += cygnus.total_links();

   Constellation delphinus("Delphinus");
   delphinus.addToDescription("The dolphin");
   delphinus.add_star(OrdStar(309.37f, 14.59f, 3.64f, OrdStar::SPEC_F, "Rotanev"));
   delphinus.add_star(OrdStar(309.90f, 15.91f, 3.77f, OrdStar::SPEC_B, "Sualocin"));
   delphinus.add_star(OrdStar(308.30f, 11.30f, 4.03f, OrdStar::SPEC_B, "Deneb Dulfim"));
   delphinus.add_star(OrdStar(311.67f, 16.12f, 4.27f, OrdStar::SPEC_K, ""));
   delphinus.add_star(OrdStar(310.86f, 15.07f, 4.43f, OrdStar::SPEC_A, ""));
   delphinus.add_link(0, 1);
   delphinus.add_link(0, 2);
   delphinus.add_link(0, 4);
   delphinus.add_link(1, 3);
   delphinus.add_link(3, 4);
   cons_list.push_back(delphinus);
   tot_stars += delphinus.total_stars();
   tot_links += delphinus.total_links();

   Constellation dorado("Dorado");
   dorado.addToDescription("The golden fish");
   dorado.add_star(OrdStar(68.50f, -55.05f, 3.30f, OrdStar::SPEC_A, ""));
   dorado.add_star(OrdStar(83.40f, -62.49f, 3.76f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(64.00f, -51.49f, 4.26f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(86.19f, -65.73f, 4.34f, OrdStar::SPEC_A, ""));
   dorado.add_star(OrdStar(88.50f, -63.09f, 4.65f, OrdStar::SPEC_K, ""));
   dorado.add_star(OrdStar(76.37f, -57.47f, 4.71f, OrdStar::SPEC_F, ""));
   dorado.add_link(0, 1);
   dorado.add_link(0, 2);
   dorado.add_link(0, 5);
   dorado.add_link(1, 3);
   dorado.add_link(1, 4);
   dorado.add_link(1, 5);
   dorado.add_link(3, 4);
   cons_list.push_back(dorado);
   tot_stars += dorado.total_stars();
   tot_links += dorado.total_links();

   Constellation draco("Draco");
   draco.addToDescription("The dragon");
   draco.add_star(OrdStar(269.15f, 51.49f, 2.24f, OrdStar::SPEC_K, "Eltanin"));
   draco.add_star(OrdStar(246.00f, 61.51f, 2.73f, OrdStar::SPEC_G, ""));
   draco.add_star(OrdStar(262.60f, 52.30f, 2.79f, OrdStar::SPEC_G, "Rastaban"));
   draco.add_star(OrdStar(288.14f, 67.66f, 3.07f, OrdStar::SPEC_G, "Altais"));
   draco.add_star(OrdStar(257.20f, 65.71f, 3.17f, OrdStar::SPEC_B, "Aldhibah"));
   draco.add_star(OrdStar(231.25f, 58.97f, 3.29f, OrdStar::SPEC_K, "Edasich"));
   draco.add_star(OrdStar(275.25f, 72.73f, 3.55f, OrdStar::SPEC_F, "Batentaban Borealis"));
   draco.add_star(OrdStar(211.10f, 64.38f, 3.67f, OrdStar::SPEC_A, "Thuban"));
   draco.add_star(OrdStar(268.37f, 56.87f, 3.73f, OrdStar::SPEC_K, "Grumium"));
   draco.add_star(OrdStar(172.85f, 69.33f, 3.82f, OrdStar::SPEC_M, "Giausar"));
   draco.add_star(OrdStar(297.05f, 70.27f, 3.84f, OrdStar::SPEC_G, "Tyl"));
   draco.add_star(OrdStar(188.37f, 69.79f, 3.85f, OrdStar::SPEC_B, "Ketu"));
   draco.add_star(OrdStar(240.50f, 58.56f, 4.01f, OrdStar::SPEC_F, ""));
   draco.add_star(OrdStar(275.19f, 71.34f, 4.89f, OrdStar::SPEC_A, "Batentaban Australis"));
   draco.add_star(OrdStar(263.06f, 55.17f, 4.89f, OrdStar::SPEC_A, "Kuma"));
   draco.add_link(0, 2);
   draco.add_link(0, 8);
   draco.add_link(1, 4);
   draco.add_link(1, 12);
   draco.add_link(2, 14);
   draco.add_link(3, 8);
   draco.add_link(3, 10);
   draco.add_link(3, 13);
   draco.add_link(4, 13);
   draco.add_link(5, 7);
   draco.add_link(5, 12);
   draco.add_link(6, 13);
   draco.add_link(7, 11);
   draco.add_link(8, 14);
   draco.add_link(9, 11);
   cons_list.push_back(draco);
   tot_stars += draco.total_stars();
   tot_links += draco.total_links();

   Constellation equuleus("Equuleus");
   equuleus.addToDescription("The pony");
   equuleus.add_star(OrdStar(318.95f, 5.25f, 3.92f, OrdStar::SPEC_G, "Kitalpha"));
   equuleus.add_star(OrdStar(318.62f, 10.00f, 4.47f, OrdStar::SPEC_F, "Pherasauval"));
   equuleus.add_star(OrdStar(317.58f, 10.13f, 4.70f, OrdStar::SPEC_F, ""));
   equuleus.add_star(OrdStar(320.71f, 6.81f, 5.16f, OrdStar::SPEC_A, ""));
   equuleus.add_star(OrdStar(314.75f, 4.29f, 5.30f, OrdStar::SPEC_F, ""));
   equuleus.add_link(0, 1);
   equuleus.add_link(1, 2);
   cons_list.push_back(equuleus);
   tot_stars += equuleus.total_stars();
   tot_links += equuleus.total_links();

   Constellation eradanus("Eradanus");
   eradanus.addToDescription("The river");
   eradanus.add_star(OrdStar(24.42f, -57.24f, 0.45f, OrdStar::SPEC_B, "Achernar"));
   eradanus.add_star(OrdStar(76.96f, -5.09f, 2.78f, OrdStar::SPEC_A, "Cursa"));
   eradanus.add_star(OrdStar(44.56f, -40.31f, 2.88f, OrdStar::SPEC_A, "Acamar"));
   eradanus.add_star(OrdStar(59.50f, -13.51f, 2.97f, OrdStar::SPEC_M, "Zaurak"));
   eradanus.add_star(OrdStar(55.81f, -9.77f, 3.52f, OrdStar::SPEC_K, "Rana"));
   eradanus.add_star(OrdStar(64.47f, -33.80f, 3.55f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(34.13f, -51.52f, 3.56f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(28.99f, -51.61f, 3.69f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(49.88f, -21.78f, 3.70f, OrdStar::SPEC_M, ""));
   eradanus.add_star(OrdStar(53.22f, -9.46f, 3.72f, OrdStar::SPEC_K, "Sadira"));
   eradanus.add_star(OrdStar(68.89f, -30.56f, 3.81f, OrdStar::SPEC_G, "Beemin"));
   eradanus.add_star(OrdStar(44.10f, -8.90f, 3.89f, OrdStar::SPEC_K, "Azha"));
   eradanus.add_star(OrdStar(69.08f, -3.35f, 3.93f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(66.01f, -34.02f, 3.97f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(71.37f, -3.25f, 4.01f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(62.96f, -6.83f, 4.04f, OrdStar::SPEC_F, "Beid"));
   eradanus.add_star(OrdStar(45.60f, -23.62f, 4.08f, OrdStar::SPEC_A, ""));
   eradanus.add_star(OrdStar(40.16f, -39.85f, 4.11f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(57.36f, -36.20f, 4.17f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(56.70f, -23.25f, 4.22f, OrdStar::SPEC_F, ""));
   eradanus.add_star(OrdStar(36.75f, -47.70f, 4.24f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(49.97f, -43.07f, 4.26f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(53.45f, -21.02f, 4.26f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(41.27f, -18.57f, 4.47f, OrdStar::SPEC_F, ""));
   eradanus.add_link(0, 7);
   eradanus.add_link(1, 14);
   eradanus.add_link(2, 17);
   eradanus.add_link(2, 21);
   eradanus.add_link(3, 4);
   eradanus.add_link(3, 15);
   eradanus.add_link(4, 9);
   eradanus.add_link(5, 13);
   eradanus.add_link(5, 18);
   eradanus.add_link(6, 7);
   eradanus.add_link(6, 20);
   eradanus.add_link(8, 16);
   eradanus.add_link(8, 22);
   eradanus.add_link(9, 11);
   eradanus.add_link(10, 13);
   eradanus.add_link(10, 19);
   eradanus.add_link(11, 23);
   eradanus.add_link(12, 14);
   eradanus.add_link(12, 15);
   eradanus.add_link(16, 23);
   eradanus.add_link(17, 20);
   eradanus.add_link(18, 21);
   eradanus.add_link(19, 22);
   cons_list.push_back(eradanus);
   tot_stars += eradanus.total_stars();
   tot_links += eradanus.total_links();

   Constellation fornax("Fornax");
   fornax.addToDescription("The furnace");
   fornax.add_star(OrdStar(48.02f, -28.99f, 3.80f, OrdStar::SPEC_F, "Dalim"));
   fornax.add_star(OrdStar(42.27f, -32.41f, 4.45f, OrdStar::SPEC_G, ""));
   fornax.add_star(OrdStar(31.13f, -29.29f, 4.68f, OrdStar::SPEC_B, ""));
   fornax.add_link(0, 1);
   fornax.add_link(1, 2);
   cons_list.push_back(fornax);
   tot_stars += fornax.total_stars();
   tot_links += fornax.total_links();

   Constellation gemini("Gemini");
   gemini.addToDescription("The twins");
   gemini.add_star(OrdStar(116.33f, 28.03f, 1.16f, OrdStar::SPEC_K, "Pollux"));
   gemini.add_star(OrdStar(113.65f, 31.89f, 1.58f, OrdStar::SPEC_A, "Castor"));
   gemini.add_star(OrdStar(99.42f, 16.40f, 1.93f, OrdStar::SPEC_A, "Alhena"));
   gemini.add_star(OrdStar(95.72f, 22.51f, 2.87f, OrdStar::SPEC_M, "Tejat"));
   gemini.add_star(OrdStar(100.97f, 25.13f, 3.06f, OrdStar::SPEC_A, "Mebsuta"));
   gemini.add_star(OrdStar(93.71f, 22.51f, 3.31f, OrdStar::SPEC_M, "Propus"));
   gemini.add_star(OrdStar(101.33f, 12.90f, 3.35f, OrdStar::SPEC_F, "Alzir"));
   gemini.add_star(OrdStar(110.03f, 21.98f, 3.50f, OrdStar::SPEC_F, "Wasat"));
   gemini.add_star(OrdStar(116.10f, 24.39f, 3.57f, OrdStar::SPEC_G, ""));
   gemini.add_star(OrdStar(109.52f, 16.54f, 3.58f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(103.02f, 33.96, 3.60f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(111.42f, 27.80f, 3.78f, OrdStar::SPEC_G, ""));
   gemini.add_star(OrdStar(106.02f, 20.57f, 4.01f, OrdStar::SPEC_G, "Mekbuda"));
   gemini.add_star(OrdStar(113.97f, 26.89f, 4.06f, OrdStar::SPEC_K, ""));
   gemini.add_star(OrdStar(97.24f, 20.21f, 4.13f, OrdStar::SPEC_B, ""));
   gemini.add_star(OrdStar(91.02f, 23.26f, 4.16f, OrdStar::SPEC_G, "Propus"));
   gemini.add_star(OrdStar(107.78f, 30.25f, 4.41f, OrdStar::SPEC_K, "P"));
   gemini.add_link(0, 13);
   gemini.add_link(1, 16);
   gemini.add_link(2, 12);
   gemini.add_link(3, 4);
   gemini.add_link(3, 5);
   gemini.add_link(4, 14);
   gemini.add_link(4, 16);
   gemini.add_link(5, 15);
   gemini.add_link(6, 9);
   gemini.add_link(7, 9);
   gemini.add_link(7, 12);
   gemini.add_link(7, 13);
   gemini.add_link(8, 13);
   gemini.add_link(10, 16);
   gemini.add_link(11, 13);
   gemini.add_link(11, 16);
   cons_list.push_back(gemini);
   tot_stars += gemini.total_stars();
   tot_links += gemini.total_links();

   Constellation grus("Grus");
   grus.addToDescription("The crane");
   grus.add_star(OrdStar(332.05f, -46.96f, 1.73f, OrdStar::SPEC_B, "Alnair"));
   grus.add_star(OrdStar(340.66f, -46.88f, 2.07f, OrdStar::SPEC_M, "Gruid"));
   grus.add_star(OrdStar(328.47f, -37.37f, 3.00f, OrdStar::SPEC_B, "Al Dhanab"));
   grus.add_star(OrdStar(342.14f, -51.31f, 3.49f, OrdStar::SPEC_A, ""));
   grus.add_star(OrdStar(337.31f, -43.50f, 3.97f, OrdStar::SPEC_G, ""));
   grus.add_star(OrdStar(345.25f, -52.75f, 4.11f, OrdStar::SPEC_G, ""));
   grus.add_star(OrdStar(331.52f, -39.54f, 4.47f, OrdStar::SPEC_M, ""));
   grus.add_star(OrdStar(333.90f, -41.34f, 4.79f, OrdStar::SPEC_G, ""));
   grus.add_link(0, 1);
   grus.add_link(0, 4);
   grus.add_link(1, 3);
   grus.add_link(1, 4);
   grus.add_link(2, 6);
   grus.add_link(3, 5);
   grus.add_link(4, 7);
   grus.add_link(6, 7);
   cons_list.push_back(grus);
   tot_stars += grus.total_stars();
   tot_links += grus.total_links();

   Constellation hercules("Hercules");
   hercules.addToDescription("The demigod son of Zeus");
   hercules.add_star(OrdStar(247.55f, 21.49f, 2.78f, OrdStar::SPEC_G, "Kornephoros"));
   hercules.add_star(OrdStar(250.32f, 31.60f, 2.81f, OrdStar::SPEC_F, "Rutilicus"));
   hercules.add_star(OrdStar(258.75f, 24.84f, 3.12f, OrdStar::SPEC_A, "Sarin"));
   hercules.add_star(OrdStar(258.75f, 36.80f, 3.16f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(258.66f, 14.39f, 3.31f, OrdStar::SPEC_M, "Ras Algethi"));
   hercules.add_star(OrdStar(266.61f, 27.72f, 3.42f, OrdStar::SPEC_G, "Marfak Al Jathih Al Aisr"));
   hercules.add_star(OrdStar(250.72f, 38.92f, 3.48f, OrdStar::SPEC_G, ""));
   hercules.add_star(OrdStar(269.44f, 29.25f, 3.70f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(245.48f, 19.15f, 3.74f, OrdStar::SPEC_A, "Hejian"));
   hercules.add_star(OrdStar(264.86f, 46.01f, 3.82f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(271.88f, 28.76f, 3.84f, OrdStar::SPEC_B, "Fekhiz al Jathih al Aisr"));
   hercules.add_star(OrdStar(269.06f, 37.25f, 3.86f, OrdStar::SPEC_K, "Rukbalgethi Genubi"));
   hercules.add_star(OrdStar(244.94f, 46.31f, 3.91f, OrdStar::SPEC_B, "Sinistrum Ingeniculi"));
   hercules.add_star(OrdStar(255.07f, 30.93f, 3.92f, OrdStar::SPEC_A, "Cujam"));
   hercules.add_star(OrdStar(260.93f, 37.15f, 4.15f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(248.52f, 42.44f, 4.20f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(242.19f, 44.94f, 4.23f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(262.69f, 26.11f, 4.41f, OrdStar::SPEC_K, "Maasym"));
   hercules.add_star(OrdStar(246.35f, 14.03f, 4.57f, OrdStar::SPEC_B, "Culam"));
   hercules.add_star(OrdStar(238.17f, 42.45f, 4.60f, OrdStar::SPEC_F, ""));
   hercules.add_star(OrdStar(248.15f, 11.49f, 4.84f, OrdStar::SPEC_K, ""));
   hercules.add_link(0, 1);
   hercules.add_link(0, 4);
   hercules.add_link(0, 8);
   hercules.add_link(1, 6);
   hercules.add_link(1, 13);
   hercules.add_link(2, 4);
   hercules.add_link(2, 13);
   hercules.add_link(2, 17);
   hercules.add_link(3, 6);
   hercules.add_link(3, 13);
   hercules.add_link(3, 14);
   hercules.add_link(5, 7);
   hercules.add_link(5, 17);
   hercules.add_link(6, 15);
   hercules.add_link(7, 10);
   hercules.add_link(8, 18);
   hercules.add_link(9, 11);
   hercules.add_link(11, 14);
   hercules.add_link(12, 15);
   hercules.add_link(12, 16);
   hercules.add_link(16, 19);
   hercules.add_link(18, 20);
   cons_list.push_back(hercules);
   tot_stars += hercules.total_stars();
   tot_links += hercules.total_links();

   Constellation horologium("Horologium");
   horologium.addToDescription("The pendulum clock");
   horologium.add_star(OrdStar(63.50f, -42.29f, 3.85f, OrdStar::SPEC_K, ""));
   horologium.add_star(OrdStar(44.69f, -64.07f, 4.98f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(45.90f, -59.74f, 5.12f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(40.16f, -54.55f, 5.21f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(39.35f, -52.54f, 5.30f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(40.64f, -50.80f, 5.40f, OrdStar::SPEC_G, ""));
   horologium.add_link(0, 5);
   horologium.add_link(1, 2);
   horologium.add_link(2, 3);
   horologium.add_link(3, 4);
   horologium.add_link(4, 5);
   cons_list.push_back(horologium);
   tot_stars += horologium.total_stars();
   tot_links += horologium.total_links();

   Constellation hydra("Hydra");
   hydra.addToDescription("The water snake");
   hydra.add_star(OrdStar(141.90f, -8.66f, 1.99f, OrdStar::SPEC_K, "Alphard"));
   hydra.add_star(OrdStar(199.73f, -23.17f, 2.99f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(133.85f, 5.95f, 3.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(162.40f, -16.19f, 3.11f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(211.59f, -26.68f, 3.25f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(131.69f, 6.42f, 3.38f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(173.25f, -31.86f, 3.54f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(152.65f, -12.35f, 3.61f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(156.525f, -16.84f, 3.83f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(138.59f, 2.32f, 3.89f, OrdStar::SPEC_B, ""));
   hydra.add_star(OrdStar(144.96f, -1.14f, 3.90f, OrdStar::SPEC_K, "Ukdah"));
   hydra.add_star(OrdStar(147.86f, -14.85f, 4.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(129.41f, 5.70f, 4.11f, OrdStar::SPEC_A, "Lisan al Shudja"));
   hydra.add_star(OrdStar(178.22f, -33.91f, 4.29f, OrdStar::SPEC_A, ""));
   hydra.add_star(OrdStar(130.80f, 3.40f, 4.30f, OrdStar::SPEC_B, ""));
   hydra.add_star(OrdStar(222.57f, -27.96f, 4.42f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(129.69f, 3.34f, 4.45f, OrdStar::SPEC_K, "Minhar al Shija"));
   // These next two are from Crater, the "Cup".
   hydra.add_star(OrdStar(164.94f, -18.30f, 4.08f, OrdStar::SPEC_K, "Alkes"));
   hydra.add_star(OrdStar(167.91f, -22.83f, 4.46f, OrdStar::SPEC_A, "Al Sharasif"));
   hydra.add_link(0, 10);
   hydra.add_link(0, 11);
   hydra.add_link(1, 4);
   hydra.add_link(1, 13);
   hydra.add_link(2, 5);
   hydra.add_link(2, 9);
   hydra.add_link(2, 14);
   hydra.add_link(3, 8);
   hydra.add_link(3, 17);
   hydra.add_link(4, 15);
   hydra.add_link(5, 12);
   hydra.add_link(6, 13);
   hydra.add_link(6, 18);
   hydra.add_link(7, 8);
   hydra.add_link(7, 11);
   hydra.add_link(9, 10);
   hydra.add_link(12, 16);
   hydra.add_link(14, 16);
   hydra.add_link(17, 18);
   cons_list.push_back(hydra);
   tot_stars += hydra.total_stars();
   tot_links += hydra.total_links();

   Constellation hydrus("Hydrus");
   hydrus.addToDescription("The lesser water snake");
   hydrus.add_star(OrdStar(6.41f, -77.26f, 2.82f, OrdStar::SPEC_G, ""));
   hydrus.add_star(OrdStar(29.69f, -61.57f, 2.86f, OrdStar::SPEC_F, ""));
   hydrus.add_star(OrdStar(56.81f, -74.24f, 3.26f, OrdStar::SPEC_M, "Foo Pih"));
   hydrus.add_star(OrdStar(35.44f, -68.66f, 4.08f, OrdStar::SPEC_A, ""));
   hydrus.add_star(OrdStar(39.90f, -68.27f, 4.12f, OrdStar::SPEC_B, ""));
   hydrus.add_star(OrdStar(28.90f, -67.65f, 4.68f, OrdStar::SPEC_G, ""));
   hydrus.add_star(OrdStar(42.62f, -75.06f, 4.76f, OrdStar::SPEC_K, ""));
   hydrus.add_star(OrdStar(41.39f, -67.62f, 4.83f, OrdStar::SPEC_A, ""));
   hydrus.add_star(OrdStar(12.15f, -74.92f, 5.09f, OrdStar::SPEC_K, ""));
   hydrus.add_star(OrdStar(45.56f, -71.90f, 5.51f, OrdStar::SPEC_B, ""));
   hydrus.add_star(OrdStar(33.62f, -67.84f, 5.57f, OrdStar::SPEC_M, ""));
   hydrus.add_link(0, 6);
   hydrus.add_link(0, 8);
   hydrus.add_link(1, 5);
   hydrus.add_link(2, 6);
   hydrus.add_link(2, 9);
   hydrus.add_link(3, 4);
   hydrus.add_link(3, 10);
   hydrus.add_link(4, 7);
   hydrus.add_link(5, 10);
   hydrus.add_link(7, 9);
   cons_list.push_back(hydrus);
   tot_stars += hydrus.total_stars();
   tot_links += hydrus.total_links();

   Constellation indus("Indus");
   indus.addToDescription("The native American");
   indus.add_star(OrdStar(309.39f, -47.29f, 3.11f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(313.70f, -58.45f, 3.67f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(319.96f, -53.45f, 4.39f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(329.48f, -54.99f, 4.40f, OrdStar::SPEC_F, ""));
   indus.add_star(OrdStar(311.01f, -51.92f, 4.51f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(330.83f, -56.78f, 4.69f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(312.38f, -46.22f, 4.90f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(312.87f, -51.61f, 5.06f, OrdStar::SPEC_K, ""));
   indus.add_link(0, 4);
   indus.add_link(0, 6);
   indus.add_link(0, 7);
   indus.add_link(1, 3);
   indus.add_link(1, 4);
   indus.add_link(2, 3);
   indus.add_link(2, 7);
   indus.add_link(3, 5);
   indus.add_link(4, 7);
   cons_list.push_back(indus);
   tot_stars += indus.total_stars();
   tot_links += indus.total_links();

   Constellation lacerta("Lacerta");
   lacerta.addToDescription("The lizard");
   lacerta.add_star(OrdStar(337.75f, 50.27f, 3.76f, OrdStar::SPEC_A, ""));
   lacerta.add_star(OrdStar(334.00f, 37.73f, 4.14f, OrdStar::SPEC_K, ""));
   lacerta.add_star(OrdStar(337.25f, 47.70f, 4.34f, OrdStar::SPEC_M, ""));
   lacerta.add_star(OrdStar(335.75f, 52.23f, 4.42f, OrdStar::SPEC_G, ""));
   lacerta.add_star(OrdStar(333.46f, 39.72f, 4.50f, OrdStar::SPEC_K, ""));
   lacerta.add_star(OrdStar(340.00f, 44.27f, 4.50f, OrdStar::SPEC_K, ""));
   lacerta.add_star(OrdStar(337.50f, 43.12f, 4.52f, OrdStar::SPEC_B, ""));
   lacerta.add_star(OrdStar(335.25f, 46.53f, 4.55f, OrdStar::SPEC_B, ""));
   lacerta.add_star(OrdStar(336.12f, 49.47f, 4.55f, OrdStar::SPEC_B, ""));
   lacerta.add_link(0, 3);
   lacerta.add_link(0, 2);
   lacerta.add_link(1, 4);
   lacerta.add_link(2, 5);
   lacerta.add_link(2, 7);
   lacerta.add_link(2, 8);
   lacerta.add_link(3, 8);
   lacerta.add_link(4, 6);
   lacerta.add_link(5, 6);
   lacerta.add_link(6, 7);
   cons_list.push_back(lacerta);
   tot_stars += lacerta.total_stars();
   tot_links += lacerta.total_links();

   Constellation leo("Leo");
   leo.addToDescription("The lion");
   leo.add_star(OrdStar(152.00f, 11.97f, 1.36f, OrdStar::SPEC_B, "Regulus"));
   leo.add_star(OrdStar(154.75f, 19.83f, 2.01f, OrdStar::SPEC_K, "Algieba"));
   leo.add_star(OrdStar(177.26f, 14.57f, 2.14f, OrdStar::SPEC_A, "Denebola"));
   leo.add_star(OrdStar(168.50f, 20.52f, 2.56f, OrdStar::SPEC_A, "Zosma"));
   leo.add_star(OrdStar(146.46f, 23.77f, 2.97f, OrdStar::SPEC_G, "Algenubi"));
   leo.add_star(OrdStar(168.55f, 15.43f, 3.33f, OrdStar::SPEC_A, "Chertan"));
   leo.add_star(OrdStar(154.17f, 23.42f, 3.43f, OrdStar::SPEC_F, "Adhafera"));
   leo.add_star(OrdStar(151.82f, 16.77f, 3.48f, OrdStar::SPEC_A, "Al Jabhah"));
   leo.add_star(OrdStar(148.19f, 26.00f, 3.88f, OrdStar::SPEC_K, "Rasalas"));
   leo.add_star(OrdStar(170.97f, 10.53f, 4.00f, OrdStar::SPEC_F, "Tsze Tseang"));
   leo.add_star(OrdStar(170.34f, 6.03f, 4.05f, OrdStar::SPEC_B, "Shishimai"));
   leo.add_star(OrdStar(142.92f, 22.97f, 4.32f, OrdStar::SPEC_K, "ALterf"));
   leo.add_star(OrdStar(141.16f, 26.18f, 4.47f, OrdStar::SPEC_K, "Al Minliar"));
   leo.add_link(0, 7);
   leo.add_link(1, 3);
   leo.add_link(1, 6);
   leo.add_link(1, 7);
   leo.add_link(2, 3);
   leo.add_link(2, 5);
   leo.add_link(3, 5);
   leo.add_link(4, 7);
   leo.add_link(4, 8);
   leo.add_link(4, 11);
   leo.add_link(5, 7);
   leo.add_link(5, 9);
   leo.add_link(6, 8);
   leo.add_link(8, 12);
   leo.add_link(9, 10);
   leo.add_link(11, 12);
   cons_list.push_back(leo);
   tot_stars += leo.total_stars();
   tot_links += leo.total_links();

   Constellation leo_minor("Leo Minor");
   leo_minor.addToDescription("The lesser lion");
   leo_minor.add_star(OrdStar(163.32f, 34.22f, 3.79f, OrdStar::SPEC_K, "Praecipua"));
   leo_minor.add_star(OrdStar(156.98f, 36.70f, 4.20f, OrdStar::SPEC_G, ""));
   leo_minor.add_star(OrdStar(151.85f, 35.25f, 4.49f, OrdStar::SPEC_A, ""));
   leo_minor.add_star(OrdStar(143.55f, 36.40f, 4.54f, OrdStar::SPEC_G, ""));
   leo_minor.add_star(OrdStar(156.47f, 33.79f, 4.68f, OrdStar::SPEC_F, ""));
   leo_minor.add_link(0, 1);
   leo_minor.add_link(0, 4);
   leo_minor.add_link(1, 2);
   leo_minor.add_link(2, 3);
   leo_minor.add_link(2, 4);
   cons_list.push_back(leo_minor);
   tot_stars += leo_minor.total_stars();
   tot_links += leo_minor.total_links();

   Constellation lepus("Lepus");
   lepus.addToDescription("The hare");
   lepus.add_star(OrdStar(83.25f, -17.82f, 2.58f, OrdStar::SPEC_F, "Arneb"));
   lepus.add_star(OrdStar(82.00f, -20.75f, 2.81f, OrdStar::SPEC_G, "Nihal"));
   lepus.add_star(OrdStar(76.25f, -22.37f, 3.19f, OrdStar::SPEC_K, ""));
   lepus.add_star(OrdStar(78.25f, -16.22f, 3.29f, OrdStar::SPEC_B, ""));
   lepus.add_star(OrdStar(86.75f, -14.82f, 3.55f, OrdStar::SPEC_A, ""));
   lepus.add_star(OrdStar(86.00f, -22.45f, 3.59f, OrdStar::SPEC_F, ""));
   lepus.add_star(OrdStar(89.00f, -14.17f, 3.71f, OrdStar::SPEC_F, ""));
   lepus.add_star(OrdStar(87.75f, -20.87f, 3.76f, OrdStar::SPEC_G, ""));
   lepus.add_star(OrdStar(80.00f, -13.18f, 4.29f, OrdStar::SPEC_B, ""));
   lepus.add_star(OrdStar(78.25f, -12.93f, 4.36f, OrdStar::SPEC_B, ""));
   lepus.add_star(OrdStar(91.50f, -14.93f, 4.67f, OrdStar::SPEC_A, ""));
   lepus.add_link(0, 1);
   lepus.add_link(0, 3);
   lepus.add_link(0, 4);
   lepus.add_link(1, 2);
   lepus.add_link(1, 5);
   lepus.add_link(2, 3);
   lepus.add_link(3, 8);
   lepus.add_link(3, 9);
   lepus.add_link(4, 6);
   lepus.add_link(5, 7);
   lepus.add_link(6, 10);
   lepus.add_link(7, 10);
   cons_list.push_back(lepus);
   tot_stars += lepus.total_stars();
   tot_links += lepus.total_links();

   Constellation libra("Libra");
   libra.addToDescription("The balance");
   libra.add_star(OrdStar(229.25f, -9.38f, 2.61f, OrdStar::SPEC_B, "Zubeneschamali"));
   libra.add_star(OrdStar(222.75f, -16.05f, 2.75f, OrdStar::SPEC_A, "Zubenelgenubi"));
   libra.add_star(OrdStar(226.00f, -25.28f, 3.25f, OrdStar::SPEC_M, "Brachium"));
   libra.add_star(OrdStar(234.25f, -28.13f, 3.60f, OrdStar::SPEC_K, ""));
   libra.add_star(OrdStar(234.75f, -29.78f, 3.66f, OrdStar::SPEC_B, ""));
   libra.add_star(OrdStar(234.00f, -14.78f, 3.91f, OrdStar::SPEC_K, "Zubenelakrab"));
   libra.add_link(0, 1);
   libra.add_link(0, 5);
   libra.add_link(1, 2);
   libra.add_link(1, 5);
   libra.add_link(3, 4);
   libra.add_link(3, 5);
   cons_list.push_back(libra);
   tot_stars += libra.total_stars();
   tot_links += libra.total_links();

   Constellation lupus("Lupus");
   lupus.addToDescription("The wolf");
   lupus.add_star(OrdStar(220.50f, -47.38f, 2.30f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(224.50f, -43.13f, 2.68f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(233.75f, -41.17f, 2.80f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(230.25f, -40.65f, 3.22f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(230.75f, -44.68f, 3.37f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(228.00f, -52.10f, 3.41f, OrdStar::SPEC_G, ""));
   lupus.add_star(OrdStar(240.00f, -38.40f, 3.42f, OrdStar::SPEC_B, ""));
   lupus.add_star(OrdStar(230.50f, -36.27f, 3.57f, OrdStar::SPEC_K, ""));
   lupus.add_star(OrdStar(237.75f, -33.63f, 3.97f, OrdStar::SPEC_B, ""));
   lupus.add_link(0, 5);
   lupus.add_link(1, 3);
   lupus.add_link(2, 3);
   lupus.add_link(2, 4);
   lupus.add_link(2, 6);
   lupus.add_link(4, 5);
   lupus.add_link(5, 6);
   lupus.add_link(6, 7);
   lupus.add_link(6, 8);
   lupus.add_link(7, 8);
   cons_list.push_back(lupus);
   tot_stars += lupus.total_stars();
   tot_links += lupus.total_links();

   Constellation lynx("Lynx");
   lynx.addToDescription("The lynx");
   lynx.add_star(OrdStar(140.25f, 34.38f, 3.14f, OrdStar::SPEC_K, "Elvashak"));
   lynx.add_star(OrdStar(139.75f, 36.80f, 3.82f, OrdStar::SPEC_A, ""));
   lynx.add_star(OrdStar(135.25f, 41.78f, 3.96f, OrdStar::SPEC_F, ""));
   lynx.add_star(OrdStar(125.75f, 43.18f, 4.25f, OrdStar::SPEC_K, ""));
   lynx.add_star(OrdStar(104.25f, 58.42f, 4.35f, OrdStar::SPEC_G, ""));
   lynx.add_star(OrdStar(95.00f, 59.02f, 4.44f, OrdStar::SPEC_A, ""));
   lynx.add_star(OrdStar(136.75f, 38.45f, 4.56f, OrdStar::SPEC_B, ""));
   lynx.add_star(OrdStar(111.75f, 49.22f, 4.61f, OrdStar::SPEC_A, ""));
   lynx.add_link(0, 1);
   lynx.add_link(1, 6);
   lynx.add_link(2, 3);
   lynx.add_link(2, 6);
   lynx.add_link(3, 7);
   lynx.add_link(4, 5);
   lynx.add_link(4, 7);
   cons_list.push_back(lynx);
   tot_stars += lynx.total_stars();
   tot_links += lynx.total_links();

   Constellation lyra("Lyra");
   lyra.addToDescription("The harp");
   lyra.add_star(OrdStar(279.25f, 38.78f, 0.03f, OrdStar::SPEC_A, "Vega"));
   lyra.add_star(OrdStar(284.75f, 32.68f, 3.25f, OrdStar::SPEC_B, "Sulafat"));
   lyra.add_star(OrdStar(282.50f, 33.37f, 3.52f, OrdStar::SPEC_A, "Shelik"));
   lyra.add_star(OrdStar(283.75f, 36.90f, 4.22f, OrdStar::SPEC_M, ""));
   lyra.add_star(OrdStar(281.25f, 37.60f, 4.34f, OrdStar::SPEC_A, ""));
   lyra.add_star(OrdStar(281.00f, 39.62f, 4.59f, OrdStar::SPEC_A, ""));
   lyra.add_link(0, 4);
   lyra.add_link(0, 5);
   lyra.add_link(1, 2);
   lyra.add_link(1, 3);
   lyra.add_link(2, 4);
   lyra.add_link(3, 4);
   lyra.add_link(4, 5);
   cons_list.push_back(lyra);
   tot_stars += lyra.total_stars();
   tot_links += lyra.total_links();

   Constellation mensa("Mensa");
   mensa.addToDescription("The table mountain");
   mensa.add_star(OrdStar(92.50f, -74.75f, 5.08f, OrdStar::SPEC_G, ""));
   mensa.add_star(OrdStar(83.00f, -76.35f, 5.18f, OrdStar::SPEC_K, ""));
   mensa.add_star(OrdStar(75.75f, -71.32f, 5.30f, OrdStar::SPEC_G, ""));
   mensa.add_star(OrdStar(104.15f, -79.42f, 5.45f, OrdStar::SPEC_B, ""));
   mensa.add_star(OrdStar(111.50f, -79.10f, 5.54f, OrdStar::SPEC_K, ""));
   mensa.add_star(OrdStar(100.00f, -80.82f, 5.61f, OrdStar::SPEC_A, ""));
   mensa.add_star(OrdStar(84.25f, -80.47f, 5.65f, OrdStar::SPEC_G, ""));
   mensa.add_star(OrdStar(64.50f, -80.22f, 5.67f, OrdStar::SPEC_K, ""));
   mensa.add_star(OrdStar(74.75f, -82.47f, 5.84f, OrdStar::SPEC_G, ""));
   mensa.add_link(0, 2);
   mensa.add_link(0, 4);
   mensa.add_link(1, 2);
   mensa.add_link(1, 7);
   mensa.add_link(3, 4);
   mensa.add_link(3, 5);
   mensa.add_link(5, 6);
   mensa.add_link(6, 8);
   mensa.add_link(7, 8);
   cons_list.push_back(mensa);
   tot_stars += mensa.total_stars();
   tot_links += mensa.total_links();

   Constellation microscopium("Microscopium");
   microscopium.addToDescription("The microscope");
   microscopium.add_star(OrdStar(315.25f, -32.25f, 4.67f, OrdStar::SPEC_G, ""));
   microscopium.add_star(OrdStar(319.50f, -32.17f, 4.71f, OrdStar::SPEC_A, ""));
   microscopium.add_star(OrdStar(320.25f, -40.82f, 4.80f, OrdStar::SPEC_A, ""));
   microscopium.add_star(OrdStar(312.50f, -33.78f, 4.89f, OrdStar::SPEC_G, ""));
   microscopium.add_star(OrdStar(312.00f, -43.98f, 5.11f, OrdStar::SPEC_F, ""));
   microscopium.add_star(OrdStar(308.50f, -44.51f, 5.12f, OrdStar::SPEC_K, ""));
   microscopium.add_star(OrdStar(316.50f, -32.85f, 5.20f, OrdStar::SPEC_K, ""));
   microscopium.add_star(OrdStar(318.25f, -39.43f, 5.25f, OrdStar::SPEC_F, ""));
   microscopium.add_star(OrdStar(315.75f, -38.63f, 5.32f, OrdStar::SPEC_F, ""));
   microscopium.add_star(OrdStar(316.50f, -41.38f, 5.55f, OrdStar::SPEC_K, ""));
   microscopium.add_link(0, 3);
   microscopium.add_link(0, 6);
   microscopium.add_link(1, 6);
   microscopium.add_link(1, 7);
   microscopium.add_link(2, 7);
   microscopium.add_link(3, 8);
   microscopium.add_link(4, 5);
   microscopium.add_link(4, 8);
   microscopium.add_link(4, 9);
   microscopium.add_link(7, 9);
   cons_list.push_back(microscopium);
   tot_stars += microscopium.total_stars();
   tot_links += microscopium.total_links();

   Constellation monoceros("Monoceros");
   monoceros.addToDescription("The unicorn");
   monoceros.add_star(OrdStar(115.25f, -9.55f, 3.94f, OrdStar::SPEC_K, ""));
   monoceros.add_star(OrdStar(93.75f, -6.28f, 3.99f, OrdStar::SPEC_K, ""));
   monoceros.add_star(OrdStar(108.00f, -0.50f, 4.15f, OrdStar::SPEC_A, ""));
   monoceros.add_star(OrdStar(122.25f, -2.98f, 4.36f, OrdStar::SPEC_G, ""));
   monoceros.add_star(OrdStar(96.00f, 4.60f, 4.39f, OrdStar::SPEC_A, ""));
   monoceros.add_star(OrdStar(98.25f, 7.33f, 4.47f, OrdStar::SPEC_A, ""));
   monoceros.add_star(OrdStar(102.00f, 2.42f, 4.48f, OrdStar::SPEC_K, ""));
   monoceros.add_star(OrdStar(97.25f, -7.03f, 4.60f, OrdStar::SPEC_B, ""));
   monoceros.add_star(OrdStar(100.25f, 9.90f, 4.66f, OrdStar::SPEC_O, ""));
   monoceros.add_link(0, 3);
   monoceros.add_link(1, 7);
   monoceros.add_link(2, 3);
   monoceros.add_link(2, 6);
   monoceros.add_link(2, 7);
   monoceros.add_link(4, 5);
   monoceros.add_link(4, 6);
   monoceros.add_link(5, 6);
   monoceros.add_link(5, 8);
   cons_list.push_back(monoceros);
   tot_stars += monoceros.total_stars();
   tot_links += monoceros.total_links();

   Constellation musca("Musca");
   musca.addToDescription("The fly");
   musca.add_star(OrdStar(189.25f, -69.13f, 2.69f, OrdStar::SPEC_B, ""));
   musca.add_star(OrdStar(191.50f, -68.10f, 3.04f, OrdStar::SPEC_B, ""));
   musca.add_star(OrdStar(195.75f, -71.55f, 3.61f, OrdStar::SPEC_K, ""));
   musca.add_star(OrdStar(176.50f, -66.73f, 3.68f, OrdStar::SPEC_A, ""));
   musca.add_star(OrdStar(188.00f, -72.13f, 3.84f, OrdStar::SPEC_B, ""));
   musca.add_star(OrdStar(184.50f, -67.97f, 4.06f, OrdStar::SPEC_M, ""));
   musca.add_link(0, 1);
   musca.add_link(0, 4);
   musca.add_link(0, 5);
   musca.add_link(1, 2);
   musca.add_link(2, 4);
   musca.add_link(3, 5);
   cons_list.push_back(musca);
   tot_stars += musca.total_stars();
   tot_links += musca.total_links();

   Constellation norma("Norma");
   norma.addToDescription("The carpenter's level");
   norma.add_star(OrdStar(245.00f, -50.15f, 4.01f, OrdStar::SPEC_G, ""));
   norma.add_star(OrdStar(246.75f, -47.55f, 4.46f, OrdStar::SPEC_B, ""));
   norma.add_star(OrdStar(240.80f, -49.23f, 4.65f, OrdStar::SPEC_G, ""));
   norma.add_star(OrdStar(241.50f, -45.17f, 4.73f, OrdStar::SPEC_A, ""));
   norma.add_link(0, 1);
   norma.add_link(0, 2);
   norma.add_link(1, 3);
   norma.add_link(2, 3);
   cons_list.push_back(norma);
   tot_stars += norma.total_stars();
   tot_links += norma.total_links();

   Constellation octans("Octans");
   octans.addToDescription("The octant");
   octans.add_star(OrdStar(325.25f, -77.38f, 3.73f, OrdStar::SPEC_K, ""));
   octans.add_star(OrdStar(341.50f, -81.38f, 4.13f, OrdStar::SPEC_A, ""));
   octans.add_star(OrdStar(216.75f, -83.67f, 4.31f, OrdStar::SPEC_K, ""));
   octans.add_link(0, 1);
   octans.add_link(0, 2);
   octans.add_link(1, 2);
   cons_list.push_back(octans);
   tot_stars += octans.total_stars();
   tot_links += octans.total_links();

   Constellation ophiuchus("Ophiuchus");
   ophiuchus.addToDescription("The serpent bearer");
   ophiuchus.add_star(OrdStar(263.75f, 12.57f, 2.08f, OrdStar::SPEC_A, "Rasalhague"));
   ophiuchus.add_star(OrdStar(257.50f, -15.73f, 2.43f, OrdStar::SPEC_A, "Sabik"));
   ophiuchus.add_star(OrdStar(249.25f, -10.57f, 2.54f, OrdStar::SPEC_O, "Han"));
   ophiuchus.add_star(OrdStar(243.58f, -3.69f, 2.73f, OrdStar::SPEC_M, "Yed Prior"));
   ophiuchus.add_star(OrdStar(265.75f, 4.57f, 2.76f, OrdStar::SPEC_K, "Cebalrai"));
   ophiuchus.add_star(OrdStar(254.50f, 9.38f, 3.19f, OrdStar::SPEC_K, ""));
   ophiuchus.add_star(OrdStar(244.58f, -4.690f, 3.23f, OrdStar::SPEC_G, "Yed Posterior"));
   ophiuchus.add_star(OrdStar(260.50f, -25.00f, 3.27f, OrdStar::SPEC_B, ""));
   ophiuchus.add_star(OrdStar(269.75f, -9.77f, 3.32f, OrdStar::SPEC_K, "Yan"));
   ophiuchus.add_star(OrdStar(267.00f, 2.70f, 3.75f, OrdStar::SPEC_A, "Tsung Ching"));
   ophiuchus.add_star(OrdStar(247.75f, 1.98f, 3.82f, OrdStar::SPEC_A, "Marfik"));
   ophiuchus.add_star(OrdStar(246.50f, -23.45f, 4.57f, OrdStar::SPEC_B, ""));
   ophiuchus.add_star(OrdStar(246.95f, -8.37f, 4.62f, OrdStar::SPEC_A, ""));
   ophiuchus.add_link(0, 4);
   ophiuchus.add_link(0, 5);
   ophiuchus.add_link(1, 2);
   ophiuchus.add_link(1, 4);
   ophiuchus.add_link(1, 7);
   ophiuchus.add_link(2, 5);
   ophiuchus.add_link(2, 11);
   ophiuchus.add_link(2, 12);
   ophiuchus.add_link(3, 6);
   ophiuchus.add_link(3, 10);
   ophiuchus.add_link(4, 9);
   ophiuchus.add_link(5, 10);
   ophiuchus.add_link(6, 12);
   ophiuchus.add_link(8, 9);
   cons_list.push_back(ophiuchus);
   tot_stars += ophiuchus.total_stars();
   tot_links += ophiuchus.total_links();

   Constellation orion("Orion");
   orion.addToDescription("The hunter");
   orion.add_star(OrdStar(78.75f, -8.00f, 0.18f, OrdStar::SPEC_B, "Rigel"));
   orion.add_star(OrdStar(88.80f, 7.41f, 0.58f, OrdStar::SPEC_M, "Betelgeuze"));
   orion.add_star(OrdStar(81.28f, 6.35f, 1.64f, OrdStar::SPEC_B, "Bellatrix"));
   orion.add_star(OrdStar(84.00f, -1.20f, 1.69f, OrdStar::SPEC_B, "Alnilam"));
   orion.add_star(OrdStar(85.25f, -1.95f, 1.74f, OrdStar::SPEC_O, "Alnitak"));
   orion.add_star(OrdStar(87.00f, -9.67f, 2.07f, OrdStar::SPEC_B, "Saiph"));
   orion.add_star(OrdStar(83.00f, -0.30f, 2.25f, OrdStar::SPEC_O, "Mintaka"));
   orion.add_star(OrdStar(72.50f, 6.97f, 3.19f, OrdStar::SPEC_F, "Tabit"));
   orion.add_star(OrdStar(81.25f, -2.40f, 3.35f, OrdStar::SPEC_B, "Algiebba"));
   orion.add_star(OrdStar(83.75f, 9.93f, 3.39f, OrdStar::SPEC_O, "Meissa"));
   orion.add_star(OrdStar(72.75f, 5.60f, 3.68f, OrdStar::SPEC_B, ""));
   orion.add_star(OrdStar(73.50f, 2.43f, 3.71f, OrdStar::SPEC_B, ""));
   orion.add_star(OrdStar(74.00f, 13.51f, 4.06f, OrdStar::SPEC_K, ""));
   orion.add_star(OrdStar(90.50f, 9.65f, 4.12f, OrdStar::SPEC_A, ""));
   orion.add_star(OrdStar(72.75f, 8.90f, 4.35f, OrdStar::SPEC_A, ""));
   orion.add_star(OrdStar(88.50f, 20.28f, 4.39f, OrdStar::SPEC_G, ""));
   orion.add_star(OrdStar(92.00f, 14.76f, 4.42f, OrdStar::SPEC_B, ""));
   orion.add_star(OrdStar(93.00f, 14.22f, 4.45f, OrdStar::SPEC_B, ""));
   orion.add_star(OrdStar(74.75f, 1.72f, 4.47f, OrdStar::SPEC_K, ""));
   orion.add_star(OrdStar(73.75f, 10.15f, 4.64f, OrdStar::SPEC_A, ""));
   orion.add_star(OrdStar(91.00f, 20.13f, 4.64f, OrdStar::SPEC_B, ""));
   orion.add_star(OrdStar(76.25f, 15.40f, 4.65f, OrdStar::SPEC_A, ""));
   orion.add_link(0, 8);
   orion.add_link(1, 2);
   orion.add_link(1, 4);
   orion.add_link(1, 9);
   orion.add_link(1, 13);
   orion.add_link(2, 6);
   orion.add_link(2, 7);
   orion.add_link(2, 9);
   orion.add_link(2, 14);
   orion.add_link(3, 4);
   orion.add_link(3, 6);
   orion.add_link(4, 5);
   orion.add_link(6, 8);
   orion.add_link(7, 10);
   orion.add_link(7, 14);
   orion.add_link(10, 11);
   orion.add_link(11, 18);
   orion.add_link(12, 19);
   orion.add_link(12, 21);
   orion.add_link(13, 17);
   orion.add_link(14, 19);
   orion.add_link(15, 16);
   orion.add_link(15, 20);
   orion.add_link(17, 20);
   cons_list.push_back(orion);
   tot_stars += orion.total_stars();
   tot_links += orion.total_links();

   Constellation pavo("Pavo");
   pavo.addToDescription("The peacock");
   pavo.add_star(OrdStar(306.50f, -56.73f, 1.94f, OrdStar::SPEC_B, "Peacock"));
   pavo.add_star(OrdStar(311.25f, -66.20f, 3.42f, OrdStar::SPEC_A, ""));
   pavo.add_star(OrdStar(302.25f, -66.18f, 3.55f, OrdStar::SPEC_G, ""));
   pavo.add_star(OrdStar(266.50f, -64.72f, 3.61f, OrdStar::SPEC_K, ""));
   pavo.add_star(OrdStar(300.25f, -72.92f, 3.97f, OrdStar::SPEC_A, ""));
   pavo.add_star(OrdStar(280.75f, -71.43f, 4.01f, OrdStar::SPEC_K, ""));
   pavo.add_star(OrdStar(321.50f, -65.37f, 4.21f, OrdStar::SPEC_F, ""));
   pavo.add_star(OrdStar(283.05f, -62.18f, 4.22f, OrdStar::SPEC_B, ""));
   pavo.add_star(OrdStar(272.25f, -63.67f, 4.33f, OrdStar::SPEC_A, ""));
   pavo.add_star(OrdStar(275.75f, -61.50f, 4.35f, OrdStar::SPEC_M, ""));
   pavo.add_star(OrdStar(285.25f, -67.23f, 4.40f, OrdStar::SPEC_F, ""));
   pavo.add_link(0, 2);
   pavo.add_link(0, 6);
   pavo.add_link(1, 2);
   pavo.add_link(1, 6);
   pavo.add_link(2, 4);
   pavo.add_link(2, 5);
   pavo.add_link(2, 7);
   pavo.add_link(2, 10);
   pavo.add_link(3, 8);
   pavo.add_link(7, 9);
   pavo.add_link(8, 9);
   pavo.add_link(8, 10);
   cons_list.push_back(pavo);
   tot_stars += pavo.total_stars();
   tot_links += pavo.total_links();

   Constellation pegasus("Pegasus");
   pegasus.addToDescription("The winged horse");
   pegasus.add_star(OrdStar(326.00f, 9.88f, 2.38f, OrdStar::SPEC_K, "Enif"));
   pegasus.add_star(OrdStar(346.00f, 28.08f, 2.44f, OrdStar::SPEC_M, "Scheat"));
   pegasus.add_star(OrdStar(346.25f, 15.20f, 2.49f, OrdStar::SPEC_B, "Markab"));
   pegasus.add_star(OrdStar(3.25f, 15.18f, 2.83f, OrdStar::SPEC_B, "Algenib"));
   pegasus.add_star(OrdStar(340.75f, 30.22f, 2.93f, OrdStar::SPEC_G, "Matar"));
   pegasus.add_star(OrdStar(340.25f, 10.83f, 3.41f, OrdStar::SPEC_B, "Homan"));
   pegasus.add_star(OrdStar(342.50f, 24.60f, 3.51f, OrdStar::SPEC_M, "Sadalbari"));
   pegasus.add_star(OrdStar(332.50f, 6.20f, 3.52f, OrdStar::SPEC_A, "Baham"));
   pegasus.add_star(OrdStar(331.75f, 25.35f, 3.77f, OrdStar::SPEC_F, ""));
   pegasus.add_star(OrdStar(341.75f, 23.56f, 3.97f, OrdStar::SPEC_G, "Sadalpheretz"));
   pegasus.add_star(OrdStar(326.25f, 25.65f, 4.14f, OrdStar::SPEC_F, "Jih"));
   pegasus.add_star(OrdStar(332.50f, 33.18f, 4.28f, OrdStar::SPEC_K, ""));
   // This next entry is actually Alpha Andromedae, but it would look silly not
   // to link it to Pegasus ie. Andromeda 'rides with' Pegasus ( + Perseus ).
   pegasus.add_star(OrdStar(2.006436f, +29.090833f, 2.07f, OrdStar::SPEC_B, "Alpheratz"));
   pegasus.add_link(0, 7);
   pegasus.add_link(1, 2);
   pegasus.add_link(1, 4);
   pegasus.add_link(1, 6);
   pegasus.add_link(1, 12);
   pegasus.add_link(2, 3);
   pegasus.add_link(2, 5);
   pegasus.add_link(3, 12);
   pegasus.add_link(4, 11);
   pegasus.add_link(5, 7);
   pegasus.add_link(6, 9);
   pegasus.add_link(8, 9);
   pegasus.add_link(8, 10);
   cons_list.push_back(pegasus);
   tot_stars += pegasus.total_stars();
   tot_links += pegasus.total_links();

   Constellation perseus("Perseus");
   perseus.addToDescription("The slayer of Medusa");
   perseus.add_star(OrdStar(51.00f, 49.85f, 1.79f, OrdStar::SPEC_F, "Mirfak"));
   perseus.add_star(OrdStar(47.00f, 40.95f, 2.09f, OrdStar::SPEC_B, "Algol"));
   perseus.add_star(OrdStar(58.50f, 31.88f, 2.84f, OrdStar::SPEC_B, "Menkhib"));
   perseus.add_star(OrdStar(59.50f, 40.00f, 2.90f, OrdStar::SPEC_B, ""));
   perseus.add_star(OrdStar(46.25f, 53.50f, 2.91f, OrdStar::SPEC_G, ""));
   perseus.add_star(OrdStar(55.75f, 47.78f, 3.01f, OrdStar::SPEC_B, ""));
   perseus.add_star(OrdStar(46.25f, 38.83f, 3.32f, OrdStar::SPEC_M, "Gorgonea Tertia"));
   perseus.add_star(OrdStar(42.75f, 55.90f, 3.77f, OrdStar::SPEC_K, "Miram"));
   perseus.add_star(OrdStar(47.25f, 44.85f, 3.79f, OrdStar::SPEC_F, "Misam"));
   perseus.add_star(OrdStar(56.00f, 32.28f, 3.84f, OrdStar::SPEC_B, "Atik"));
   perseus.add_star(OrdStar(43.50f, 52.76f, 3.93f, OrdStar::SPEC_G, ""));
   perseus.add_star(OrdStar(62.25f, 47.72f, 3.96f, OrdStar::SPEC_B, ""));
   perseus.add_star(OrdStar(59.75f, 35.78f, 3.98f, OrdStar::SPEC_O, "Menkib"));
   perseus.add_star(OrdStar(47.25f, 49.62f, 4.05f, OrdStar::SPEC_G, ""));
   perseus.add_star(OrdStar(41.00f, 49.23f, 4.10f, OrdStar::SPEC_F, ""));
   perseus.add_star(OrdStar(63.75f, 48.42f, 4.12f, OrdStar::SPEC_G, ""));
   perseus.add_star(OrdStar(61.75f, 50.35f, 4.25f, OrdStar::SPEC_A, ""));
   perseus.add_star(OrdStar(26.00f, 50.68f, 4.01f, OrdStar::SPEC_B, "Seif"));
   perseus.add_link(0, 4);
   perseus.add_link(0, 5);
   perseus.add_link(0, 13);
   perseus.add_link(1, 3);
   perseus.add_link(1, 6);
   perseus.add_link(1, 8);
   perseus.add_link(2, 9);
   perseus.add_link(2, 12);
   perseus.add_link(3, 5);
   perseus.add_link(3, 12);
   perseus.add_link(4, 7);
   perseus.add_link(4, 10);
   perseus.add_link(5, 11);
   perseus.add_link(7, 10);
   perseus.add_link(8, 13);
   perseus.add_link(10, 13);
   perseus.add_link(11, 15);
   perseus.add_link(13, 14);
   perseus.add_link(14, 17);
   perseus.add_link(15, 16);
   cons_list.push_back(perseus);
   tot_stars += perseus.total_stars();
   tot_links += perseus.total_links();

   Constellation phoenix("Phoenix");
   phoenix.addToDescription("The firebird");
   phoenix.add_star(OrdStar(6.50f, -42.30f, 2.40f, OrdStar::SPEC_K, "Ankaa"));
   phoenix.add_star(OrdStar(16.50f, -46.72f, 3.32f, OrdStar::SPEC_G, ""));
   phoenix.add_star(OrdStar(22.00f, -43.32f, 3.41f, OrdStar::SPEC_K, ""));
   phoenix.add_star(OrdStar(2.25f, -45.75f, 3.88f, OrdStar::SPEC_K, ""));
   phoenix.add_star(OrdStar(22.75f, -49.06f, 3.93f, OrdStar::SPEC_K, ""));
   phoenix.add_star(OrdStar(17.00f, -55.25f, 3.94f, OrdStar::SPEC_B, ""));
   phoenix.add_link(0, 1);
   phoenix.add_link(0, 3);
   phoenix.add_link(1, 2);
   phoenix.add_link(1, 3);
   phoenix.add_link(1, 5);
   phoenix.add_link(2, 4);
   phoenix.add_link(4, 5);
   cons_list.push_back(phoenix);
   tot_stars += phoenix.total_stars();
   tot_links += phoenix.total_links();

   Constellation pictor("Pictor");
   pictor.addToDescription("The easel");
   pictor.add_star(OrdStar(102.00f, -61.95f, 3.24f, OrdStar::SPEC_A, ""));
   pictor.add_star(OrdStar(86.75f, -51.06f, 3.85f, OrdStar::SPEC_A, ""));
   pictor.add_star(OrdStar(87.50f, -56.17f, 4.50f, OrdStar::SPEC_K, ""));
   pictor.add_link(0, 2);
   pictor.add_link(1, 2);
   cons_list.push_back(pictor);
   tot_stars += pictor.total_stars();
   tot_links += pictor.total_links();

   Constellation pisces("Pisces");
   pisces.addToDescription("The fish");
   pisces.add_star(OrdStar(22.75f, 15.35f, 3.62f, OrdStar::SPEC_G, "Alpherg"));
   pisces.add_star(OrdStar(349.25f, 3.28f, 3.70f, OrdStar::SPEC_G, "Simmah"));
   pisces.add_star(OrdStar(30.55f, 2.76f, 3.82f, OrdStar::SPEC_A, "Alrescha"));
   pisces.add_star(OrdStar(359.75f, 6.86f, 4.03f, OrdStar::SPEC_F, "Vernalis"));
   pisces.add_star(OrdStar(355.00f, 5.63f, 4.13f, OrdStar::SPEC_F, ""));
   pisces.add_star(OrdStar(26.25f, 9.15f, 4.26f, OrdStar::SPEC_K, "Torcularis"));
   pisces.add_star(OrdStar(15.75f, 7.88f, 4.27f, OrdStar::SPEC_K, "Kaht"));
   pisces.add_star(OrdStar(352.00f, 6.38f, 4.27f, OrdStar::SPEC_K, ""));
   pisces.add_star(OrdStar(25.25f, 5.48f, 4.45f, OrdStar::SPEC_K, ""));
   pisces.add_star(OrdStar(355.50f, 1.78f, 4.49f, OrdStar::SPEC_A, ""));
   pisces.add_star(OrdStar(18.00f, 30.38f, 4.51f, OrdStar::SPEC_K, "Anunitium"));
   pisces.add_star(OrdStar(18.50f, 24.58f, 4.67f, OrdStar::SPEC_K, ""));
   pisces.add_star(OrdStar(19.75f, 27.27f, 4.74f, OrdStar::SPEC_A, ""));
   pisces.add_star(OrdStar(351.75f, 1.25f, 4.95f, OrdStar::SPEC_A, ""));
   pisces.add_link(0, 5);
   pisces.add_link(0, 11);
   pisces.add_link(1, 7);
   pisces.add_link(1, 13);
   pisces.add_link(2, 5);
   pisces.add_link(2, 8);
   pisces.add_link(3, 4);
   pisces.add_link(3, 6);
   pisces.add_link(4, 7);
   pisces.add_link(4, 9);
   pisces.add_link(6, 8);
   pisces.add_link(9, 13);
   pisces.add_link(10, 11);
   pisces.add_link(10, 12);
   pisces.add_link(11, 12);
   cons_list.push_back(pisces);
   tot_stars += pisces.total_stars();
   tot_links += pisces.total_links();

   Constellation piscis_austrinus("Piscis Austrinus");
   piscis_austrinus.addToDescription("The southern fish");
   piscis_austrinus.add_star(OrdStar(344.50f, -29.62f, 1.17f, OrdStar::SPEC_A, "Fomalhaut"));
   piscis_austrinus.add_star(OrdStar(340.25f, -27.05f, 4.18f, OrdStar::SPEC_B, ""));
   piscis_austrinus.add_star(OrdStar(344.00f, -32.53f, 4.20f, OrdStar::SPEC_G, ""));
   piscis_austrinus.add_star(OrdStar(338.00f, -32.35f, 4.29f, OrdStar::SPEC_A, "Tien Kang"));
   piscis_austrinus.add_star(OrdStar(326.25f, -33.03f, 4.35f, OrdStar::SPEC_B, ""));
   piscis_austrinus.add_star(OrdStar(343.25f, -32.88f, 4.46f, OrdStar::SPEC_A, ""));
   piscis_austrinus.add_star(OrdStar(332.00f, -32.98f, 4.50f, OrdStar::SPEC_A, ""));
   piscis_austrinus.add_star(OrdStar(327.00f, -30.90f, 5.02f, OrdStar::SPEC_A, ""));
   piscis_austrinus.add_link(0, 1);
   piscis_austrinus.add_link(0, 2);
   piscis_austrinus.add_link(1, 6);
   piscis_austrinus.add_link(2, 5);
   piscis_austrinus.add_link(3, 5);
   piscis_austrinus.add_link(3, 6);
   piscis_austrinus.add_link(4, 6);
   piscis_austrinus.add_link(4, 7);
   piscis_austrinus.add_link(6, 7);
   cons_list.push_back(piscis_austrinus);
   tot_stars += piscis_austrinus.total_stars();
   tot_links += piscis_austrinus.total_links();

   Constellation puppis("Puppis");
   puppis.addToDescription("The poop deck");
   puppis.add_star(OrdStar(121.00f, -40.00f, 2.21f, OrdStar::SPEC_O, "Naos"));
   puppis.add_star(OrdStar(109.25f, -37.10f, 2.71f, OrdStar::SPEC_K, ""));
   puppis.add_star(OrdStar(122.00f, -24.30f, 2.83f, OrdStar::SPEC_F, "Tureis"));
   puppis.add_star(OrdStar(99.50f, -43.20f, 3.17f, OrdStar::SPEC_B, ""));
   puppis.add_star(OrdStar(117.25f, -24.87f, 3.34f, OrdStar::SPEC_G, "Asmidiske"));
   puppis.add_star(OrdStar(116.00f, -28.95f, 3.94f, OrdStar::SPEC_A, ""));
   puppis.add_star(OrdStar(114.75f, -26.80f, 4.50f, OrdStar::SPEC_B, "Naos"));
   // This next entry is actually Alpha Carina, but it would look silly not
   // to link it to Puppis ie. the keel with the poop-deck ( of the ship ).
   puppis.add_star(OrdStar(95.77f, -52.68f, -0.72f, OrdStar::SPEC_F, "Canopus"));
   puppis.add_link(0, 2);
   puppis.add_link(1, 3);
   puppis.add_link(1, 6);
   puppis.add_link(2, 4);
   puppis.add_link(3, 7);
   puppis.add_link(4, 5);
   puppis.add_link(4, 6);
   puppis.add_link(5, 6);
   cons_list.push_back(puppis);
   tot_stars += puppis.total_stars();
   tot_links += puppis.total_links();

   Constellation pyxis("Pyxis");
   pyxis.addToDescription("The mariner's compass");
   pyxis.add_star(OrdStar(131.00f, -33.18f, 3.68f, OrdStar::SPEC_B, "Al Sumut"));
   pyxis.add_star(OrdStar(130.00f, -35.30f, 3.97f, OrdStar::SPEC_G, ""));
   pyxis.add_star(OrdStar(132.75f, -27.72f, 4.02f, OrdStar::SPEC_K, ""));
   // This next entry is actually Zeta Puppis, but it would look silly not
   // to link it to Pyxis ie. the poop-deck with the mariner's compass ( of the ship ).
   pyxis.add_star(OrdStar(121.00f, -40.00f, 2.21f, OrdStar::SPEC_O, "Naos"));
   pyxis.add_link(0, 1);
   pyxis.add_link(0, 2);
   pyxis.add_link(1, 3);
   cons_list.push_back(pyxis);
   tot_stars += pyxis.total_stars();
   tot_links += pyxis.total_links();

   Constellation reticulum("Reticulum");
   reticulum.addToDescription("The eyepiece graticule");
   reticulum.add_star(OrdStar(63.50f, -62.46f, 3.33f, OrdStar::SPEC_G, ""));
   reticulum.add_star(OrdStar(56.00f, -64.80f, 3.84f, OrdStar::SPEC_K, ""));
   reticulum.add_star(OrdStar(64.00f, -59.30f, 4.40f, OrdStar::SPEC_K, ""));
   reticulum.add_star(OrdStar(59.75f, -61.04f, 4.56f, OrdStar::SPEC_M, ""));
   reticulum.add_link(0, 1);
   reticulum.add_link(0, 2);
   reticulum.add_link(1, 3);
   reticulum.add_link(2, 3);
   cons_list.push_back(reticulum);
   tot_stars += reticulum.total_stars();
   tot_links += reticulum.total_links();

   Constellation sagitta("Sagitta");
   sagitta.addToDescription("The arrow");
   sagitta.add_star(OrdStar(299.69f, 19.49f, 3.51f, OrdStar::SPEC_K, ""));
   sagitta.add_star(OrdStar(296.85f, 18.53f, 3.68f, OrdStar::SPEC_M, ""));
   sagitta.add_star(OrdStar(295.02f, 18.08f, 4.39f, OrdStar::SPEC_G, "Sham"));
   sagitta.add_star(OrdStar(295.26f, 17.48f, 4.39f, OrdStar::SPEC_G, ""));
   sagitta.add_link(0, 1);
   sagitta.add_link(1, 2);
   sagitta.add_link(1, 3);
   cons_list.push_back(sagitta);
   tot_stars += sagitta.total_stars();
   tot_links += sagitta.total_links();

   Constellation sagittarius("Sagittarius");
   sagittarius.addToDescription("The archer");
   sagittarius.add_star(OrdStar(276.04f, -34.57f, 1.79f, OrdStar::SPEC_B, "Kaus Australis"));
   sagittarius.add_star(OrdStar(283.82f, -26.30f, 2.05f, OrdStar::SPEC_B, "Nunki"));
   sagittarius.add_star(OrdStar(285.65f, -29.88f, 2.60f, OrdStar::SPEC_A, "Ascella"));
   sagittarius.add_star(OrdStar(275.25f, -29.83f, 2.72f, OrdStar::SPEC_K, "Kaus Media"));
   sagittarius.add_star(OrdStar(277.00f, -25.42f, 2.82f, OrdStar::SPEC_K, "Kaus Borealis"));
   sagittarius.add_star(OrdStar(287.44f, -21.02f, 2.88f, OrdStar::SPEC_F, "Al Baldah"));
   sagittarius.add_star(OrdStar(271.45f, -30.42f, 2.98f, OrdStar::SPEC_K, "Alnasl"));
   sagittarius.add_star(OrdStar(274.41f, -36.76f, 3.10f, OrdStar::SPEC_M, ""));
   sagittarius.add_star(OrdStar(281.41f, -26.99f, 3.17f, OrdStar::SPEC_B, ""));
   sagittarius.add_star(OrdStar(286.73f, -27.67f, 3.32f, OrdStar::SPEC_K, ""));
   sagittarius.add_star(OrdStar(284.43f, -21.11f, 3.52f, OrdStar::SPEC_G, ""));
   sagittarius.add_star(OrdStar(286.17f, -21.74f, 3.76f, OrdStar::SPEC_K, ""));
   sagittarius.add_star(OrdStar(273.44f, -21.06f, 3.84f, OrdStar::SPEC_B, "Polis"));
   sagittarius.add_star(OrdStar(290.42f, -17.85f, 3.92f, OrdStar::SPEC_F, ""));
   sagittarius.add_link(0, 2);
   sagittarius.add_link(0, 3);
   sagittarius.add_link(0, 6);
   sagittarius.add_link(0, 7);
   sagittarius.add_link(1, 8);
   sagittarius.add_link(1, 9);
   sagittarius.add_link(2, 8);
   sagittarius.add_link(2, 9);
   sagittarius.add_link(3, 4);
   sagittarius.add_link(3, 6);
   sagittarius.add_link(3, 8);
   sagittarius.add_link(4, 8);
   sagittarius.add_link(4, 12);
   sagittarius.add_link(5, 10);
   sagittarius.add_link(5, 11);
   sagittarius.add_link(5, 13);
   sagittarius.add_link(10, 11);
   cons_list.push_back(sagittarius);
   tot_stars += sagittarius.total_stars();
   tot_links += sagittarius.total_links();

   Constellation scorpius("Scorpius");
   scorpius.addToDescription("The scorpion");
   scorpius.add_star(OrdStar(247.35f, -26.43f, 1.06f, OrdStar::SPEC_M, "Antares"));
   scorpius.add_star(OrdStar(263.40f, -37.10f, 1.62f, OrdStar::SPEC_B, "Shaula"));
   scorpius.add_star(OrdStar(264.32f, -43.00f, 1.86f, OrdStar::SPEC_F, "Sargas"));
   scorpius.add_star(OrdStar(240.08f, -22.62f, 2.29f, OrdStar::SPEC_B, "Dshubba"));
   scorpius.add_star(OrdStar(252.54f, -34.29f, 2.29f, OrdStar::SPEC_K, "Wei"));
   scorpius.add_star(OrdStar(265.62f, -39.02f, 2.39f, OrdStar::SPEC_B, "Girtab"));
   scorpius.add_star(OrdStar(241.35f, -19.81f, 2.56f, OrdStar::SPEC_B, "Acrab"));
   scorpius.add_star(OrdStar(262.69f, -37.29f, 2.70f, OrdStar::SPEC_B, "Lesath"));
   scorpius.add_star(OrdStar(248.97f, -28.22f, 2.82f, OrdStar::SPEC_B, "Alniyat"));
   scorpius.add_star(OrdStar(239.71f, -26.11f, 2.89f, OrdStar::SPEC_B, "Vrischika Iclil"));
   scorpius.add_star(OrdStar(245.29f, -25.59f, 2.90f, OrdStar::SPEC_B, "Alniyat"));
   scorpius.add_star(OrdStar(266.90f, -40.13f, 2.99f, OrdStar::SPEC_F, "Apollyon"));
   scorpius.add_star(OrdStar(252.97f, -38.05f, 3.00f, OrdStar::SPEC_B, "Denebakrab"));
   scorpius.add_star(OrdStar(267.47f, -37.04f, 3.19f, OrdStar::SPEC_K, "Basanismus"));
   scorpius.add_star(OrdStar(258.04f, -43.24f, 3.32f, OrdStar::SPEC_F, ""));
   scorpius.add_star(OrdStar(253.65f, -42.36f, 3.62f, OrdStar::SPEC_K, ""));
   scorpius.add_star(OrdStar(239.22f, -29.21f, 3.87f, OrdStar::SPEC_B, ""));
   scorpius.add_link(0, 8);
   scorpius.add_link(0, 10);
   scorpius.add_link(1, 7);
   scorpius.add_link(1, 13);
   scorpius.add_link(2, 11);
   scorpius.add_link(2, 14);
   scorpius.add_link(3, 6);
   scorpius.add_link(3, 9);
   scorpius.add_link(3, 10);
   scorpius.add_link(4, 8);
   scorpius.add_link(4, 12);
   scorpius.add_link(5, 7);
   scorpius.add_link(5, 11);
   scorpius.add_link(9, 16);
   scorpius.add_link(12, 15);
   scorpius.add_link(14, 15);
   cons_list.push_back(scorpius);
   tot_stars += scorpius.total_stars();
   tot_links += scorpius.total_links();

   Constellation sculptor("Sculptor");
   sculptor.addToDescription("The sculptor");
   sculptor.add_star(OrdStar(14.89f, -29.36f, 4.30f, OrdStar::SPEC_B, ""));
   sculptor.add_star(OrdStar(353.24f, -37.81f, 4.38f, OrdStar::SPEC_K, ""));
   sculptor.add_star(OrdStar(349.70f, -32.53f, 4.41f, OrdStar::SPEC_K, ""));
   sculptor.add_star(OrdStar(357.23f, -28.13f, 4.59f, OrdStar::SPEC_M, ""));
   sculptor.add_star(OrdStar(5.37f, -28.98f, 5.18f, OrdStar::SPEC_M, ""));
   sculptor.add_link(0, 4);
   sculptor.add_link(1, 2);
   sculptor.add_link(2, 3);
   sculptor.add_link(3, 4);
   cons_list.push_back(sculptor);
   tot_stars += sculptor.total_stars();
   tot_links += sculptor.total_links();

   Constellation scutum("Scutum");
   scutum.addToDescription("The shield");
   scutum.add_star(OrdStar(278.80f, -8.24f, 3.85f, OrdStar::SPEC_K, ""));
   scutum.add_star(OrdStar(281.79f, -4.75f, 4.22f, OrdStar::SPEC_G, ""));
   scutum.add_star(OrdStar(277.30f, -14.57f, 4.67f, OrdStar::SPEC_A, ""));
   scutum.add_star(OrdStar(280.57f, -9.05f, 4.70f, OrdStar::SPEC_F, ""));
   scutum.add_link(0, 1);
   scutum.add_link(0, 2);
   scutum.add_link(1, 3);
   scutum.add_link(2, 3);
   cons_list.push_back(scutum);
   tot_stars += scutum.total_stars();
   tot_links += scutum.total_links();

   Constellation serpens_caput("Serpens Caput");
   serpens_caput.addToDescription("The serpent's head");
   serpens_caput.add_star(OrdStar(236.07f, 6.43f, 2.63f, OrdStar::SPEC_K, "Unukalhai"));
   serpens_caput.add_star(OrdStar(237.40f, -3.43f, 3.54f, OrdStar::SPEC_A, "Tianru"));
   serpens_caput.add_star(OrdStar(236.55f, 15.42f, 3.65f, OrdStar::SPEC_A, "Chow"));
   serpens_caput.add_star(OrdStar(237.70f, 4.48f, 3.71f, OrdStar::SPEC_A, "Ba"));
   serpens_caput.add_star(OrdStar(233.70f, 10.54f, 3.80f, OrdStar::SPEC_F, "Qin"));
   serpens_caput.add_star(OrdStar(239.11f, 15.66f, 3.85f, OrdStar::SPEC_F, "Zheng"));
   serpens_caput.add_star(OrdStar(237.19f, 18.14f, 4.09f, OrdStar::SPEC_M, ""));
   serpens_caput.add_star(OrdStar(235.39f, 19.67f, 4.51f, OrdStar::SPEC_A, ""));
   // From Ophiuchus or "Serpent Bearer'.
   serpens_caput.add_star(OrdStar(243.58f, -3.69f, 2.73f, OrdStar::SPEC_M, "Yed Prior"));
   serpens_caput.add_link(0, 3);
   serpens_caput.add_link(0, 4);
   serpens_caput.add_link(1, 3);
   serpens_caput.add_link(1, 3);
   serpens_caput.add_link(1, 8);
   serpens_caput.add_link(2, 4);
   serpens_caput.add_link(2, 5);
   serpens_caput.add_link(2, 7);
   serpens_caput.add_link(5, 6);
   serpens_caput.add_link(6, 7);
   cons_list.push_back(serpens_caput);
   tot_stars += serpens_caput.total_stars();
   tot_links += serpens_caput.total_links();

   Constellation serpens_cauda("Serpens Cauda");
   serpens_cauda.addToDescription("The serpent's tail");
   serpens_cauda.add_star(OrdStar(264.40f, -15.40f, 3.54f, OrdStar::SPEC_F, "Nanhai"));
   serpens_cauda.add_star(OrdStar(265.57f, -12.88f, 4.24f, OrdStar::SPEC_A, ""));
   serpens_cauda.add_star(OrdStar(275.33f, -2.90f, 1.84f, OrdStar::SPEC_K, "Donghai"));
   serpens_cauda.add_star(OrdStar(284.05f, 4.20f, 4.62f, OrdStar::SPEC_A, "Alya"));
   // From Ophiuchus or "Serpent Bearer'.
   serpens_cauda.add_star(OrdStar(257.50f, -15.73f, 2.43f, OrdStar::SPEC_A, "Sabik"));
   serpens_cauda.add_star(OrdStar(269.75f, -9.77f, 3.32f, OrdStar::SPEC_K, "Yan"));
   serpens_cauda.add_link(0, 1);
   serpens_cauda.add_link(0, 4);
   serpens_cauda.add_link(1, 5);
   serpens_cauda.add_link(2, 3);
   serpens_cauda.add_link(2, 5);
   cons_list.push_back(serpens_cauda);
   tot_stars += serpens_cauda.total_stars();
   tot_links += serpens_cauda.total_links();

   Constellation sextans("Sextans");
   sextans.addToDescription("The sextant");
   sextans.add_star(OrdStar(151.98f, -0.37f, 4.48f, OrdStar::SPEC_A, ""));
   sextans.add_star(OrdStar(148.15f, -8.11f, 5.07f, OrdStar::SPEC_A, ""));
   sextans.add_star(OrdStar(157.58f, -0.64f, 5.08f, OrdStar::SPEC_B, ""));
   sextans.add_star(OrdStar(157.37f, -2.74f, 5.19f, OrdStar::SPEC_B, ""));
   sextans.add_link(0, 1);
   sextans.add_link(0, 2);
   sextans.add_link(2, 3);
   cons_list.push_back(sextans);
   tot_stars += sextans.total_stars();
   tot_links += sextans.total_links();

   Constellation taurus("Taurus");
   taurus.addToDescription("The bull");
   taurus.add_star(OrdStar(68.97f, 16.51f, 0.87f, OrdStar::SPEC_K, "Aldebaran"));
   taurus.add_star(OrdStar(81.56f, 28.60f, 1.65f, OrdStar::SPEC_B, "Elnath"));
   taurus.add_star(OrdStar(84.41f, 21.14f, 2.97f, OrdStar::SPEC_B, "Tien Kwan"));
   taurus.add_star(OrdStar(67.17f, 15.87f, 3.40f, OrdStar::SPEC_A, ""));
   taurus.add_star(OrdStar(60.17f, 12.49f, 3.41f, OrdStar::SPEC_B, ""));
   taurus.add_star(OrdStar(67.15f, 19.18f, 3.53f, OrdStar::SPEC_K, "Ain"));
   taurus.add_star(OrdStar(51.20f, 9.03f, 3.61f, OrdStar::SPEC_G, ""));
   taurus.add_star(OrdStar(64.94f, 15.63f, 3.65f, OrdStar::SPEC_G, "Hyadum I"));
   taurus.add_star(OrdStar(51.79f, 9.73f, 3.73f, OrdStar::SPEC_B, ""));
   taurus.add_star(OrdStar(65.73f, 17.54f, 3.77f, OrdStar::SPEC_G, "Hyadum II"));
   taurus.add_star(OrdStar(60.79f, 5.99f, 3.91f, OrdStar::SPEC_A, ""));
   taurus.add_star(OrdStar(54.22f, 0.40f, 4.29f, OrdStar::SPEC_F, ""));
   taurus.add_link(0, 2);
   taurus.add_link(0, 3);
   taurus.add_link(1, 5);
   taurus.add_link(3, 7);
   taurus.add_link(4, 7);
   taurus.add_link(4, 8);
   taurus.add_link(5, 9);
   taurus.add_link(6, 8);
   taurus.add_link(6, 11);
   taurus.add_link(7, 9);
   taurus.add_link(8, 10);
   cons_list.push_back(taurus);
   tot_stars += taurus.total_stars();
   tot_links += taurus.total_links();

   Constellation telescopium("Telescopium");
   telescopium.addToDescription("The telescope");
   telescopium.add_star(OrdStar(276.75f, -45.97f, 3.49f, OrdStar::SPEC_B, ""));
   telescopium.add_star(OrdStar(277.20f, -49.07f, 4.10f, OrdStar::SPEC_G, ""));
   telescopium.add_star(OrdStar(272.80f, -45.95f, 4.52f, OrdStar::SPEC_G, ""));
   telescopium.add_star(OrdStar(284.61f, -52.947f, 4.85f, OrdStar::SPEC_A, ""));
   telescopium.add_star(OrdStar(301.85f, -52.88f, 4.93f, OrdStar::SPEC_M, ""));
   telescopium.add_star(OrdStar(290.71f, -54.42f, 5.03f, OrdStar::SPEC_A, ""));
   telescopium.add_star(OrdStar(286.58f, -52.34f, 5.17f, OrdStar::SPEC_F, ""));
   telescopium.add_star(OrdStar(283.16f, -52.11f, 5.18f, OrdStar::SPEC_G, ""));
   telescopium.add_star(OrdStar(297.00f, -56.361f, 5.33f, OrdStar::SPEC_A, ""));
   telescopium.add_link(0, 2);
   telescopium.add_link(0, 7);
   telescopium.add_link(1, 2);
   telescopium.add_link(1, 3);
   telescopium.add_link(3, 5);
   telescopium.add_link(4, 6);
   telescopium.add_link(4, 8);
   telescopium.add_link(5, 8);
   telescopium.add_link(6, 7);
   cons_list.push_back(telescopium);
   tot_stars += telescopium.total_stars();
   tot_links += telescopium.total_links();

   Constellation triangulum("Triangulum");
   triangulum.addToDescription("The triangle");
   triangulum.add_star(OrdStar(32.39f, 34.99f, 3.00f, OrdStar::SPEC_A, "Deltotum"));
   triangulum.add_star(OrdStar(28.27f, 29.58f, 3.42f, OrdStar::SPEC_F, "Mothallah"));
   triangulum.add_star(OrdStar(34.33f, 33.85f, 4.03f, OrdStar::SPEC_A, ""));
   triangulum.add_star(OrdStar(34.26f, 34.22f, 4.84f, OrdStar::SPEC_G, ""));
   triangulum.add_link(0, 1);
   triangulum.add_link(0, 3);
   triangulum.add_link(1, 2);
   cons_list.push_back(triangulum);
   tot_stars += triangulum.total_stars();
   tot_links += triangulum.total_links();

   Constellation triangulum_australae("Triangulum Australae");
   triangulum_australae.addToDescription("The southern triangle");
   triangulum_australae.add_star(OrdStar(252.17f, -69.03f, 1.91f, OrdStar::SPEC_K, "Atria"));
   triangulum_australae.add_star(OrdStar(238.79f, -63.43f, 2.83f, OrdStar::SPEC_F, ""));
   triangulum_australae.add_star(OrdStar(229.73f, -68.68f, 2.87f, OrdStar::SPEC_A, ""));
   triangulum_australae.add_link(0, 1);
   triangulum_australae.add_link(0, 2);
   triangulum_australae.add_link(1, 2);
   cons_list.push_back(triangulum_australae);
   tot_stars += triangulum_australae.total_stars();
   tot_links += triangulum_australae.total_links();

   Constellation tucana("Tucana");
   tucana.addToDescription("The tucan");
   tucana.add_star(OrdStar(334.63f, -60.26f, 2.87f, OrdStar::SPEC_K, ""));
   tucana.add_star(OrdStar(349.35f, -58.24f, 3.99f, OrdStar::SPEC_F, ""));
   tucana.add_star(OrdStar(5.01f, -64.88f, 4.23f, OrdStar::SPEC_F, ""));
   tucana.add_star(OrdStar(7.89f, -62.96f, 4.36f, OrdStar::SPEC_B, ""));
   tucana.add_star(OrdStar(359.98f, -65.58f, 4.49f, OrdStar::SPEC_B, ""));
   tucana.add_star(OrdStar(336.83f, -64.97f, 4.51f, OrdStar::SPEC_B, ""));
   tucana.add_link(0, 1);
   tucana.add_link(0, 5);
   tucana.add_link(1, 3);
   tucana.add_link(2, 3);
   tucana.add_link(2, 4);
   tucana.add_link(4, 5);
   cons_list.push_back(tucana);
   tot_stars += tucana.total_stars();
   tot_links += tucana.total_links();

   Constellation ursa_major("Ursa Major");
   ursa_major.addToDescription("The greater bear");
   ursa_major.add_star(OrdStar(193.51f, 55.96f, 1.76f, OrdStar::SPEC_A, "Alioth"));
   ursa_major.add_star(OrdStar(165.93f, 61.75f, 1.81f, OrdStar::SPEC_F, "Dubhe"));
   ursa_major.add_star(OrdStar(206.89f, 49.31f, 1.85f, OrdStar::SPEC_B, "Alkaid"));
   ursa_major.add_star(OrdStar(200.98f, 54.93f, 2.23f, OrdStar::SPEC_A, "Mizar"));
   ursa_major.add_star(OrdStar(165.46f, 56.38f, 2.34f, OrdStar::SPEC_A, "Merak"));
   ursa_major.add_star(OrdStar(178.46f, 53.69f, 2.41f, OrdStar::SPEC_A, "Phecda"));
   ursa_major.add_star(OrdStar(167.42f, 44.50f, 3.00f, OrdStar::SPEC_K, "Ta Tsun"));
   ursa_major.add_star(OrdStar(155.58f, 41.50f, 3.06f, OrdStar::SPEC_M, "Tania Australis"));
   ursa_major.add_star(OrdStar(134.80f, 48.04f, 3.12f, OrdStar::SPEC_A, "Talitha Borealis"));
   ursa_major.add_star(OrdStar(143.22f, 51.68f, 3.17f, OrdStar::SPEC_F, "Al Haud"));
   ursa_major.add_star(OrdStar(183.86f, 57.03f, 3.32f, OrdStar::SPEC_A, "Megrez"));
   ursa_major.add_star(OrdStar(127.57f, 60.72f, 3.35f, OrdStar::SPEC_G, "Muscida"));
   ursa_major.add_star(OrdStar(154.28f, 42.91f, 3.45f, OrdStar::SPEC_A, "Tania Borealis"));
   ursa_major.add_star(OrdStar(169.62f, 33.09f, 3.49f, OrdStar::SPEC_K, "Alula Borealis"));
   ursa_major.add_star(OrdStar(135.91f, 47.16f, 3.57f, OrdStar::SPEC_A, "Talitha Australis"));
   ursa_major.add_star(OrdStar(142.88f, 63.06f, 3.65f, OrdStar::SPEC_F, ""));
   ursa_major.add_star(OrdStar(176.51f, 47.78f, 3.69f, OrdStar::SPEC_K, "Alkafzah"));
   ursa_major.add_star(OrdStar(147.75f, 59.04f, 3.78f, OrdStar::SPEC_F, ""));
   ursa_major.add_star(OrdStar(169.55f, 31.53f, 3.79f, OrdStar::SPEC_G, ""));
   ursa_major.add_link(0, 3);
   ursa_major.add_link(0, 10);
   ursa_major.add_link(1, 4);
   ursa_major.add_link(1, 10);
   ursa_major.add_link(1, 15);
   ursa_major.add_link(2, 3);
   ursa_major.add_link(4, 5);
   ursa_major.add_link(4, 17);
   ursa_major.add_link(5, 10);
   ursa_major.add_link(5, 16);
   ursa_major.add_link(6, 7);
   ursa_major.add_link(6, 16);
   ursa_major.add_link(7, 12);
   ursa_major.add_link(8, 14);
   ursa_major.add_link(9, 14);
   ursa_major.add_link(9, 17);
   ursa_major.add_link(11, 15);
   ursa_major.add_link(11, 17);
   ursa_major.add_link(13, 16);
   ursa_major.add_link(13, 18);
   ursa_major.add_link(15, 17);
   cons_list.push_back(ursa_major);
   tot_stars += ursa_major.total_stars();
   tot_links += ursa_major.total_links();

   Constellation ursa_minor("Ursa Minor");
   ursa_minor.addToDescription("The lesser bear");
   ursa_minor.add_star(OrdStar(37.95f, 89.26f, 1.97f, OrdStar::SPEC_F, "Polaris"));
   ursa_minor.add_star(OrdStar(222.68f, 74.15f, 2.07f, OrdStar::SPEC_K, "Kochab"));
   ursa_minor.add_star(OrdStar(230.18f, 71.83f, 3.00f, OrdStar::SPEC_A, "Pherkad"));
   ursa_minor.add_star(OrdStar(251.49f, 82.04f, 4.21f, OrdStar::SPEC_G, "Urodelus"));
   ursa_minor.add_star(OrdStar(236.02f, 77.79f, 4.29f, OrdStar::SPEC_A, "Ahfa al Farkadain"));
   ursa_minor.add_star(OrdStar(263.05f, 86.59f, 4.35f, OrdStar::SPEC_A, "Yildun"));
   ursa_minor.add_star(OrdStar(244.38f, 75.75f, 4.95f, OrdStar::SPEC_F, "Anwar al Farkadian"));
   ursa_minor.add_link(0, 5);
   ursa_minor.add_link(1, 2);
   ursa_minor.add_link(1, 4);
   ursa_minor.add_link(2, 6);
   ursa_minor.add_link(3, 4);
   ursa_minor.add_link(3, 5);
   ursa_minor.add_link(4, 6);
   cons_list.push_back(ursa_minor);
   tot_stars += ursa_minor.total_stars();
   tot_links += ursa_minor.total_links();

   Constellation vela("Vela");
   vela.addToDescription("The sails");
   vela.add_star(OrdStar(122.38f, -47.34f, 1.75f, OrdStar::SPEC_O, "Regor"));
   vela.add_star(OrdStar(131.18f, -54.71f, 1.93f, OrdStar::SPEC_A, "Koo She"));
   vela.add_star(OrdStar(137.00f, -43.10f, 2.23f, OrdStar::SPEC_K, "Suhail"));
   vela.add_star(OrdStar(140.53f, -55.01f, 2.47f, OrdStar::SPEC_B, "Markab"));
   vela.add_star(OrdStar(161.69f, -49.42f, 2.69f, OrdStar::SPEC_G, "Peregrini"));
   vela.add_star(OrdStar(149.22f, -54.57f, 3.52f, OrdStar::SPEC_B, "Tseen Ke"));
   vela.add_star(OrdStar(142.68f, -40.47f, 3.60f, OrdStar::SPEC_F, ""));
   vela.add_star(OrdStar(153.68f, -42.12f, 3.85f, OrdStar::SPEC_A, ""));
   // Actually part of Puppis : the Poop Deck.
   vela.add_star(OrdStar(121.00f, -40.00f, 2.21f, OrdStar::SPEC_O, "Naos"));
   // Actually part of Carina : the Keel.
   vela.add_star(OrdStar(139.25f, -59.28f, 2.21f, OrdStar::SPEC_A, "Aspidiske"));
   vela.add_link(0, 1);
   vela.add_link(0, 2);
   vela.add_link(0, 8);
   vela.add_link(1, 3);
   vela.add_link(1, 9);
   vela.add_link(2, 6);
   vela.add_link(3, 5);
   vela.add_link(4, 5);
   vela.add_link(4, 7);
   vela.add_link(6, 7);
   cons_list.push_back(vela);
   tot_stars += vela.total_stars();
   tot_links += vela.total_links();

   Constellation virgo("Virgo");
   virgo.addToDescription("The maiden");
   virgo.add_star(OrdStar(201.3f, -11.16f, 0.98f, OrdStar::SPEC_B, "Spica"));
   virgo.add_star(OrdStar(190.42f, -1.45f, 2.74f, OrdStar::SPEC_F, "Porrima"));
   virgo.add_star(OrdStar(195.55f, 10.96f, 2.85f, OrdStar::SPEC_G, "Vindemiatrix"));
   virgo.add_star(OrdStar(203.68f, -0.56f, 3.38f, OrdStar::SPEC_A, "Heze"));
   virgo.add_star(OrdStar(193.90f, 3.40f, 3.39f, OrdStar::SPEC_M, "Auva"));
   virgo.add_star(OrdStar(177.67f, 1.77f, 3.59f, OrdStar::SPEC_F, "Zavijava"));
   virgo.add_star(OrdStar(221.56f, 1.89f, 3.73f, OrdStar::SPEC_A, ""));
   virgo.add_star(OrdStar(220.77f, -5.66f, 3.87f, OrdStar::SPEC_F, "Rijl al Awwa"));
   virgo.add_star(OrdStar(184.98f, -0.67f, 3.89f, OrdStar::SPEC_A, "Zaniah"));
   virgo.add_star(OrdStar(176.47f, 6.53f, 4.04f, OrdStar::SPEC_M, ""));
   virgo.add_star(OrdStar(214.00f, -5.99f, 4.07f, OrdStar::SPEC_F, "Syrma"));
   virgo.add_star(OrdStar(181.30f, 8.73f, 4.12f, OrdStar::SPEC_G, ""));
   virgo.add_star(OrdStar(210.41f, 1.54f, 4.23f, OrdStar::SPEC_A, ""));
   virgo.add_star(OrdStar(197.49f, -5.54f, 4.38f, OrdStar::SPEC_A, ""));
   virgo.add_link(0, 13);
   virgo.add_link(1, 3);
   virgo.add_link(1, 4);
   virgo.add_link(1, 8);
   virgo.add_link(1, 13);
   virgo.add_link(2, 4);
   virgo.add_link(3, 10);
   virgo.add_link(3, 12);
   virgo.add_link(5, 8);
   virgo.add_link(5, 9);
   virgo.add_link(6, 12);
   virgo.add_link(7, 10);
   virgo.add_link(8, 11);
   virgo.add_link(9, 11);
   cons_list.push_back(virgo);
   tot_stars += virgo.total_stars();
   tot_links += virgo.total_links();

   Constellation volans("Volans");
   volans.addToDescription("The flying fish");
   volans.add_star(OrdStar(126.43f, -66.14f, 3.77f, OrdStar::SPEC_K, ""));
   volans.add_star(OrdStar(107.19f, -70.50f, 3.78f, OrdStar::SPEC_G, ""));
   volans.add_star(OrdStar(109.21f, -67.96f, 3.97f, OrdStar::SPEC_F, ""));
   volans.add_star(OrdStar(135.61f, -66.40f, 4.00f, OrdStar::SPEC_A, ""));
   volans.add_star(OrdStar(121.98f, -68.62f, 4.35f, OrdStar::SPEC_B, ""));
   volans.add_link(0, 3);
   volans.add_link(0, 4);
   volans.add_link(1, 2);
   volans.add_link(1, 4);
   volans.add_link(2, 4);
   volans.add_link(3, 4);
   cons_list.push_back(volans);
   tot_stars += volans.total_stars();
   tot_links += volans.total_links();

   Constellation vulpecula("Vulpecula");
   vulpecula.addToDescription("The little fox");
   vulpecula.add_star(OrdStar(292.18f, 24.67f, 4.44f, OrdStar::SPEC_M, "Anser"));
   vulpecula.add_star(OrdStar(313.03f, 27.81f, 4.50f, OrdStar::SPEC_K, ""));
   vulpecula.add_star(OrdStar(121.98f, 27.10f, 4.56f, OrdStar::SPEC_G, ""));
   vulpecula.add_star(OrdStar(298.37f, 24.08f, 4.57f, OrdStar::SPEC_B, ""));
   vulpecula.add_star(OrdStar(300.28f, 27.75f, 4.66f, OrdStar::SPEC_A, ""));
   vulpecula.add_star(OrdStar(303.82f, 25.59f, 4.79f, OrdStar::SPEC_B, ""));
   vulpecula.add_star(OrdStar(309.63f, 21.20f, 4.81f, OrdStar::SPEC_A, ""));
   vulpecula.add_star(OrdStar(297.77f, 22.61f, 4.90f, OrdStar::SPEC_B, ""));
   vulpecula.add_star(OrdStar(311.22f, 25.27f, 4.92f, OrdStar::SPEC_K, ""));
   vulpecula.add_star(OrdStar(309.63f, 24.12f, 5.06f, OrdStar::SPEC_B, ""));
   vulpecula.add_star(OrdStar(301.72f, 23.61f, 5.08f, OrdStar::SPEC_B, ""));
   vulpecula.add_star(OrdStar(314.57f, 22.36f, 5.30f, OrdStar::SPEC_K, ""));
   // Actually part of Sagitta : the arrow.
   vulpecula.add_star(OrdStar(299.69f, 19.49f, 3.51f, OrdStar::SPEC_K, ""));
   vulpecula.add_link(0, 3);
   vulpecula.add_link(1, 5);
   vulpecula.add_link(1, 10);
   vulpecula.add_link(2, 8);
   vulpecula.add_link(3, 4);
   vulpecula.add_link(3, 7);
   vulpecula.add_link(4, 10);
   vulpecula.add_link(5, 9);
   vulpecula.add_link(6, 9);
   vulpecula.add_link(7, 12);
   vulpecula.add_link(8, 9);
   vulpecula.add_link(9, 11);
   cons_list.push_back(vulpecula);
   tot_stars += vulpecula.total_stars();
   tot_links += vulpecula.total_links();
   }

Constellations::~Constellations() {
   }

unsigned int Constellations::numberOfWayPoints(void) const {
	return cons_list.size();
	}

LookOut Constellations::getView(unsigned int sequence) const {
    // Default view - in absence of any constellations - the
    // simulation's initial view.
    LookOut ret_val;

   // Assuming there is at least one constellation.
   if(cons_list.size() > 0) {
      if(sequence > (cons_list.size() - 1)) {
         sequence = cons_list.size() - 1;
         }

      Constellation current_con = cons_list.at(sequence);

      std::pair<float, float> cons_centroid = current_con.centre();

      VectorSP centroid = VectorSP(cons_centroid.first, cons_centroid.second, radius);

      Vector3D viewpoint = centroid - centroid.unit() * VIEW_OFFSET;

      ret_val.setPosition(viewpoint);
      ret_val.setFocus(centroid);
      ret_val.setOrientation(VIEW_UP);

      ret_val.addToDescription(current_con.name());

      const std::vector<std::string>& current_description = current_con.getDescription();

      for(unsigned int index = 0; index < current_description.size(); ++index) {
         ret_val.addToDescription(current_description[index]);
         }
      }

	return ret_val;
	}

void Constellations::cycleActivation(void) {
   // Mimic five state cycling switch.
   switch (current_cycle_state) {
      case ALL_OFF :
         current_cycle_state = STARS;
         activate();
         break;
      case STARS :
         current_cycle_state = STARS_N_LINKS;
         break;
      case STARS_N_LINKS :
         current_cycle_state = STARS_N_NAMES;
         break;
      case STARS_N_NAMES :
         current_cycle_state = ALL_ON;
         break;
      case ALL_ON :
         current_cycle_state = ALL_OFF;
         inactivate();
         break;
      default:
         ErrorHandler::record("GridGlobe::cycleActivation() - bad switch case reached (default)", ErrorHandler::FATAL);
         break;
      }
   }

void Constellations::prepare(SolarSystemGlobals::render_quality rq) {
   // Get the OpenGL buffer objects.
   buff_obj_points.acquire();
   buff_obj_indices.acquire();

   /// Preparations may depend upon the requested rendering quality level.
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST :
         loadVertexBuffer();
         loadIndexBuffer();
         createMarkerLists();
         break;
      default :
         // Ought not get here !!
         ErrorHandler::record("Constellations::prepare() - bad switch case reached (default)", ErrorHandler::FATAL);
         break;
      }
   }

void Constellations::release(void) {
   // Release the buffer objects' resources.
   buff_obj_points.release();
   buff_obj_indices.release();

	// Then the marker lists.
   for(std::vector<std::vector<GLuint> >::const_iterator lists = marker_lists.begin();
       lists != marker_lists.end();
       ++lists) {
      for(std::vector<GLuint>::const_iterator list = lists->begin();
          list != lists->end();
          ++list) {
         glDeleteLists(*list, 1);
         }
      }

   // This destroys any contained vectors too ...
   marker_lists.clear();
   }

void Constellations::render(void) {
   // Provided we should be showing anything at all.
   if(current_cycle_state != ALL_OFF) {
      // Plot the stars first ie. a color at a point.
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());
      glEnableClientState(GL_COLOR_ARRAY);

      // Using interleaved color/position data
      glInterleavedArrays(GL_C3F_V3F, 0, BUFFER_OFFSET(0));

      glPointSize(Constellations::MAG_SIZE);
      glDrawArrays(GL_POINTS, 0, tot_stars);

      // Not using color values now.
      glDisableClientState(GL_COLOR_ARRAY);

      // Provided we show the links
      if((current_cycle_state == STARS_N_LINKS) || (current_cycle_state == ALL_ON)) {
         // The GL_ELEMENT_ARRAY_BUFFER target will be serviced by the other buffer.
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_indices.ID());

         // OK, set up the vertex array within.
         glVertexPointer(COORDS_PER_VERTEX,
                         GL_FLOAT,
                         BYTE_STRIDE_PER_VERTEX*sizeof(GL_FLOAT),
                         BUFFER_OFFSET(COLORS_PER_VERTEX*sizeof(GL_FLOAT)));

         glColor3f(LINK_RGB_RED, LINK_RGB_GREEN, LINK_RGB_BLUE);
         glLineWidth(LINK_WIDTH);
         glLineStipple(LINK_STIPPLE_FACTOR, LINK_STIPPLE_PATTERN);
         glDrawElements(GL_LINES, tot_links*INDICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
         }

      // Disable various capabilities from the OpenGL state machine.
      glDisableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);

      // Provided we show the names.
      if((current_cycle_state == STARS_N_NAMES) || (current_cycle_state == ALL_ON)) {
         // Now print the names at the centroids.
         glEnable(GL_TEXTURE_2D);
         glDisable(GL_CULL_FACE);

         // Go through and call every call list.
         for(std::vector<std::vector<GLuint> >::const_iterator lists = marker_lists.begin();
             lists != marker_lists.end();
             ++lists) {
            // Only call the first, as others are called by it.
            glCallList((*lists)[0]);
            }

         glDisable(GL_TEXTURE_2D);
         }
      }
   }

void Constellations::class_color(OrdStar::spectral_type spectral_class, Vert* vt) {
   switch (spectral_class) {
      case OrdStar::SPEC_A :
         // Hottish and white.
         (*vt).col.red = 1.0f;
         (*vt).col.green = 1.0f;
         (*vt).col.blue = 1.0f;
         break;
      case OrdStar::SPEC_B :
         // Quite hot and blue.
      	(*vt).col.red = 0.65f;
      	(*vt).col.green = 0.85f;
      	(*vt).col.blue = 0.95f;
         break;
      case OrdStar::SPEC_F :
         // Bit hotter than Sol and yellowish white.
      	(*vt).col.red = 1.00f;
      	(*vt).col.green = 0.95f;
      	(*vt).col.blue = 0.75f;
         break;
      case OrdStar::SPEC_G :
         // Like our Sun and yellow.
      	(*vt).col.red = 0.95f;
      	(*vt).col.green = 0.90f;
      	(*vt).col.blue = 0.0f;
         break;
      case OrdStar::SPEC_K :
         // Lukewarm and orange.
      	(*vt).col.red = 0.95f;
      	(*vt).col.green = 0.65f;
      	(*vt).col.blue = 0.15f;
         break;
      case OrdStar::SPEC_M :
         // Quite cool and deep red.
      	(*vt).col.red = 1.00f;
      	(*vt).col.green = 0.05f;
      	(*vt).col.blue = 0.05f;
         break;
      case OrdStar::SPEC_O :
         // Cracking hot and very blue.
      	(*vt).col.red = 0.05f;
      	(*vt).col.green = 0.05f;
      	(*vt).col.blue = 1.00f;
         break;
      default:
         // Ought not get here !!
         std::string msg = "Constellations::class_color() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   return;
   }

void Constellations::loadVertexBuffer(void) {
   // We need to look at all constellations and create a consecutive
   // listing of colors and positions for all stars to be rendered.

	// First create a heap based map of the data.
	// Total buffer size is the storage for color/vertex interleaved data.
   GLsizeiptr buffer_size = sizeof(Vert) * tot_stars;

   Vert* buffer_base_ptr = new Vert[tot_stars];
   Vert* buffer_vert_ptr = buffer_base_ptr;

   // Work through the constellations one by one.
	for(std::vector<Constellation>::const_iterator cs = cons_list.begin();
		 cs < cons_list.end();
		 cs++ ) {
		// Process the color and location of the stars within this
		// constellation. Get access to the star list.
		const std::vector<OrdStar>& star_list = cs->stars();
		for(std::vector<OrdStar>::const_iterator st = star_list.begin();
		st < star_list.end();
		st++ ) {
			// Take the star's spectral type and convert that to RGB color values.
			class_color(st->spectral_class(), buffer_vert_ptr);

			// This is the current star with spherical polar co-ordinates.
			VectorSP c_star = VectorSP(st->right_ascension(), st->declination(), radius);
			// But Cartesian co-ordinates will be stored in the buffer.
			buffer_vert_ptr->x_pos = c_star.x();
			buffer_vert_ptr->y_pos = c_star.y();
			buffer_vert_ptr->z_pos = c_star.z();

			++buffer_vert_ptr;
			}
		}

	buff_obj_points.loadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);

   // Free the heap.
   delete[] buffer_base_ptr;
   }

void Constellations::loadIndexBuffer(void) {
   // We need to look at all constellations and create a consecutive
   // listing of star indices for all links to be rendered. Obviously
   // there are two star indices per link, where each link will be
   // represented as a line in the render() routine.

	// First create a heap based map of the data.
	// Total buffer size is the storage for index data.
	GLsizeiptr buffer_size = sizeof(unsigned int) * INDICES_PER_LINK * tot_links;

	unsigned int* buffer_base_ptr = new unsigned int[buffer_size];
	unsigned int* buffer_index_ptr = buffer_base_ptr;

	// Need to keep track of the indices with respect to the entire set of constellations.
	unsigned int link_base_index = 0;
	// Now go through all the constellations.
	for(std::vector<Constellation>::const_iterator cs = cons_list.begin();
		 cs < cons_list.end();
		 cs++ ) {
		// Process the index data within this constellation. Get access to the
		// index list.
		const std::vector< std::pair<unsigned int, unsigned int> >& link_list = cs->links();
		// Beware of an 'empty' constellation.
		GLuint total_stars_this_con = cs->total_stars();

		if(total_stars_this_con != 0) {
			for(std::vector< std::pair<unsigned int, unsigned int> >::const_iterator lk = link_list.begin();
			lk < link_list.end();
			lk++ ) {
				// For each linkage, store the indices at two per link.
				// Store indices in the buffer, but with the indices relative to
				// entire set of stars that all the constellations represent.
				// Need to check that any index within a constellation does not
				// go out of bounds with respect to the number of stars in said
				// constellation. The link indices are unsigned so only need to
				// check upper index bound.
				if((lk->first < total_stars_this_con) && (lk->first < total_stars_this_con)) {
					*(buffer_index_ptr) = link_base_index + lk->first;
					buffer_index_ptr++;
					*(buffer_index_ptr) = link_base_index + lk->second;
					buffer_index_ptr++;
					}
				else {
					// Doesn't need to be FATAL but ought be noted.
					std::stringstream msg;
					msg << "Constellations::loadIndexBuffer() - bad link for ";
					msg << cs->name();
					msg << " : ";
					msg << lk->first;
					msg << " <-> ";
					msg << lk->second;
					ErrorHandler::record(msg.str(), ErrorHandler::WARN);
					}
				}
			// Update the index into all stars within all constellations.
			link_base_index += total_stars_this_con;
			}
		else {
			// Doesn't need to be FATAL but ought be noted.
			std::stringstream msg;
			msg << "Constellations::loadIndexBuffer() - empty constellation : ";
			msg << cs->name();
			ErrorHandler::record(msg.str(), ErrorHandler::WARN);
			}
		}

	buff_obj_indices.loadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, buffer_size, buffer_base_ptr);

	// Free the heap.
   delete[] buffer_base_ptr;
   }

void Constellations::createMarkerLists(void) {
	// Get the OGLFT font for this class.
   OGLFT_ft* myFont = this->getFont();

   // This destroys any contained vectors too ...
   marker_lists.clear();

   // An independent scaling.
   GLfloat text_scale_factor = radius/TEXT_RATIO;

   // Work through the constellations one by one.
   for(std::vector<Constellation>::iterator cs = cons_list.begin();
       cs < cons_list.end();
       cs++) {
   	// A temporary STL vector for populating with display list ID's.
      std::vector<GLuint> temp;

      // The constellation's name.
      std::string con_name = cs->name();

      // The co-ordinates of the centroid of the constellation.
      std::pair<GLfloat, GLfloat> con_centre = cs->centre();

      // A display list for the constellation's name, OGLFT constructs this for us.
      GLuint cons_draw_ID = myFont->compile(con_name.c_str());

      // Ask OGL for a display list ID that will represent the transform from the origin.
      GLuint transform_ID = glGenLists(1);

      // Now create and compile that list.
      glNewList(transform_ID, GL_COMPILE);
         // Isolate the transforms.
         glPushMatrix();
            // Transforms etc ... in reverse order if "world space" description.

            // Rotate to desired right ascension.
            // Rotate anti-clockwise, seen looking from +ve z-axis to origin.
            glRotatef(con_centre.first, 0, 0, 1);

            // Rotate to desired declination.
            // Rotate anti-clockwise, seen looking from -ve y-axis to origin.
            glRotatef(con_centre.second, 0, -1, 0);

            // Shift out to grid radius nearby the First Point of Aries.
            glTranslatef(radius, 0, 0);

            // Rotate 90 degrees clockwise, seen looking from +ve y-axis to origin.
            glRotatef(-90, 0, 1, 0);

            // Rotate 90 degrees clockwise, seen looking from +ve z-axis to origin.
            glRotatef(-90, 0, 0, 1);

            // When lying on the x-y plane, expand the text.
            glScalef(text_scale_factor, text_scale_factor, 1);

            // Ascertain the dimensions of the bounding box for the entire constellation name string.
            OGLFT::BBox con_box = myFont->measure(con_name.c_str());

            // Place one-half of rendered string length away from the centroid
            glTranslatef(-con_box.x_max_/2, 0, 0);

            glCallList(cons_draw_ID);
         // Restore the prior transform state.
         glPopMatrix();
      glEndList();

		// Store the diplay list ID for the transform followed by that which does the drawing.
      temp.push_back(transform_ID);
      temp.push_back(cons_draw_ID);

      // Make an entry for this constellation in a master list.
      marker_lists.push_back(temp);
      }
   }
