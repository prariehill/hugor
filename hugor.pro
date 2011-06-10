QT += core gui
TEMPLATE = app
CONFIG += warn_on silent
VERSION = 0.1.0.99
TARGET = hugor

!sound_sdl:!sound_fmod {
    error("Use CONFIG+=sound_sdl or CONFIG+=sound_fmod to select a sound engine")
}
sound_sdl {
    DEFINES += SOUND_SDL
}
sound_fmod {
    DEFINES += SOUND_FMOD
}

macx {
    QMAKE_INFO_PLIST = Info.plist

    sound_sdl {
        QMAKE_LFLAGS += -F./Frameworks
        LIBS += -framework SDL_mixer -framework SDL
        INCLUDEPATH += \
            ./Frameworks/SDL.framework/Headers \
            ./Frameworks/SDL_mixer.framework/Headers \
            ./Frameworks/smpeg.framework/Headers
    }

    sound_fmod {
        LIBS += -lfmodex
        INCLUDEPATH += ./fmod/api/inc
    }

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
    TARGET = Hugor
} else {
    sound_sdl {
        CONFIG += link_pkgconfig
        PKGCONFIG += sdl
        # Normally we would use pkg-config for SDL_mixer too, but it has
        # to appear in the linker flags before SDL_sound, which lacks
        # pkg-config support, or else we crash.
        LIBS += -lSDL_mixer
    }

    sound_fmod {
        contains(QMAKE_HOST.arch, x86_64) {
            LIBS += -lfmodex64
        } else {
            LIBS += -lfmodex32
        }
    }
}
win32 {
    sound_sdl {
        LIBS += -lmad -lmikmod
    }
    TARGET = Hugor
}

# We use warn_off to allow only default warnings, not to supress them all.
QMAKE_CXXFLAGS_WARN_OFF =
QMAKE_CFLAGS_WARN_OFF =

*-g++* {
    # Avoid a flood of "unused parameter" warnings.
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
    QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter
}

INCLUDEPATH += /usr/local/include
INCLUDEPATH += src hugo
OBJECTS_DIR = obj
MOC_DIR = tmp
UI_DIR = tmp

DEFINES += QT

RESOURCES += resources.qrc

FORMS += \
    src/aboutdialog.ui \
    src/confdialog.ui

HEADERS += \
    src/aboutdialog.h \
    src/confdialog.h \
    src/happlication.h \
    src/hframe.h \
    src/hmainwindow.h \
    src/hugodefs.h \
    src/kcolorbutton.h \
    src/settings.h \
    \
    hugo/heheader.h \
    hugo/htokens.h

SOURCES += \
    src/aboutdialog.cc \
    src/confdialog.cc \
    src/happlication.cc \
    src/heqt.cc \
    src/hframe.cc \
    src/hmainwindow.cc \
    src/kcolorbutton.cc \
    src/main.cc \
    src/settings.cc \
    \
    #hugo/heblank.c \
    #hugo/iotest.c \
    hugo/he.c \
    hugo/hebuffer.c \
    hugo/heexpr.c \
    hugo/hemisc.c \
    hugo/heobject.c \
    hugo/heparse.c \
    hugo/heres.c \
    hugo/herun.c \
    hugo/heset.c \
    hugo/stringfn.c

sound_sdl {
    SOURCES += src/soundsdl.cc
}
sound_fmod {
    SOURCES += src/soundfmod.cc
}
