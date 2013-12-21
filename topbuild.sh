#!/bin/bash

###########################################################################
#   Copyright (C) 2008-2009 by Oliver Bock                                #
#   oliver.bock[AT]aei.mpg.de                                             #
#                                                                         #
#   Copyright (C) 2013 by Mike Hewson  [ any & all errors are mine :-) ]  #
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

### globals ################################################################

# Set paths.
ROOT=`pwd`

# Set the topbuild logfile.
LOGFILE=$ROOT/topbuild.log

# Required for correct access to the BOINC repository.
TAG_GFXAPPS="current_gfx_apps"

# For some source fetches I use version strings as I'm
# not entirely happy to commit to whatever some latest
# library build might be.
FREETYPE_VERSION=2.5.1
GLEW_VERSION=1.10.0
LIBXML_VERSION=2.9.0
SDL_VERSION=2.0.1

# Target variants.
TARGET_NONE=0
TARGET_ALL=1
TARGET_ANDROID=2
TARGET_IOS=3
TARGET_LINUX=4
TARGET_MAC_OSX=5
TARGET_WIN32=6
TARGET_DOC=7
TARGET_CLEAN=8

# No target set initially.
TARGET=$TARGET_NONE

# Build mode variants.
MODE_NONE=0
MODE_DEBUG=1
MODE_RELEASE=2
MODE_MEMCHECK=3
MODE_CALLGRIND=4

# Default mode is DEBUG
MODE=$MODE_DEBUG

# Common build stages.
TBS_NONE=0
TBS_PREREQUISITES=1
TBS_BOINC_RETRIEVED=2
TBS_FREETYPE_RETRIEVED=3
TBS_GLEW_RETRIEVED=4
TBS_LIBXML_RETRIEVED=5
TBS_SDL_RETRIEVED=6
TBS_RETRIEVED=7

# No topbuild state set initially.
TOPBUILDSTATE=$TBS_NONE

### functions to obtain sources ####################################

retrieve_boinc() {
    log "Retrieving BOINC..."
    mkdir -p $ROOT/retrieval/boinc >> $LOGFILE || failure


    cd $ROOT/retrieval/boinc || failure
    if [ -d .git ]; then
        log "Updating BOINC (tag: $1)..."
        # make sure local changes (patches) are reverted to ensure fast-forward merge
        git checkout -f $1 >> $LOGFILE  2>&1 || failure
        # update tag info
        git remote update >> $LOGFILE  2>&1 || failure
        git fetch --tags >> $LOGFILE  2>&1 || failure
        # checkout build revision
        git checkout -f $1 >> $LOGFILE  2>&1 || failure
    else
        # workaround for old git versions
        rm -rf $ROOT/retrieval/boinc >> $LOGFILE || failure

        log "Retrieving BOINC (tag: $1) (this may take a while)..."
        cd $ROOT/retrieval || failure
        git clone git://git.aei.uni-hannover.de/shared/einsteinathome/boinc.git boinc >> $LOGFILE 2>&1 || failure
        cd $ROOT/retrieval/boinc || failure
        git checkout $1 >> $LOGFILE  2>&1 || failure
    fi

    save_topbuild_state $TBS_BOINC_RETRIEVED

    return 0
    }

retrieve_freetype() {
    FREETYPE_RETRIEVE_STR=freetype-$FREETYPE_VERSION
    FREETYPE_RETRIEVE_FILE=$FREETYPE_RETRIEVE_STR.tar.bz2
    FREETYPE_RETRIEVE_DOMAIN=http://download.savannah.gnu.org/releases/freetype/
    FREETYPE_RETRIEVE_PATH=$FREETYPE_RETRIEVE_DOMAIN$FREETYPE_RETRIEVE_FILE

    log "Retrieving Freetype2 (this may take a while)..."
    mkdir -p $ROOT/retrieval/freetype2 >> $LOGFILE || failure

    cd $ROOT/retrieval || failure
    wget $FREETYPE_RETRIEVE_PATH >> $LOGFILE 2>&1 || failure
    tar -xjf $FREETYPE_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    rm $FREETYPE_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf freetype2 >> $LOGFILE 2>&1 || failure
    mv $FREETYPE_RETRIEVE_STR freetype2 >> $LOGFILE 2>&1 || failure

    save_topbuild_state $TBS_FREETYPE_RETRIEVED

    return 0
    }

retrieve_glew() {
    GLEW_RETRIEVE_STR=glew-$GLEW_VERSION
    GLEW_RETRIEVE_FILE=$GLEW_RETRIEVE_STR.tgz
    GLEW_RETRIEVE_DOMAIN=http://sourceforge.net/projects/glew/files/glew/$GLEW_VERSION/
    GLEW_RETRIEVE_PATH=$GLEW_RETRIEVE_DOMAIN$GLEW_RETRIEVE_FILE

    log "Preparing GLEW..."
    mkdir -p $ROOT/retrieval/glew >> $LOGFILE || failure

    cd $ROOT/retrieval || failure
    wget $GLEW_RETRIEVE_PATH >> $LOGFILE 2>&1 || failure
    tar -xf $GLEW_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    rm $GLEW_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf glew >> $LOGFILE 2>&1 || failure
    mv $GLEW_RETRIEVE_STR glew >> $LOGFILE 2>&1 || failure

    save_topbuild_state $TBS_GLEW_RETRIEVED

    return 0
    }

