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

# Target variants.
TARGET_NONE=0
TARGET_ALL=1
TARGET_LINUX=2
TARGET_MAC=3
TARGET_MAC_SDK=4
TARGET_WIN32=5
TARGET_DOC=6
TARGET_CLEAN=7

# No target set initially.
TARGET=TARGET_NONE

# Build mode variants.
MODE_NONE=0
MODE_DEBUG=1
MODE_RELEASE=2
MODE_MEMCHECK=3
MODE_CALLGRIND=4

# Default mode is DEBUG
MODE=MODE_DEBUG

### functions (utility) ################################################################################################

log() {
    echo $1 | tee -a $LOGFILE
    return 0
    }

mode_check() {
    if [ $TARGET != $TARGET_LINUX ]; then
        if [ $MODE = $MODE_MEMCHECK ]; then
            log "Alas, ${1:2} mode not available for ${2:2} target !!"
            print_usage
            exit 1
        fi
        if [ $MODE = $MODE_CALLGRIND ]; then
            log "Alas, ${1:2} mode not available for ${2:2} target !!"
            print_usage
            exit 1
        fi
    fi
    return 0
    }

prepare_directories() {
    log "Preparing $1 directories ... "
    mkdir -p $ROOT/$1
    rm -rf $ROOT/$1/src
    rm -rf $ROOT/$1/patches
    rm -f $ROOT/$1/build.sh

    mkdir -p $ROOT/$1/install

    cp -rf $ROOT/src $ROOT/$1/src
    cp -rf $ROOT/patches $ROOT/$1/patches
    cp -f $ROOT/build.sh $ROOT/$1/build.sh
    }

print_usage() {
    # Wherever you came from, come back to $ROOT where you invoked this script.
    cd $ROOT

    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    echo "                  TOPBUILD Usage"
    echo "                  --------------"
    echo
    echo "  With three parameters ( create executables ) :"
    echo "  --------------------------------------------"
    echo
    echo "          `basename $0` <target> <mode> <product>"
    echo
    echo "      Available targets:"
    echo "          --all"
    echo "          --linux"
    echo "          --mac"
    echo "          --mac-sdk           ( Mac OS 10.4 x86 SDK )"
    echo "          --win32"
    echo
    echo "      Available modes:"
    echo "          --debug"
    echo "          --release"
    echo "          --memcheck          ( linux only )"
    echo "          --callgrind         ( linux only )"
    echo
    echo "      Available products:"
    echo "          --starsphere"
    echo "          --solarsystem"
    echo
    echo "  With one parameter :"
    echo "  ------------------"
    echo
    echo "          `basename $0` <target>"
    echo
    echo "      Available targets:"
    echo "          --distclean"
    echo "          --doc"
    echo
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    log "Wrong usage. Stopping!"

    return 0
    }

### main control #######################################################################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "`date`"
log "Top level build entry"
log "Selected ${1:2} target ( using ${2:2} mode ) for product ${3:2}"

# Improved command line parsing :-)

if [ $# -eq 0 ]; then
    log "No command line arguments given !!"
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
            log "Wrong argument given !!"
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -eq 2 ]; then
    log "Either one or three command line arguments required !!"
    print_usage
    exit 1
fi

if [ $# -eq 3 ]; then
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
            log "Incorrect first argument given !!"
            print_usage
            exit 1
            ;;
    esac

    case "$2" in
        "--debug")
            MODE=$MODE_DEBUG
            ;;
        "--release")
            MODE=$MODE_RELEASE
            ;;
        "--memcheck")
            MODE=$MODE_MEMCHECK
            ;;
        "--callgrind")
            MODE=$MODE_CALLGRIND
            ;;
        *)
            log "Incorrect second argument given !!"
            print_usage
            exit 1
        ;;
    esac

    case "$3" in
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
            log "Incorrect third argument given !!"
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -gt 3 ]; then
    log "Too many command line arguments given !!"
    print_usage
    exit 1
fi

# here we go...

mode_check $2 $1

case $TARGET in
    $TARGET_ALL)
        # ./topbuild.sh --linux $2 $3
        # ./topbuild.sh --win32 $2 $3
        ;;
    $TARGET_LINUX)
        prepare_directories linux
        log "For $PRODUCT_NAME : invoking linux build script ... "
        cd linux
        ./build.sh --linux $2 $3
        cd ..
        ;;
    $TARGET_MAC)
        prepare_directories mac
        log "For $PRODUCT_NAME : invoking mac build script ... "
        cd mac
        ./build.sh --mac $2 $3
        cd ..
        ;;
    $TARGET_MAC_SDK)
        prepare_directories mac-sdk
        log "For $PRODUCT_NAME : invoking mac-sdk build script ... "
        cd mac-sdk
        ./build.sh --mac-sdk $2 $3
        cd ..
        ;;
    $TARGET_WIN32)
        prepare_directories win32
        log "For $PRODUCT_NAME : invoking win32 build script ... "
        cd win32
        ./build.sh --win32 $2 $3
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

log "Top level build exit"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
