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

### globals ############################################################################################################

# Set paths.
ROOT=`pwd`
PATH_ORG="$PATH"
PATH_MINGW="$PATH"

# Set the logfile.
LOGFILE=$ROOT/build.log

# Required for correct access to the BOINC repository.
TAG_GFXAPPS="current_gfx_apps"

# For some source fetches we use version strings as I'm
# not entirely happy to commit to whatever some latest
# library build might be.
GLEW_VERSION=1.7.0
GLFW_VERSION=2.7.5
FREETYPE_VERSION=2.4.11
LIBXML_VERSION=2.9.0

# Target variants.
TARGET_NONE=0
TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3
TARGET_DOC=4

# No target set initially.
TARGET=TARGET_NONE

# Build mode variants.
MODE_NONE=0
MODE_DEBUG=1
MODE_RELEASE=2
MODE_MEMCHECK=3
MODE_CALLGRIND=4

# Assume DEBUG mode as default.
MODE=$MODE_DEBUG

# Common build stages.
BS_NONE=0
BS_PREREQUISITES=1
BS_PREPARE_TREE=2

# Non-MinGW build stages
BS_BUILD_GLEW=3
BS_BUILD_GLFW=4
BS_BUILD_FREETYPE=5
BS_BUILD_LIBXML=6
BS_BUILD_OGLFT=7
BS_BUILD_BOINC=8

# MinGW build stages
BS_BUILD_GLEW_MINGW=9
BS_BUILD_GLFW_MINGW=10
BS_BUILD_FREETYPE_MINGW=11
BS_BUILD_LIBXML_MINGW=12
BS_BUILD_OGLFT_MINGW=13
BS_BUILD_BOINC_MINGW=14

# No buildstate set initially.
BUILDSTATE=$BS_NONE

### functions (utility) ################################################################################################

check_last_build() {
    log "Checking previous build target..."

    # Get the contents, if any, of the file recording the most recent build.
    LASTBUILD=`cat $ROOT/.lastbuild 2>/dev/null`

    # Assuming you have a record of the most recent build,
    # then is it the currently requested one?
    if [[ ( -f $ROOT/.lastbuild ) && ( "$LASTBUILD" != "$1" ) ]]; then
        # No, the target has changed, purge and (re-)prepare the trees.
        purge_tree
        prepare_tree
    fi

    # Record the currently requested build target for examination next time.
    echo "$1" > $ROOT/.lastbuild || failure

    return 0
    }

check_prerequisites() {
    if [ $BUILDSTATE -ge $BS_PREREQUISITES ]; then
        return 0
    fi

    log "Checking prerequisites..."

    # required toolchain
    TOOLS="ar automake autoconf cmake cvs g++ gcc hg ld lex libtool m4 patch pkg-config svn tar wget yacc"

    for tool in $TOOLS; do
        if ! ( type $tool >/dev/null 2>&1 ); then
            log "Missing \"$tool\" which is a required tool!"
            return 1
        fi
    done

    save_build_state $BS_PREREQUISITES
    return 0
    }

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

mode_check() {
    if [ $TARGET != $TARGET_LINUX ]; then
        if [ $MODE = $MODE_MEMCHECK ]; then
            log "$1 mode not available for $2 target !!"
            return 1
        fi
        if [ $MODE = $MODE_CALLGRIND ]; then
            log "$1 mode not available for $2 target !!"
            return 1
        fi
    fi
    return 0
    }

prepare_tree() {
    if [ $BUILDSTATE -ge $BS_PREPARE_TREE ]; then
        return 0
    fi

    log "Preparing tree..."

    mkdir -p $ROOT/3rdparty >> $LOGFILE || failure
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
    echo "  With three parameters ( create executables ) :"
    echo "  --------------------------------------------"
    echo
    echo "              `basename $0` <target> <mode> <product>"
    echo
    echo "      Available targets:"
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
    echo "              `basename $0` <target>"
    echo
    echo "      Available targets:"
    echo "          --distclean"
    echo "          --doc"
    echo
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    log "Wrong usage. Stopping!"

    return 0
    }