retrieve_libxml() {
    LIBXML_RETRIEVE_STR=libxml2-sources-$LIBXML_VERSION
    LIBXML_DELETE_STR=libxml2-$LIBXML_VERSION
    LIBXML_RETRIEVE_FILE=$LIBXML_RETRIEVE_STR.tar.gz
    LIBXML_RETRIEVE_DOMAIN=ftp://xmlsoft.org/libxml2/
    LIBXML_RETRIEVE_PATH=$LIBXML_RETRIEVE_DOMAIN$LIBXML_RETRIEVE_FILE

    log "Retrieving libxml2 (this may take a while)..."
    mkdir -p $ROOT/retrieval/libxml2 >> $LOGFILE || failure

    cd $ROOT/retrieval || failure
    rm -f $LIBXML_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    wget --passive-ftp $LIBXML_RETRIEVE_PATH >> $LOGFILE 2>&1 || failure
    tar -xzf $LIBXML_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    rm $LIBXML_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf libxml2 >> $LOGFILE 2>&1 || failure
    mv $LIBXML_DELETE_STR libxml2 >> $LOGFILE 2>&1 || failure

    save_topbuild_state $TBS_LIBXML_RETRIEVED

    return 0
    }

retrieve_sdl() {
    SDL_RETRIEVE_STR=SDL2-$SDL_VERSION
    SDL_RETRIEVE_FILE=$SDL_RETRIEVE_STR.tar.gz
    SDL_RETRIEVE_DOMAIN=http://www.libsdl.org/release/
    SDL_RETRIEVE_PATH=$SDL_RETRIEVE_DOMAIN$SDL_RETRIEVE_FILE

    log "Retrieving SDL2 (this may take a while)..."
    mkdir -p $ROOT/retrieval/sdl2 >> $LOGFILE || failure

    cd $ROOT/retrieval || failure
    rm -f $SDL_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    wget $SDL_RETRIEVE_PATH >> $LOGFILE 2>&1 || failure
    tar -xzf $SDL_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    rm $SDL_RETRIEVE_FILE >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf sdl2 >> $LOGFILE 2>&1 || failure
    mv $SDL_RETRIEVE_STR sdl2 >> $LOGFILE 2>&1 || failure

    save_topbuild_state $TBS_SDL_RETRIEVED

    return 0
    }

### functions (utility) ####################################################

check_prerequisites() {
    if [ $TOPBUILDSTATE -ge $TBS_PREREQUISITES ]; then
        return 0
    fi

    log "Checking prerequisites..."

    # required toolchain
    TOOLS="ar automake autoconf cmake cvs doxygen g++ gcc hg ld lex libtool m4 patch pkg-config svn tar wget yacc"

    for tool in $TOOLS; do
        if ! ( type $tool >/dev/null 2>&1 ); then
            log "Missing \"$tool\" which is a required tool!"
            exit 1
        fi
    done

    save_topbuild_state $TBS_PREREQUISITES
    return 0
    }

