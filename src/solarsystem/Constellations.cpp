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
   andromeda.add_star(OrdStar(0.139769f, -29.090833f, 2.07f, OrdStar::SPEC_B, "Alpheratz"));
   andromeda.add_star(OrdStar(15.162167f, -35.620833f, 2.07f, OrdStar::SPEC_M, "Mirach"));
   andromeda.add_star(OrdStar(30.064978f, -42.329861f, 2.10f, OrdStar::SPEC_B, "Almach"));
   andromeda.add_star(OrdStar(0.655444f, -30.861222f, 3.27f, OrdStar::SPEC_K, "Delta"));
   andromeda.add_star(OrdStar(15.633194f, -48.628500f, 3.59f, OrdStar::SPEC_K, "Nembus"));
   andromeda.add_star(OrdStar(345.032014f, -42.325972f, 3.62f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(345.626031f, -46.459167f, 3.81f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(0.945861f, -38.499250f, 3.86f, OrdStar::SPEC_A, ""));
   andromeda.add_star(OrdStar(0.788997f, -24.267389f, 4.08f, OrdStar::SPEC_K, ""));
   andromeda.add_star(OrdStar(15.613328f, -41.406389f, 4.10f, OrdStar::SPEC_F, ""));
   andromeda.add_star(OrdStar(345.673456f, -44.333972f, 4.15f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(15.158367f, -47.241833f, 4.26f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(345.635606f, -43.268083f, 4.29f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(0.614678f, -33.719361f, 4.34f, OrdStar::SPEC_B, ""));
   andromeda.add_star(OrdStar(0.642639f, -29.312361f, 4.34f, OrdStar::SPEC_G, ""));
   andromeda.add_star(OrdStar(0.953453f, -23.417750f, 4.40f, OrdStar::SPEC_G, ""));
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
   antlia.add_star(OrdStar(135.487428f, -35.951361f, 4.51f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(150.452544f, -31.067806f, 4.28f, OrdStar::SPEC_K, ""));
   antlia.add_star(OrdStar(150.945278f, -37.137472f, 4.60f, OrdStar::SPEC_K, ""));
   antlia.add_link(0, 1);
   antlia.add_link(1, 2);
   cons_list.push_back(antlia);
   tot_stars += antlia.total_stars();
   tot_links += antlia.total_links();

   Constellation apus("Apus");
   apus.add_star(OrdStar(210.797703f, 78.955278f, 3.83f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(240.557628f, 77.103028f, 3.86f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(240.718172f, 76.483417f, 4.23f, OrdStar::SPEC_K, ""));
   apus.add_star(OrdStar(240.339122f, 77.304333f, 4.68f, OrdStar::SPEC_M, ""));
   apus.add_link(0, 3);
   apus.add_link(1, 3);
   apus.add_link(1, 2);
   cons_list.push_back(apus);
   tot_stars += apus.total_stars();
   tot_links += apus.total_links();

   Constellation aquarius("Aquarius");
   aquarius.add_star(OrdStar(315.525978f, 4.428833f, 2.90f, OrdStar::SPEC_G, "Sadalsuud"));
   aquarius.add_star(OrdStar(330.096397f, -0.319833f, 2.95f, OrdStar::SPEC_G, "Sadalmelik"));
   aquarius.add_star(OrdStar(330.910844f, 14.179250f, 3.27f, OrdStar::SPEC_A, "Skat"));
   aquarius.add_star(OrdStar(330.480500f, -0.020056f, 3.65f, OrdStar::SPEC_F, "Sadaltager"));
   aquarius.add_star(OrdStar(345.157433f, 20.827528f, 3.68f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(330.876906f, 6.420333f, 3.73f, OrdStar::SPEC_M, "Hydor"));
   aquarius.add_star(OrdStar(300.794592f, 8.504306f, 3.78f, OrdStar::SPEC_A, "Albali"));
   aquarius.add_star(OrdStar(330.360917f, 0.612639f, 3.86f, OrdStar::SPEC_A, "Sadachbia"));
   aquarius.add_star(OrdStar(345.382861f, 19.899667f, 3.96f, OrdStar::SPEC_K, ""));
   aquarius.add_star(OrdStar(330.589258f, -0.117361f, 4.04f, OrdStar::SPEC_B, ""));
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
   aquila.add_star(OrdStar(285.846300f, -8.867389f, 0.76f, OrdStar::SPEC_A, "Altair"));
   aquila.add_star(OrdStar(285.770992f, -10.613278f, 2.72f, OrdStar::SPEC_K, "Tarazed"));
   aquila.add_star(OrdStar(285.090169f, -13.863722f, 2.99f, OrdStar::SPEC_A, "Deneb"));
   aquila.add_star(OrdStar(300.188406f, -0.821472f, 3.24f, OrdStar::SPEC_B, "Tseen Foo"));
   aquila.add_star(OrdStar(285.424931f, -3.114583f, 3.36f, OrdStar::SPEC_F, "Denebokab"));
   aquila.add_star(OrdStar(285.104153f, -4.882333f, 3.43f, OrdStar::SPEC_B, "Al Thalimain"));
   aquila.add_star(OrdStar(285.921881f, -6.407944f, 3.71f, OrdStar::SPEC_G, "Alshain"));
   aquila.add_star(OrdStar(285.874544f, -1.005667f, 3.87f, OrdStar::SPEC_F, ""));
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
   ara.add_star(OrdStar(255.421667f, 54.470167f, 2.84f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(255.530700f, 48.124028f, 2.84f, OrdStar::SPEC_B, "Choo"));
   ara.add_star(OrdStar(240.977008f, 54.009944f, 3.12f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(255.423239f, 55.622306f, 3.31f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(255.518328f, 59.316389f, 3.60f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(270.110522f, 49.908556f, 3.65f, OrdStar::SPEC_B, ""));
   ara.add_star(OrdStar(240.829753f, 58.958694f, 3.77f, OrdStar::SPEC_K, ""));
   ara.add_star(OrdStar(240.993069f, 52.839500f, 4.06f, OrdStar::SPEC_K, ""));
   ara.add_link(0, 1);
   ara.add_link(0, 2);
   ara.add_link(2, 6);
   ara.add_link(2, 7);
   ara.add_link(3, 4);
   cons_list.push_back(ara);
   tot_stars += ara.total_stars();
   tot_links += ara.total_links();

   Constellation aries("Aries");
   aries.add_star(OrdStar(30.119525f, -23.462778f, 2.01f, OrdStar::SPEC_K, "Hamal"));
   aries.add_star(OrdStar(15.910653f, -20.808306f, 2.64f, OrdStar::SPEC_A, "Sheratan"));
   aries.add_star(OrdStar(30.833053f, -27.260778f, 3.61f, OrdStar::SPEC_B, "Bharani"));
   aries.add_star(OrdStar(15.892158f, -19.294083f, 3.88f, OrdStar::SPEC_B, "Mesarthim"));
   aries.add_star(OrdStar(45.193797f, -19.726694f, 4.35f, OrdStar::SPEC_K, "Botein"));
   aries.add_link(0, 1);
   aries.add_link(0, 2);
   aries.add_link(1, 3);
   aries.add_link(2, 4);
   cons_list.push_back(aries);
   tot_stars += aries.total_stars();
   tot_links += aries.total_links();

   Constellation auriga("Auriga");
   auriga.add_star(OrdStar(75.278139f, -45.999028f, 0.08f, OrdStar::SPEC_G, "Capella"));
   auriga.add_star(OrdStar(75.992158f, -44.947444f, 1.90f, OrdStar::SPEC_A, "Menkalinan"));
   auriga.add_star(OrdStar(75.995344f, -37.212778f, 2.65f, OrdStar::SPEC_A, "Mahasim"));
   auriga.add_star(OrdStar(60.949894f, -33.166139f, 2.69f, OrdStar::SPEC_K, "Kabdhilinan"));
   auriga.add_star(OrdStar(75.032814f, -43.823306f, 3.03f, OrdStar::SPEC_F, "Almaaz"));
   auriga.add_star(OrdStar(75.108575f, -41.234639f, 3.18f, OrdStar::SPEC_B, "Haedus""II"));
   auriga.add_star(OrdStar(75.041300f, -41.075889f, 3.69f, OrdStar::SPEC_K, "Sadatoni"));
   auriga.add_star(OrdStar(75.992097f, -54.284972f, 3.72f, OrdStar::SPEC_K, "Prijipati"));
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
   bootes.add_star(OrdStar(210.261208f, -19.187278f, -0.05f, OrdStar::SPEC_K, "Arcturus"));
   bootes.add_star(OrdStar(210.749792f, -27.074167f, 2.35f, OrdStar::SPEC_A, "Izar"));
   bootes.add_star(OrdStar(195.911422f, -18.398583f, 2.68f, OrdStar::SPEC_G, "Muphrid"));
   bootes.add_star(OrdStar(210.534656f, -38.307889f, 3.04f, OrdStar::SPEC_A, "Seginus"));
   bootes.add_star(OrdStar(225.258361f, -33.315111f, 3.46f, OrdStar::SPEC_G, ""));
   bootes.add_star(OrdStar(225.032442f, -40.390639f, 3.49f, OrdStar::SPEC_G, "Nekkar"));
   bootes.add_star(OrdStar(210.530517f, -30.371139f, 3.57f, OrdStar::SPEC_K, ""));
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
   caelum.add_star(OrdStar(60.676061f, 40.136417f, 4.44f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(75.073419f, 34.517139f, 4.55f, OrdStar::SPEC_K, ""));
   caelum.add_star(OrdStar(60.700958f, 37.144778f, 5.04f, OrdStar::SPEC_F, ""));
   caelum.add_star(OrdStar(60.513917f, 44.953750f, 5.07f, OrdStar::SPEC_B, ""));
   caelum.add_link(0, 2);
   caelum.add_link(0, 3);
   caelum.add_link(1, 2);
   cons_list.push_back(caelum);
   tot_stars += caelum.total_stars();
   tot_links += caelum.total_links();

   Constellation camelopardalis("Camelopardalis");
   camelopardalis.add_star(OrdStar(75.056972f, -60.442278f, 4.03f, OrdStar::SPEC_G, ""));
   camelopardalis.add_star(OrdStar(45.484481f, -59.940333f, 4.21f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(60.900836f, -66.342667f, 4.26f, OrdStar::SPEC_B, ""));
   camelopardalis.add_star(OrdStar(45.825358f, -65.526000f, 4.39f, OrdStar::SPEC_M, ""));
   camelopardalis.add_star(OrdStar(60.954783f, -53.752083f, 4.43f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(105.001069f, -76.977444f, 4.55f, OrdStar::SPEC_K, ""));
   camelopardalis.add_star(OrdStar(45.839300f, -71.332361f, 4.59f, OrdStar::SPEC_A, ""));
   camelopardalis.add_star(OrdStar(90.314106f, -69.320028f, 4.76f, OrdStar::SPEC_A, ""));
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
   cancer.add_star(OrdStar(120.275264f, -9.185667f, 3.53f, OrdStar::SPEC_K, "Al Tarf"));
   cancer.add_star(OrdStar(120.744753f, -18.154861f, 3.94f, OrdStar::SPEC_K, "Asellus Australis"));
   cancer.add_star(OrdStar(120.778286f, -28.760000f, 4.03f, OrdStar::SPEC_G, ""));
   cancer.add_star(OrdStar(120.974778f, -11.857778f, 4.26f, OrdStar::SPEC_A, "Acubens"));
   cancer.add_star(OrdStar(120.721447f, -21.468583f, 4.66f, OrdStar::SPEC_A, "Asellus Borealis"));
   cancer.add_star(OrdStar(120.203531f, -17.648139f, 4.67f, OrdStar::SPEC_G, "Tegmine"));
   cancer.add_link(0, 1);
   cancer.add_link(1, 3);
   cancer.add_link(1, 4);
   cancer.add_link(2, 4);
   cons_list.push_back(cancer);
   tot_stars += cancer.total_stars();
   tot_links += cancer.total_links();

   Constellation canes_venatici("Canes Venatici");
   canes_venatici.add_star(OrdStar(180.933844f, -38.318250f, 2.89f, OrdStar::SPEC_A, "Cor Caroli"));
   canes_venatici.add_star(OrdStar(180.562525f, -41.356778f, 4.24f, OrdStar::SPEC_G, "Chara"));
   canes_venatici.add_link(0, 1);
   cons_list.push_back(canes_venatici);
   tot_stars += canes_venatici.total_stars();
   tot_links += canes_venatici.total_links();

   Constellation canis_major("Canis Major");
   canis_major.add_star(OrdStar(90.752569f, 16.713139f, 1.46f, OrdStar::SPEC_A, "Sirius"));
   canis_major.add_star(OrdStar(90.977097f, 28.972083f, 1.50f, OrdStar::SPEC_B, "Adhara"));
   canis_major.add_star(OrdStar(105.139858f, 26.393194f, 1.83f, OrdStar::SPEC_F, "Wezen"));
   canis_major.add_star(OrdStar(90.378331f, 17.955917f, 1.98f, OrdStar::SPEC_B, "Mirzam"));
   canis_major.add_star(OrdStar(105.401586f, 29.303111f, 2.45f, OrdStar::SPEC_B, "Aludra"));
   canis_major.add_star(OrdStar(90.338553f, 30.063389f, 3.02f, OrdStar::SPEC_B, "Furud"));
   canis_major.add_star(OrdStar(105.050408f, 23.833306f, 3.02f, OrdStar::SPEC_B, ""));
   canis_major.add_star(OrdStar(105.028653f, 27.934833f, 3.49f, OrdStar::SPEC_K, ""));
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
   canis_minor.add_star(OrdStar(105.655150f, -5.227500f, 0.34f, OrdStar::SPEC_F, "Procyon"));
   canis_minor.add_star(OrdStar(105.452519f, -8.289417f, 2.89f, OrdStar::SPEC_B, "Gomeisa"));
   canis_minor.add_link(0, 1);
   cons_list.push_back(canis_minor);
   tot_stars += canis_minor.total_stars();
   tot_links += canis_minor.total_links();

   Constellation capricornus("Capricornus");
   capricornus.add_star(OrdStar(315.783969f, 16.126556f, 2.85f, OrdStar::SPEC_A, "Deneb"));
   capricornus.add_star(OrdStar(300.350181f, 14.781389f, 3.05f, OrdStar::SPEC_A, "Dabih"));
   capricornus.add_star(OrdStar(300.300894f, 12.544861f, 3.58f, OrdStar::SPEC_G, "Algedi Secunda"));
   capricornus.add_star(OrdStar(315.668150f, 16.662250f, 3.69f, OrdStar::SPEC_A, "Nashira"));
   capricornus.add_star(OrdStar(315.444453f, 22.411389f, 3.77f, OrdStar::SPEC_G, ""));
   capricornus.add_star(OrdStar(315.099106f, 16.767278f, 4.08f, OrdStar::SPEC_A, ""));
   capricornus.add_star(OrdStar(300.863694f, 26.919139f, 4.12f, OrdStar::SPEC_K, "Baten Algiedi"));
   capricornus.add_star(OrdStar(300.768269f, 25.270528f, 4.13f, OrdStar::SPEC_F, "Yue"));
   capricornus.add_star(OrdStar(315.370772f, 16.834556f, 4.28f, OrdStar::SPEC_G, ""));
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
   carina.add_star(OrdStar(90.399192f, 51.304278f, -0.72f, OrdStar::SPEC_F, "Canopus"));
   carina.add_star(OrdStar(135.220067f, 68.282528f, 1.67f, OrdStar::SPEC_A, "Miaplacidus"));
   carina.add_star(OrdStar(120.375239f, 58.490472f, 1.86f, OrdStar::SPEC_K, "Avior"));
   carina.add_star(OrdStar(135.284842f, 58.724750f, 2.21f, OrdStar::SPEC_A, "Aspidiske"));
   carina.add_star(OrdStar(150.715953f, 63.605528f, 2.74f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(135.785039f, 64.927972f, 2.92f, OrdStar::SPEC_A, ""));
   carina.add_star(OrdStar(150.228967f, 69.962083f, 3.29f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(150.533744f, 60.314639f, 3.30f, OrdStar::SPEC_B, ""));
   carina.add_star(OrdStar(150.284725f, 60.667667f, 3.39f, OrdStar::SPEC_K, ""));
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
   cassiopeia.add_star(OrdStar(0.945139f, -60.716750f, 2.15f, OrdStar::SPEC_B, "Tsih"));
   cassiopeia.add_star(OrdStar(0.675108f, -56.537417f, 2.24f, OrdStar::SPEC_K, "Schedar"));
   cassiopeia.add_star(OrdStar(0.152803f, -59.150222f, 2.28f, OrdStar::SPEC_F, "Caph"));
   cassiopeia.add_star(OrdStar(15.430167f, -60.235417f, 2.66f, OrdStar::SPEC_A, "Ruchbah"));
   cassiopeia.add_star(OrdStar(15.906578f, -63.670139f, 3.35f, OrdStar::SPEC_B, "Segin"));
   cassiopeia.add_star(OrdStar(0.818083f, -57.816556f, 3.46f, OrdStar::SPEC_G, "Achird"));
   cassiopeia.add_star(OrdStar(0.616186f, -53.896944f, 3.69f, OrdStar::SPEC_B, ""));
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
   centaurus.add_star(OrdStar(210.661361f, 59.164861f, -0.01f, OrdStar::SPEC_G, "Rigil Kentaurus"));
   centaurus.add_star(OrdStar(210.063733f, 59.627028f, 0.61f, OrdStar::SPEC_B, "Hadar"));
   centaurus.add_star(OrdStar(210.111478f, 35.631306f, 2.06f, OrdStar::SPEC_K, "Menkent"));
   centaurus.add_star(OrdStar(180.692000f, 47.040111f, 2.20f, OrdStar::SPEC_A, "Muhlifain"));
   centaurus.add_star(OrdStar(195.664797f, 52.533639f, 2.29f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(210.591792f, 41.842250f, 2.33f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(195.925675f, 46.711722f, 2.55f, OrdStar::SPEC_B, "Alnair"));
   centaurus.add_star(OrdStar(195.343353f, 35.287917f, 2.75f, OrdStar::SPEC_A, ""));
   centaurus.add_star(OrdStar(180.139317f, 49.277583f, 2.58f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(210.986028f, 41.895861f, 3.13f, OrdStar::SPEC_B, ""));
   centaurus.add_star(OrdStar(195.826947f, 41.526306f, 3.47f, OrdStar::SPEC_B, ""));
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
   cepheus.add_star(OrdStar(315.309606f, -62.585444f, 2.45f, OrdStar::SPEC_A, "Alderamin"));
   cepheus.add_star(OrdStar(345.655828f, -77.631972f, 3.21f, OrdStar::SPEC_K, "Arrai"));
   cepheus.add_star(OrdStar(315.477661f, -70.560694f, 3.23f, OrdStar::SPEC_B, "Alfirk"));
   cepheus.add_star(OrdStar(330.180906f, -58.201250f, 3.39f, OrdStar::SPEC_K, "Tsao Fu"));
   cepheus.add_star(OrdStar(300.754797f, -61.836806f, 3.41f, OrdStar::SPEC_K, ""));
   cepheus.add_star(OrdStar(330.828031f, -66.200722f, 3.50f, OrdStar::SPEC_K, ""));
   cepheus.add_star(OrdStar(330.486181f, -58.415194f, 4.07f, OrdStar::SPEC_G, "Alrediph"));
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
   cetus.add_star(OrdStar(0.726453f, 16.013306f, 2.04f, OrdStar::SPEC_K, "Deneb Kaitos"));
   cetus.add_star(OrdStar(45.037994f, -4.089917f, 2.54f, OrdStar::SPEC_M, "Menkar"));
   cetus.add_star(OrdStar(30.322442f, 1.022944f, 3.04f, OrdStar::SPEC_M, "Mira Ceti"));
   cetus.add_star(OrdStar(15.143128f, 9.818083f, 3.46f, OrdStar::SPEC_K, "Deneb Algenubi"));
   cetus.add_star(OrdStar(30.721700f, -3.236167f, 3.47f, OrdStar::SPEC_A, "Kaffaljidhmah"));
   cetus.add_star(OrdStar(15.734758f, 14.060444f, 3.49f, OrdStar::SPEC_G, "Durre Menthor"));
   cetus.add_star(OrdStar(0.323800f, 7.176167f, 3.56f, OrdStar::SPEC_K, "Schemali"));
   cetus.add_star(OrdStar(15.400403f, 7.817250f, 3.60f, OrdStar::SPEC_K,  ""));
   cetus.add_star(OrdStar(15.857669f, 9.665056f, 3.74f, OrdStar::SPEC_K, "Baten Kaitos"));
   cetus.add_star(OrdStar(30.000061f, 20.922222f, 3.99f, OrdStar::SPEC_M,  ""));
   cetus.add_star(OrdStar(30.658042f, -0.328528f, 4.08f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(30.748992f, -10.114222f, 4.27f, OrdStar::SPEC_F,  ""));
   cetus.add_star(OrdStar(30.469311f, -8.460083f, 4.30f, OrdStar::SPEC_B,  ""));
   cetus.add_star(OrdStar(30.995250f, -8.907389f, 4.71f, OrdStar::SPEC_B, "Menkar"));
   cetus.add_star(OrdStar(30.597914f, -5.593306f, 4.87f, OrdStar::SPEC_G,  ""));
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
   chamaeleon.add_star(OrdStar(120.308686f, 75.080028f, 4.05f, OrdStar::SPEC_F, ""));
   chamaeleon.add_star(OrdStar(150.591172f, 77.392194f, 4.11f, OrdStar::SPEC_M, ""));
   chamaeleon.add_star(OrdStar(180.305817f, 78.687722f, 4.24f, OrdStar::SPEC_B, ""));
   chamaeleon.add_star(OrdStar(120.344136f, 76.515417f, 4.34f, OrdStar::SPEC_K, ""));
   chamaeleon.add_star(OrdStar(150.754550f, 79.530472f, 5.46f, OrdStar::SPEC_K, ""));
   chamaeleon.add_link(0, 1);
   chamaeleon.add_link(0, 3);
   chamaeleon.add_link(1, 2);
   chamaeleon.add_link(1, 4);
   cons_list.push_back(chamaeleon);
   tot_stars += chamaeleon.total_stars();
   tot_links += chamaeleon.total_links();

   Constellation circinus("Circinus");
   circinus.add_star(OrdStar(210.708525f, 63.025417f, 3.18f, OrdStar::SPEC_F, ""));
   circinus.add_star(OrdStar(225.291933f, 57.199111f, 4.07f, OrdStar::SPEC_A, ""));
   circinus.add_star(OrdStar(225.389628f, 58.679306f, 4.48f, OrdStar::SPEC_B, ""));
   circinus.add_link(0, 1);
   circinus.add_link(0, 2);
   cons_list.push_back(circinus);
   tot_stars += circinus.total_stars();
   tot_links += circinus.total_links();

   Constellation columba("Columba");
   columba.add_star(OrdStar(75.660817f, 33.925944f, 2.65f, OrdStar::SPEC_B, "Phact"));
   columba.add_star(OrdStar(75.849319f, 34.230694f, 3.12f, OrdStar::SPEC_K, "Wezn"));
   columba.add_star(OrdStar(90.368569f, 32.563722f, 3.85f, OrdStar::SPEC_G, ""));
   columba.add_star(OrdStar(75.520206f, 34.529556f, 3.86f, OrdStar::SPEC_K, ""));
   columba.add_star(OrdStar(75.958947f, 34.716694f, 4.36f, OrdStar::SPEC_B, ""));
   columba.add_link(0, 1);
   columba.add_link(0, 3);
   columba.add_link(1, 4);
   columba.add_link(2, 4);
   cons_list.push_back(columba);
   tot_stars += columba.total_stars();
   tot_links += columba.total_links();

   Constellation coma_berenices("Coma Berenices");
   coma_berenices.add_star(OrdStar(195.198033f, -27.876028f, 4.23f, OrdStar::SPEC_G, ""));
   coma_berenices.add_star(OrdStar(195.166542f, -17.529111f, 4.32f, OrdStar::SPEC_F, "Diadem"));
   coma_berenices.add_star(OrdStar(180.448981f, -28.268611f, 4.35f, OrdStar::SPEC_K, ""));
   coma_berenices.add_link(0, 1);
   coma_berenices.add_link(0, 2);
   cons_list.push_back(coma_berenices);
   tot_stars += coma_berenices.total_stars();
   tot_links += coma_berenices.total_links();

   Constellation corona_australis("Corona Australis");
   corona_australis.add_star(OrdStar(285.167153f, 38.659306f, 4.10f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(285.157856f, 36.095750f, 4.11f, OrdStar::SPEC_A, "Alphekka Meridiana"));
   corona_australis.add_star(OrdStar(285.106956f, 36.937250f, 4.23f, OrdStar::SPEC_F, ""));
   corona_australis.add_star(OrdStar(285.139147f, 39.503361f, 4.57f, OrdStar::SPEC_K, ""));
   corona_australis.add_star(OrdStar(285.051897f, 41.905000f, 4.74f, OrdStar::SPEC_A, ""));
   corona_australis.add_star(OrdStar(270.978742f, 36.892917f, 4.83f, OrdStar::SPEC_F, ""));
   corona_australis.add_link(0, 1);
   corona_australis.add_link(0, 3);
   corona_australis.add_link(1, 2);
   corona_australis.add_link(2, 5);
   corona_australis.add_link(3, 4);
   cons_list.push_back(corona_australis);
   tot_stars += corona_australis.total_stars();
   tot_links += corona_australis.total_links();

   Constellation corona_borealis("Corona Borealis");
   corona_borealis.add_star(OrdStar(225.578108f, -26.714917f, 2.22f, OrdStar::SPEC_A, "Alphecca"));
   corona_borealis.add_star(OrdStar(225.463847f, -29.105500f, 3.66f, OrdStar::SPEC_F, "Nusakan"));
   corona_borealis.add_star(OrdStar(225.712400f, -26.295528f, 3.81f, OrdStar::SPEC_A, ""));
   corona_borealis.add_star(OrdStar(225.548833f, -31.359167f, 4.14f, OrdStar::SPEC_B, ""));
   corona_borealis.add_star(OrdStar(225.959806f, -26.878028f, 4.14f, OrdStar::SPEC_K, ""));
   corona_borealis.add_star(OrdStar(225.826583f, -26.068556f, 4.59f, OrdStar::SPEC_G, ""));
   corona_borealis.add_link(0, 1);
   corona_borealis.add_link(0, 2);
   corona_borealis.add_link(1, 3);
   corona_borealis.add_link(4, 5);
   cons_list.push_back(corona_borealis);
   tot_stars += corona_borealis.total_stars();
   tot_links += corona_borealis.total_links();

   Constellation corvus("Corvus");
   corvus.add_star(OrdStar(180.263464f, 16.458028f, 2.58f, OrdStar::SPEC_B, "Gienah Corvi"));
   corvus.add_star(OrdStar(180.573119f, 22.603389f, 2.65f, OrdStar::SPEC_G, "Kraz"));
   corvus.add_star(OrdStar(180.497772f, 15.484917f, 2.94f, OrdStar::SPEC_B, "Algorab"));
   corvus.add_star(OrdStar(180.168758f, 21.380194f, 3.02f, OrdStar::SPEC_K, "Minkar"));
   corvus.add_star(OrdStar(180.140208f, 23.271222f, 4.02f, OrdStar::SPEC_F, "Al Chiba"));
   corvus.add_link(0, 2);
   corvus.add_link(0, 3);
   corvus.add_link(1, 2);
   corvus.add_link(1, 3);
   cons_list.push_back(corvus);
   tot_stars += corvus.total_stars();
   tot_links += corvus.total_links();

   Constellation crater("Crater");
   crater.add_star(OrdStar(165.322367f, 13.220944f, 3.56f, OrdStar::SPEC_K, ""));
   crater.add_star(OrdStar(165.414717f, 16.315972f, 4.06f, OrdStar::SPEC_A, ""));
   crater.add_star(OrdStar(150.996319f, 17.700889f, 4.08f, OrdStar::SPEC_K, "Alkes"));
   crater.add_star(OrdStar(165.194303f, 21.174389f, 4.46f, OrdStar::SPEC_A, ""));
   crater.add_link(0, 1);
   crater.add_link(0, 2);
   crater.add_link(2, 3);
   cons_list.push_back(crater);
   tot_stars += crater.total_stars();
   tot_links += crater.total_links();

   Constellation crux("Crux");
   crux.add_star(OrdStar(180.795367f, 58.311278f, 1.25f, OrdStar::SPEC_B, "Mimosa"));
   crux.add_star(OrdStar(180.443317f, 62.900944f, 1.4f, OrdStar::SPEC_B, "Acrux"));
   crux.add_star(OrdStar(180.519425f, 56.887444f, 1.59f, OrdStar::SPEC_M, "Gacrux"));
   crux.add_star(OrdStar(180.252433f, 57.251111f, 2.79f, OrdStar::SPEC_B, "Palida"));
   crux.add_star(OrdStar(180.356058f, 59.598639f, 3.59f, OrdStar::SPEC_K, "Intrometida"));
   crux.add_link(0, 3);
   crux.add_link(1, 2);
   cons_list.push_back(crux);
   tot_stars += crux.total_stars();
   tot_links += crux.total_links();

   Constellation cygnus("Cygnus");
   cygnus.add_star(OrdStar(300.690531f, -45.280333f, 1.25f, OrdStar::SPEC_A, "Deneb Cygni"));
   cygnus.add_star(OrdStar(300.370472f, -40.256694f, 2.23f, OrdStar::SPEC_F, "Sadr"));
   cygnus.add_star(OrdStar(300.770119f, -33.969444f, 2.48f, OrdStar::SPEC_K, "Gienah"));
   cygnus.add_star(OrdStar(285.749567f, -45.130694f, 2.86f, OrdStar::SPEC_B, "Ruc"));
   cygnus.add_star(OrdStar(285.512025f, -27.959694f, 3.05f, OrdStar::SPEC_K, "Al Minhar al Dajajah"));
   cygnus.add_star(OrdStar(315.215606f, -30.227083f, 3.21f, OrdStar::SPEC_G, ""));
   cygnus.add_star(OrdStar(285.495094f, -51.729472f, 3.76f, OrdStar::SPEC_A, ""));
   cygnus.add_star(OrdStar(285.285031f, -53.368167f, 3.80f, OrdStar::SPEC_K, ""));
   cygnus.add_star(OrdStar(285.938444f, -35.083500f, 3.89f, OrdStar::SPEC_K, ""));
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
   delphinus.add_star(OrdStar(300.625797f, -14.595194f, 3.64f, OrdStar::SPEC_F, "Rotanev"));
   delphinus.add_star(OrdStar(300.660625f, -15.912056f, 3.77f, OrdStar::SPEC_B, "Sualocin"));
   delphinus.add_star(OrdStar(300.553544f, -11.303333f, 4.03f, OrdStar::SPEC_B, "Deneb Dulfim"));
   delphinus.add_star(OrdStar(300.777644f, -16.124778f, 4.27f, OrdStar::SPEC_K, ""));
   delphinus.add_star(OrdStar(300.724319f, -15.074694f, 4.43f, OrdStar::SPEC_A, ""));
   delphinus.add_link(0, 1);
   delphinus.add_link(0, 2);
   delphinus.add_link(0, 4);
   delphinus.add_link(1, 3);
   delphinus.add_link(3, 4);
   cons_list.push_back(delphinus);
   tot_stars += delphinus.total_stars();
   tot_links += delphinus.total_links();

   Constellation dorado("Dorado");
   dorado.add_star(OrdStar(60.566589f, 54.955000f, 3.30f, OrdStar::SPEC_A, ""));
   dorado.add_star(OrdStar(75.560422f, 61.510139f, 3.76f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(60.267081f, 50.512917f, 4.26f, OrdStar::SPEC_F, ""));
   dorado.add_star(OrdStar(75.746228f, 64.264472f, 4.34f, OrdStar::SPEC_A, ""));
   dorado.add_link(0, 1);
   dorado.add_link(0, 2);
   cons_list.push_back(dorado);
   tot_stars += dorado.total_stars();
   tot_links += dorado.total_links();

   Constellation draco("Draco");
   draco.add_star(OrdStar(255.943439f, -51.488944f, 2.24f, OrdStar::SPEC_K, "Eltanin"));
   draco.add_star(OrdStar(240.399864f, -61.514083f, 2.73f, OrdStar::SPEC_G, ""));
   draco.add_star(OrdStar(255.507217f, -52.301361f, 2.79f, OrdStar::SPEC_G, "Rastaban"));
   draco.add_star(OrdStar(285.209208f, -67.661306f, 3.07f, OrdStar::SPEC_G, "Altais"));
   draco.add_star(OrdStar(255.146453f, -65.714639f, 3.17f, OrdStar::SPEC_B, "Aldhibah"));
   draco.add_star(OrdStar(225.415494f, -58.966028f, 3.29f, OrdStar::SPEC_K, "Edasich"));
   draco.add_star(OrdStar(270.350650f, -72.733694f, 3.55f, OrdStar::SPEC_F, "Batentaban Borealis"));
   draco.add_star(OrdStar(210.073175f, -64.375806f, 3.67f, OrdStar::SPEC_A, "Adib"));
   draco.add_star(OrdStar(255.892119f, -56.872444f, 3.73f, OrdStar::SPEC_K, "Grumium"));
   draco.add_star(OrdStar(165.523414f, -69.331111f, 3.82f, OrdStar::SPEC_M, "Giausar"));
   draco.add_star(OrdStar(285.802836f, -70.267833f, 3.84f, OrdStar::SPEC_G, "Tyl"));
   draco.add_star(OrdStar(180.558067f, -69.788222f, 3.85f, OrdStar::SPEC_B, "Ketu"));
   draco.add_star(OrdStar(240.031583f, -58.564444f, 4.01f, OrdStar::SPEC_F, ""));
   draco.add_star(OrdStar(255.536228f, -55.184111f, 4.89f, OrdStar::SPEC_A, "Kuma"));
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
   equuleus.add_star(OrdStar(315.263722f, -5.248083f, 3.92f, OrdStar::SPEC_G, "Kitalpha"));
   equuleus.add_star(OrdStar(315.241331f, -10.007722f, 4.47f, OrdStar::SPEC_F, "Pherasauval"));
   equuleus.add_star(OrdStar(315.172353f, -10.131944f, 4.70f, OrdStar::SPEC_F, ""));
   equuleus.add_star(OrdStar(315.381550f, -6.811111f, 5.16f, OrdStar::SPEC_A, ""));
   equuleus.add_star(OrdStar(300.984594f, -4.293833f, 5.30f, OrdStar::SPEC_F, ""));
   equuleus.add_link(0, 1);
   equuleus.add_link(1, 2);
   cons_list.push_back(equuleus);
   tot_stars += equuleus.total_stars();
   tot_links += equuleus.total_links();

   Constellation eradanus("Eradanus");
   eradanus.add_star(OrdStar(15.628542f, 56.763333f, 0.45f, OrdStar::SPEC_B, "Eridani"));
   eradanus.add_star(OrdStar(75.130842f, 4.913750f, 2.78f, OrdStar::SPEC_A, "Cursa"));
   eradanus.add_star(OrdStar(30.971033f, 39.695278f, 2.88f, OrdStar::SPEC_A, "Acamar"));
   eradanus.add_star(OrdStar(45.967147f, 12.491750f, 2.97f, OrdStar::SPEC_M, "Zaurak"));
   eradanus.add_star(OrdStar(45.720822f, 8.234806f, 3.52f, OrdStar::SPEC_K, "Rana"));
   eradanus.add_star(OrdStar(60.298228f, 32.201667f, 3.55f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(30.275139f, 50.487889f, 3.56f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(15.932453f, 50.390417f, 3.69f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(45.325269f, 20.242056f, 3.70f, OrdStar::SPEC_M, ""));
   eradanus.add_star(OrdStar(45.549006f, 8.541694f, 3.72f, OrdStar::SPEC_K, "Sadira"));
   eradanus.add_star(OrdStar(60.592519f, 29.437694f, 3.81f, OrdStar::SPEC_G, "Beemin"));
   eradanus.add_star(OrdStar(30.940444f, 7.102389f, 3.89f, OrdStar::SPEC_K, "Azha"));
   eradanus.add_star(OrdStar(60.605317f, 2.647556f, 3.93f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(60.400603f, 33.983000f, 3.97f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(60.758372f, 2.745389f, 4.01f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(60.197758f, 5.162222f, 4.04f, OrdStar::SPEC_F, "Beid"));
   eradanus.add_star(OrdStar(45.039886f, 22.375667f, 4.08f, OrdStar::SPEC_A, ""));
   eradanus.add_star(OrdStar(30.677758f, 38.144694f, 4.11f, OrdStar::SPEC_K, ""));
   eradanus.add_star(OrdStar(45.824244f, 35.799889f, 4.17f, OrdStar::SPEC_G, ""));
   eradanus.add_star(OrdStar(45.780831f, 22.751556f, 4.22f, OrdStar::SPEC_F, ""));
   eradanus.add_star(OrdStar(30.449750f, 46.296167f, 4.24f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(45.563125f, 20.367194f, 4.26f, OrdStar::SPEC_B, ""));
   eradanus.add_star(OrdStar(30.751661f, 17.427361f, 4.47f, OrdStar::SPEC_F, ""));
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
   fornax.add_star(OrdStar(45.201189f, 27.010889f, 3.80f, OrdStar::SPEC_F, "Dalim"));
   fornax.add_star(OrdStar(30.818156f, 31.593722f, 4.45f, OrdStar::SPEC_G, ""));
   fornax.add_star(OrdStar(30.074842f, 28.703167f, 4.68f, OrdStar::SPEC_B, ""));
   fornax.add_link(0, 1);
   cons_list.push_back(fornax);
   tot_stars += fornax.total_stars();
   tot_links += fornax.total_links();

   Constellation gemini("Gemini");
   gemini.add_star(OrdStar(105.755378f, -28.026306f, 1.16f, OrdStar::SPEC_K, "Pollux"));
   gemini.add_star(OrdStar(105.576667f, -31.888639f, 1.58f, OrdStar::SPEC_A, "Castor"));
   gemini.add_star(OrdStar(90.628528f, -16.399417f, 1.93f, OrdStar::SPEC_A, "Alhena"));
   gemini.add_star(OrdStar(90.382664f, -22.513861f, 2.87f, OrdStar::SPEC_M, "Tejat"));
   gemini.add_star(OrdStar(90.732203f, -25.131167f, 3.06f, OrdStar::SPEC_A, "Mebsuta"));
   gemini.add_star(OrdStar(90.247972f, -22.506833f, 3.31f, OrdStar::SPEC_M, "Propus"));
   gemini.add_star(OrdStar(90.754842f, -12.896056f, 3.35f, OrdStar::SPEC_F, "Alzir"));
   gemini.add_star(OrdStar(105.335386f, -21.982333f, 3.50f, OrdStar::SPEC_F, "Wasat"));
   gemini.add_star(OrdStar(105.740797f, -24.398139f, 3.57f, OrdStar::SPEC_G, ""));
   gemini.add_star(OrdStar(105.301558f, -16.540472f, 3.58f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(90.879817f, -33.961361f, 3.60f, OrdStar::SPEC_A, ""));
   gemini.add_star(OrdStar(105.428800f, -27.798278f, 3.78f, OrdStar::SPEC_G, "Propus"));
   gemini.add_star(OrdStar(105.068483f, -20.570306f, 4.01f, OrdStar::SPEC_G, "Mekbuda"));
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
   grus.add_star(OrdStar(330.137189f, 45.039389f, 1.73f, OrdStar::SPEC_B, "Alnair"));
   grus.add_star(OrdStar(330.711092f, 45.115444f, 2.07f, OrdStar::SPEC_M, "Gruid"));
   grus.add_star(OrdStar(315.898792f, 36.635167f, 3.00f, OrdStar::SPEC_B, "Al Dhanab"));
   grus.add_star(OrdStar(330.809222f, 50.683306f, 3.49f, OrdStar::SPEC_A, ""));
   grus.add_star(OrdStar(330.487819f, 42.504444f, 3.97f, OrdStar::SPEC_G, ""));
   grus.add_star(OrdStar(330.101917f, 38.456944f, 4.47f, OrdStar::SPEC_M, ""));
   grus.add_star(OrdStar(330.260244f, 40.653250f, 4.79f, OrdStar::SPEC_G, ""));
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
   hercules.add_star(OrdStar(240.503683f, -21.489639f, 2.78f, OrdStar::SPEC_G, "Kornephoros"));
   hercules.add_star(OrdStar(240.688189f, -31.601889f, 2.81f, OrdStar::SPEC_F, "Rutilicus"));
   hercules.add_star(OrdStar(255.250533f, -24.839583f, 3.12f, OrdStar::SPEC_A, "Sarin"));
   hercules.add_star(OrdStar(255.250792f, -36.809167f, 3.16f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(255.244128f, -14.390250f, 3.31f, OrdStar::SPEC_M, "Ras Algethi"));
   hercules.add_star(OrdStar(255.774367f, -27.722500f, 3.42f, OrdStar::SPEC_G, ""));
   hercules.add_star(OrdStar(240.714928f, -38.922472f, 3.48f, OrdStar::SPEC_G, ""));
   hercules.add_star(OrdStar(255.962731f, -29.247917f, 3.70f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(240.365344f, -19.153028f, 3.74f, OrdStar::SPEC_A, ""));
   hercules.add_star(OrdStar(270.125708f, -28.762472f, 3.84f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(255.937550f, -37.250528f, 3.86f, OrdStar::SPEC_K, ""));
   hercules.add_star(OrdStar(240.329014f, -46.313278f, 3.91f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(255.004836f, -30.926333f, 3.92f, OrdStar::SPEC_A, "Cujam"));
   hercules.add_star(OrdStar(255.394714f, -37.145917f, 4.15f, OrdStar::SPEC_B, ""));
   hercules.add_star(OrdStar(240.568386f, -42.436889f, 4.20f, OrdStar::SPEC_B, ""));
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
   horologium.add_star(OrdStar(60.233356f, 41.706139f, 3.85f, OrdStar::SPEC_K, ""));
   horologium.add_star(OrdStar(30.979936f, 63.928694f, 4.98f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(45.060250f, 58.262389f, 5.12f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(30.677661f, 53.450083f, 5.21f, OrdStar::SPEC_F, ""));
   horologium.add_star(OrdStar(30.623406f, 51.456917f, 5.30f, OrdStar::SPEC_A, ""));
   horologium.add_star(OrdStar(30.709211f, 49.199167f, 5.40f, OrdStar::SPEC_G, ""));
   horologium.add_link(0, 5);
   horologium.add_link(1, 2);
   horologium.add_link(2, 3);
   horologium.add_link(3, 4);
   horologium.add_link(4, 5);
   cons_list.push_back(horologium);
   tot_stars += horologium.total_stars();
   tot_links += horologium.total_links();

   Constellation hydra("Hydra");
   hydra.add_star(OrdStar(135.459792f, 7.341306f, 1.99f, OrdStar::SPEC_K, "Alfard"));
   hydra.add_star(OrdStar(195.315347f, 22.828583f, 2.99f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(120.923244f, -5.945528f, 3.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(150.827064f, 15.805861f, 3.11f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(210.106186f, 25.317972f, 3.25f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(120.779625f, -6.418917f, 3.38f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(165.550072f, 30.142472f, 3.54f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(150.176500f, 11.646167f, 3.61f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(150.434864f, 15.163917f, 3.83f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(135.239386f, -2.315028f, 3.89f, OrdStar::SPEC_B, ""));
   hydra.add_star(OrdStar(135.664258f, 0.857333f, 3.90f, OrdStar::SPEC_K, ""));
   hydra.add_star(OrdStar(135.857967f, 13.153444f, 4.11f, OrdStar::SPEC_G, ""));
   hydra.add_star(OrdStar(165.881822f, 32.091861f, 4.29f, OrdStar::SPEC_A, ""));
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
   hydrus.add_star(OrdStar(0.427556f, 76.744972f, 2.82f, OrdStar::SPEC_G, ""));
   hydrus.add_star(OrdStar(15.979408f, 60.430083f, 2.86f, OrdStar::SPEC_F, ""));
   hydrus.add_star(OrdStar(45.787286f, 73.760750f, 3.26f, OrdStar::SPEC_M, "Foo Pih"));
   hydrus.add_star(OrdStar(30.362506f, 67.340583f, 4.08f, OrdStar::SPEC_A, ""));
   hydrus.add_star(OrdStar(30.659783f, 67.733056f, 4.12f, OrdStar::SPEC_B, ""));
   hydrus.add_link(0, 2);
   hydrus.add_link(1, 3);
   hydrus.add_link(2, 4);
   hydrus.add_link(3, 4);
   cons_list.push_back(hydrus);
   tot_stars += hydrus.total_stars();
   tot_links += hydrus.total_links();

   Constellation indus("Indus");
   indus.add_star(OrdStar(300.626108f, 46.708333f, 3.11f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(300.913494f, 57.545917f, 3.67f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(315.331078f, 52.550722f, 4.39f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(315.965286f, 53.007444f, 4.40f, OrdStar::SPEC_F, ""));
   indus.add_star(OrdStar(300.733942f, 50.079167f, 4.51f, OrdStar::SPEC_A, ""));
   indus.add_star(OrdStar(330.054844f, 55.220194f, 4.69f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(300.824703f, 45.773111f, 4.90f, OrdStar::SPEC_K, ""));
   indus.add_star(OrdStar(300.858347f, 50.391833f, 5.06f, OrdStar::SPEC_K, ""));
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