purge_tree() {
    log "Purging build and install directories..."

    rm -rf $ROOT/build >> $LOGFILE || failure
    rm -rf $ROOT/install >> $LOGFILE || failure

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

    # Assuming the $ROOT directory has a git repository.
    if [ -d $ROOT/.git ]; then
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
    }

### functions to obtain sources ########################################################################################

prepare_boinc() {
    log "Preparing BOINC..."
    mkdir -p $ROOT/3rdparty/boinc >> $LOGFILE || failure
    mkdir -p $ROOT/build/boinc >> $LOGFILE || failure

    cd $ROOT/3rdparty/boinc || failure
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
        rm -rf $ROOT/3rdparty/boinc >> $LOGFILE || failure

        log "Retrieving BOINC (tag: $1) (this may take a while)..."
        cd $ROOT/3rdparty || failure
        git clone git://git.aei.uni-hannover.de/shared/einsteinathome/boinc.git boinc >> $LOGFILE 2>&1 || failure
        cd $ROOT/3rdparty/boinc || failure
        git checkout $1 >> $LOGFILE  2>&1 || failure
    fi

    return 0
    }

prepare_freetype() {
    log "Preparing Freetype2..."
    mkdir -p $ROOT/build/freetype2 >> $LOGFILE || failure

    log "Retrieving Freetype2 (this may take a while)..."
    cd $ROOT/3rdparty || failure
    wget http://download.savannah.gnu.org/releases/freetype/freetype-$FREETYPE_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    tar -xjf freetype-$FREETYPE_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    rm freetype-$FREETYPE_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf freetype2 >> $LOGFILE 2>&1 || failure
    mv freetype-$FREETYPE_VERSION freetype2 >> $LOGFILE 2>&1 || failure

    return 0
    }

prepare_glew() {
    log "Preparing GLEW..."
    mkdir -p $ROOT/3rdparty/glew >> $LOGFILE || failure

    cd $ROOT/3rdparty || failure
    wget http://sourceforge.net/projects/glew/files/glew/$GLEW_VERSION/glew-$GLEW_VERSION.tgz >> $LOGFILE 2>&1 || failure
    tar -xf glew-$GLEW_VERSION.tgz >> $LOGFILE 2>&1 || failure
    rm glew-$GLEW_VERSION.tgz >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf glew >> $LOGFILE 2>&1 || failure
    mv glew-$GLEW_VERSION glew >> $LOGFILE 2>&1 || failure

    return 0
    }

prepare_glfw() {
    log "Preparing GLFW..."
    mkdir -p $ROOT/3rdparty/glfw >> $LOGFILE || failure
    mkdir -p $ROOT/build/glfw >> $LOGFILE || failure

    log "Retrieving GLFW (this may take a while)..."
    cd $ROOT/3rdparty || failure
    wget http://sourceforge.net/projects/glfw/files/glfw/$GLFW_VERSION/glfw-$GLFW_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    tar -xjf glfw-$GLFW_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    rm glfw-$GLFW_VERSION.tar.bz2 >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf glfw >> $LOGFILE 2>&1 || failure
    mv glfw-$GLFW_VERSION glfw >> $LOGFILE 2>&1 || failure

    return 0
    }

prepare_libxml() {
    log "Preparing libxml2..."
    mkdir -p $ROOT/build/libxml2 >> $LOGFILE || failure

    log "Retrieving libxml2 (this may take a while)..."
    cd $ROOT/3rdparty || failure
    rm -f libxml2-sources-$LIBXML_VERSION.tar.gz >> $LOGFILE 2>&1 || failure
    wget --passive-ftp ftp://xmlsoft.org/libxml2/libxml2-sources-$LIBXML_VERSION.tar.gz >> $LOGFILE 2>&1 || failure
    tar -xzf libxml2-sources-$LIBXML_VERSION.tar.gz >> $LOGFILE 2>&1 || failure
    rm libxml2-sources-$LIBXML_VERSION.tar.gz >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf libxml2 >> $LOGFILE 2>&1 || failure
    mv libxml2-$LIBXML_VERSION libxml2 >> $LOGFILE 2>&1 || failure

    return 0
    }