check_retrieval() {
    if [ $TOPBUILDSTATE -ge $TBS_RETRIEVED ]; then
        return 0
    fi

    log "Retrieving common library/sources..."

    purge_toptree
    prepare_toptree

    if [ $TOPBUILDSTATE -lt $TBS_BOINC_RETRIEVED ]; then
        retrieve_boinc $TAG_GFXAPPS || failure
    fi

    if [ $TOPBUILDSTATE -lt $TBS_FREETYPE_RETRIEVED ]; then
        retrieve_freetype || failure
    fi

    if [ $TOPBUILDSTATE -lt $TBS_GLEW_RETRIEVED ]; then
        retrieve_glew || failure
    fi

    if [ $TOPBUILDSTATE -lt $TBS_LIBXML_RETRIEVED ]; then
        retrieve_libxml || failure
    fi

    if [ $TOPBUILDSTATE -lt $TBS_SDL_RETRIEVED ]; then
        retrieve_sdl || failure
    fi

    save_topbuild_state $TBS_RETRIEVED
    return 0
    }

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

    # If not present, create the given target's top level directory.
    mkdir -p $ROOT/$1

    # If not present, create an install directory for the given target.
    mkdir -p $ROOT/$1/install

    # If not present, create a 3rd party source directory for the given target.
    mkdir -p $ROOT/$1/3rdparty

    # For common source retrieval only update
    # ( replace if newer, copy if non-existent )
    cp -rfu $ROOT/retrieval/* $ROOT/$1/3rdparty

    # Populate with latest developer ( ie. YOU ) source code.
    rm -rf $ROOT/$1/src
    cp -rf $ROOT/src $ROOT/$1/src

    # Populate with latest patches.
    rm -rf $ROOT/$1/patches
    cp -rf $ROOT/patches $ROOT/$1/patches

    # Remove any prior build script.
    rm -f $ROOT/$1/build.sh
    }

prepare_toptree() {
    log "Preparing retrieval directory..."

    mkdir -p $ROOT/retrieval >> $LOGFILE || failure

    return 0
    }

print_usage() {
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
    echo "          --android"
    echo "          --ios"
    echo "          --linux"
    echo "          --mac_osx"
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
    echo "          --projectclean         ( BEWARE : cleans all targets )"
    echo "          --doc"
    echo
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    return 0
    }

purge_toptree() {
    log "Purging topbuild common retrieval directories..."

    rm -rf $ROOT/retrieval >> $LOGFILE || failure

    return 0
    }

obtain_topbuild_state() {
    log "Checking for previous topbuild checkpoints..."

    # Do we have a record of the most recent topbuild state?
    if [ ! -f .topbuildstate ]; then
        # No. Start from the beginning.
        cd $ROOT || failure
        log "No previous topbuild checkpoints found! Starting from scratch..."
        TOPBUILDSTATE=$TBS_NONE
    else
        # Yes. Get that topbuild state.
        TOPBUILDSTATE=`cat $ROOT/.topbuildstate 2>/dev/null`
        log "Recovering previous topbuild..."
    fi

    return 0
    }

save_topbuild_state() {
    log "Saving topbuild checkpoint..."

    echo "$1" > $ROOT/.topbuildstate || failure

    return 0
    }

### main control ############################################################

# Delete any prior topbuild log
rm -f ./topbuild.log

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
        "--projectclean")
            TARGET=$TARGET_CLEAN
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
        "--android")
            TARGET=$TARGET_ANDROID
            ;;
        "--ios")
            TARGET=$TARGET_IOS
            ;;
        "--linux")
            TARGET=$TARGET_LINUX
            ;;
        "--mac_osx")
            TARGET=$TARGET_MAC_OSX
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
        "--solarsystem")
            PRODUCT=solarsystem
            PRODUCT_NAME="SolarSystem"
            ;;
        "--starsphere")
            PRODUCT=starsphere
            PRODUCT_NAME="Starsphere"
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

log "++++++++++++++++++++++++++++++++++++"
log "`date`"
log "Top level build entry"
log "Selected ${1:2} target ( using ${2:2} mode ) for product ${3:2}"

# here we go...

mode_check $2 $1

obtain_topbuild_state || failure

check_prerequisites || failure

check_retrieval || failure

case $TARGET in
    $TARGET_ANDROID)
        prepare_directories android
        cp -f $ROOT/build_android.sh $ROOT/android/build.sh
        log "For $PRODUCT_NAME : invoking Android build script ... "
        cd $ROOT/android
        ./build.sh $2 $3
        ;;
    $TARGET_IOS)
        prepare_directories ios
        cp -f $ROOT/build_ios.sh $ROOT/ios/build.sh
        log "For $PRODUCT_NAME : invoking iOS build script ... "
        cd $ROOT/ios
        ./build.sh $2 $3
        ;;
    $TARGET_LINUX)
        prepare_directories linux
        cp -f $ROOT/build_linux.sh $ROOT/linux/build.sh
        log "For $PRODUCT_NAME : invoking Linux build script ... "
        cd $ROOT/linux
        ./build.sh $2 $3
        cd ..
        ;;
    $TARGET_MAC_OSX)
        prepare_directories mac_osx
        cp -f $ROOT/build_mac_osx.sh $ROOT/mac_osx/build.sh
        log "For $PRODUCT_NAME : invoking Mac OSX build script ... "
        cd $ROOT/mac_osx
        ./build.sh $2 $3
        cd ..
        ;;
    $TARGET_WIN32)
        prepare_directories win32
        cp -f $ROOT/build_win32.sh $ROOT/win32/build.sh
        log "For $PRODUCT_NAME : invoking Win32 build script ... "
        cd $ROOT/win32
        ./build.sh $2 $3
        cd ..
        ;;
    $TARGET_CLEAN)
        log "Cleaning Android... "
        cd $ROOT/android
        ./build.sh --distclean

        log "Cleaning iOS... "
        cd $ROOT/ios
        ./build.sh --distclean

        log "Cleaning Linux... "
        cd $ROOT/linux
        ./build.sh --distclean

        log "Cleaning Mac OSX... "
        cd $ROOT/mac_osx --distclean
        ./build.sh --distclean

        log "Cleaning Win32... "
        cd $ROOT/win32
        ./build.sh --distclean

        cd $ROOT

        log "Cleaning common retrieval directory...  "
        rm -rf $ROOT/retrieval || failure

        log "Cleaning all doxygen output"
        rm -rf $ROOT/doc/html
        rm -rf $ROOT/doc/*.*

        save_topbuild_state $TBS_NONE
        ;;
    $TARGET_DOC)
        log "Building documentation ... "
        cd $ROOT
        doxygen Doxyfile >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.png $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.gif $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        ;;
    *)
        # should be unreachable
        log "Unreachable case for TARGET"
        print_usage
        exit 1
        ;;
esac

log "Top level build exit"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
