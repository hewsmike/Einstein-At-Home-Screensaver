#!/bin/bash

###########################################################################
#   Copyright (C) 2008-2009 by Oliver Bock                                #
#   oliver.bock[AT]aei.mpg.de                                             #
#                                                                         #
#   Copyright (C) 2015 by Mike Hewson  [ any & all errors are mine :-) ]  #
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
BS_BUILD_SDL_TTF=7
BS_BUILD_GLM=8

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
    mkdir -p $ROOT/build/sdl2_ttf >> $LOGFILE || failure

    mkdir -p $ROOT/install >> $LOGFILE || failure
    mkdir -p $ROOT/install/bin >> $LOGFILE || failure
    mkdir -p $ROOT/install/include >> $LOGFILE || failure
    mkdir -p $ROOT/install/include/GL >> @LOGFILE || failure
    mkdir -p $ROOT/install/include/glm >> @LOGFILE || failure
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

    # Move to parent of $ROOT ( which ought have repo )
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
    
set_mingw() {
    # general config
    PREFIX=$ROOT/install
    BUILD_HOST=$BUILD_SYSTEM
    PATH_MINGW="$PREFIX/bin:$PREFIX/$TARGET_SYSTEM/bin:$PATH"
    # echo $PATH_MINGW
    PATH="$PATH_MINGW"
    export PATH
    
    export CC=`which ${TARGET_SYSTEM}-gcc`
    export CXX=`which ${TARGET_SYSTEM}-g++`
    
    echo "CC = $CC"
    echo "CXX = $CXX"
    
    export CPPFLAGS="-D_WIN32_WINDOWS=0x0410 -DMINGW_WIN32 $CPPFLAGS"
	}    


### functions to build from sources for Win32 targets #################################################################

build_boinc_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_BOINC ]; then
        return 0
    fi

    cd $ROOT/3rdparty/boinc/lib || failure
    log "Building BOINC (this may take a while)..."
    BOINC_SRC="$ROOT/3rdparty/boinc" AR="${TARGET_SYSTEM}-ar" make -f Makefile.mingw >> $LOGFILE 2>&1 || failure
    BOINC_PREFIX="$ROOT/install" RANLIB="${TARGET_SYSTEM}-ranlib" make -f Makefile.mingw install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed BOINC!"

    # save_build_state $BS_BUILD_BOINC_MINGW || failure
    return 0
    }

build_freetype_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_FREETYPE ]; then
        return 0
    fi
    
    log "Building Freetype2 (this may take a while)..."
    cd $ROOT/3rdparty/freetype2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    if [ -f "$PREFIX/$TARGET_SYSTEM/bin/$TARGET_SYSTEM-freetype-config" ]; then
        FT_CONFIG="$PREFIX/$TARGET_SYSTEM/bin/$TARGET_SYSTEM-freetype-config"
        export FT_CONFIG
        echo "Cross-compile FT_CONFIG: $FT_CONFIG" >> $LOGFILE
    fi
    cd $ROOT/build/freetype2 || failure
    # note: freetype probably doesn't need *no* configure when static -> ansi build, see readme!
    $ROOT/3rdparty/freetype2/configure --host=$TARGET_SYSTEM --build=$BUILD_SYSTEM --prefix=$PREFIX --without-png --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    ranlib #ROOT/install/lib/libfreetype.a
    log "Successfully built and installed Freetype2!"

    # save_build_state $BS_BUILD_FREETYPE || failure

    return 0
    }

build_glew_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLEW ]; then
        return 0
    fi

    cd $ROOT/3rdparty/glew

    log "Building GLEW (this may take a while)..."
    make glew.lib >> $LOGFILE 2>&1 || failure
    make GLEW_DEST=$ROOT/install/ install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed GLEW!"

    #save_build_state $BS_BUILD_GLEW || failure

    return 0
    }

build_libxml_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_LIBXML ]; then
        return 0
    fi
    
    log "Configuring libxml2"
    cd $ROOT/3rdparty/libxml2 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
        
    # '--with-threads=no' configure option gets around a weird bug with mingw32 !!    
    $ROOT/3rdparty/libxml2/configure --host=$TARGET_SYSTEM --build=$BUILD_SYSTEM --prefix=$PREFIX --enable-shared=no --enable-static=yes --without-python --with-threads=no >> $LOGFILE 2>&1 || failure
    
    log "Building libxml2 (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed libxml2!"

    # save_build_state $BS_BUILD_LIBXML || failure
    return 0
    }

build_sdl_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_SDL ]; then
        return 0
    fi

    log "Configuring SDL"

    cd $ROOT/3rdparty/sdl2 || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/sdl2 || failure
    $ROOT/3rdparty/sdl2/configure --build=$BUILD_SYSTEM  --host=$TARGET_SYSTEM --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure

    log "Building SDL (this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed SDL!"

    #save_build_state $BS_BUILD_SDL || failure

    return 0
    }