prepare_oglft() {
    log "Preparing OGLFT..."
    mkdir -p $ROOT/3rdparty/oglft >> $LOGFILE || failure
    mkdir -p $ROOT/build/oglft >> $LOGFILE || failure

    cd $ROOT/3rdparty/oglft || failure
    if [ -d .svn ]; then
        log "Updating OGLFT..."
        # make sure local changes (patches) are reverted, hence also updated
        svn revert -R . >> $LOGFILE  2>&1 || failure
        svn update >> $LOGFILE  2>&1 || failure
    else
        log "Retrieving OGLFT (this may take a while)..."
        svn checkout http://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $LOGFILE 2>&1 || failure
    fi
    }

### functions to build from sources for linux targets #################################################################

build_glew() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLEW ]; then
        return 0
    fi

    prepare_glew || failure

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

build_glfw() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLFW ]; then
        return 0
    fi

    prepare_glfw || failure

    log "Building GLFW (this may take a while)..."
    cd $ROOT/3rdparty/glfw || failure

    export PREFIX=$ROOT/install

    make x11-install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed GLFW!"

    save_build_state $BS_BUILD_GLFW || failure
    return 0
    }

build_freetype() {
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE ]; then
        return 0
    fi

    prepare_freetype || failure

    log "Building Freetype2 (this may take a while)..."
    cd $ROOT/3rdparty/freetype2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/freetype2 || failure
    # note: freetype probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed Freetype2!"

    save_build_state $BS_BUILD_FREETYPE || failure
    return 0
    }

build_libxml() {
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML ]; then
        return 0
    fi

    prepare_libxml || failure

    log "Building libxml2 (this may take a while)..."
    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/libxml2 || failure
    $ROOT/3rdparty/libxml2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --without-python >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed libxml2!"

    save_build_state $BS_BUILD_LIBXML || failure
    return 0
    }

build_oglft() {
    if [ $BUILDSTATE -ge $BS_BUILD_OGLFT ]; then
        return 0
    fi

    prepare_oglft || failure

    log "Patching OGLFT..."
    cd $ROOT/3rdparty/oglft || failure
    # note: svn has no force/overwrite switch. patched files might not be updated
    # patch: use fixed settings for freetype, deactivate FindFreetype
    FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
    FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
    patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
    log "Building OGLFT..."
    cd $ROOT/build/oglft || failure
    # TODO: do we wanna create universal binaries on mac? If so, add -DCMAKE_OSX_ARCHITECTURES=ppc;i386
    cmake -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed OGLFT!"

    save_build_state $BS_BUILD_OGLFT || failure
    return 0
    }

build_boinc() {
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC ]; then
        return 0
    fi

    prepare_boinc $TAG_GFXAPPS || failure

    log "Configuring BOINC..."
    cd $ROOT/3rdparty/boinc || failure
    chmod +x _autosetup >> $LOGFILE 2>&1 || failure
    ./_autosetup >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/boinc || failure
    if [ "$1" == "$TARGET_MAC" ]; then
        export CPPFLAGS="-I/sw/include -I/opt/local/include $CPPFLAGS"
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --with-apple-opengl-framework --enable-install-headers --enable-libraries --disable-manager --disable-fcgi >> $LOGFILE 2>&1 || failure
    elif [ -d "/usr/local/ssl" ]; then
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

### functions to build from sources for Mac targets ####################################################################

build_glew_mac() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLEW ]; then
        return 0
    fi

    prepare_glew || failure

    log "Building GLEW (this may take a while)..."

    cd $ROOT/3rdparty/glew
    GLEW_DEST="$ROOT/install" make install >> $LOGFILE 2>&1 || failure
    # don't use shared GLEW libraries
    rm -f $ROOT/install/lib/libGLEW*.dylib

    log "Successfully built and installed GLEW!"

    save_build_state $BS_BUILD_GLEW || failure
    return 0
    }

build_glfw_mac() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLFW ]; then
        return 0
    fi

    prepare_glfw || failure

    log "Building GLFW (this may take a while)..."
    cd $ROOT/3rdparty/glfw || failure

    export PREFIX=$ROOT/install

    make cocoa-install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed GLFW!"

    save_build_state $BS_BUILD_GLFW || failure
    return 0
    }

