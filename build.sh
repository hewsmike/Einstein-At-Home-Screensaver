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

GLEW_VERSION=1.7.0
GLFW_VERSION=2.7.5
FREETYPE_VERSION=2.3.5
LIBXML_VERSION=2.6.32

TARGET=0
TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3
TARGET_DOC=4

BUILDSTATE=0
BS_PREREQUISITES=1
BS_PREPARE_TREE=2
BS_BUILD_GLEW=3
BS_BUILD_GLFW=4
BS_BUILD_FREETYPE=5
BS_BUILD_LIBXML=6
BS_BUILD_OGLFT=7
BS_BUILD_BOINC=8
BS_BUILD_GLEW_MINGW=9
BS_BUILD_GLFW_MINGW=10
BS_BUILD_FREETYPE_MINGW=11
BS_BUILD_LIBXML_MINGW=12
BS_BUILD_OGLFT_MINGW=13
BS_BUILD_BOINC_MINGW=14

### functions (tools) #############################################################

log() {
	echo $1 | tee -a $LOGFILE
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

distclean() {
   cd $ROOT || failure

   log "Purging build system..."

   rm -rf 3rdparty || failure
   rm -rf build || failure
   rm -rf install || failure
   rm -rf doc/html || failure
   rm -f doc/*.tag || failure

   rm -f .lastbuild || failure
   rm -f .buildstate || failure
   }

check_last_build() {
   log "Checking previous build target..."

   LASTBUILD=`cat .lastbuild 2>/dev/null`

   if [[ ( -f .lastbuild ) && ( "$LASTBUILD" != "$1" ) ]]; then
      cd $ROOT || failure
      log "Build target changed! Purging build and install trees..."
      rm -rf build >> $LOGFILE || failure
      rm -rf install >> $LOGFILE || failure
      prepare_tree || failure
   fi

   echo "$1" > .lastbuild || failure

   return 0
   }

check_build_state() {
   log "Checking for previous build checkpoints..."

   if [ ! -f .buildstate ]; then
      cd $ROOT || failure
      log "No previous build checkpoints found! Starting from scratch..."
   else
      BUILDSTATE=`cat $ROOT/.buildstate 2>/dev/null`
      echo "Recovering previous build..."
   fi

   return 0
   }

store_build_state() {
   log "Saving build checkpoint..."
   echo "$1" > $ROOT/.buildstate || failure

   return 0
   }

### functions (features) #############################################################

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

   return 0
   }

prepare_tree() {
   if [ $BUILDSTATE -ge $BS_PREPARE_TREE ]; then
      return 0
   fi

   log "Preparing tree..."
   mkdir -p 3rdparty >> $LOGFILE || failure
   mkdir -p install/bin >> $LOGFILE || failure
   mkdir -p install/include >> $LOGFILE || failure
   mkdir -p install/include/GL >> @LOGFILE || failure
   mkdir -p install/lib >> $LOGFILE || failure

   store_build_state $BS_PREPARE_TREE
   return 0
   }

prepare_version_header() {
   HEADER_FILE="$ROOT/src/erp_git_version.h"

   cd $ROOT || failure

   log "Retrieving git version information..."

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
   wget http://sourceforge.net/projects/glfw/files/glfw/2.7.5/glfw-2.7.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   tar -xjf glfw-2.7.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   rm glfw-2.7.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   # substitute old source tree
   rm -rf glfw >> $LOGFILE 2>&1 || failure
   mv glfw-2.7.5 glfw >> $LOGFILE 2>&1 || failure

   return 0
   }

prepare_freetype() {
   log "Preparing Freetype2..."
   mkdir -p $ROOT/build/freetype2 >> $LOGFILE || failure

   log "Retrieving Freetype2 (this may take a while)..."
   cd $ROOT/3rdparty || failure
   wget http://download.savannah.gnu.org/releases/freetype/freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   tar -xjf freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   rm freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
   # substitute old source tree
   rm -rf freetype2 >> $LOGFILE 2>&1 || failure
   mv freetype-2.3.5 freetype2 >> $LOGFILE 2>&1 || failure

   return 0
   }

prepare_libxml() {
   log "Preparing libxml2..."
   mkdir -p $ROOT/build/libxml2 >> $LOGFILE || failure

   log "Retrieving libxml2 (this may take a while)..."
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

   store_build_state $BS_BUILD_GLEW || failure
   return 0
   }

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
      
   store_build_state $BS_BUILD_GLEW || failure
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

   store_build_state $BS_BUILD_GLFW || failure
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

   store_build_state $BS_BUILD_GLFW || failure
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

   store_build_state $BS_BUILD_FREETYPE || failure
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

   store_build_state $BS_BUILD_LIBXML || failure
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

   store_build_state $BS_BUILD_OGLFT || failure
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

   store_build_state $BS_BUILD_BOINC || failure
   return 0
   }

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

   store_build_state $BS_BUILD_GLEW_MINGW || failure
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

   store_build_state $BS_BUILD_GLFW_MINGW || failure
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

   store_build_state $BS_BUILD_FREETYPE_MINGW || failure
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

   store_build_state $BS_BUILD_LIBXML_MINGW || failure
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

   store_build_state $BS_BUILD_OGLFT_MINGW || failure
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

   store_build_state $BS_BUILD_BOINC_MINGW || failure
   return 0
   }

build_product() {
   # make sure ORC is always compiled for host platform (it's executed during $PRODUCT_NAME build!)
   log "Preparing $PRODUCT_NAME..."
   mkdir -p $ROOT/build/orc >> $LOGFILE || failure
   mkdir -p $ROOT/build/framework >> $LOGFILE || failure
   mkdir -p $ROOT/build/$PRODUCT >> $LOGFILE || failure
   export PATH=$PATH_ORG

   prepare_version_header || failure

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
   export FRAMEWORK_INSTALL=$ROOT/install || failure
   cd $ROOT/build/framework || failure
   if [ "$1" == "$TARGET_WIN32" ]; then
      cp -f $ROOT/src/framework/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
   else
      cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure
   fi
   make $2 PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
   make install >> $LOGFILE 2>&1 || failure
   log "Successfully built and installed $PRODUCT_NAME [Framework]!"

   log "Building $PRODUCT_NAME [Application]..."
   export PROJECT_ROOT=$ROOT || failure
   cd $ROOT/build/$PRODUCT || failure
   cp $ROOT/src/$PRODUCT/*.res . >> $LOGFILE 2>&1 || failure
   cp -f $ROOT/src/$PRODUCT/Makefile.common Makefile >> $LOGFILE 2>&1 || failure
   if [ "$1" == "$TARGET_MAC" ]; then
   		make release SYSTEM="mac" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
      elif [ "$1" == "$TARGET_WIN32" ]; then
      	make release SYSTEM="win32" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
      else
			make release SYSTEM="linux" PRODUCT=$PRODUCT_NAME >> $LOGFILE 2>&1 || failure
      fi
   make install >> $LOGFILE 2>&1 || failure
   log "Successfully built and installed $PRODUCT_NAME [Application]!"

   return 0
   }

build_linux() {
   echo "Important for an official build: let CC and CXX point to gcc/g++ 4.6+ !"
   build_glew || failure
   build_glfw || failure
   build_freetype || failure
   build_libxml || failure
   build_oglft || failure
   build_boinc || failure
   build_product $1 $2 || failure

   return 0
   }

build_mac() {
   build_glew_mac || failure
   build_glfw_mac $1 || failure
   build_freetype || failure
   build_libxml || failure
   build_oglft || failure
   build_boinc $1 || failure
   build_product $1 || failure

   return 0
   }

build_win32() {
   # no more prepare/build steps for MinGW
   # we use Debian's MinGW with GCC 4.4 support
   set_mingw || failure

   build_glew_mingw || failure
   build_glfw_mingw || failure
   # build_oglplus_mingw || failure
   build_freetype_mingw || failure
   build_libxml_mingw || failure
   build_oglft_mingw || failure
   build_boinc_mingw || failure
   build_product $TARGET_WIN32 || failure

   return 0
   }

print_usage() {
   cd $ROOT

   echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
   echo "                         Usage"
   echo "                        +++++++"
   echo "With two parameters :" 
   echo 
   echo "         `basename $0` <target> <product>"
   echo
   echo "Available targets:"
   echo " --linux"
   echo " --mac"
   echo " --mac-sdk    ( build with Mac OS 10.4 x86 SDK )"
   echo " --win32"
   echo " --doc"
   echo
   echo "Available products:"
   echo " --starsphere"
   echo " --solarsystem"
   echo
   echo "OR with a single parameter:"
   echo 
   echo "         `basename $0` --linux         ( implies --solarsystem )"
   echo "         `basename $0` --solarsystem   ( implies --linux )"
   echo "         `basename $0` --doc"
   echo "         `basename $0` --distclean" 
   echo
   echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

   echo "Wrong usage. Stopping!" >> $LOGFILE

   return 0
   }

### main control ##########################################################

# Delete any prior build log
rm -f ./build.log

log "++++++++++++++++++++++++++++++++++++"
log "Starting new build!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

# crude command line parsing :-)

if [ $# -ne 2 ]; then
print_usage
   exit 1
fi

case "$1" in
   "--linux")
      TARGET=$TARGET_LINUX
      check_last_build "$1" || failure
      log "Building linux version:"
      check_build_state || failure
      ;;
   "--mac")
      TARGET=$TARGET_MAC
      check_last_build "$1" || failure
      log "Building mac (Intel) version:"
      check_build_state || failure
      ;;
   "--mac-sdk")
      TARGET=$TARGET_MAC
      SDK="yes"
      check_last_build "$1" || failure
      log "Building mac (Intel) version with SDK:"
      check_build_state || failure
      ;;
   "--win32")
      TARGET=$TARGET_WIN32
      check_last_build "$1" || failure
      log "Building win32 version:"
      check_build_state || failure
      ;;
   "--doc")
      TARGET=$TARGET_DOC
      log "Building documentation..."
      ;;
   "--distclean")
      distclean || failure
      exit 0
      ;;
   "--product_test")
      # "hidden" bonus option :-)
      TARGET=$TARGET_LINUX
      build_product $TARGET "memcheck" || failure
      exit 0
      ;;
   *)
      print_usage
      exit 1
      ;;
esac

case "$2" in
   "--starsphere")
      PRODUCT=starsphere
      PRODUCT_NAME="Starsphere"
      log "Building $PRODUCT_NAME"
      ;;
   "--solarsystem")
      PRODUCT=solarsystem
      PRODUCT_NAME="SolarSystem"
      log "Building Solarsystem"
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
      if [ ".$SDK" = "." ] ; then
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

log "++++++++++++++++++++++++++++++++++++"
log "Build finished successfully!"
log "`date`"
log "++++++++++++++++++++++++++++++++++++"

exit 0
