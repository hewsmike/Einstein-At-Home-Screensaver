#!/bin/bash

###########################################################################
#   Copyright (C) 2008-2009 by Oliver Bock                                #
#   oliver.bock[AT]aei.mpg.de                                             #
#                                                                         #
#   Copyright (C) 2012 by Mike Hewson  [ any & all errors are mine :-) ]  #
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

### globals ############################################################################################################

# Set paths.
ROOT=`pwd`

# Set the logfile.
LOGFILE=$ROOT/build.log

# No target set initially each time we run ( to be discovered later ).
TARGET=0

# Target variants.
TARGET_ALL=1
TARGET_LINUX=2
TARGET_MAC=3
TARGET_MAC_SDK=4
TARGET_WIN32=5
TARGET_DOC=6
TARGET_CLEAN=7

### functions (utility) ################################################################################################

log() {
    echo $1 | tee -a $LOGFILE

    return 0
    }

prepare_directories() {
    log "Preparing $1 directories ... "
    mkdir -p $ROOT/$1
    rm -rf $ROOT/$1/src
    rm -rf $ROOT/$1/patches
    rm -f $ROOT/$1/build.sh

    cp -rf $ROOT/src $ROOT/$1/src
    cp -rf $ROOT/patches $ROOT/$1/patches
    cp -f $ROOT/build.sh $ROOT/$1/build.sh
    }

### main control #######################################################################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "Top level build entry"
log "given : $1 $2"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

# Improved command line parsing :-)

if [ $# -eq 0 ]; then
    print_usage
    exit 1
fi

if [ $# -eq 1 ]; then
    case "$1" in
        "--doc")
            TARGET=$TARGET_DOC
            ;;
        "--distclean")
            TARGET=$TARGET_CLEAN
            exit 0
            ;;
        *)
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -eq 2 ]; then
    case "$2" in
        "--starsphere")
            PRODUCT=starsphere
            PRODUCT_NAME="Starsphere"
            ;;
        "--solarsystem")
            PRODUCT=solarsystem
            PRODUCT_NAME="SolarSystem"
            ;;
        "--all")
            ;;
        *)
            print_usage
            exit 1
            ;;
    esac

    case "$1" in
        "--all")
            TARGET=$TARGET_ALL
            ;;
        "--linux")
            TARGET=$TARGET_LINUX
            ;;
        "--mac")
            TARGET=$TARGET_MAC
            ;;
        "--mac-sdk")
            TARGET=$TARGET_MAC_SDK
            ;;
        "--win32")
            TARGET=$TARGET_WIN32
            ;;
        *)
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -gt 2 ]; then
    print_usage
    exit 1
fi

# here we go...

case $TARGET in
    $TARGET_ALL)
        ./topbuild.sh --linux --$PRODUCT
        ./topbuild.sh --win32 --$PRODUCT
        ;;
    $TARGET_LINUX)
        prepare_directories linux
        log "For $PRODUCT_NAME : invoking linux build script ... "
        cd linux
        ./build.sh --linux --$PRODUCT
        cd ..
        ;;
    $TARGET_MAC)
        prepare_directories mac
        log "For $PRODUCT_NAME : invoking mac build script ... "
        cd mac
        ./build.sh --mac --$PRODUCT
        cd ..
        ;;
    $TARGET_MAC_SDK)
        prepare_directories mac-sdk
        log "For $PRODUCT_NAME : invoking mac-sdk build script ... "
        cd mac_sdk
        ./build.sh --mac-sdk --$PRODUCT
        cd ..
        ;;
    $TARGET_WIN32)
        prepare_directories win32
        log "For $PRODUCT_NAME : invoking win32 build script ... "
        cd win32
        ./build.sh --win32 --$PRODUCT
        cd ..
        ;;
    $TARGET_DOC)
        log "Building documentation ... "
        doxygen Doxyfile >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.png $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.gif $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        ;;
    *)
        # should be unreachable
        print_usage
        exit 1
        ;;
esac

log "++++++++++++++++++++++++++++++++++++"
log "Top level build exit"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
