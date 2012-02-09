#!/bin/bash

###########################################################################
#   Copyright (C) 2008-2009 by Oliver Bock                                #
#   oliver.bock[AT]aei.mpg.de                                             #
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

ROOT=`pwd`
PATH_ORG="$PATH"
PATH_MINGW="$PATH"
LOGFILE=$ROOT/build.log

TAG_GFXAPPS="current_gfx_apps"

TARGET=0
TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3
TARGET_DOC=4

BUILDSTATE=0
BS_PREREQUISITES=1
BS_PREPARE_TREE=2
BS_BUILD_SDL=3
BS_BUILD_FREETYPE=4
BS_BUILD_LIBXML=5
BS_BUILD_OGLFT=6
BS_BUILD_BOINC=7
BS_PREPARE_MINGW=8
BS_BUILD_MINGW=9
BS_BUILD_SDL_MINGW=10
BS_BUILD_FREETYPE_MINGW=11
BS_BUILD_LIBXML_MINGW=12
BS_BUILD_OGLFT_MINGW=13
BS_BUILD_BOINC_MINGW=14

### functions (tools) #############################################################

failure()
{
    echo "************************************" | tee -a $LOGFILE
    echo "Error detected! Stopping build!" | tee -a $LOGFILE
    echo "`date`" | tee -a $LOGFILE

    if [ -f "$LOGFILE" ]; then
        echo "------------------------------------"
        echo "Please check logfile: `basename $LOGFILE`"
        echo "These are the final ten lines:"
        echo "------------------------------------"
        tail -n 14 $LOGFILE | head -n 10
    fi

    echo "************************************" | tee -a $LOGFILE

    exit 1
}