### functions to build from sources for Win32 targets ##################################################################

set_mingw() {
    # general config
    PREFIX=$ROOT/install
    # the following target host spec is Debian specific!
    # use "i586-pc-mingw32" when building MinGW automatically

    export TARGET_HOST=i586-mingw32msvc
    BUILD_HOST=i386-linux
    PATH_MINGW="/usr/bin:$PREFIX/bin:$PREFIX/$TARGET_HOST/bin:$PATH"
    PATH="$PATH_MINGW"
    export PATH

    export CC=`which ${TARGET_HOST}-gcc`
    export CXX=`which ${TARGET_HOST}-g++`

    export CPPFLAGS="-D_WIN32_WINDOWS=0x0410 -DMINGW_WIN32 $CPPFLAGS"
    }

build_glew_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLEW_MINGW ]; then
        return 0
    fi

    prepare_glew || failure

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

    save_build_state $BS_BUILD_GLEW_MINGW || failure
    return 0
    }

build_glfw_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLFW_MINGW ]; then
        return 0
    fi

    prepare_glfw || failure

    log "Building GLFW (this may take a while)..."
    cd $ROOT/3rdparty/glfw || failure

    export PREFIX=$ROOT/install

    make cross-mgw-install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed GLFW!"

    save_build_state $BS_BUILD_GLFW_MINGW || failure
    return 0
    }

build_freetype_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE_MINGW ]; then
        return 0
    fi

    prepare_freetype || failure

    log "Patching Freetype2..."
    cd $ROOT/3rdparty/freetype2/builds || failure
    # patch: deactivating invocation of apinames (would run win32 binary on linux host)
    patch < $ROOT/patches/freetype2.exports.mk.patch >> $LOGFILE 2>&1 || failure
    log "Building Freetype2 (this may take a while)..."
    cd $ROOT/3rdparty/freetype2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-freetype-config" ]; then
        FT_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-freetype-config"
        export FT_CONFIG
        echo "Cross-compile FT_CONFIG: $FT_CONFIG" >> $LOGFILE
    fi
    cd $ROOT/build/freetype2 || failure
    # note: freetype probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed Freetype2!"

    save_build_state $BS_BUILD_FREETYPE_MINGW || failure
    return 0
    }

build_libxml_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML_MINGW ]; then
        return 0
    fi

    prepare_libxml || failure

    log "Building libxml2 (this may take a while)..."
    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config" ]; then
        LIBXML2_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config"
        export LIBXML2_CONFIG
        echo "Cross-compile LIBXML2_CONFIG: $LIBXML2_CONFIG" >> $LOGFILE
    fi
    cd $ROOT/build/libxml2 || failure
    $ROOT/3rdparty/libxml2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes --without-python >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed libxml2!"

    save_build_state $BS_BUILD_LIBXML_MINGW || failure
    return 0
    }

build_oglft_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_OGLFT_MINGW ]; then
        return 0
    fi

    prepare_oglft || failure

    log "Patching OGLFT..."
    cd $ROOT/3rdparty/oglft || failure
    # note: svn has no force/overwrite switch. patched files might not be updated
    # patch: use fixed settings for freetype, deactivate FindFreetype
    FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
    FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
    patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
    cp $ROOT/patches/toolchain-linux-mingw.oglft.cmake $ROOT/build/oglft >> $LOGFILE 2>&1 || failure
    export OGLFT_INSTALL=$ROOT/install
    log "Building OGLFT..."
    cd $ROOT/build/oglft || failure
    cmake -DCMAKE_RC_COMPILER="/usr/bin/i586-mingw32msvc-windres" -DCMAKE_TOOLCHAIN_FILE="toolchain-linux-mingw.oglft.cmake" -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed OGLFT!"

    save_build_state $BS_BUILD_OGLFT_MINGW || failure
    return 0
    }

