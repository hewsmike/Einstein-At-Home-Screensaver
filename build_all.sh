#!/bin/bash

###########################################################################
#   Copyright (C) 2020 by Mike Hewson  [ any & all errors are mine :-) ]  #
#   hewsmike[AT]iinet.net.au                                              #
#                                                                         #
#   This file is part of Einstein@Home.                                   #
#                                                                         #
#   Einstein@Home is free software: you can redistribute it and/or modify #
#   it under the terms of the GNU General Public License as published     #
#   by the Free Software Foundation, version 2 of the License.            #
#                                                                         #
#   Einstein@Home is distributed in the hope that it will be useful,      #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. #
#                                                                         #
###########################################################################

ROOT=`pwd`

echo "*****************************************************************"
echo "***** Building linux, win32 & win 64 targets for starsphere *****"
echo "*****************************************************************"

# First try to build for linux.
./topbuild.sh --linux --debug --starsphere
if [ $? -ne 0 ]; then
    echo "An error occurred while making the linux product"
    exit 1
fi

# Only proceed with the win32 build if the linux build was OK.
./topbuild.sh --win32 --debug --starsphere
if [ $? -ne 0 ]; then
    echo "An error occurred while making the win32 product"
    exit 2
fi

# Only proceed with the win64 build if the linux & win32 builds were OK.
./topbuild.sh --win64 --debug --starsphere
if [ $? -ne 0 ]; then
    echo "An error occurred while making the win64 product"
    exit 3
fi

# All builds good, so get their products to local directory.
mkdir -p dist
cd dist
cp -f $ROOT/linux/dist/graphics_app=einsteinGravity_RUNLABEL_VERSION_graphics_x86_64-pc-linux-gnu graphics_Gravity_starsphere_TEST0$1_x86_64-pc-linux
cp -f $ROOT/linux/dist/graphics_app=einsteinGamma_RUNLABEL_VERSION_graphics_x86_64-pc-linux-gnu graphics_Gamma_starsphere_TEST0$1_x86_64-pc-linux
cp -f $ROOT/linux/dist/graphics_app=einsteinRadio_RUNLABEL_VERSION_graphics_x86_64-pc-linux-gnu graphics_Radio_starsphere_TEST0$1_x86_64-pc-linux

cp -f $ROOT/win32/dist/graphics_app=einsteinGravity_RUNLABEL_VERSION_graphics_windows_intelx86.exe graphics_Gravity_starsphere_TEST0$1_i686-mingw32.exe
cp -f $ROOT/win32/dist/graphics_app=einsteinGamma_RUNLABEL_VERSION_graphics_windows_intelx86.exe graphics_Gamma_starsphere_TEST0$1_i686-mingw32.exe
cp -f $ROOT/win32/dist/graphics_app=einsteinRadio_RUNLABEL_VERSION_graphics_windows_intelx86.exe graphics_Radio_starsphere_TEST0$1_i686-mingw32.exe

cp -f $ROOT/win64/dist/graphics_app=einsteinGravity_RUNLABEL_VERSION_graphics_windows_intelx86_64.exe graphics_Gravity_starsphere_TEST0$1_x86_win64.exe
cp -f $ROOT/win64/dist/graphics_app=einsteinGamma_RUNLABEL_VERSION_graphics_windows_intelx86_64.exe graphics_Gamma_starsphere_TEST0$1_x86_win64.exe
cp -f $ROOT/win64/dist/graphics_app=einsteinRadio_RUNLABEL_VERSION_graphics_windows_intelx86_64.exe graphics_Radio_starsphere_TEST0$1_x86_win64.exe
echo "*****************************************************************"
echo "***** Successful build of all products **************************"
echo "*****************************************************************"

exit 0
