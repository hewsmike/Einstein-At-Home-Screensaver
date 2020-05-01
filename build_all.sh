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
echo "**** Building linux and win32 targets for product starsphere ****"
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

# Both builds good, so get their products to local directory.
mkdir -p dist
cd dist
cp -f $ROOT/linux/dist/graphics_app=einstein_RUNLABEL_VERSION_graphics_x86_64-pc-linux-pc-linux-gnu graphics_starsphere_TEST0$1_x86_64-pc-linux
cp -f $ROOT/win32/dist/graphics_app=einstein_RUNLABEL_VERSION_graphics_windows_intelx86.exe graphics_starsphere_TEST0$1_i686-w64-mingw32.exe

echo "*****************************************************************"
echo "**** Successful build of both products **************************"
echo "*****************************************************************"

exit 0