build_boinc_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC_MINGW ]; then
        return 0
    fi

    prepare_boinc $TAG_GFXAPPS || failure

    cd $ROOT/3rdparty/boinc/lib || failure
    log "Building BOINC (this may take a while)..."
    BOINC_SRC="$ROOT/3rdparty/boinc" AR="${TARGET_HOST}-ar" make -f Makefile.mingw >> $LOGFILE 2>&1 || failure
    BOINC_PREFIX="$ROOT/install" RANLIB="${TARGET_HOST}-ranlib" make -f Makefile.mingw install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed BOINC!"

    save_build_state $BS_BUILD_BOINC_MINGW || failure
    return 0
    }

### generic build function #############################################################################################

build_product() {
    # make sure ORC is always compiled for host platform
    log "Preparing $PRODUCT_NAME..."
    mkdir -p $ROOT/build/orc >> $LOGFILE || failure
    mkdir -p $ROOT/build/framework >> $LOGFILE || failure
    mkdir -p $ROOT/build/$PRODUCT >> $LOGFILE || failure
    export PATH=$PATH_ORG

    # Create the header containing ( if available ) any git commit,
    # build machine and working directory information.
    write_version_header || failure

    log "Building $PRODUCT_NAME [ORC]..."
    export ORC_SRC=$ROOT/src/orc || failure
    export ORC_INSTALL=$ROOT/install || failure
    cd $ROOT/build/orc || failure
    cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure
    if [ "$1" == "$TARGET_WIN32" ]; then
        # backup MinGW compiler settings
        CC_MINGW=$CC
        CXX_MINGW=$CXX
        # set the native compilers (ORC will be run on host, not on target)
        export CC=`which gcc`
        export CXX=`which g++`
    fi
    make >> $LOGFILE 2>&1 || failure
    if [ "$1" == "$TARGET_WIN32" ]; then
        # restore MinGW compiler settings
        export CC=$CC_MINGW
        export CXX=$CXX_MINGW
    fi
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [ORC]!"

    # set main include directory
    if [ "$1" == "$TARGET_WIN32" ]; then
        export PATH=$PATH_MINGW
    else
        export PATH=$PATH_ORG
    fi

    log "Building $PRODUCT_NAME [Framework]..."
    export FRAMEWORK_SRC=$ROOT/src/framework || failure
    export OGL_UTILITY_SRC=$ROOT/src/ogl_utility || failure
    export FRAMEWORK_INSTALL=$ROOT/install || failure
    cd $ROOT/build/framework || failure
    if [ "$1" == "$TARGET_WIN32" ]; then
        cp -f $ROOT/src/framework/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
    else
        cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure
    fi
    make ${2:2} PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [Framework]!"

    log "Building $PRODUCT_NAME [Application]..."
    export PROJECT_ROOT=$ROOT || failure
    cd $ROOT/build/$PRODUCT || failure
    cp $ROOT/src/$PRODUCT/*.res . >> $LOGFILE 2>&1 || failure
    cp -f $ROOT/src/$PRODUCT/Makefile.common Makefile >> $LOGFILE 2>&1 || failure
    if [ "$1" == "$TARGET_MAC" ]; then
        make ${2:2} SYSTEM="mac" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    elif [ "$1" == "$TARGET_WIN32" ]; then
        make ${2:2} SYSTEM="win32" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    else
        make ${2:2} SYSTEM="linux" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    fi
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [Application]!"

    return 0
    }

### specific build functions ###########################################################################################

build_linux() {
    log "Important for an official build: let CC and CXX point to gcc/g++ 4.6+ !"

    # Intercept build stages at the non-MinGW group to build the libraries
    # that the framework build will depend upon. Except that GLEW actually
    # is incorporated into the framework library via GLEW source files
    # during the framework build.

    # However use linux specific functions to get GLEW and GLFW.
    build_glew || failure
    build_glfw || failure

    # Common linux/Mac build stages.
    build_freetype || failure
    build_libxml || failure
    build_oglft || failure
    build_boinc || failure
    build_product $TARGET_LINUX $2 || failure

    return 0
    }

build_mac() {

    # Intercept build stages at the non-MinGW group to build the libraries
    # that the framework build will depend upon.
    # However use Mac specific functions to get GLEW and GLFW.

    # Avoid shared GLEW libraries.
    build_glew_mac || failure

    # Use the GLFW cocoa library.
    build_glfw_mac $TARGET_MAC $2 || failure

    # Common linux/Mac build stages.
    build_freetype || failure
    build_libxml || failure
    build_oglft || failure
    build_boinc $TARGET_MAC $2 || failure
    build_product $TARGET_MAC $2 || failure

    return 0
    }

build_win32() {
    # No more prepare/build steps for MinGW as we use Debian's MinGW with GCC 4.6 support !!

    # Get ready to use MinGW.
    set_mingw || failure

    # Intercept build stages at the MinGW group to build the libraries
    # that the framework build will depend upon. Except that GLEW actually
    # is incorporated into the framework library via GLEW source files
    # during the framework build.
    build_glew_mingw || failure
    build_glfw_mingw || failure
    build_freetype_mingw || failure
    build_libxml_mingw || failure
    build_oglft_mingw || failure
    build_boinc_mingw || failure

    # Finally build our specific product
    log "In build_win32() : \$1 = $1"
    log "In build_win32() : \$2 = $2"
    build_product $TARGET_WIN32 $2 || failure

    return 0
    }

### main control #######################################################################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "Starting new build!"
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
            log "Building documentation..."
            ;;
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
    print_usage
    exit 1
fi

if [ $# -eq 3 ]; then
    case "$1" in
        "--linux")
            TARGET=$TARGET_LINUX
            check_last_build "$1" || failure
            log "Building linux version "
            retrieve_build_state || failure
            ;;
        "--mac")
            TARGET=$TARGET_MAC
            mode_check
            check_last_build "$1" || failure
            log "Building mac (Intel) version "
            retrieve_build_state || failure
            ;;
        "--mac-sdk")
            TARGET=$TARGET_MAC
            mode_check
            SDK="yes"
            check_last_build "$1" || failure
            log "Building mac (Intel) version with SDK "
            retrieve_build_state || failure
            ;;
        "--win32")
            TARGET=$TARGET_WIN32
            mode_check
            check_last_build "$1" || failure
            log "Building win32 version:"
            retrieve_build_state || failure
            ;;
        *)
            log "Incorrect first argument given !!"
            print_usage
            exit 1
            ;;
    esac

    case "$2" in
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

    case "$3" in
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

if [ $# -gt 3 ]; then
    log "Too many command line arguments given !!"
    print_usage
    exit 1
fi

# here we go...

case $TARGET in
    $TARGET_LINUX)
        check_prerequisites || failure
        prepare_tree || failure
        build_linux $1 $2 || failure
        ;;
    $TARGET_MAC)
        if [ ".$SDK" = "." ]; then
            :
        elif [ -d /Developer/SDKs/MacOSX10.4u.sdk ]; then
            log "Preparing Mac OS X 10.4 SDK build environment..."
            # use 10.4 (Tiger) SDK because of BOINC/10.5 incompatibility (http://boinc.berkeley.edu/doxygen/api/html/QBacktrace_8h.html)
            export LDFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -arch i386 $LDFLAGS"
            export CPPFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CPPFLAGS"
            export CFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CFLAGS"
            export CXXFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CXXFLAGS"
            export SDKROOT="/Developer/SDKs/MacOSX10.4u.sdk"
            export MACOSX_DEPLOYMENT_TARGET=10.4
        else
            log "Mac OS X 10.4 SDK required but missing!"
            failure
        fi
        check_prerequisites || failure
        prepare_tree || failure
        build_mac $1 $2 || failure
        ;;
    $TARGET_WIN32)
        check_prerequisites || failure
        prepare_tree || failure
        build_win32 $1 $2 || failure
        ;;
    $TARGET_DOC)
        doxygen Doxyfile >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.png $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        cp -f $ROOT/doc/default/*.gif $ROOT/doc/html >> $LOGFILE 2>&1 || failure
        exit 0
        ;;
    *)
        # should be unreachable
        print_usage
        exit 1
        ;;
esac

log "++++++++++++++++++++++++++++++++++++"
log "Build finished successfully!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
