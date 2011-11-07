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

#include "Constellations.h"

const GLfloat Constellations::OFFSET(-20.0f);
const GLfloat Constellations::LINK_WIDTH(1.0f);
const GLushort Constellations::LINK_STIPPLE_PATTERN(0x8888);
const GLint Constellations::LINK_STIPPLE_FACTOR(3.0f);
const GLfloat Constellations::LINK_RGB_RED(0.06f);
const GLfloat Constellations::LINK_RGB_GREEN(0.22f);
const GLfloat Constellations::LINK_RGB_BLUE(0.32f);

const GLfloat Constellations::MAG_SIZE(2.0f);

const GLuint Constellations::INDICES_PER_LINK(2);
const GLuint Constellations::COLORS_PER_VERTEX(3);
const GLuint Constellations::COORDS_PER_VERTEX(3);
const GLuint Constellations::BYTE_STRIDE_PER_VERTEX(Constellations::COLORS_PER_VERTEX + Constellations::COORDS_PER_VERTEX);

Constellations::Constellations(vec_t rad) : c_rad(rad) {
   // TODO - There's a HUGE 'BEWARE' with this method. The link numbers are used as
   // indices to a vector that stores the star data AS PER THE LISTED ORDER.
   // Do not exceed bounds.

   Constellation andromeda("Andromeda");
   andromeda.add_star(OrdStar(2.006436f, -29.090833f, 2.07f, OrdStar::SPEC_B, "Alpheratz"));
   andromeda.add_star(OrdStar(17.262167f, -35.620833f, 2.07f, OrdStar::SPEC_M, "Mirach"));
   andromeda.add_star(OrdStar(30.764978f, -42.329861f, 2.10f, OrdStar::SPEC_B, "Almach"));
   andromeda.add_star(OrdStar(9.755444f, -30.861222f, 3.27f, OrdStar::SPEC_K, "Delta"));
   andromeda.add_star(OrdStar(24.266528f, -48.628500f, 3.59f, OrdStar::SPEC_K, "Nembus"));
   andromeda.add_star(OrdStar(345.265347f, -42.325972f, 3.62f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(354.259364f, -46.459167f, 3.81f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(14.012528f, -38.499250f, 3.86f, OrdStar::SPEC_A, ""));
   andromeda.add_star(OrdStar(11.755664f, -24.267389f, 4.08f, OrdStar::SPEC_K, ""));
   andromeda.add_star(OrdStar(24.013328f, -41.406389f, 4.10f, OrdStar::SPEC_F, ""));
   andromeda.add_star(OrdStar(355.006789f, -44.333972f, 4.15f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(17.258367f, -47.241833f, 4.26f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(354.502272f, -43.268083f, 4.29f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(9.014678f, -33.719361f, 4.34f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(9.509306f, -29.312361f, 4.34f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(14.253453f, -23.417750f, 4.40f, OrdStar::SPEC_G, ""));
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
   antlia.add_star(OrdStar(142.254094f, -35.951361f, 4.51f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(156.752544f, -31.067806f, 4.28f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(164.011944f, -37.137472f, 4.60f, OrdStar::SPEC_K, ""));
   antlia.add_link(0, 1);
   antlia.add_link(1, 2);
   cons_list.push_back(antlia);
   tot_stars += antlia.total_stars();
   tot_links += antlia.total_links();

   Constellation apus("Apus");
   apus.add_star(OrdStar(221.764369f, 78.955278f, 3.83f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(248.257628f, 77.103028f, 3.86f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(250.751506f, 76.483417f, 4.23f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(245.005789f, 77.304333f, 4.68f, OrdStar::SPEC_M, ""));
   apus.add_link(0, 3);
   apus.add_link(1, 3);
   apus.add_link(1, 2);
   cons_list.push_back(apus);
   tot_stars += apus.total_stars();
   tot_links += apus.total_links();

   Constellation aquarius("Aquarius");
   aquarius.add_star(OrdStar(322.759311f, 4.428833f, 2.90f, OrdStar::SPEC_G, "Sadalsuud"));
   aquarius.add_star(OrdStar(331.263064f, -0.319833f, 2.95f, OrdStar::SPEC_G, "Sadalmelik"));
   aquarius.add_star(OrdStar(343.510844f, 14.179250f, 3.27f, OrdStar::SPEC_A, "Skat"));
   aquarius.add_star(OrdStar(337.013833f, -0.020056f, 3.65f, OrdStar::SPEC_F, "Sadaltager"));
   aquarius.add_star(OrdStar(347.257433f, 20.827528f, 3.68f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(343.010239f, 6.420333f, 3.73f, OrdStar::SPEC_M, "Hydor"));
   aquarius.add_star(OrdStar(311.761258f, 8.504306f, 3.78f, OrdStar::SPEC_A, "Albali"));
   aquarius.add_star(OrdStar(335.260917f, 0.612639f, 3.86f, OrdStar::SPEC_A, "Sadachbia"));
   aquarius.add_star(OrdStar(350.516194f, 19.899667f, 3.96f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(338.755925f, -0.117361f, 4.04f, OrdStar::SPEC_B, ""));
   aquarius.add_link(0, 1);
   aquarius.add_link(0, 6);
   aquarius.add_link(1, 7);
   aquarius.add_link(2, 4);
   aquarius.add_link(2, 8);
   aquarius.add_link(3, 7);
   aquarius.add_link(5, 8);
   aquarius.add_link(5, 9);
   cons_list.push_back(aquarius);
   tot_stars += aquarius.total_stars();
   tot_links += aquarius.total_links();

   Constellation aquila("Aquila");
   aquila.add_star(OrdStar(297.512967f, -8.867389f, 0.76f, OrdStar::SPEC_A, "Altair"));
   aquila.add_star(OrdStar(296.504325f, -10.613278f, 2.72f, OrdStar::SPEC_K, "Tarazed"));
   aquila.add_star(OrdStar(286.256836f, -13.863722f, 2.99f, OrdStar::SPEC_A, "Deneb"));
   aquila.add_star(OrdStar(302.755072f, -0.821472f, 3.24f, OrdStar::SPEC_B, "Tseen Foo"));
   aquila.add_star(OrdStar(291.258264f, -3.114583f, 3.36f, OrdStar::SPEC_F, "Denebokab"));
   aquila.add_star(OrdStar(286.504153f, -4.882333f, 3.43f, OrdStar::SPEC_B, "Al Thalimain"));
   aquila.add_star(OrdStar(298.755214f, -6.407944f, 3.71f, OrdStar::SPEC_G, "Alshain"));
   aquila.add_star(OrdStar(298.007878f, -1.005667f, 3.87f, OrdStar::SPEC_F, ""));
   aquila.add_link(0, 1);
   aquila.add_link(0, 2);
   aquila.add_link(0, 6);
   aquila.add_link(2, 4);
   aquila.add_link(3, 6);
   aquila.add_link(3, 7);
   aquila.add_link(4, 5);
   aquila.add_link(4, 7);
   cons_list.push_back(aquila);
   tot_stars += aquila.total_stars();
   tot_links += aquila.total_links();

   Constellation ara("Ara");
   ara.add_star(OrdStar(261.255000f, 54.470167f, 2.84f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(262.764033f, 48.124028f, 2.84f, OrdStar::SPEC_B, "Choo"));
   ara.add_star(OrdStar(254.510342f, 54.009944f, 3.12f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(261.256572f, 55.622306f, 3.31f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(262.751661f, 59.316389f, 3.60f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(271.510522f, 49.908556f, 3.65f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(252.263086f, 58.958694f, 3.77f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(254.759736f, 52.839500f, 4.06f, OrdStar::SPEC_K, ""));
   ara.add_link(0, 1);
   ara.add_link(0, 2);
   ara.add_link(2, 6);
   ara.add_link(2, 7);
   ara.add_link(3, 4);
   cons_list.push_back(ara);
   tot_stars += ara.total_stars();
   tot_links += ara.total_links();

   Constellation aries("Aries");
   aries.add_star(OrdStar(31.752858f, -23.462778f, 2.01f, OrdStar::SPEC_K, "Hamal"));
   aries.add_star(OrdStar(28.510653f, -20.808306f, 2.64f, OrdStar::SPEC_A, "Sheratan"));
   aries.add_star(OrdStar(42.266386f, -27.260778f, 3.61f, OrdStar::SPEC_B, "Bharani"));
   aries.add_star(OrdStar(28.258825f, -19.294083f, 3.88f, OrdStar::SPEC_B, "Mesarthim"));
   aries.add_star(OrdStar(47.760464f, -19.726694f, 4.35f, OrdStar::SPEC_K, "Botein"));
   aries.add_link(0, 1);
   aries.add_link(0, 2);
   aries.add_link(1, 3);
   aries.add_link(2, 4);
   cons_list.push_back(aries);
   tot_stars += aries.total_stars();
   tot_links += aries.total_links();

   Constellation auriga("Auriga");
   auriga.add_star(OrdStar(79.011472f, -45.999028f, 0.08f, OrdStar::SPEC_G, "Capella"));
   auriga.add_star(OrdStar(89.758825f, -44.947444f, 1.90f, OrdStar::SPEC_A, "Menkalinan"));
   auriga.add_star(OrdStar(89.762011f, -37.212778f, 2.65f, OrdStar::SPEC_A, "Mahasim"));
   auriga.add_star(OrdStar(74.016561f, -33.166139f, 2.69f, OrdStar::SPEC_K, "Kabdhilinan"));
   auriga.add_star(OrdStar(75.266147f, -43.823306f, 3.03f, OrdStar::SPEC_F, "Almaaz"));
   auriga.add_star(OrdStar(76.508575f, -41.234639f, 3.18f, OrdStar::SPEC_B, "Haedus""II"));
   auriga.add_star(OrdStar(75.507967f, -41.075889f, 3.69f, OrdStar::SPEC_K, "Sadatoni"));
   auriga.add_star(OrdStar(89.758764f, -54.284972f, 3.72f, OrdStar::SPEC_K, "Prijipati"));
   auriga.add_link(0, 1);
   auriga.add_link(0, 4);
   auriga.add_link(1, 2);
   auriga.add_link(3, 6);
   auriga.add_link(4, 5);
   auriga.add_link(5, 6);
   cons_list.push_back(auriga);
   tot_stars += auriga.total_stars();
   tot_links += auriga.total_links();

   Constellation bootes("Bootes");
   bootes.add_star(OrdStar(213.761208f, -19.187278f, -0.05f, OrdStar::SPEC_K, "Arcturus"));
   bootes.add_star(OrdStar(221.016458f, -27.074167f, 2.35f, OrdStar::SPEC_A, "Izar"));
   bootes.add_star(OrdStar(208.511422f, -18.398583f, 2.68f, OrdStar::SPEC_G, "Muphrid"));
   bootes.add_star(OrdStar(218.001322f, -38.307889f, 3.04f, OrdStar::SPEC_A, "Seginus"));
   bootes.add_star(OrdStar(228.758361f, -33.315111f, 3.46f, OrdStar::SPEC_G, ""));
   bootes.add_star(OrdStar(225.265775f, -40.390639f, 3.49f, OrdStar::SPEC_G, "Nekkar"));
   bootes.add_star(OrdStar(217.763850f, -30.371139f, 3.57f, OrdStar::SPEC_K, ""));
   bootes.add_link(0, 1);
   bootes.add_link(0, 2);
   bootes.add_link(0, 6);
   bootes.add_link(1, 4);
   bootes.add_link(3, 5);
   bootes.add_link(3, 6);
   bootes.add_link(4, 5);
   cons_list.push_back(bootes);
   tot_stars += bootes.total_stars();
   tot_links += bootes.total_links();

   Constellation caelum("Caelum");
   caelum.add_star(OrdStar(70.009394f, 40.136417f, 4.44f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(76.006753f, 34.517139f, 4.55f, OrdStar::SPEC_K, ""));
   caelum.add_star(OrdStar(70.500958f, 37.144778f, 5.04f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(67.513917f, 44.953750f, 5.07f, OrdStar::SPEC_B, ""));
   caelum.add_link(0, 2);
   caelum.add_link(0, 3);
   caelum.add_link(1, 2);
   cons_list.push_back(caelum);
   tot_stars += caelum.total_stars();
   tot_links += caelum.total_links();

   Constellation camelopardalis("Camelopardalis");
   camelopardalis.add_star(OrdStar(75.756972f, -60.442278f, 4.03f, OrdStar::SPEC_G, ""));
   camelopardalis.add_star(OrdStar(52.251147f, -59.940333f, 4.21f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(73.500836f, -66.342667f, 4.26f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(57.258692f, -65.526000f, 4.39f, OrdStar::SPEC_M, ""));
   camelopardalis.add_star(OrdStar(74.254783f, -53.752083f, 4.43f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(105.001069f, -76.977444f, 4.55f, OrdStar::SPEC_K, ""));
   camelopardalis.add_star(OrdStar(57.505967f, -71.332361f, 4.59f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(94.514106f, -69.320028f, 4.76f, OrdStar::SPEC_A, ""));
   camelopardalis.add_link(0, 2);
   camelopardalis.add_link(0, 3);
   camelopardalis.add_link(0, 4);
   camelopardalis.add_link(1, 3);
   camelopardalis.add_link(2, 5);
   camelopardalis.add_link(2, 6);
   camelopardalis.add_link(3, 6);
   camelopardalis.add_link(5, 7);
   cons_list.push_back(camelopardalis);
   tot_stars += camelopardalis.total_stars();
   tot_links += camelopardalis.total_links();

   Constellation cancer("Cancer");
   cancer.add_star(OrdStar(124.008597f, -9.185667f, 3.53f, OrdStar::SPEC_K, "Al Tarf"));
   cancer.add_star(OrdStar(131.011419f, -18.154861f, 3.94f, OrdStar::SPEC_K, "Asellus Australis"));
   cancer.add_star(OrdStar(131.511619f, -28.760000f, 4.03f, OrdStar::SPEC_G, ""));
   cancer.add_star(OrdStar(134.508111f, -11.857778f, 4.26f, OrdStar::SPEC_A, "Acubens"));
   cancer.add_star(OrdStar(130.754781f, -21.468583f, 4.66f, OrdStar::SPEC_A, "Asellus Borealis"));
   cancer.add_star(OrdStar(123.003531f, -17.648139f, 4.67f, OrdStar::SPEC_G, "Tegmine"));
   cancer.add_link(0, 1);
   cancer.add_link(1, 3);
   cancer.add_link(1, 4);
   cancer.add_link(2, 4);
   cons_list.push_back(cancer);
   tot_stars += cancer.total_stars();
   tot_links += cancer.total_links();

   Constellation canes_venatici("Canes Venatici");
   canes_venatici.add_star(OrdStar(194.000511f, -38.318250f, 2.89f, OrdStar::SPEC_A, "Cor Caroli"));
   canes_venatici.add_star(OrdStar(188.262525f, -41.356778f, 4.24f, OrdStar::SPEC_G, "Chara"));
   canes_venatici.add_link(0, 1);
   cons_list.push_back(canes_venatici);
   tot_stars += canes_venatici.total_stars();
   tot_links += canes_venatici.total_links();

   Constellation canis_major("Canis Major");
   canis_major.add_star(OrdStar(101.252569f, 16.713139f, 1.46f, OrdStar::SPEC_A, "Sirius"));
   canis_major.add_star(OrdStar(104.510431f, 28.972083f, 1.50f, OrdStar::SPEC_B, "Adhara"));
   canis_major.add_star(OrdStar(107.006525f, 26.393194f, 1.83f, OrdStar::SPEC_F, "Wezen"));
   canis_major.add_star(OrdStar(95.511664f, 17.955917f, 1.98f, OrdStar::SPEC_B, "Mirzam"));
   canis_major.add_star(OrdStar(111.001586f, 29.303111f, 2.45f, OrdStar::SPEC_B, "Aludra"));
   canis_major.add_star(OrdStar(95.005219f, 30.063389f, 3.02f, OrdStar::SPEC_B, "Furud"));
   canis_major.add_star(OrdStar(105.750408f, 23.833306f, 3.02f, OrdStar::SPEC_B, ""));
   canis_major.add_star(OrdStar(105.261986f, 27.934833f, 3.49f, OrdStar::SPEC_K, ""));
   canis_major.add_link(0, 3);
   canis_major.add_link(0, 6);
   canis_major.add_link(1, 5);
   canis_major.add_link(1, 7);
   canis_major.add_link(2, 4);
   canis_major.add_link(2, 6);
   canis_major.add_link(2, 7);
   cons_list.push_back(canis_major);
   tot_stars += canis_major.total_stars();
   tot_links += canis_major.total_links();

   Constellation canis_minor("Canis Minor");
   canis_minor.add_star(OrdStar(114.755150f, -5.227500f, 0.34f, OrdStar::SPEC_F, "Procyon"));
   canis_minor.add_star(OrdStar(111.752519f, -8.289417f, 2.89f, OrdStar::SPEC_B, "Gomeisa"));
   canis_minor.add_link(0, 1);
   cons_list.push_back(canis_minor);
   tot_stars += canis_minor.total_stars();
   tot_links += canis_minor.total_links();

   Constellation capricornus("Capricornus");
   capricornus.add_star(OrdStar(326.750636f, 16.126556f, 2.85f, OrdStar::SPEC_A, "Deneb"));
   capricornus.add_star(OrdStar(305.250181f, 14.781389f, 3.05f, OrdStar::SPEC_A, "Dabih"));
   capricornus.add_star(OrdStar(304.500894f, 12.544861f, 3.58f, OrdStar::SPEC_G, "Algedi Secunda"));
   capricornus.add_star(OrdStar(325.001483f, 16.662250f, 3.69f, OrdStar::SPEC_A, "Nashira"));
   capricornus.add_star(OrdStar(321.511119f, 22.411389f, 3.77f, OrdStar::SPEC_G, ""));
   capricornus.add_star(OrdStar(316.265772f, 16.767278f, 4.08f, OrdStar::SPEC_A, ""));
   capricornus.add_star(OrdStar(312.763694f, 26.919139f, 4.12f, OrdStar::SPEC_K, "Baten Algiedi"));
   capricornus.add_star(OrdStar(311.501603f, 25.270528f, 4.13f, OrdStar::SPEC_F, "Yue"));
   capricornus.add_star(OrdStar(320.504106f, 16.834556f, 4.28f, OrdStar::SPEC_G, ""));
   capricornus.add_link(0, 3);
   capricornus.add_link(0, 4);
   capricornus.add_link(1, 2);
   capricornus.add_link(1, 5);
   capricornus.add_link(1, 7);
   capricornus.add_link(3, 8);
   capricornus.add_link(4, 6);
   capricornus.add_link(5, 8);
   capricornus.add_link(6, 7);
   cons_list.push_back(capricornus);
   tot_stars += capricornus.total_stars();
   tot_links += capricornus.total_links();

   Constellation carina("Carina");
   carina.add_star(OrdStar(95.765858f, 51.304278f, -0.72f, OrdStar::SPEC_F, "Canopus"));
   carina.add_star(OrdStar(138.253400f, 68.282528f, 1.67f, OrdStar::SPEC_A, "Miaplacidus"));
   carina.add_star(OrdStar(125.508572f, 58.490472f, 1.86f, OrdStar::SPEC_K, "Avior"));
   carina.add_star(OrdStar(139.251508f, 58.724750f, 2.21f, OrdStar::SPEC_A, "Aspidiske"));
   carina.add_star(OrdStar(160.515953f, 63.605528f, 2.74f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(146.751706f, 64.927972f, 2.92f, OrdStar::SPEC_A, ""));
   carina.add_star(OrdStar(153.262300f, 69.962083f, 3.29f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(158.000411f, 60.314639f, 3.30f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(154.251392f, 60.667667f, 3.39f, OrdStar::SPEC_K, ""));
   carina.add_link(0, 2);
   carina.add_link(1, 5);
   carina.add_link(1, 6);
   carina.add_link(2, 3);
   carina.add_link(3, 5);
   carina.add_link(3, 8);
   cons_list.push_back(carina);
   tot_stars += carina.total_stars();
   tot_links += carina.total_links();

   Constellation cassiopeia("Cassiopeia");
   cassiopeia.add_star(OrdStar(14.011806f, -60.716750f, 2.15f, OrdStar::SPEC_B, "Tsih"));
   cassiopeia.add_star(OrdStar(10.008442f, -56.537417f, 2.24f, OrdStar::SPEC_K, "Schedar"));
   cassiopeia.add_star(OrdStar(2.252803f, -59.150222f, 2.28f, OrdStar::SPEC_F, "Caph"));
   cassiopeia.add_star(OrdStar(21.263500f, -60.235417f, 2.66f, OrdStar::SPEC_A, "Ruchbah"));
   cassiopeia.add_star(OrdStar(28.506578f, -63.670139f, 3.35f, OrdStar::SPEC_B, "Segin"));
   cassiopeia.add_star(OrdStar(12.251417f, -57.816556f, 3.46f, OrdStar::SPEC_G, "Achird"));
   cassiopeia.add_star(OrdStar(9.016186f, -53.896944f, 3.69f, OrdStar::SPEC_B, ""));
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
   centaurus.add_star(OrdStar(219.761361f, 59.164861f, -0.01f, OrdStar::SPEC_G, "Rigil Kentaurus"));
   centaurus.add_star(OrdStar(210.763733f, 59.627028f, 0.61f, OrdStar::SPEC_B, "Hadar"));
   centaurus.add_star(OrdStar(211.511478f, 35.631306f, 2.06f, OrdStar::SPEC_K, "Menkent"));
   centaurus.add_star(OrdStar(190.258667f, 47.040111f, 2.20f, OrdStar::SPEC_A, "Muhlifain"));
   centaurus.add_star(OrdStar(204.764797f, 52.533639f, 2.29f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(218.758458f, 41.842250f, 2.33f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(208.759008f, 46.711722f, 2.55f, OrdStar::SPEC_B, "Alnair"));
   centaurus.add_star(OrdStar(200.010019f, 35.287917f, 2.75f, OrdStar::SPEC_A, ""));
   centaurus.add_star(OrdStar(182.005983f, 49.277583f, 2.58f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(224.752694f, 41.895861f, 3.13f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(207.260281f, 41.526306f, 3.47f, OrdStar::SPEC_B, ""));
   centaurus.add_link(0, 1);
   centaurus.add_link(1, 4);
   centaurus.add_link(2, 5);
   centaurus.add_link(2, 10);
   centaurus.add_link(3, 4);
   centaurus.add_link(3, 7);
   centaurus.add_link(4, 6);
   centaurus.add_link(5, 9);
   centaurus.add_link(6, 10);
   centaurus.add_link(8, 10);
   cons_list.push_back(centaurus);
   tot_stars += centaurus.total_stars();
   tot_links += centaurus.total_links();

   Constellation cepheus("Cepheus");
   cepheus.add_star(OrdStar(319.509606f, -62.585444f, 2.45f, OrdStar::SPEC_A, "Alderamin"));
   cepheus.add_star(OrdStar(354.755828f, -77.631972f, 3.21f, OrdStar::SPEC_K, "Arrai"));
   cepheus.add_star(OrdStar(322.010994f, -70.560694f, 3.23f, OrdStar::SPEC_B, "Alfirk"));
   cepheus.add_star(OrdStar(332.514239f, -58.201250f, 3.39f, OrdStar::SPEC_K, "Tsao Fu"));
   cepheus.add_star(OrdStar(311.254797f, -61.836806f, 3.41f, OrdStar::SPEC_K, ""));
   cepheus.add_star(OrdStar(342.261364f, -66.200722f, 3.50f, OrdStar::SPEC_K, ""));
   cepheus.add_star(OrdStar(337.252847f, -58.415194f, 4.07f, OrdStar::SPEC_G, "Alrediph"));
   cepheus.add_link(0, 2);
   cepheus.add_link(0, 3);
   cepheus.add_link(0, 4);
   cepheus.add_link(1, 2);
   cepheus.add_link(3, 6);
   cepheus.add_link(5, 6);
   cons_list.push_back(cepheus);
   tot_stars += cepheus.total_stars();
   tot_links += cepheus.total_links();

   Constellation cetus("Cetus");
   cetus.add_star(OrdStar(10.759786f, 16.013306f, 2.04f, OrdStar::SPEC_K, "Deneb Kaitos"));
   cetus.add_star(OrdStar(45.504661f, -4.089917f, 2.54f, OrdStar::SPEC_M, "Menkar"));
   cetus.add_star(OrdStar(34.755775f, 1.022944f, 3.04f, OrdStar::SPEC_M, "Mira Ceti"));
   cetus.add_star(OrdStar(17.009794f, 9.818083f, 3.46f, OrdStar::SPEC_K, "Deneb Algenubi"));
   cetus.add_star(OrdStar(40.755033f, -3.236167f, 3.47f, OrdStar::SPEC_A, "Kaffaljidhmah"));
   cetus.add_star(OrdStar(26.001425f, 14.060444f, 3.49f, OrdStar::SPEC_G, "Durre Menthor"));
   cetus.add_star(OrdStar(4.757133f, 7.176167f, 3.56f, OrdStar::SPEC_K, "Schemali"));
   cetus.add_star(OrdStar(21.000403f, 7.817250f, 3.60f, OrdStar::SPEC_K,  ""));
   cetus.add_star(OrdStar(27.757669f, 9.665056f, 3.74f, OrdStar::SPEC_K, "Baten Kaitos"));
   cetus.add_star(OrdStar(30.000061f, 20.922222f, 3.99f, OrdStar::SPEC_M,  ""));
   cetus.add_star(OrdStar(39.758042f, -0.328528f, 4.08f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(41.015658f, -10.114222f, 4.27f, OrdStar::SPEC_F,  ""));
   cetus.add_star(OrdStar(37.002644f, -8.460083f, 4.30f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(44.761917f, -8.907389f, 4.71f, OrdStar::SPEC_B, "Menkar"));
   cetus.add_star(OrdStar(38.764581f, -5.593306f, 4.87f, OrdStar::SPEC_G,  ""));
   cetus.add_link(0, 3);
   cetus.add_link(0, 6);
   cetus.add_link(1, 4);
   cetus.add_link(1, 13);
   cetus.add_link(2, 8);
   cetus.add_link(2, 10);
   cetus.add_link(3, 6);
   cetus.add_link(3, 7);
   cetus.add_link(4, 10);
   cetus.add_link(4, 14);
   cetus.add_link(5, 8);
   cetus.add_link(5, 9);
   cetus.add_link(7, 8);
   cetus.add_link(11, 12);
   cetus.add_link(11, 13);
   cetus.add_link(12, 14);
   cons_list.push_back(cetus);
   tot_stars += cetus.total_stars();
   tot_links += cetus.total_links();

   Constellation chamaeleon("Chamaeleon");
   chamaeleon.add_star(OrdStar(124.508686f, 75.080028f, 4.05f, OrdStar::SPEC_F, ""));
   chamaeleon.add_star(OrdStar(158.757839f, 77.392194f, 4.11f, OrdStar::SPEC_M, ""));
   chamaeleon.add_star(OrdStar(184.505817f, 78.687722f, 4.24f, OrdStar::SPEC_B, ""));
   chamaeleon.add_star(OrdStar(125.010803f, 76.515417f, 4.34f, OrdStar::SPEC_K, ""));
   chamaeleon.add_star(OrdStar(161.254550f, 79.530472f, 5.46f, OrdStar::SPEC_K, ""));
   chamaeleon.add_link(0, 1);
   chamaeleon.add_link(0, 3);
   chamaeleon.add_link(1, 2);
   chamaeleon.add_link(1, 4);
   cons_list.push_back(chamaeleon);
   tot_stars += chamaeleon.total_stars();
   tot_links += chamaeleon.total_links();

   Constellation circinus("Circinus");
   circinus.add_star(OrdStar(220.508525f, 63.025417f, 3.18f, OrdStar::SPEC_F, ""));
   circinus.add_star(OrdStar(229.258600f, 57.199111f, 4.07f, OrdStar::SPEC_A, ""));
   circinus.add_star(OrdStar(230.756294f, 58.679306f, 4.48f, OrdStar::SPEC_B, ""));
   circinus.add_link(0, 1);
   circinus.add_link(0, 2);
   cons_list.push_back(circinus);
   tot_stars += circinus.total_stars();
   tot_links += circinus.total_links();

   Constellation columba("Columba");
   columba.add_star(OrdStar(84.760817f, 33.925944f, 2.65f, OrdStar::SPEC_B, "Phact"));
   columba.add_star(OrdStar(87.515986f, 34.230694f, 3.12f, OrdStar::SPEC_K, "Wezn"));
   columba.add_star(OrdStar(95.501903f, 32.563722f, 3.85f, OrdStar::SPEC_G, ""));
   columba.add_star(OrdStar(82.753539f, 34.529556f, 3.86f, OrdStar::SPEC_K, ""));
   columba.add_star(OrdStar(89.258947f, 34.716694f, 4.36f, OrdStar::SPEC_B, ""));
   columba.add_link(0, 1);
   columba.add_link(0, 3);
   columba.add_link(1, 4);
   columba.add_link(2, 4);
   cons_list.push_back(columba);
   tot_stars += columba.total_stars();
   tot_links += columba.total_links();

   Constellation coma_berenices("Coma Berenices");
   coma_berenices.add_star(OrdStar(197.764700f, -27.876028f, 4.23f, OrdStar::SPEC_G, ""));
   coma_berenices.add_star(OrdStar(197.266542f, -17.529111f, 4.32f, OrdStar::SPEC_F, "Diadem"));
   coma_berenices.add_star(OrdStar(186.515647f, -28.268611f, 4.35f, OrdStar::SPEC_K, ""));
   coma_berenices.add_link(0, 1);
   coma_berenices.add_link(0, 2);
   cons_list.push_back(coma_berenices);
   tot_stars += coma_berenices.total_stars();
   tot_links += coma_berenices.total_links();

   Constellation corona_australis("Corona Australis");
   corona_australis.add_star(OrdStar(287.500486f, 38.659306f, 4.10f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(287.257856f, 36.095750f, 4.11f, OrdStar::SPEC_A, "Alphekka Meridiana"));
   corona_australis.add_star(OrdStar(286.506956f, 36.937250f, 4.23f, OrdStar::SPEC_F, ""));
   corona_australis.add_star(OrdStar(287.005814f, 39.503361f, 4.57f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(285.751897f, 41.905000f, 4.74f, OrdStar::SPEC_A, ""));
   corona_australis.add_star(OrdStar(284.512075f, 36.892917f, 4.83f, OrdStar::SPEC_F, ""));
   corona_australis.add_link(0, 1);
   corona_australis.add_link(0, 3);
   corona_australis.add_link(1, 2);
   corona_australis.add_link(2, 5);
   corona_australis.add_link(3, 4);
   cons_list.push_back(corona_australis);
   tot_stars += corona_australis.total_stars();
   tot_links += corona_australis.total_links();

   Constellation corona_borealis("Corona Borealis");
   corona_borealis.add_star(OrdStar(233.511442f, -26.714917f, 2.22f, OrdStar::SPEC_A, "Alphecca"));
   corona_borealis.add_star(OrdStar(231.763847f, -29.105500f, 3.66f, OrdStar::SPEC_F, "Nusakan"));
   corona_borealis.add_star(OrdStar(235.512400f, -26.295528f, 3.81f, OrdStar::SPEC_A, ""));
   corona_borealis.add_star(OrdStar(233.015500f, -31.359167f, 4.14f, OrdStar::SPEC_B, ""));
   corona_borealis.add_star(OrdStar(239.259806f, -26.878028f, 4.14f, OrdStar::SPEC_K, ""));
   corona_borealis.add_star(OrdStar(237.259917f, -26.068556f, 4.59f, OrdStar::SPEC_G, ""));
   corona_borealis.add_link(0, 1);
   corona_borealis.add_link(0, 2);
   corona_borealis.add_link(1, 3);
   corona_borealis.add_link(4, 5);
   cons_list.push_back(corona_borealis);
   tot_stars += corona_borealis.total_stars();
   tot_links += corona_borealis.total_links();

   Constellation corvus("Corvus");
   corvus.add_star(OrdStar(183.763464f, 16.458028f, 2.58f, OrdStar::SPEC_B, "Gienah Corvi"));
   corvus.add_star(OrdStar(188.506453f, 22.603389f, 2.65f, OrdStar::SPEC_G, "Kraz"));
   corvus.add_star(OrdStar(187.264439f, 15.484917f, 2.94f, OrdStar::SPEC_B, "Algorab"));
   corvus.add_star(OrdStar(182.502092f, 21.380194f, 3.02f, OrdStar::SPEC_K, "Minkar"));
   corvus.add_star(OrdStar(182.006875f, 23.271222f, 4.02f, OrdStar::SPEC_F, "Al Chiba"));
   corvus.add_link(0, 2);
   corvus.add_link(0, 3);
   corvus.add_link(1, 2);
   corvus.add_link(1, 3);
   cons_list.push_back(corvus);
   tot_stars += corvus.total_stars();
   tot_links += corvus.total_links();

   Constellation crater("Crater");
   crater.add_star(OrdStar(169.755700f, 13.220944f, 3.56f, OrdStar::SPEC_K, ""));
   crater.add_star(OrdStar(171.014717f, 16.315972f, 4.06f, OrdStar::SPEC_A, ""));
   crater.add_star(OrdStar(164.762986f, 17.700889f, 4.08f, OrdStar::SPEC_K, "Alkes"));
   crater.add_star(OrdStar(167.760969f, 21.174389f, 4.46f, OrdStar::SPEC_A, ""));
   crater.add_link(0, 1);
   crater.add_link(0, 2);
   crater.add_link(2, 3);
   cons_list.push_back(crater);
   tot_stars += crater.total_stars();
   tot_links += crater.total_links();

   Constellation crux("Crux");
   crux.add_star(OrdStar(191.762033f, 58.311278f, 1.25f, OrdStar::SPEC_B, "Mimosa"));
   crux.add_star(OrdStar(186.509983f, 62.900944f, 1.4f, OrdStar::SPEC_B, "Acrux"));
   crux.add_star(OrdStar(187.752758f, 56.887444f, 1.59f, OrdStar::SPEC_M, "Gacrux"));
   crux.add_star(OrdStar(183.752433f, 57.251111f, 2.79f, OrdStar::SPEC_B, "Palida"));
   crux.add_star(OrdStar(185.256058f, 59.598639f, 3.59f, OrdStar::SPEC_K, "Intrometida"));
   crux.add_link(0, 3);
   crux.add_link(1, 2);
   cons_list.push_back(crux);
   tot_stars += crux.total_stars();
   tot_links += crux.total_links();

   Constellation cygnus("Cygnus");
   cygnus.add_star(OrdStar(310.257197f, -45.280333f, 1.25f, OrdStar::SPEC_A, "Deneb Cygni"));
   cygnus.add_star(OrdStar(305.503806f, -40.256694f, 2.23f, OrdStar::SPEC_F, "Sadr"));
   cygnus.add_star(OrdStar(311.503453f, -33.969444f, 2.48f, OrdStar::SPEC_K, "Gienah"));
   cygnus.add_star(OrdStar(296.016233f, -45.130694f, 2.86f, OrdStar::SPEC_B, "Ruc"));
   cygnus.add_star(OrdStar(292.512025f, -27.959694f, 3.05f, OrdStar::SPEC_K, "Al Minhar al Dajajah"));
   cygnus.add_star(OrdStar(318.015606f, -30.227083f, 3.21f, OrdStar::SPEC_G, ""));
   cygnus.add_star(OrdStar(292.261761f, -51.729472f, 3.76f, OrdStar::SPEC_A, ""));
   cygnus.add_star(OrdStar(289.251697f, -53.368167f, 3.80f, OrdStar::SPEC_K, ""));
   cygnus.add_star(OrdStar(299.005111f, -35.083500f, 3.89f, OrdStar::SPEC_K, ""));
   cygnus.add_link(0, 1);
   cygnus.add_link(1, 2);
   cygnus.add_link(1, 3);
   cygnus.add_link(1, 8);
   cygnus.add_link(2, 5);
   cygnus.add_link(3, 6);
   cygnus.add_link(4, 8);
   cygnus.add_link(6, 7);
   cons_list.push_back(cygnus);
   tot_stars += cygnus.total_stars();
   tot_links += cygnus.total_links();

   Constellation delphinus("Delphinus");
   delphinus.add_star(OrdStar(309.259131f, -14.595194f, 3.64f, OrdStar::SPEC_F, "Rotanev"));
   delphinus.add_star(OrdStar(309.760625f, -15.912056f, 3.77f, OrdStar::SPEC_B, "Sualocin"));
   delphinus.add_star(OrdStar(308.253544f, -11.303333f, 4.03f, OrdStar::SPEC_B, "Deneb Dulfim"));
   delphinus.add_star(OrdStar(311.510978f, -16.124778f, 4.27f, OrdStar::SPEC_K, ""));
   delphinus.add_star(OrdStar(310.757653f, -15.074694f, 4.43f, OrdStar::SPEC_A, ""));
   delphinus.add_link(0, 1);
   delphinus.add_link(0, 2);
   delphinus.add_link(0, 4);
   delphinus.add_link(1, 3);
   delphinus.add_link(3, 4);
   cons_list.push_back(delphinus);
   tot_stars += delphinus.total_stars();
   tot_links += delphinus.total_links();

   Constellation dorado("Dorado");
   dorado.add_star(OrdStar(68.266589f, 54.955000f, 3.30f, OrdStar::SPEC_A, ""));
   dorado.add_star(OrdStar(83.260422f, 61.510139f, 3.76f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(64.000414f, 50.512917f, 4.26f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(86.012894f, 64.264472f, 4.34f, OrdStar::SPEC_A, ""));
   dorado.add_link(0, 1);
   dorado.add_link(0, 2);
   cons_list.push_back(dorado);
   tot_stars += dorado.total_stars();
   tot_links += dorado.total_links();

   Constellation draco("Draco");
   draco.add_star(OrdStar(269.010106f, -51.488944f, 2.24f, OrdStar::SPEC_K, "Eltanin"));
   draco.add_star(OrdStar(245.766531f, -61.514083f, 2.73f, OrdStar::SPEC_G, ""));
   draco.add_star(OrdStar(262.507217f, -52.301361f, 2.79f, OrdStar::SPEC_G, "Rastaban"));
   draco.add_star(OrdStar(288.009208f, -67.661306f, 3.07f, OrdStar::SPEC_G, "Altais"));
   draco.add_star(OrdStar(257.013119f, -65.714639f, 3.17f, OrdStar::SPEC_B, "Aldhibah"));
   draco.add_star(OrdStar(231.015494f, -58.966028f, 3.29f, OrdStar::SPEC_K, "Edasich"));
   draco.add_star(OrdStar(275.250650f, -72.733694f, 3.55f, OrdStar::SPEC_F, "Batentaban Borealis"));
   draco.add_star(OrdStar(211.006508f, -64.375806f, 3.67f, OrdStar::SPEC_A, "Adib"));
   draco.add_star(OrdStar(268.258786f, -56.872444f, 3.73f, OrdStar::SPEC_K, "Grumium"));
   draco.add_star(OrdStar(172.756747f, -69.331111f, 3.82f, OrdStar::SPEC_M, "Giausar"));
   draco.add_star(OrdStar(297.002836f, -70.267833f, 3.84f, OrdStar::SPEC_G, "Tyl"));
   draco.add_star(OrdStar(188.258067f, -69.788222f, 3.85f, OrdStar::SPEC_B, "Ketu"));
   draco.add_star(OrdStar(240.264917f, -58.564444f, 4.01f, OrdStar::SPEC_F, ""));
   draco.add_star(OrdStar(263.002894f, -55.184111f, 4.89f, OrdStar::SPEC_A, "Kuma"));
   draco.add_link(0, 2);
   draco.add_link(0, 8);
   draco.add_link(1, 4);
   draco.add_link(1, 12);
   draco.add_link(2, 13);
   draco.add_link(3, 8);
   draco.add_link(3, 10);
   draco.add_link(4, 6);
   draco.add_link(5, 7);
   draco.add_link(5, 12);
   draco.add_link(6, 10);
   draco.add_link(7, 11);
   draco.add_link(8, 13);
   draco.add_link(9, 11);
   cons_list.push_back(draco);
   tot_stars += draco.total_stars();
   tot_links += draco.total_links();

   Constellation equuleus("Equuleus");
   equuleus.add_star(OrdStar(318.763722f, -5.248083f, 3.92f, OrdStar::SPEC_G, "Kitalpha"));
   equuleus.add_star(OrdStar(318.507997f, -10.007722f, 4.47f, OrdStar::SPEC_F, "Pherasauval"));
   equuleus.add_star(OrdStar(317.505686f, -10.131944f, 4.70f, OrdStar::SPEC_F, ""));
   equuleus.add_star(OrdStar(320.514883f, -6.811111f, 5.16f, OrdStar::SPEC_A, ""));
   equuleus.add_star(OrdStar(314.751261f, -4.293833f, 5.30f, OrdStar::SPEC_F, ""));
   equuleus.add_link(0, 1);
   equuleus.add_link(1, 2);
   cons_list.push_back(equuleus);
   tot_stars += equuleus.total_stars();
   tot_links += equuleus.total_links();

   Constellation eradanus("Eradanus");
   eradanus.add_star(OrdStar(24.261875f, 56.763333f, 0.45f, OrdStar::SPEC_B, "Eridani"));
   eradanus.add_star(OrdStar(76.764175f, 4.913750f, 2.78f, OrdStar::SPEC_A, "Cursa"));
   eradanus.add_star(OrdStar(44.504367f, 39.695278f, 2.88f, OrdStar::SPEC_A, "Acamar"));
   eradanus.add_star(OrdStar(59.500481f, 12.491750f, 2.97f, OrdStar::SPEC_M, "Zaurak"));
   eradanus.add_star(OrdStar(55.754156f, 8.234806f, 3.52f, OrdStar::SPEC_K, "Rana"));
   eradanus.add_star(OrdStar(64.264894f, 32.201667f, 3.55f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(34.008472f, 50.487889f, 3.56f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(28.765786f, 50.390417f, 3.69f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(49.758603f, 20.242056f, 3.70f, OrdStar::SPEC_M, ""));
   eradanus.add_star(OrdStar(53.015672f, 8.541694f, 3.72f, OrdStar::SPEC_K, "Sadira"));
   eradanus.add_star(OrdStar(68.759186f, 29.437694f, 3.81f, OrdStar::SPEC_G, "Beemin"));
   eradanus.add_star(OrdStar(44.007111f, 7.102389f, 3.89f, OrdStar::SPEC_K, "Azha"));
   eradanus.add_star(OrdStar(69.005317f, 2.647556f, 3.93f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(66.000603f, 33.983000f, 3.97f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(71.258372f, 2.745389f, 4.01f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(62.764425f, 5.162222f, 4.04f, OrdStar::SPEC_F, "Beid"));
   eradanus.add_star(OrdStar(45.506553f, 22.375667f, 4.08f, OrdStar::SPEC_A, ""));
   eradanus.add_star(OrdStar(40.011092f, 38.144694f, 4.11f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(57.257578f, 35.799889f, 4.17f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(56.514164f, 22.751556f, 4.22f, OrdStar::SPEC_F, ""));
   eradanus.add_star(OrdStar(36.516417f, 46.296167f, 4.24f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(53.263125f, 20.367194f, 4.26f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(41.251661f, 17.427361f, 4.47f, OrdStar::SPEC_F, ""));
   eradanus.add_link(0, 7);
   eradanus.add_link(1, 14);
   eradanus.add_link(2, 17);
   eradanus.add_link(2, 18);
   eradanus.add_link(3, 4);
   eradanus.add_link(3, 15);
   eradanus.add_link(4, 9);
   eradanus.add_link(5, 13);
   eradanus.add_link(5, 18);
   eradanus.add_link(6, 7);
   eradanus.add_link(6, 20);
   eradanus.add_link(8, 16);
   eradanus.add_link(8, 21);
   eradanus.add_link(9, 11);
   eradanus.add_link(10, 13);
   eradanus.add_link(10, 19);
   eradanus.add_link(11, 22);
   eradanus.add_link(12, 14);
   eradanus.add_link(12, 15);
   eradanus.add_link(16, 22);
   eradanus.add_link(17, 20);
   eradanus.add_link(19, 21);
   cons_list.push_back(eradanus);
   tot_stars += eradanus.total_stars();
   tot_links += eradanus.total_links();

   Constellation fornax("Fornax");
   fornax.add_star(OrdStar(48.001189f, 27.010889f, 3.80f, OrdStar::SPEC_F, "Dalim"));
   fornax.add_star(OrdStar(42.251489f, 31.593722f, 4.45f, OrdStar::SPEC_G, ""));
   fornax.add_star(OrdStar(31.008175f, 28.703167f, 4.68f, OrdStar::SPEC_B, ""));
   fornax.add_link(0, 1);
   cons_list.push_back(fornax);
   tot_stars += fornax.total_stars();
   tot_links += fornax.total_links();

   Constellation gemini("Gemini");
   gemini.add_star(OrdStar(116.255378f, -28.026306f, 1.16f, OrdStar::SPEC_K, "Pollux"));
   gemini.add_star(OrdStar(113.510000f, -31.888639f, 1.58f, OrdStar::SPEC_A, "Castor"));
   gemini.add_star(OrdStar(99.261861f, -16.399417f, 1.93f, OrdStar::SPEC_A, "Alhena"));
   gemini.add_star(OrdStar(95.515997f, -22.513861f, 2.87f, OrdStar::SPEC_M, "Tejat"));
   gemini.add_star(OrdStar(100.765536f, -25.131167f, 3.06f, OrdStar::SPEC_A, "Mebsuta"));
   gemini.add_star(OrdStar(93.514639f, -22.506833f, 3.31f, OrdStar::SPEC_M, "Propus"));
   gemini.add_star(OrdStar(101.254842f, -12.896056f, 3.35f, OrdStar::SPEC_F, "Alzir"));
   gemini.add_star(OrdStar(110.002053f, -21.982333f, 3.50f, OrdStar::SPEC_F, "Wasat"));
   gemini.add_star(OrdStar(116.007464f, -24.398139f, 3.57f, OrdStar::SPEC_G, ""));
   gemini.add_star(OrdStar(109.501558f, -16.540472f, 3.58f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(103.013150f, -33.961361f, 3.60f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(111.262133f, -27.798278f, 3.78f, OrdStar::SPEC_G, "Propus"));
   gemini.add_star(OrdStar(106.001817f, -20.570306f, 4.01f, OrdStar::SPEC_G, "Mekbuda"));
   gemini.add_link(0, 1);
   gemini.add_link(1, 4);
   gemini.add_link(2, 12);
   gemini.add_link(3, 4);
   gemini.add_link(3, 5);
   gemini.add_link(7, 12);
   cons_list.push_back(gemini);
   tot_stars += gemini.total_stars();
   tot_links += gemini.total_links();

   Constellation grus("Grus");
   grus.add_star(OrdStar(332.003856f, 45.039389f, 1.73f, OrdStar::SPEC_B, "Alnair"));
   grus.add_star(OrdStar(340.511092f, 45.115444f, 2.07f, OrdStar::SPEC_M, "Gruid"));
   grus.add_star(OrdStar(328.265458f, 36.635167f, 3.00f, OrdStar::SPEC_B, "Al Dhanab"));
   grus.add_star(OrdStar(342.009222f, 50.683306f, 3.49f, OrdStar::SPEC_A, ""));
   grus.add_star(OrdStar(337.254486f, 42.504444f, 3.97f, OrdStar::SPEC_G, ""));
   grus.add_star(OrdStar(331.501917f, 38.456944f, 4.47f, OrdStar::SPEC_M, ""));
   grus.add_star(OrdStar(333.760244f, 40.653250f, 4.79f, OrdStar::SPEC_G, ""));
   grus.add_link(0, 4);
   grus.add_link(1, 3);
   grus.add_link(1, 4);
   grus.add_link(2, 5);
   grus.add_link(4, 6);
   grus.add_link(5, 6);
   cons_list.push_back(grus);
   tot_stars += grus.total_stars();
   tot_links += grus.total_links();

   Constellation hercules("Hercules");
   hercules.add_star(OrdStar(247.503683f, -21.489639f, 2.78f, OrdStar::SPEC_G, "Kornephoros"));
   hercules.add_star(OrdStar(250.254856f, -31.601889f, 2.81f, OrdStar::SPEC_F, "Rutilicus"));
   hercules.add_star(OrdStar(258.750533f, -24.839583f, 3.12f, OrdStar::SPEC_A, "Sarin"));
   hercules.add_star(OrdStar(258.750792f, -36.809167f, 3.16f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(258.510794f, -14.390250f, 3.31f, OrdStar::SPEC_M, "Ras Algethi"));
   hercules.add_star(OrdStar(266.507700f, -27.722500f, 3.42f, OrdStar::SPEC_G, ""));
   hercules.add_star(OrdStar(250.514928f, -38.922472f, 3.48f, OrdStar::SPEC_G, ""));
   hercules.add_star(OrdStar(269.262731f, -29.247917f, 3.70f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(245.265344f, -19.153028f, 3.74f, OrdStar::SPEC_A, ""));
   hercules.add_star(OrdStar(271.759042f, -28.762472f, 3.84f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(269.004217f, -37.250528f, 3.86f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(244.762347f, -46.313278f, 3.91f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(255.004836f, -30.926333f, 3.92f, OrdStar::SPEC_A, "Cujam"));
   hercules.add_star(OrdStar(260.761381f, -37.145917f, 4.15f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(248.501719f, -42.436889f, 4.20f, OrdStar::SPEC_B, ""));
   hercules.add_link(0, 1);
   hercules.add_link(0, 8);
   hercules.add_link(1, 6);
   hercules.add_link(1, 12);
   hercules.add_link(2, 5);
   hercules.add_link(2, 12);
   hercules.add_link(3, 6);
   hercules.add_link(3, 12);
   hercules.add_link(3, 13);
   hercules.add_link(5, 7);
   hercules.add_link(6, 14);
   hercules.add_link(7, 9);
   hercules.add_link(10, 13);
   hercules.add_link(11, 14);
   cons_list.push_back(hercules);
   tot_stars += hercules.total_stars();
   tot_links += hercules.total_links();

   Constellation horologium("Horologium");
   horologium.add_star(OrdStar(63.500022f, 41.706139f, 3.85f, OrdStar::SPEC_K, ""));
   horologium.add_star(OrdStar(44.513269f, 63.928694f, 4.98f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(45.760250f, 58.262389f, 5.12f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(40.010994f, 53.450083f, 5.21f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(39.256739f, 51.456917f, 5.30f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(40.509211f, 49.199167f, 5.40f, OrdStar::SPEC_G, ""));
   horologium.add_link(0, 5);
   horologium.add_link(1, 2);
   horologium.add_link(2, 3);
   horologium.add_link(3, 4);
   horologium.add_link(4, 5);
   cons_list.push_back(horologium);
   tot_stars += horologium.total_stars();
   tot_links += horologium.total_links();

   Constellation hydra("Hydra");
   hydra.add_star(OrdStar(141.759792f, 7.341306f, 1.99f, OrdStar::SPEC_K, "Alfard"));
   hydra.add_star(OrdStar(199.515347f, 22.828583f, 2.99f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(133.756578f, -5.945528f, 3.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(162.260397f, 15.805861f, 3.11f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(211.506186f, 25.317972f, 3.25f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(131.512958f, -6.418917f, 3.38f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(173.250072f, 30.142472f, 3.54f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(152.509833f, 11.646167f, 3.61f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(156.501531f, 15.163917f, 3.83f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(138.506053f, -2.315028f, 3.89f, OrdStar::SPEC_B, ""));
   hydra.add_star(OrdStar(144.764258f, 0.857333f, 3.90f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(147.757967f, 13.153444f, 4.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(178.015156f, 32.091861f, 4.29f, OrdStar::SPEC_A, ""));
   hydra.add_link(0, 10);
   hydra.add_link(0, 11);
   hydra.add_link(1, 4);
   hydra.add_link(1, 12);
   hydra.add_link(2, 5);
   hydra.add_link(2, 9);
   hydra.add_link(3, 8);
   hydra.add_link(6, 12);
   hydra.add_link(7, 8);
   hydra.add_link(7, 11);
   hydra.add_link(9, 10);
   cons_list.push_back(hydra);
   tot_stars += hydra.total_stars();
   tot_links += hydra.total_links();

   Constellation hydrus("Hydrus");
   hydrus.add_star(OrdStar(6.260889f, 76.744972f, 2.82f, OrdStar::SPEC_G, ""));
   hydrus.add_star(OrdStar(29.512742f, 60.430083f, 2.86f, OrdStar::SPEC_F, ""));
   hydrus.add_star(OrdStar(56.753953f, 73.760750f, 3.26f, OrdStar::SPEC_M, "Foo Pih"));
   hydrus.add_star(OrdStar(35.262506f, 67.340583f, 4.08f, OrdStar::SPEC_A, ""));
   hydrus.add_star(OrdStar(39.759783f, 67.733056f, 4.12f, OrdStar::SPEC_B, ""));
   hydrus.add_link(0, 2);
   hydrus.add_link(1, 3);
   hydrus.add_link(2, 4);
   hydrus.add_link(3, 4);
   cons_list.push_back(hydrus);
   tot_stars += hydrus.total_stars();
   tot_links += hydrus.total_links();

   Constellation indus("Indus");
   indus.add_star(OrdStar(309.259442f, 46.708333f, 3.11f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(313.513494f, 57.545917f, 3.67f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(319.764411f, 52.550722f, 4.39f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(329.265286f, 53.007444f, 4.40f, OrdStar::SPEC_F, ""));
   indus.add_star(OrdStar(311.000608f, 50.079167f, 4.51f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(330.754844f, 55.220194f, 4.69f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(312.258036f, 45.773111f, 4.90f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(312.758347f, 50.391833f, 5.06f, OrdStar::SPEC_K, ""));
   indus.add_link(0, 4);
   indus.add_link(1, 4);
   cons_list.push_back(indus);
   tot_stars += indus.total_stars();
   tot_links += indus.total_links();
   }

Constellations::~Constellations() {
   }

void Constellations::prepare(SolarSystemGlobals::render_quality rq) {
   // Get the OpenGL buffer objects.
   buff_obj_points.acquire();
   buff_obj_indices.acquire();

   /// Preparations depend upon the requested rendering quality level.
   switch (rq) {
      case SolarSystemGlobals::RENDER_LOWEST :
      case SolarSystemGlobals::RENDER_MEDIUM :
      case SolarSystemGlobals::RENDER_HIGHEST :
         loadVertexBuffer();
         loadIndexBuffer();
         break;
      default :
         // Ought not get here !!
         std::string msg = "Constellations::prepare() - bad switch case reached (default)";
         ErrorHandler::record(msg, ErrorHandler::FATAL);
         break;
      }
   }

void Constellations::release(void) {
   // Release the buffer object's resources.
   buff_obj_points.release();
   buff_obj_indices.release();
   }


void Constellations::render(void) {
   /// Plot the stars first ie. a color at a point.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   /// The GL_ARRAY_BUFFER target will be serviced by one buffer.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   /// Using interleaved color/position data
   glInterleavedArrays(GL_C3F_V3F, 0, BUFFER_OFFSET(0));

   glPointSize(Constellations::MAG_SIZE);
   glDrawArrays(GL_POINTS, 0, tot_stars);

   /// Not using color values now.
   glDisableClientState(GL_COLOR_ARRAY);
   /// The GL_ELEMENT_ARRAY_BUFFER target will be serviced by the other buffer.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_indices.ID());

   /// OK, set up the vertex array within.
   glVertexPointer(COORDS_PER_VERTEX,
                   GL_FLOAT,
                   BYTE_STRIDE_PER_VERTEX*sizeof(GL_FLOAT),
                   BUFFER_OFFSET(COLORS_PER_VERTEX*sizeof(GL_FLOAT)));

   glColor3f(LINK_RGB_RED, LINK_RGB_GREEN, LINK_RGB_BLUE);
   glLineWidth(LINK_WIDTH);
   glLineStipple(LINK_STIPPLE_FACTOR, LINK_STIPPLE_PATTERN);
   glDrawElements(GL_LINES, tot_links*INDICES_PER_LINK, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

   /// Disable various capabilities from the OpenGL state machine.
   glDisableClientState(GL_VERTEX_ARRAY);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Constellations::class_color(OrdStar::spectral_type spectral_class, GLfloat* colors) {
   switch (spectral_class) {
      case OrdStar::SPEC_A :
         // Hottish and white.
         colors[0] = 1.0f;
         colors[1] = 1.0f;
         colors[2] = 1.0f;
         break;
      case OrdStar::SPEC_B :
         // Quite hot and blue.
         colors[0] = 0.65f;
         colors[1] = 0.85f;
         colors[2] = 0.95f;
         break;
      case OrdStar::SPEC_F :
         // Bit hotter than Sol and yellowish white.
         colors[0] = 1.00f;
         colors[1] = 0.95f;
         colors[2] = 0.75f;
         break;
      case OrdStar::SPEC_G :
         // Like our Sun and yellow.
         colors[0] = 0.95f;
         colors[1] = 0.90f;
         colors[2] = 0.0f;
         break;
      case OrdStar::SPEC_K :
         // Lukewarm and orange.
         colors[0] = 0.95f;
         colors[1] = 0.65f;
         colors[2] = 0.15f;
         break;
      case OrdStar::SPEC_M :
         // Quite cool and deep red.
         colors[0] = 1.00f;
         colors[1] = 0.05f;
         colors[2] = 0.05f;
         break;
      case OrdStar::SPEC_O :
         // Cracking hot and very blue.
         colors[0] = 0.05f;
         colors[1] = 0.05f;
         colors[2] = 1.00f;
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
   /// We need to look at all constellations and create a consecutive
   /// listing of colors and positions for all stars to be rendered.

   /// Bind the buffer for vertex array use.
   glBindBuffer(GL_ARRAY_BUFFER, buff_obj_points.ID());

   /// Total buffer size is the storage for color/vertex
   /// interleaved data.
   GLsizeiptr vert_size = sizeof(vec_t) * BYTE_STRIDE_PER_VERTEX * tot_stars;

   /// Allocate but don't store, yet.
   glBufferData(GL_ARRAY_BUFFER, vert_size, NULL, GL_STATIC_DRAW);
   /// Need a pointer into the color/vertex interleaved area.
   vec_t* buffer_vert_ptr = static_cast<vec_t*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

   /// Work through the constellations one by one.
   for(std::vector<Constellation>::const_iterator cs = cons_list.begin();
      cs < cons_list.end();
      cs++ ) {
      /// Process the color and location of the stars within this
      /// constellation. Get access to the star list.
      const std::vector<OrdStar>& star_list = cs->stars();
      for(std::vector<OrdStar>::const_iterator st = star_list.begin();
      st < star_list.end();
      st++ ) {
         /// Take the star's spectral type and convert that to RGB color values.
         class_color(st->spectral_class(), buffer_vert_ptr);
         /// Move the pointer along for vertex data.
         buffer_vert_ptr += COLORS_PER_VERTEX;

         /// This is the current star with spherical polar co-ordinates.
         VectorSP c_star = VectorSP(st->right_ascension(), st->declination(), c_rad);

         /// But Cartesian co-ordinates will be stored in the buffer.
         *(buffer_vert_ptr) = c_star.x();
         buffer_vert_ptr++;
         *(buffer_vert_ptr) = c_star.y();
         buffer_vert_ptr++;
         *(buffer_vert_ptr) = c_star.z();
         buffer_vert_ptr++;
         }
      }

   // Disconnect the mapping and the buffers from OpenGL.
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }

void Constellations::loadIndexBuffer(void) {
   /// We need to look at all constellations and create a consecutive
   /// listing of star indices for all links to be rendered. Obviously
   /// there are two star indices per link, and each link will be
   /// represented as a line in the render() routine.

   /// Bind the buffer for index array use.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff_obj_indices.ID());
   /// Total buffer size is the storage for index data.
   GLsizeiptr index_size = sizeof(unsigned int) * INDICES_PER_LINK * tot_links;

   /// Allocate but don't store, yet.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, NULL, GL_STATIC_DRAW);
   /// Need a pointer into the index area.
   unsigned int* buffer_index_ptr = static_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

   /// Need to keep track of the indices with respect to the entire
   /// set of constellations.
   unsigned int link_base_index = 0;
   /// Now go through all the constellations.
   for(std::vector<Constellation>::const_iterator cs = cons_list.begin();
      cs < cons_list.end();
      cs++ ) {
      /// Process the index data within this constellation. Get access to the
      /// index list.
      const std::vector< std::pair<unsigned int, unsigned int> >& link_list = cs->links();
      /// Beware of an 'empty' constellation.
      GLuint total_stars_this_con = cs->total_stars();

      if(total_stars_this_con != 0) {
         for(std::vector< std::pair<unsigned int, unsigned int> >::const_iterator lk = link_list.begin();
         lk < link_list.end();
         lk++ ) {
            /// For each linkage, store the indices at two per link.
            /// Store indices in the buffer, but with the indices relative to
            /// entire set of stars that all the constellations represent.
            /// Need to check that any index within a constellation does not
            /// go out of bounds with respect to the number of stars in said
            /// constellation. The link indices are unsigned so only need to
            /// check upper index bound.
            if((lk->first < total_stars_this_con) && (lk->first < total_stars_this_con)) {
               *(buffer_index_ptr) = link_base_index + lk->first;
               buffer_index_ptr++;
               *(buffer_index_ptr) = link_base_index + lk->second;
               buffer_index_ptr++;
               }
            else {
               // Doesn't need to be FATAL but ought be noted.
               std::string msg = "Constellations::loadIndexBuffer() - bad link for ";
               msg += cs->name();
               msg += " : ";
               msg += lk->first;
               msg += " <-> ";
               msg += lk->second;
               ErrorHandler::record(msg, ErrorHandler::WARN);
               }
            }
         /// Update the index into all stars within all constellations.
         link_base_index += total_stars_this_con;
         }
      else {
         // Doesn't need to be FATAL but ought be noted.
         std::string msg = "Constellations::loadIndexBuffer() - empty constellation : ";
         msg += cs->name();
         ErrorHandler::record(msg, ErrorHandler::WARN);
         }
      }

   // Disconnect the mapping and the buffers from OpenGL.
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_OBJ::NO_ID);
   }