distclean()
{
    cd $ROOT || failure

    echo "Purging build system..." | tee -a $LOGFILE

    rm -rf 3rdparty || failure
    rm -rf build || failure
    rm -rf install || failure
    rm -rf doc/html || failure
    rm -f doc/*.tag || failure

    rm -f .lastbuild || failure
    rm -f .buildstate || failure
}


check_last_build()
{
    echo "Checking previous build target..." | tee -a $LOGFILE

    LASTBUILD=`cat .lastbuild 2>/dev/null`

    if [[ ( -f .lastbuild ) && ( "$LASTBUILD" != "$1" ) ]]; then
        cd $ROOT || failure
        echo "Build target changed! Purging build and install trees..." | tee -a $LOGFILE
        rm -rf build >> $LOGFILE || failure
        rm -rf install >> $LOGFILE || failure
        prepare_tree || failure
    fi

    echo "$1" > .lastbuild || failure

    return 0
}


check_build_state()
{
    echo "Checking for previous build checkpoints..." | tee -a $LOGFILE

    if [ ! -f .buildstate ]; then
        cd $ROOT || failure
        echo "No previous build checkpoints found! Starting from scratch..." | tee -a $LOGFILE
    else
        BUILDSTATE=`cat $ROOT/.buildstate 2>/dev/null`
        echo "Recovering previous build..."
    fi

    return 0
}


store_build_state()
{
    echo "Saving build checkpoint..." | tee -a $LOGFILE
    echo "$1" > $ROOT/.buildstate || failure

    return 0
}


### functions (features) #############################################################

check_prerequisites()
{
    if [ $BUILDSTATE -ge $BS_PREREQUISITES ]; then
        return 0
    fi

    echo "Checking prerequisites..." | tee -a $LOGFILE

    # required toolchain
    TOOLS="automake autoconf m4 cmake wget svn cvs tar patch gcc g++ ld libtool ar lex yacc pkg-config hg"

    for tool in $TOOLS; do
        if ! ( type $tool >/dev/null 2>&1 ); then
            echo "Missing \"$tool\" which is a required tool!" | tee -a $LOGFILE
            return 1
        fi
    done

    return 0
}


prepare_tree()
{
    if [ $BUILDSTATE -ge $BS_PREPARE_TREE ]; then
        return 0
    fi

    echo "Preparing tree..." | tee -a $LOGFILE
    mkdir -p 3rdparty >> $LOGFILE || failure
    mkdir -p install/bin >> $LOGFILE || failure
    mkdir -p install/include >> $LOGFILE || failure
    mkdir -p install/lib >> $LOGFILE || failure

    store_build_state $BS_PREPARE_TREE
    return 0
}


prepare_version_header()
{
    HEADER_FILE="$ROOT/src/erp_git_version.h"

    cd $ROOT || failure

    echo "Retrieving git version information..." | tee -a $LOGFILE

    if [ -d .git ]; then
        GIT_LOG=`git log -n1 --pretty="format:%H"` || failure
        HOST=`hostname` || failure
    fi

    echo "#ifndef ERP_GIT_VERSION_H" > $HEADER_FILE || failure
    echo "#define ERP_GIT_VERSION_H" >> $HEADER_FILE || failure
    echo "" >> $HEADER_FILE || failure

    if [ "no$GIT_LOG" != "no" ]; then
        echo "#define ERP_GIT_VERSION \"$GIT_LOG ($HOST:$PWD)\"" >> $HEADER_FILE || failure
    else
        echo "#define ERP_GIT_VERSION \"unknown (git repository not found!)\"" >> $HEADER_FILE || failure
    fi

    echo "" >> $HEADER_FILE || failure
    echo "#endif" >> $HEADER_FILE || failure
}


prepare_mingw()
{
    if [ $BUILDSTATE -ge $BS_PREPARE_MINGW ]; then
        return 0
    fi

    cd $ROOT || failure

    echo "Preparing MinGW source tree..." | tee -a $LOGFILE
    mkdir -p 3rdparty/mingw/xscripts >> $LOGFILE || failure
    cd 3rdparty/mingw/xscripts || failure

    if [ -d CVS ]; then
        echo "Updating MinGW build script..." | tee -a $LOGFILE
        cvs update -C >> $LOGFILE 2>&1 || failure
    else
        cd .. || failure
        echo "Retrieving MinGW build script (this may take a while)..." | tee -a $LOGFILE
        cvs -z3 -d:pserver:anonymous@mingw.cvs.sourceforge.net:/cvsroot/mingw checkout -P xscripts >> $LOGFILE 2>&1 || failure
    fi

    echo "Preparing MinGW build script..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/mingw/xscripts || failure
    # note: svn has no force/overwrite switch. the file might not be updated when patched
    # patch x86-mingw32-build.sh.conf < $ROOT/patches/x86-mingw32-build.sh.conf.patch >> $LOGFILE || failure
    cp $ROOT/patches/x86-mingw32-build.sh.conf x86-mingw32-build.sh.conf || failure
    chmod +x x86-mingw32-build.sh >> $LOGFILE || failure

    store_build_state $BS_PREPARE_MINGW
    return 0
}


prepare_sdl()
{
    echo "Preparing SDL..." | tee -a $LOGFILE
    mkdir -p $ROOT/3rdparty/sdl >> $LOGFILE || failure
    mkdir -p $ROOT/build/sdl >> $LOGFILE || failure

    cd $ROOT/3rdparty/sdl || failure
    if [ -d .hg ]; then
        echo "Updating SDL..." | tee -a $LOGFILE
        # local changes (patches) are reverted, hence also updated
        hg update --clean >> $LOGFILE 2>&1 || failure
    else
        echo "Retrieving SDL (this may take a while)..." | tee -a $LOGFILE
        hg clone -r SDL-1.2 http://hg.libsdl.org/SDL . >> $LOGFILE 2>&1 || failure
    fi

    return 0
}


prepare_freetype()
{
    echo "Preparing Freetype2..." | tee -a $LOGFILE
    mkdir -p $ROOT/build/freetype2 >> $LOGFILE || failure

    echo "Retrieving Freetype2 (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty || failure
    wget http://download.savannah.gnu.org/releases/freetype/freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
    tar -xjf freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
    rm freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf freetype2 >> $LOGFILE 2>&1 || failure
    mv freetype-2.3.5 freetype2 >> $LOGFILE 2>&1 || failure

    return 0
}


prepare_libxml()
{
    echo "Preparing libxml2..." | tee -a $LOGFILE
    mkdir -p $ROOT/build/libxml2 >> $LOGFILE || failure

    echo "Retrieving libxml2 (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty || failure
    rm -f libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
    wget --passive-ftp ftp://xmlsoft.org/libxml2/old/libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
    tar -xzf libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
    rm libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
    # substitute old source tree
    rm -rf libxml2 >> $LOGFILE 2>&1 || failure
    mv libxml2-2.6.32 libxml2 >> $LOGFILE 2>&1 || failure

    return 0
}


prepare_oglft()
{
    echo "Preparing OGLFT..." | tee -a $LOGFILE
    mkdir -p $ROOT/3rdparty/oglft >> $LOGFILE || failure
    mkdir -p $ROOT/build/oglft >> $LOGFILE || failure

    cd $ROOT/3rdparty/oglft || failure
    if [ -d .svn ]; then
        echo "Updating OGLFT..." | tee -a $LOGFILE
        # make sure local changes (patches) are reverted, hence also updated
        svn revert -R . >> $LOGFILE  2>&1 || failure
        svn update >> $LOGFILE  2>&1 || failure
    else
        echo "Retrieving OGLFT (this may take a while)..." | tee -a $LOGFILE
        svn checkout http://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $LOGFILE 2>&1 || failure
    fi
}


prepare_boinc()
{
    echo "Preparing BOINC..." | tee -a $LOGFILE
    mkdir -p $ROOT/3rdparty/boinc >> $LOGFILE || failure
    mkdir -p $ROOT/build/boinc >> $LOGFILE || failure

    cd $ROOT/3rdparty/boinc || failure
    if [ -d .git ]; then
        echo "Updating BOINC (tag: $1)..." | tee -a $LOGFILE
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

        echo "Retrieving BOINC (tag: $1) (this may take a while)..." | tee -a $LOGFILE
        cd $ROOT/3rdparty || failure
        git clone git://git.aei.uni-hannover.de/shared/einsteinathome/boinc.git boinc >> $LOGFILE 2>&1 || failure
        cd $ROOT/3rdparty/boinc || failure
        git checkout $1 >> $LOGFILE  2>&1 || failure
    fi

    return 0
}


build_sdl()
{
    if [ $BUILDSTATE -ge $BS_BUILD_SDL ]; then
        return 0
    fi

    prepare_sdl || failure

    echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/sdl || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/sdl || failure
    if [ "$1" == "$TARGET_MAC" ]; then
        $ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --enable-screensaver=yes --enable-video-x11=no >> $LOGFILE 2>&1 || failure
    else
        $ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --enable-screensaver=yes >> $LOGFILE 2>&1 || failure
    fi
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed SDL!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_SDL || failure
    return 0
}


build_freetype()
{
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE ]; then
        return 0
    fi

    prepare_freetype || failure

    echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/freetype2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/freetype2 || failure
    # note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_FREETYPE || failure
    return 0
}


build_libxml()
{
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML ]; then
        return 0
    fi

    prepare_libxml || failure

    echo "Building libxml2 (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/libxml2 || failure
    $ROOT/3rdparty/libxml2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed libxml2!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_LIBXML || failure
    return 0
}


build_oglft()
{
    if [ $BUILDSTATE -ge $BS_BUILD_OGLFT ]; then
        return 0
    fi

    prepare_oglft || failure

    echo "Patching OGLFT..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/oglft || failure
    # note: svn has no force/overwrite switch. patched files might not be updated
    # patch: use fixed settings for freetype, deactivate FindFreetype
    FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
    FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
    patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
    echo "Building OGLFT..." | tee -a $LOGFILE
    cd $ROOT/build/oglft || failure
    # TODO: do we wanna create universal binaries on mac? If so, add -DCMAKE_OSX_ARCHITECTURES=ppc;i386
    cmake -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_OGLFT || failure
    return 0
}


build_boinc()
{
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC ]; then
        return 0
    fi

    prepare_boinc $TAG_GFXAPPS || failure

    echo "Configuring BOINC..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/boinc || failure
    chmod +x _autosetup >> $LOGFILE 2>&1 || failure
    ./_autosetup >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/boinc || failure
    if [ "$1" == "$TARGET_MAC" ]; then
        export CPPFLAGS="-I/sw/include -I/opt/local/include $CPPFLAGS"
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --with-apple-opengl-framework --enable-install-headers --enable-libraries --disable-manager --disable-fcgi >> $LOGFILE 2>&1 || failure
    elif [ -d "/usr/local/ssl" ]; then
        echo "Using local SSL library..." | tee -a $LOGFILE
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --enable-install-headers --enable-libraries --disable-manager --disable-fcgi CPPFLAGS=-I/usr/local/ssl/include LDFLAGS=-L/usr/local/ssl/lib >> $LOGFILE 2>&1 || failure
    else
        $ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --enable-install-headers --enable-libraries --disable-manager --disable-fcgi >> $LOGFILE 2>&1 || failure
    fi
    echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed BOINC!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_BOINC || failure
    return 0
}


build_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_MINGW ]; then
        return 0
    fi
    TARGET_HOST=i586-pc-mingw32

    echo "Building MinGW (this will take quite a while)..." | tee -a $LOGFILE
    # note: the script's current config for unattended setup expects it to be run from three levels below root!
    cd $ROOT/3rdparty/mingw/xscripts || failure
    ./x86-mingw32-build.sh --unattended $TARGET_HOST >> $LOGFILE 2>&1 || failure

    store_build_state $BS_BUILD_MINGW
    return 0
}


set_mingw()
{
    # general config
    PREFIX=$ROOT/install
    TARGET_HOST=i586-pc-mingw32
    BUILD_HOST=i386-linux
    PATH_MINGW="$PREFIX/bin:$PREFIX/$TARGET_HOST/bin:$PATH"
    PATH="$PATH_MINGW"
    export PATH
}


build_sdl_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_SDL_MINGW ]; then
        return 0
    fi

    prepare_sdl || failure

    echo "Patching SDL..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/sdl/src/video/wincommon || failure
    # patch: amend window class name (required by BOINC v6 screensaver!)
    patch < $ROOT/patches/SDL_sysevents.c.patch >> $LOGFILE 2>&1 || failure
    echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/sdl || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-sdl-config" ]; then
        SDL_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-sdl-config"
        export SDL_CONFIG
        echo "Cross-compile SDL_CONFIG: $SDL_CONFIG" >> $LOGFILE
    fi
    cd $ROOT/build/sdl || failure
    $ROOT/3rdparty/sdl/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes --enable-screensaver=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed SDL!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_SDL_MINGW || failure
    return 0
}


build_freetype_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE_MINGW ]; then
        return 0
    fi

    prepare_freetype || failure

    echo "Patching Freetype2..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/freetype2/builds || failure
    # patch: deactivating invocation of apinames (would run win32 binary on linux host)
    patch < $ROOT/patches/freetype2.exports.mk.patch >> $LOGFILE 2>&1 || failure
    echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
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
    # note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_FREETYPE_MINGW || failure
    return 0
}


build_libxml_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML_MINGW ]; then
        return 0
    fi

    prepare_libxml || failure

    echo "Building libxml2 (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config" ]; then
        LIBXML2_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config"
        export LIBXML2_CONFIG
        echo "Cross-compile LIBXML2_CONFIG: $LIBXML2_CONFIG" >> $LOGFILE
    fi
    cd $ROOT/build/libxml2 || failure
    $ROOT/3rdparty/libxml2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed libxml2!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_LIBXML_MINGW || failure
    return 0
}


build_oglft_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_OGLFT_MINGW ]; then
        return 0
    fi

    prepare_oglft || failure

    echo "Patching OGLFT..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/oglft || failure
    # note: svn has no force/overwrite switch. patched files might not be updated
    # patch: use fixed settings for freetype, deactivate FindFreetype
    FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
    FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
    patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
    cp $ROOT/patches/toolchain-linux-mingw.oglft.cmake $ROOT/build/oglft >> $LOGFILE 2>&1 || failure
    export OGLFT_INSTALL=$ROOT/install
    echo "Building OGLFT..." | tee -a $LOGFILE
    cd $ROOT/build/oglft || failure
    cmake -DCMAKE_TOOLCHAIN_FILE="toolchain-linux-mingw.oglft.cmake" -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
    cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_OGLFT_MINGW || failure
    return 0
}

build_boinc_mingw()
{
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC_MINGW ]; then
        return 0
    fi

    prepare_boinc $TAG_GFXAPPS || failure

    echo "Patching BOINC..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/boinc/lib || failure
    # patch: fix a couple of BOINC vs. MinGW issues
    patch boinc_win.h < $ROOT/patches/boinc.boinc_win.h.minggw.patch >> $LOGFILE 2>&1 || failure
    patch filesys.cpp < $ROOT/patches/boinc.filesys.cpp.mingw.patch >> $LOGFILE 2>&1 || failure
    echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
    cd $ROOT/3rdparty/boinc || failure
    chmod +x _autosetup >> $LOGFILE 2>&1 || failure
    ./_autosetup >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/boinc || failure
    # note: configure is still required but we don't use the generated Makefile
    $ROOT/3rdparty/boinc/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$ROOT/install --includedir=$ROOT/install/include --oldincludedir=$ROOT/install/include --enable-shared=no --enable-static=yes --disable-server --disable-client --enable-install-headers --enable-libraries --disable-manager --disable-fcgi >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/boinc/api || failure
    cp $ROOT/3rdparty/boinc/api/Makefile.mingw . >> $LOGFILE 2>&1 || failure
    # patch: add graphics2 and customize build path (see below)
    patch Makefile.mingw < $ROOT/patches/boinc.Makefile.mingw.patch >> $LOGFILE 2>&1 || failure
    export BOINC_SRC=$ROOT/3rdparty/boinc || failure
    cd $ROOT/build/boinc || failure
    # required for out-of-tree build
    cp config.h $ROOT/3rdparty/boinc >> $LOGFILE 2>&1 || failure
    make -f api/Makefile.mingw >> $LOGFILE 2>&1 || failure
    cp $ROOT/build/boinc/libboinc.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
    mkdir -p $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/build/boinc/config.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/build/boinc/version.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/api/boinc_api.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/api/graphics2.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/app_ipc.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/boinc_win.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/common_defs.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/diagnostics.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/diagnostics_win.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/filesys.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/hostinfo.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/proxy_info.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/prefs.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/miofile.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/mfile.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/parse.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    cp $ROOT/3rdparty/boinc/lib/util.h $ROOT/install/include/boinc >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed BOINC!" | tee -a $LOGFILE

    store_build_state $BS_BUILD_BOINC_MINGW || failure
    return 0
}


build_solarsystem()
{
    # make sure ORC is always compiled for host platform (it's executed during solarsystem build!)
    echo "Preparing SolarSystem..." | tee -a $LOGFILE
    mkdir -p $ROOT/build/orc >> $LOGFILE || failure
    mkdir -p $ROOT/build/framework >> $LOGFILE || failure
    mkdir -p $ROOT/build/solarsystem >> $LOGFILE || failure
    export PATH=$PATH_ORG

    prepare_version_header || failure

    echo "Building SolarSystem [ORC]..." | tee -a $LOGFILE
    export ORC_SRC=$ROOT/src/orc || failure
    export ORC_INSTALL=$ROOT/install || failure
    cd $ROOT/build/orc || failure
    cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure
    make $2 >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed SolarSystem [ORC]!" | tee -a $LOGFILE

    # set main include directory
    if [ "$1" == "$TARGET_WIN32" ]; then
        export PATH=$PATH_MINGW
    else
        export PATH=$PATH_ORG
    fi

    echo "Building SolarSystem [Framework]..." | tee -a $LOGFILE
    export FRAMEWORK_SRC=$ROOT/src/framework || failure
    export FRAMEWORK_INSTALL=$ROOT/install || failure
    cd $ROOT/build/framework || failure
    if [ "$1" == "$TARGET_WIN32" ]; then
        cp -f $ROOT/src/framework/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
    else
        cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure
    fi
    make $2 >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed SolarSystem [Framework]!" | tee -a $LOGFILE

    echo "Building SolarSystem [Application]..." | tee -a $LOGFILE
    export SOLARSYSTEM_SRC=$ROOT/src/solarsystem || failure
    export SOLARSYSTEM_INSTALL=$ROOT/install || failure
    cd $ROOT/build/solarsystem || failure
    cp $ROOT/src/solarsystem/*.res . >> $LOGFILE 2>&1 || failure
    if [ "$1" == "$TARGET_MAC" ]; then
        cp -f $ROOT/src/solarsystem/Makefile.macos Makefile >> $LOGFILE 2>&1 || failure
    elif [ "$1" == "$TARGET_WIN32" ]; then
        cp -f $ROOT/src/solarsystem/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
    else
        cp -f $ROOT/src/solarsystem/Makefile . >> $LOGFILE 2>&1 || failure
    fi
    make $2 >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    echo "Successfully built and installed SolarSystem [Application]!" | tee -a $LOGFILE

    return 0
}


build_linux()
{
    echo "Important for an official build: let CC and CXX point to gcc/g++ 4.0.3!"
    build_sdl || failure
    build_freetype || failure
    build_libxml || failure
    build_oglft || failure
    build_boinc || failure
    build_solarsystem $1 $2 || failure

    return 0
}


build_mac()
{
    build_sdl $1 || failure
    build_freetype || failure
    build_libxml || failure
    build_oglft || failure
    build_boinc $1 || failure
    build_solarsystem $1 || failure

    return 0
}


build_win32()
{
    export CPPFLAGS="-D_WIN32_WINDOWS=0x0410 $CPPFLAGS"

    prepare_mingw || failure
    build_mingw || failure
    set_mingw || failure
    build_sdl_mingw || failure
    build_freetype_mingw || failure
    build_libxml_mingw || failure
    build_oglft_mingw || failure
    build_boinc_mingw || failure
    build_solarsystem $TARGET_WIN32 || failure

    return 0
}


print_usage()
{
    cd $ROOT

    echo "*************************"
    echo "Usage: `basename $0` <target>"
    echo
    echo "Available targets:"
    echo "  --linux"
    echo "  --mac"
    echo "  --win32"
    echo "  --doc"
    echo "*************************"

    echo "Wrong usage. Stopping!" >> $LOGFILE

    return 0
}


### main control ##########################################################

# Delete any prior build log
rm -f ./build.log

echo "************************************" | tee -a $LOGFILE
echo "Starting new build!" | tee -a $LOGFILE
echo "`date`" | tee -a $LOGFILE
echo "************************************" | tee -a $LOGFILE

# crude command line parsing :-)

if [ $# -ne 1 ]; then
  print_usage
  exit 1
fi

case "$1" in
    "--linux")
        TARGET=$TARGET_LINUX
        check_last_build "$1" || failure
        echo "Building linux version:" | tee -a $LOGFILE
        check_build_state || failure
        ;;
    "--mac")
        TARGET=$TARGET_MAC
        check_last_build "$1" || failure
        echo "Building mac (Intel) version:" | tee -a $LOGFILE
        check_build_state || failure
        ;;
    "--win32")
        TARGET=$TARGET_WIN32
        check_last_build "$1" || failure
        echo "Building win32 version:" | tee -a $LOGFILE
        check_build_state || failure
        ;;
    "--doc")
        TARGET=$TARGET_DOC
        echo "Building documentation..." | tee -a $LOGFILE
        ;;
    "--distclean")
        distclean || failure
        exit 0
        ;;
    "--solarsystem")
        # "hidden" bonus option :-)
        TARGET=$TARGET_LINUX
        build_solarsystem $TARGET "debug" || failure
        exit 0
        ;;
    *)
        print_usage
        exit 1
        ;;
esac

# here we go...

case $TARGET in
    $TARGET_LINUX)
        check_prerequisites || failure
        prepare_tree || failure
        build_linux || failure
        ;;
    $TARGET_MAC)
        if [ -d /Developer/SDKs/MacOSX10.4u.sdk ]; then
            echo "Preparing Mac OS X 10.4 SDK build environment..." | tee -a $LOGFILE
            # use 10.4 (Tiger) SDK because of BOINC/10.5 incompatibility (http://boinc.berkeley.edu/doxygen/api/html/QBacktrace_8h.html)
            export LDFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -arch i386 $LDFLAGS"
            export CPPFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CPPFLAGS"
            export CFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CFLAGS"
            export CXXFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 $CXXFLAGS"
            export SDKROOT="/Developer/SDKs/MacOSX10.4u.sdk"
            export MACOSX_DEPLOYMENT_TARGET=10.4
        else
            echo "Mac OS X 10.4 SDK required but missing!" | tee -a $LOGFILE
            failure
        fi
        check_prerequisites || failure
        prepare_tree || failure
        build_mac $TARGET_MAC || failure
        ;;
    $TARGET_WIN32)
        check_prerequisites || failure
        prepare_tree || failure
        build_win32 || failure
        ;;
    $TARGET_DOC)
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

echo "************************************" | tee -a $LOGFILE
echo "Build finished successfully!" | tee -a $LOGFILE
echo "`date`" | tee -a $LOGFILE
echo "************************************" | tee -a $LOGFILE

exit 0
