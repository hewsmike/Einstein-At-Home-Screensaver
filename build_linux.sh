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

### globals ###############################################################

# Set paths.
ROOT=`pwd`

# Set the logfile.
LOGFILE=$ROOT/build.log

# Build mode variants.
MODE_NONE=0
MODE_DEBUG=1
MODE_RELEASE=2
MODE_MEMCHECK=3
MODE_CALLGRIND=4

# Assume DEBUG mode as default.
MODE=$MODE_DEBUG

# Build stages.
BS_NONE=0
BS_PREPARE_TREE=1
BS_BUILD_BOINC=2
BS_BUILD_FREETYPE=3
BS_BUILD_GLEW=4
BS_BUILD_LIBXML=5
BS_BUILD_SDL=6

# No buildstate set initially.
BUILDSTATE=$BS_NONE

### functions (utility) ####################################################

distclean() {
    log "Purging build system..."

    rm -rf $ROOT/3rdparty || failure
    rm -rf $ROOT/build || failure
    rm -rf $ROOT/install || failure
    rm -rf $ROOT/doc/html || failure
    rm -f $ROOT/doc/*.tag || failure

    rm -f $ROOT/.lastbuild || failure
    rm -f $ROOT/.buildstate || failure

    return 0
    }

failure() {
    log "++++++++++++++++++++++++++++++++++++"
    log "Error detected! Stopping build!"
    log "`date`"

    if [ -f "$LOGFILE" ]; then
        echo "------------------------------------"
        echo "Please check logfile: `basename $LOGFILE`"
        echo "These are the final ten lines:"
        echo "------------------------------------"
        tail -n 14 $LOGFILE | head -n 10
    fi

    log "++++++++++++++++++++++++++++++++++++"

    exit 1
    }

log() {
    echo $1 | tee -a $LOGFILE
    return 0
    }

prepare_tree() {
    if [ $BUILDSTATE -ge $BS_PREPARE_TREE ]; then
        return 0
    fi

    log "Preparing tree..."

    mkdir -p $ROOT/build >> $LOGFILE || failure
    mkdir -p $ROOT/build/boinc >> $LOGFILE || failure
    mkdir -p $ROOT/build/freetype2 >> $LOGFILE || failure
    mkdir -p $ROOT/build/glew >> $LOGFILE || failure
    mkdir -p $ROOT/build/libxml2 >> $LOGFILE || failure
    mkdir -p $ROOT/build/sdl2 >> $LOGFILE || failure

    mkdir -p $ROOT/install >> $LOGFILE || failure
    mkdir -p $ROOT/install/bin >> $LOGFILE || failure
    mkdir -p $ROOT/install/include >> $LOGFILE || failure
    mkdir -p $ROOT/install/include/GL >> @LOGFILE || failure
    mkdir -p $ROOT/install/lib >> $LOGFILE || failure

    save_build_state $BS_PREPARE_TREE
    return 0
    }

print_usage() {
    # Wherever you came from, come back to $ROOT where you invoked this script.
    cd $ROOT

    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    echo "                      BUILD Usage"
    echo "                      -----------"
    echo
    echo "  With two parameters ( create executables ) :"
    echo "  --------------------------------------------"
    echo
    echo "              `basename $0` <mode> <product>"
    echo
    echo "      Available modes:"
    echo "          --debug"
    echo "          --release"
    echo "          --memcheck"
    echo "          --callgrind"
    echo
    echo "      Available products:"
    echo "          --starsphere"
    echo "          --solarsystem"
    echo
    echo "  With one parameter :"
    echo "  ------------------"
    echo
    echo "              `basename $0` <target>"
    echo
    echo "      Available targets:"
    echo "          --distclean"
    echo
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    log "Wrong usage. Stopping!"

    return 0
    }

retrieve_build_state() {
    log "Checking for previous build checkpoints..."

    # Do we have a record of the most recent build state?
    if [ ! -f .buildstate ]; then
        # No. Start from the beginning.
        cd $ROOT || failure
        log "No previous build checkpoints found! Starting from scratch..."
    else
        # Yes. Get that build state.
        BUILDSTATE=`cat $ROOT/.buildstate 2>/dev/null`
        log "Recovering previous build..."
    fi

    return 0
    }

save_build_state() {
    log "Saving build checkpoint..."

    echo "$1" > $ROOT/.buildstate || failure

    return 0
    }

write_version_header() {
    log "Retrieving git version information..."

    # Path/name to the version header file.
    HEADER_FILE="$ROOT/src/erp_git_version.h"

    # Assuming the parent to $ROOT directory has a git repository.
    # Save current directory.
    OLD_DIR=`pwd`

    # Move to parent of $ROOT
    cd $ROOT
    cd ..
    if [ -d .git ]; then
        log "Yup, found git repo !"
        # Get the name of the most recent commit ( ie. the 40 digit hex code of same ).
        GIT_LOG=`git log -n1 --pretty="format:%H"` || failure
        # Get the name of the machine we are building on.
        HOST=`hostname` || failure

    fi

    # Start constructing the header file with include guards.
    echo "#ifndef ERP_GIT_VERSION_H" > $HEADER_FILE || failure
    echo "#define ERP_GIT_VERSION_H" >> $HEADER_FILE || failure
    echo "" >> $HEADER_FILE || failure

    # Did we have a git repository for the $ROOT directory?
    if [ "no$GIT_LOG" != "no" ]; then
        # Yes. Write the git version variable combining the commit name, the host name
        # and the top level build directory.
        echo "#define ERP_GIT_VERSION \"$GIT_LOG ($HOST:$ROOT)\"" >> $HEADER_FILE || failure
    else
        # No. Write the git version variable to show that.
        echo "#define ERP_GIT_VERSION \"unknown (git repository not found!)\"" >> $HEADER_FILE || failure
    fi

    # Close off include guard.
    echo "" >> $HEADER_FILE || failure
    echo "#endif" >> $HEADER_FILE || failure

    # Return to prior directory.
    cd $OLD_DIR
    }


### functions to build from sources for linux targets #################################################################

build_boinc() {
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC ]; then
        return 0
    fi

    log "Configuring BOINC..."
    cd $ROOT/3rdparty/boinc || failure
    chmod +x _autosetup >> $LOGFILE 2>&1 || failure
    ./_autosetup >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/boinc || failure
    if [ -d "/usr/local/ssl" ]; then
        log "Using local SSL library..."
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --enable-install-headers --enable-libraries --disable-manager --disable-fcgi CPPFLAGS=-I/usr/local/ssl/include LDFLAGS=-L/usr/local/ssl/lib >> $LOGFILE 2>&1 || failure
    else
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --enable-install-headers --enable-libraries --disable-manager --disable-fcgi >> $LOGFILE 2>&1 || failure
    fi

    log "Building BOINC (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed BOINC!"

    save_build_state $BS_BUILD_BOINC || failure
    return 0
    }

build_freetype() {
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE ]; then
        return 0
    fi

    cd $ROOT/3rdparty/freetype2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/freetype2 || failure
    # note: freetype probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure

    log "Building Freetype2 (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed Freetype2!"

    save_build_state $BS_BUILD_FREETYPE || failure

    return 0
    }

build_glew() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLEW ]; then
        return 0
    fi

    log "Building GLEW (this may take a while)..."

    # Not actually building a distinct GLEW library here.
    # Incorporate GLEW via an object file ( glew.o ) to later
    # link to instead. Currently the glew.org provided makefile
    # is problematic for user defined intallation targets
    # ( ie. doesn't work ). See makefile(s) in our framework
    # sub-directory.

    # GLEW headers copied
    cd $ROOT/3rdparty/glew/include/GL || failure
    cp -f *.h $ROOT/install/include/GL || failure

    # Put glew.c into our framework sub-directory
    cd $ROOT/3rdparty/glew/src || failure
    cp -f glew.c $ROOT/src/framework  || failure

    log "Successfully built and installed GLEW!"

    save_build_state $BS_BUILD_GLEW || failure

    return 0
    }

build_libxml() {
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML ]; then
        return 0
    fi

    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/libxml2 || failure
    $ROOT/3rdparty/libxml2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --without-python >> $LOGFILE 2>&1 || failure

    log "Building libxml2 (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed libxml2!"

    save_build_state $BS_BUILD_LIBXML || failure

    return 0
    }

build_sdl() {
    if [ $BUILDSTATE -ge $BS_BUILD_SDL ]; then
        return 0
    fi

    cd $ROOT/3rdparty/sdl2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/sdl2 || failure
    $ROOT/3rdparty/sdl2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --enable-screensaver=yes >> $LOGFILE 2>&1 || failure


    log "Building SDL (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed SDL!"

    save_build_state $BS_BUILD_SDL || failure

    return 0
    }

### specific build functions ###########################################################################################

build_orc() {
    # make sure ORC is always compiled for host platform
    log "Preparing $PRODUCT_NAME..."
    mkdir -p $ROOT/build/orc >> $LOGFILE || failure
    mkdir -p $ROOT/build/framework >> $LOGFILE || failure
    mkdir -p $ROOT/build/$PRODUCT >> $LOGFILE || failure

    # Create the header containing ( if available ) any git commit,
    # build machine and working directory information.
    write_version_header || failure

    export ORC_SRC=$ROOT/src/orc || failure
    export ORC_INSTALL=$ROOT/install || failure
    cd $ROOT/build/orc || failure
    cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure

    log "Building $PRODUCT_NAME [ORC]..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed $PRODUCT_NAME [ORC]!"
    return 0
    }

build_framework() {
    # Now compile framework for host platform
    log "Building $PRODUCT_NAME [Framework]..."
    export FRAMEWORK_SRC=$ROOT/src/framework || failure
    export OGL_UTILITY_SRC=$ROOT/src/ogl_utility || failure
    export FRAMEWORK_INSTALL=$ROOT/install || failure
    cd $ROOT/build/framework || failure
    cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure

    make ${2:2} PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [Framework]!"

    return 0
    }

build_linux() {
    log "Important for an official build: let CC and CXX point to gcc/g++ 4.6+ !"

    # Make sure the base libraries are built.
    build_boinc || failure
    build_freetype || failure
    build_glew || failure
    build_libxml || failure
    build_sdl || failure

    # Now client code.
    build_orc || failure
    # build_framework || failure

    return 0
    }

### main control #######################################################################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "Starting new Linux build!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

# Improved command line parsing :-)

if [ $# -eq 0 ]; then
    print_usage
    exit 1
fi

if [ $# -eq 1 ]; then
        case "$1" in
        "--distclean")
            distclean || failure
            exit 0
            ;;
        *)
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -eq 2 ]; then
    case "$1" in
        "--debug")
            log "Debug build chosen"
            MODE=$MODE_DEBUG
            ;;
        "--release")
            log "Release build chosen"
            MODE=$MODE_RELEASE
            ;;
        "--memcheck")
            log "Memcheck facility chosen"
            MODE=$MODE_MEMCHECK
            ;;
        "--callgrind")
            log "Callgrind facility chosen"
            MODE=$MODE_CALLGRIND
            ;;
        *)
            log "Incorrect second argument given !!"
            print_usage
            exit 1
            ;;
    esac

    case "$2" in
        "--starsphere")
            PRODUCT=starsphere
            PRODUCT_NAME="Starsphere"
            log "Building $PRODUCT_NAME..."
            ;;
        "--solarsystem")
            PRODUCT=solarsystem
            PRODUCT_NAME="SolarSystem"
            log "Building $PRODUCT_NAME..."
            ;;
        *)
            log "Incorrect third argument given !!"
            print_usage
            exit 1
            ;;
    esac
fi

if [ $# -gt 2 ]; then
    log "Too many command line arguments given !!"
    print_usage
    exit 1
fi

# here we go...

retrieve_build_state || failure
prepare_tree || failure
build_linux $2 || failure

log "++++++++++++++++++++++++++++++++++++"
log "Build finished successfully!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
