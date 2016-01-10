# ***** BEGIN LICENSE BLOCK *****
# This file is part of Natron <http://www.natron.fr/>,
# Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
#
# Natron is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Natron is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
# ***** END LICENSE BLOCK *****

TARGET = OpenFX
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

include(../global.pri)
include(../config.pri)

!noexpat: CONFIG += expat
 
contains(CONFIG,trace_ofx_actions) {
    DEFINES += OFX_DEBUG_ACTIONS
}

contains(CONFIG,trace_ofx_params) {
    DEFINES += OFX_DEBUG_PARAMETERS
}

contains(CONFIG,trace_ofx_properties) {
    DEFINES += OFX_DEBUG_PROPERTIES
}

precompile_header {
  #message("Using precompiled header")
  # Use Precompiled headers (PCH)
  # we specify PRECOMPILED_DIR, or qmake places precompiled headers in Natron/c++.pch, thus blocking the creation of the Unix executable
  PRECOMPILED_DIR = pch
  PRECOMPILED_HEADER = pch.h
}

#OpenFX C api includes and OpenFX c++ layer includes that are located in the submodule under /submodules/OpenFX
INCLUDEPATH += $$PWD/../submodules/OpenFX/include
INCLUDEPATH += $$PWD/../submodules/OpenFX_extensions
INCLUDEPATH += $$PWD/../submodules/OpenFX/HostSupport/include
INCLUDEPATH += $$PWD/../submodules/OpenFX/include/nuke
INCLUDEPATH += $$PWD/../submodules/OpenFX/include/tuttle

win32-msvc* {
	CONFIG(64bit) {
		QMAKE_LFLAGS += /MACHINE:X64
	} else {
		QMAKE_LFLAGS += /MACHINE:X86
	}
}

win32 {
	DEFINES *= WIN32
	CONFIG(64bit){
		DEFINES *= WIN64
	}
}

noexpat {
    SOURCES += \
    ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/xmlparse.c \
    ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/xmltok.c \
    ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/xmltok_impl.c \
    
    HEADERS += \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/expat.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/expat_external.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/ascii.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/xmltok.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/xmltok_impl.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/lib/asciitab.h \
        ../submodules/OpenFX/HostSupport/expat-2.1.0/expat_config.h \

    DEFINES += HAVE_EXPAT_CONFIG_H

    INCLUDEPATH += $$PWD/../submodules/OpenFX/HostSupport/expat-2.1.0/lib
}

SOURCES += \
    ../submodules/OpenFX/HostSupport/src/ofxhBinary.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhClip.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhHost.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhImageEffect.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhImageEffectAPI.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhInteract.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhMemory.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhParam.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhPluginAPICache.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhPluginCache.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhPropertySuite.cpp \
    ../submodules/OpenFX/HostSupport/src/ofxhUtilities.cpp \
    ../submodules/OpenFX_extensions/ofxhParametricParam.cpp \
    ../submodules/OpenFX/Support/Library/ofxsCore.cpp \
    ../submodules/OpenFX/Support/Library/ofxsImageEffect.cpp \
    ../submodules/OpenFX/Support/Library/ofxsInteract.cpp \
    ../submodules/OpenFX/Support/Library/ofxsLog.cpp \
    ../submodules/OpenFX/Support/Library/ofxsMultiThread.cpp \
    ../submodules/OpenFX/Support/Library/ofxsParams.cpp \
    ../submodules/OpenFX/Support/Library/ofxsProperty.cpp \
    ../submodules/OpenFX/Support/Library/ofxsPropertyValidation.cpp

HEADERS += \
    ../submodules/OpenFX/HostSupport/include/ofxhBinary.h \
    ../submodules/OpenFX/HostSupport/include/ofxhClip.h \
    ../submodules/OpenFX/HostSupport/include/ofxhHost.h \
    ../submodules/OpenFX/HostSupport/include/ofxhImageEffect.h \
    ../submodules/OpenFX/HostSupport/include/ofxhImageEffectAPI.h \
    ../submodules/OpenFX/HostSupport/include/ofxhInteract.h \
    ../submodules/OpenFX/HostSupport/include/ofxhMemory.h \
    ../submodules/OpenFX/HostSupport/include/ofxhParam.h \
    ../submodules/OpenFX/HostSupport/include/ofxhPluginAPICache.h \
    ../submodules/OpenFX/HostSupport/include/ofxhPluginCache.h \
    ../submodules/OpenFX/HostSupport/include/ofxhProgress.h \
    ../submodules/OpenFX/HostSupport/include/ofxhPropertySuite.h \
    ../submodules/OpenFX/HostSupport/include/ofxhTimeLine.h \
    ../submodules/OpenFX/HostSupport/include/ofxhUtilities.h \
    ../submodules/OpenFX/HostSupport/include/ofxhXml.h \
    ../submodules/OpenFX/include/ofxCore.h \
    ../submodules/OpenFX/include/ofxDialog.h \
    ../submodules/OpenFX/include/ofxImageEffect.h \
    ../submodules/OpenFX/include/ofxInteract.h \
    ../submodules/OpenFX/include/ofxKeySyms.h \
    ../submodules/OpenFX/include/ofxMemory.h \
    ../submodules/OpenFX/include/ofxMessage.h \
    ../submodules/OpenFX/include/ofxMultiThread.h \
    ../submodules/OpenFX/include/ofxNatron.h \
    ../submodules/OpenFX/include/ofxOpenGLRender.h \
    ../submodules/OpenFX/include/ofxParam.h \
    ../submodules/OpenFX/include/ofxParametricParam.h \
    ../submodules/OpenFX/include/ofxPixels.h \
    ../submodules/OpenFX/include/ofxProgress.h \
    ../submodules/OpenFX/include/ofxProperty.h \
    ../submodules/OpenFX/include/ofxSonyVegas.h \
    ../submodules/OpenFX/include/ofxTimeLine.h \
    ../submodules/OpenFX/include/nuke/camera.h \
    ../submodules/OpenFX/include/nuke/fnOfxExtensions.h \
    ../submodules/OpenFX/include/nuke/fnPublicOfxExtensions.h \
    ../submodules/OpenFX/include/tuttle/ofxReadWrite.h \
    ../submodules/OpenFX_extensions/ofxhParametricParam.h \
    ../submodules/OpenFX/Support/include/ofxsCore.h \
    ../submodules/OpenFX/Support/include/ofxsImageEffect.h \
    ../submodules/OpenFX/Support/include/ofxsInteract.h \
    ../submodules/OpenFX/Support/include/ofxsLog.h \
    ../submodules/OpenFX/Support/include/ofxsMemory.h \
    ../submodules/OpenFX/Support/include/ofxsMessage.h \
    ../submodules/OpenFX/Support/include/ofxsMultiThread.h \
    ../submodules/OpenFX/Support/include/ofxsParam.h \
    ../submodules/OpenFX/Support/Library/ofxsSupportPrivate.h