build_sdl_ttf_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_SDL_TTF ]; then
        return 0
    fi
    log "Configuring SDL TTF"
    cd $ROOT/3rdparty/sdl2_ttf || failure
    chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
    ./autogen.sh >> $LOGFILE 2>&1 || failure
    chmod +x configure >> $LOGFILE 2>&1 || failure
    cd $ROOT/build/sdl2_ttf || failure

    # To get around a lame error in the config below
    # cp -f $ROOT/build/freetype2/freetype-config $ROOT/build/sdl2_ttf/freetype-config

    FREETYPE_LOC="$ROOT/install"
    export PATH=$PATH:$ROOT/install/bin

    $ROOT/3rdparty/sdl2_ttf/configure --build=$BUILD_SYSTEM --host=$TARGET_SYSTEM  --with-freetype-prefix=$FREETYPE_LOC --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure

    log "Building SDL TTF(this may take a while)..."
    make >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed SDL TTF!"

    #save_build_state $BS_BUILD_SDL_TTF || failure

    return 0
    }
    
build_glm_mingw() {
    if [ $BUILDSTATE -ge $BS_BUILD_GLM ]; then
        return 0
    fi

    cd $ROOT/install/include || failure

    log "Building GLM (this may take a while)..."
    
    # Special instance here as glm is not a build but header only inclusion.
    cp -rfu $ROOT/3rdparty/glm/glm/* $ROOT/install/include/glm >> $LOGFILE 2>&1 || failure 
    
    log "Successfully built and installed GLEW!"

    #save_build_state $BS_BUILD_GLM || failure

    return 0
    }    

### specific build functions ###########################################################################################

build_orc_mingw() {
    # make sure ORC is always compiled for host platform
    log "Preparing $PRODUCT_NAME..."
    mkdir -p $ROOT/build/orc >> $LOGFILE || failure

    # Create the header containing ( if available ) any git commit,
    # build machine and working directory information.
    write_version_header || failure

    export ORC_SRC=$ROOT/src/orc || failure
    export ORC_INSTALL=$ROOT/install || failure

    cd $ROOT/build/orc || failure
    cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure

    log "Building $PRODUCT_NAME [Object Resource Compiler]..."
    make $makemode >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure

    log "Successfully built and installed $PRODUCT_NAME [Object Resource Compiler]!"
    return 0
    }

build_framework_mingw() {
    # Now compile framework for host platform
    log "Building $PRODUCT_NAME [Framework]..."
    mkdir -p $ROOT/build/framework >> $LOGFILE || failure
    export FRAMEWORK_SRC=$ROOT/src/framework || failure
    export OGL_UTILITY_SRC=$ROOT/src/ogl_utility || failure
    export FRAMEWORK_INSTALL=$ROOT/install || failure

    cd $ROOT/build/framework || failure
    cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure

    make $makemode PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [Framework]!"

    return 0
    }

build_ogl_utility_mingw() {
    # Now compile OpenGL utility classes for host platform
    log "Building $PRODUCT_NAME [OpenGL Utilities]..."
    mkdir -p $ROOT/build/ogl_utility >> $LOGFILE || failure
    export FRAMEWORK_INSTALL=$ROOT/install || failure
    export UTILITY_SRC=$ROOT/src/ogl_utility || failure
    export UTILITY_INSTALL=$ROOT/install || failure
    cd $ROOT/build/ogl_utility || failure
    cp -f $ROOT/src/ogl_utility/Makefile . >> $LOGFILE 2>&1 || failure


    make $makemode PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [OpenGL Utilities]!"

    return 0
    }

build_product_mingw() {
    log "Building $PRODUCT_NAME [Application]..."
    export PROJECT_ROOT=$ROOT || failure
    cd $ROOT/build/$PRODUCT || failure
    cp $ROOT/src/$PRODUCT/*.res . >> $LOGFILE 2>&1 || failure
    cp -f $ROOT/src/$PRODUCT/Makefile.common Makefile >> $LOGFILE 2>&1 || failure
    make $makemode SYSTEM="linux" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure

    make install >> $LOGFILE 2>&1 || failure
    log "Successfully built and installed $PRODUCT_NAME [Application]!"

    return 0
    }

build_win32() {
    log "Important for an official build: let CC and CXX point to mingw 4.9+ !"
    mkdir -p $ROOT/build/$PRODUCT >> $LOGFILE || failure

	export CPPFLAGS="-D_WIN32_WINDOWS=0x0410 $CPPFLAGS"

    set_mingw || failure
      
    # Make sure the base libraries are built.    
   	# build_boinc_mingw || failure
    # build_glew_mingw || failure    
    build_freetype_mingw || failure
    # build_libxml_mingw || failure
    build_sdl_mingw || failure
    build_sdl_ttf_mingw || failure
    # build_glm_mingw || failure
	
	# build_starsphere $TARGET_WIN32 || failure
	
    # Now client code.
    #     build_orc_mingw || failure
    #     build_framework_mingw || failure
    #     build_ogl_utility_mingw || failure
    #     build_product_mingw || failure

    return 0
    }

### main control #######################################################################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "Starting new Win32 build!"
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
            makemode=debug
            ;;
        "--release")
            log "Release build chosen"
            MODE=$MODE_RELEASE
            makemode=release
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
build_win32 $2 || failure

log "++++++++++++++++++++++++++++++++++++"
log "Build finished successfully!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
